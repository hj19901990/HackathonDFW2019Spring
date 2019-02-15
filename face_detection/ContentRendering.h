#pragma once
#include "app\AppConfig.h"
#include "DrawDevice.h"
#include "Com.h"

#define WM_SAVE			WM_USER+1
#define IMG_WID			640
#define IMG_HGT			480
#define IMG_CHN			3
#define UDP_LINES		24
using namespace System;
using namespace System::IO;
using namespace System::Xml;
using namespace System::Text;
using namespace System::Collections::Generic;			// container support
using namespace System::Drawing;
using namespace System::Drawing::Imaging;
using namespace System::Windows::Forms;
using namespace System::Globalization;

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
	ContentRendering(HWND main_form, PictureBox^  stream, app::AppConfig^ sys_configuration);
	~ContentRendering();
	void ScanInit(app::AppConfig^ sys_config, PictureBox^  stream);
	void ScanReconfig(app::AppConfig^ sys_config, PictureBox^  stream);
	void ApplyConfigChange(app::AppConfig^ sys_config, PictureBox^  stream);
	void BindMCU();
	void SysInitialization();
	//void SaveToImage(Bitmap^ saveBmp);
	void CaptureImage();
	void StopCapturing();
	void ResizeImg(PictureBox^  stream, Panel^ frame, bool orginal_img);
	void DrawBox(System::Windows::Forms::PaintEventArgs^  e);

	bool CreateThreadFun();
	bool AssignDrawWindow(HWND hWndImg);

private:
	bool ApplyVideoRequest();
	void copyToBuffer(std::vector<BYTE>& src);
private:
	static DWORD WINAPI ListenThread(LPVOID pParam);
	//static DWORD WINAPI DataPreProcessingThread(LPVOID pParam);
	static DWORD WINAPI DisplayThread(LPVOID pParam);
	DWORD WINAPI ReceiveData_RaspPi();
	//void ShowMessage(UINT nMsgID, UINT nType);
	void UpdataScreen();
	const RGBQUAD* GetCapturedBufferRGB();

private:
	UDP_Income_Data*_video_request;
	HWND          _hWndMain;
	CComm*	_etherComm;
	
	const char* _host;
	int _port;
	
	HANDLE  _thread_listen;
	HANDLE  _thread_processing;
	HANDLE  _thread_display;
	HANDLE	_img_refreshed;
	int _udp_pkg_num;
	int _udp_pkg_idx;
	

	
	int	_iDispRotation;
	int _display_line_update;
	std::vector<std::vector<BYTE>>	_buf_image;
	std::vector<std::vector<BYTE>>	_process_image;

	bool		 _img_updated;
	unsigned int _img_cnt;
	Int64		_cur_time;
	unsigned int _prev_time;
	unsigned int	_prev_stamp;
	RGBQUAD*	  _rgbDis;
	HWND          _hWndImg;
	DrawDevice*   _pDrawImg;
	ID2D1Factory* _pD2DFactory;
	
public:
	sockaddr_in					  _client;			// receiving data address
	int							  _img_height;
	int							  _img_width;
	int							  _img_chn;
	//std::vector<std::vector<BYTE>>_saving_Frame[2];
	bool						  _program_start;
};


