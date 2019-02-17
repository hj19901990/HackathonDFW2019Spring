#pragma once
//#include"stdafx.h"
#include "Com.h"
#include "DrawDevice.h"
#include "Img_Prepro.h"
//#include "Protocols.h"
#include "CloudClient.h"


#define WM_SAVE			WM_USER+1
#define IMG_WID			640
#define IMG_HGT			480
#define IMG_CHN			3
#define UDP_LINES		24


struct Personal_Info {
	std::string name;
	std::string age;
	std::string gender;
	std::string association;
	std::string hobbies;
	std::string emotion;

public:
	Personal_Info() {};
};


struct UDP_General_Header {
	unsigned int headerRecognitionByte;
	unsigned int timeStamp;
public:
	UDP_General_Header(unsigned int headerByte, unsigned int time)
		:headerRecognitionByte(headerByte)
		, timeStamp(time) {};
};
struct UDP_Income_Data {
	UDP_General_Header generalHeader;
public:
	UDP_Income_Data()
		: generalHeader(0xA1, 0)
	{

	};
};

class ContentRendering
{
public:
	ContentRendering(HWND hWndImg);
	~ContentRendering();
	//void ScanInit(app::AppConfig^ sys_config, PictureBox^  stream);
	//void ScanReconfig(app::AppConfig^ sys_config, PictureBox^  stream);
	//void ApplyConfigChange(app::AppConfig^ sys_config, PictureBox^  stream);
	void BindMCU();
	void SysInitialization();
	//void SaveToImage(Bitmap^ saveBmp);
	void CaptureImage();
	void StopCapturing();
	//void DrawBox(System::Windows::Forms::PaintEventArgs^  e);

	bool CreateThreadFun();
	bool AssignDrawWindow(HWND hWndImg);

private:
	//bool ApplyVideoRequest();
	void copyToBuffer(std::vector<BYTE>& src);
	void ImgProcessing();
private:
	static DWORD WINAPI ListenThread(LPVOID pParam);
	static DWORD WINAPI DataPreProcessingThread(LPVOID pParam);
	static DWORD WINAPI DisplayThread(LPVOID pParam);
	static DWORD WINAPI ResultThread(LPVOID pParam);
	DWORD WINAPI ReceiveData_RaspPi();
	//void ShowMessage(UINT nMsgID, UINT nType);
	void UpdataScreen();
	void ReceiveRst(Individual *_indi);
	const RGBQUAD* GetCapturedBufferRGB();

private:
	CloudClient *_client_ssl;
	//Individual _indi;
	UDP_Income_Data*_video_request;
	HWND          _hWndMain;
	CComm*	_etherComm;
	
	const char* _host;
	int _port;

	std::string	_server_ip;
	std::string _server_port;
	
	HANDLE  _thread_listen;
	HANDLE  _thread_processing;
	HANDLE  _thread_display;
	HANDLE	_img_refreshed;
	HANDLE	_rst_received;
	HANDLE	_thread_result;
	int _udp_pkg_num;
	int _udp_pkg_idx;
	int _prev_timestamp;

	
	int	_iDispRotation;
	int _display_line_update;
	std::vector<std::vector<BYTE>>	_buf_image;
	std::vector<std::vector<BYTE>>	_process_image;
	char* _server_buf;
	cv::Mat _img_dis;
	int			_pre_id;
	bool		 _img_updated;
	bool		_img_processed;
	bool		_img_sent;
	unsigned int _img_cnt;
	int			_face_detected_cnt;
	//Int64		_cur_time;
	unsigned int _prev_time;
	unsigned int	_prev_stamp;
	RGBQUAD*	  _rgbDis;
	HWND          _hWndImg;
	DrawDevice*   _pDrawImg;
	ID2D1Factory* _pD2DFactory;
	Img_Prepro*	_img_preprocessing;
	
public:
	CEdit*						_edit_box;
	sockaddr_in					  _client;			// receiving data address
	int							  _img_height;
	int							  _img_width;
	int							  _img_chn;
	//std::vector<std::vector<BYTE>>_saving_Frame[2];
	bool						  _program_start;
	
	CString							_text_rst;
};


