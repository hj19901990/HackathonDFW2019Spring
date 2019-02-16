#include "ContentRendering.h"

ContentRendering::ContentRendering(HWND main_form, PictureBox^  stream, app::AppConfig^ sys_configuration)
{
	_cur_time = DateTime::Now.Ticks;
	_img_height = IMG_HGT;
	_img_width = IMG_WID;
	_img_chn = IMG_CHN;
	_udp_pkg_num = IMG_HGT/UDP_LINES;
	_prev_time = 0;
	_img_updated = false;
	_img_processed = true;
	_display_line_update = 0;
	_prev_stamp = 0;
	_hWndMain = main_form;
	_program_start = false;
	_iDispRotation = 180;
	_udp_pkg_idx = 0;
	_rgbDis = NULL;
	_thread_listen = NULL;
	_thread_processing = NULL;
	_thread_display = NULL;
	_img_refreshed = NULL;
	_pDrawImg = NULL;
	// Init Direct2D
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_pD2DFactory);
	_img_preprocessing = new Img_Prepro();
	_etherComm = new CComm();
	_video_request = new UDP_Income_Data();

	ScanInit(sys_configuration, stream);
}

ContentRendering::~ContentRendering() {
	_program_start = false;
	if (_thread_listen) {
		WaitForSingleObject(_thread_listen, INFINITE);
		CloseHandle(_thread_listen);
		_thread_listen = NULL;
	}

	Sleep(50);

	delete 		_etherComm;
	delete		_video_request;
	delete		_rgbDis;
	delete		_img_preprocessing;
	if (_pDrawImg) {
		delete _pDrawImg;
		_pDrawImg = NULL;
	}
}
void ContentRendering::ScanInit(app::AppConfig^ sys_config, PictureBox^  stream) {
	ApplyConfigChange(sys_config, stream);
	BindMCU();
	SysInitialization();
}
void ContentRendering::ScanReconfig(app::AppConfig^ sys_config, PictureBox^  stream) {
	if (_rgbDis) {
		delete _rgbDis;
		_rgbDis = NULL;
	}
	ApplyConfigChange(sys_config, stream);
	SysInitialization();
}
void ContentRendering::ApplyConfigChange(app::AppConfig^ sys_config, PictureBox^  stream) {
	_host = sys_config->_cfgDv->_host;
	_port = sys_config->_cfgDv->_port;											//port number
	_rgbDis = new RGBQUAD[_img_height*_img_width];
	//tile_size.height = 1;
	//tile_size.width = 24;
	//clahe->setTilesGridSize(tile_size);
	//stream->Location = System::Drawing::Point(10,50);
	//stream->Size = System::Drawing::Size(frame->Width - 10, frame->Height - 10);
	AssignDrawWindow((HWND)(stream->Handle.ToPointer()));
}

void ContentRendering::BindMCU() {

	_etherComm->_UDP_host = _host;
	_etherComm->_UDP_port = _port;

	if (!_etherComm->Listen(_port))	// Binding MCU
	{
		//String^	strErr = String::Format("Port {0} Binding failure", _port);
		//MessageBox::Show(strErr);
		std::cout << "Port Binding failure" << std::endl;
		return;
	}
}

void ContentRendering::SysInitialization() {

}

void ContentRendering::CaptureImage() {
	if (!_program_start) {
		_program_start = true;
			CreateThreadFun();
		}
}
void ContentRendering::StopCapturing() {
	if (_program_start) {
		_program_start = false;
		WaitForSingleObject(_thread_listen, INFINITE);
		CloseHandle(_thread_listen);
		_thread_listen = NULL;

	}

}
/*void ContentRendering::SaveToImage(Bitmap^	saveBmp) {
	for (size_t i = 0; i < _img_height; i++)
	{
		std::vector<BYTE> cur_line = _saving_Frame[(int)(!_saving_idx)][i];

		for (size_t j = 0; j < _img_width; j++)
		{
			Color curColor = Color::FromArgb(cur_line[j], cur_line[j], cur_line[j]);
			saveBmp->SetPixel(j, i, curColor);
		}
	}
	_saving_Frame[(int)(!_saving_idx)].clear();
	String^ file_name = String::Format("Img_{0}.bmp", _img_cnt++);
	saveBmp->Save(file_name, System::Drawing::Imaging::ImageFormat::Bmp);
}*/

bool ContentRendering::ApplyVideoRequest() {
	char buffer[16];

	_video_request->generalHeader.timeStamp = DateTime::Now.Ticks / 100;
	
	int struct_size = sizeof(*_video_request);
	memcpy(buffer, _video_request, struct_size);
	return _etherComm->SendMsg(buffer, struct_size, _host, _port);
}

//--------------------------Receiving Data Thread-------------------------------//
bool ContentRendering::CreateThreadFun() {

	int ThreadID;

	_thread_listen = CreateThread(NULL, 0, ListenThread, this, 0, NULL);

	ThreadID = (int)_thread_listen ? 0 : 1;

	if (!ThreadID) {
		
		_thread_processing = CreateThread(NULL, 0, DataPreProcessingThread, this, 0, NULL);
		ThreadID = (int)_thread_processing ? 0 : 1;
	}

	if (!ThreadID) {
		_img_refreshed = CreateEvent(NULL, TRUE, FALSE, NULL);
		_thread_display = CreateThread(NULL, 0, DisplayThread, this, 0, NULL);
		ThreadID = (int)_thread_display ? 0 : 1;
	}

	if (ThreadID)
		return false;
	else
		return true;
}



DWORD WINAPI ContentRendering::ListenThread(LPVOID pParam)
{
	ContentRendering* ethernet_para = (ContentRendering *)pParam;
	while (ethernet_para->_program_start)
		ethernet_para->ReceiveData_RaspPi();
	return 0;
}

DWORD WINAPI ContentRendering::DataPreProcessingThread(LPVOID pParam)
{
	ContentRendering* ethernet_para = (ContentRendering *)pParam;
	while (ethernet_para->_program_start)
		ethernet_para->ImgProcessing();
	return 0;
}

DWORD WINAPI ContentRendering::DisplayThread(LPVOID pParam)
{
	ContentRendering* ethernet_para = (ContentRendering *)pParam;
	while (ethernet_para->_program_start)
		ethernet_para->UpdataScreen();
	return 0;
}

DWORD WINAPI ContentRendering::ReceiveData_RaspPi() {//need to be replace by https
	char buf[IMG_WID*IMG_CHN*UDP_LINES +8];
	int len = sizeof(_client);
	int rst_udp_pkg_num = _udp_pkg_num;
	std::vector<BYTE>	temp_line_buf;
	
	while (rst_udp_pkg_num--) {

		//long pre_t = DateTime::Now.Ticks;
		int SelectTiming = _etherComm->RecvfromTimeOutUDP(_etherComm->ListenSocket, 1, 50000);
		switch (SelectTiming)
		{
		case 0:
			// Timed out, do whatever you want to handle this situation
			//printf("Server: Timeout lor while waiting you bastard client!...\n");
			rst_udp_pkg_num = -1;
			_udp_pkg_idx = 0;
			return 0;
		case -1:
			// Error occurred, maybe we should display an error message?
			// Need more tweaking here and the recvfromTimeOutUDP()...
			//printf("Server: Some error encountered with code number: %ld\n", WSAGetLastError());
			rst_udp_pkg_num = -1;
			_udp_pkg_idx = 0;
			return 0;
		default:
			//Receiving data
			int buf_size = sizeof(buf);
			int result = recvfrom(_etherComm->ListenSocket, buf, sizeof(buf), 0, (sockaddr *)&_client, (socklen_t *)&len);
			if (result >0)
			{
				int rev_idx;
				memcpy(&rev_idx, buf+4, sizeof(int));
				if (_udp_pkg_idx == rev_idx) {
					temp_line_buf.insert(temp_line_buf.end(),buf + 8, buf + result);
					//_process_image.push_back(std::vector<BYTE>(buf + 8, buf + result));
					long cur_t = DateTime::Now.Ticks;

					//std::cout << "Time:" << (cur_t - pre_t) / 10000.0 << std::endl;
				}
				else {
					_udp_pkg_idx = 0;
					rst_udp_pkg_num = _udp_pkg_num;
					//std::cout << "idx failed to match" << std::endl;
					break;
				}
				_udp_pkg_idx++;
				//monitor++;
				//copyToBuffer(buf);
				
				fflush(0);
			}
			else
				return 0;

			break;
		}
	};

	if (rst_udp_pkg_num < 0) {
		
		copyToBuffer(temp_line_buf);
		_img_updated = true;

		//#ifdef MULTI_THREADS_DEBUG
		//std::cout << "Line #:" << _display_line_update << std::endl;
		//#endif // MULTI_THREADS_DEBUG

		WaitForSingleObject(_img_refreshed, 100);
		ResetEvent(_img_refreshed);
		
		_buf_image.clear();
		_udp_pkg_idx = 0;
		
	}
	return 0;
	
}
void ContentRendering::copyToBuffer(std::vector<BYTE>& src) {
	if (_img_processed) {
		_img_dis = cv::Mat(_img_height, _img_width, CV_8UC3, src.data());
		_img_processed = false;
	}
	
	size_t width_bits = _img_chn*_img_width;
	std::vector<BYTE> temp_line(width_bits);
	for (int ii=0, i = _img_height-1; i >=0; i--,ii++) {
	//for (int i = 0; i < _img_height; i++) {
		for (int j = 0,k=0; j < width_bits; j += 3,k++) {

			temp_line[j] = src[j + width_bits*i];
			temp_line[j+1] = src[j+1 + width_bits*i];
			temp_line[j+2] = src[j+2 + width_bits*i];
			_rgbDis[k + ii*_img_width].rgbRed = temp_line[j];
			_rgbDis[k + ii*_img_width].rgbGreen = temp_line[j + 1];
			_rgbDis[k + ii*_img_width].rgbBlue = temp_line[j + 2];
		}
		_buf_image.push_back(temp_line);	
	}
	//cv::imwrite("filtered.bmp", _img_dis);
	//cv::imshow("Video_stream", _img_dis);
	//cv::waitKey(10);
	//cv::destroyAllWindows();
}
bool ContentRendering::AssignDrawWindow(HWND hWndImg) {

	if (NULL == _pD2DFactory) {
		return false;
	}
	if (_hWndImg) {
		_hWndImg = NULL;
	}

	if (_pDrawImg) {
		delete _pDrawImg;
		_pDrawImg = NULL;
	}

	_hWndImg = hWndImg;
	_pDrawImg = new DrawDevice();
	bool result = _pDrawImg->Initialize(
		_hWndImg,
		_pD2DFactory,
		_img_width,
		_img_height,
		_img_width * 4,
		_iDispRotation
	);
	return result;
}

void ContentRendering::ImgProcessing() {
	if (!_img_processed) {
		std::vector<cv::Rect> face_rst = _img_preprocessing->FaceDetection(_img_dis);
		if (face_rst.size()) {
			_pDrawImg->face_rect = face_rst[0];
			_pDrawImg->fact_detected = true;
		}
		else
			_pDrawImg->fact_detected = false;
		_img_processed = true;
	}
			
}

void ContentRendering::UpdataScreen() {
	
#ifdef MULTI_THREADS_DEBUG
		std::cout << "Line cnt " << updated_line << std::endl;
#endif
		if (_img_updated) {
			//GetCapturedBufferRGB();
			_img_updated = false;
			SetEvent(_img_refreshed);
			_pDrawImg->Draw(reinterpret_cast<BYTE*>(_rgbDis), _img_width * _img_height * sizeof(RGBQUAD));
		}

#ifdef MULTI_THREADS_DEBUG
		std::cout << "Screen Updated" << std::endl;
#endif // MULTI_THREADS_DEBUG	
}



const RGBQUAD* ContentRendering::GetCapturedBufferRGB() {
	//Int64 init_time = DateTime::Now.Ticks;
		for (int i = 0; i < _img_height; i++) {
			std::vector<BYTE> cur_line = _buf_image[i];
			//int img_width_bits = cur_line.size();
			for (int j = 0; j < _img_width; j+=3) {
				_rgbDis[j + i*_img_width].rgbRed = cur_line[j];
				_rgbDis[j + i*_img_width].rgbGreen = cur_line[j+1];
				_rgbDis[j + i*_img_width].rgbBlue = cur_line[j+2];
			}
		}
	return _rgbDis;
}

void ContentRendering::DrawBox(System::Windows::Forms::PaintEventArgs^  e) {
	
	System::Drawing::Pen^ pen = gcnew System::Drawing::Pen(Color::Green, 2.0f);
	PointF pt1(200, 90);
	PointF pt2(200+50, 90);
	e->Graphics->DrawLine(pen, pt1, pt2);
	PointF pt3(200, 90);
	PointF pt4(200, 90+50);
	e->Graphics->DrawLine(pen, pt3, pt4);

	PointF pt5(400, 90);
	PointF pt6(400 - 50, 90);
	e->Graphics->DrawLine(pen, pt5, pt6);
	PointF pt7(400, 90);
	PointF pt8(400, 90+50);
	e->Graphics->DrawLine(pen, pt7, pt8);

	PointF pt9(200, 340);
	PointF pt10(200 + 50, 340);
	e->Graphics->DrawLine(pen, pt9, pt10);
	PointF pt11(200, 340);
	PointF pt12(200, 340 - 50);
	e->Graphics->DrawLine(pen, pt11, pt12);

	PointF pt13(400, 340);
	PointF pt14(400 - 50, 340);
	e->Graphics->DrawLine(pen, pt13, pt14);
	PointF pt15(400, 340);
	PointF pt16(400, 340 - 50);
	e->Graphics->DrawLine(pen, pt15, pt16);

}
