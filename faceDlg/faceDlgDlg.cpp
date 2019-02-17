// faceDlgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "faceDlg.h"
#include "faceDlgDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CfaceDlgDlg dialog




CfaceDlgDlg::CfaceDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CfaceDlgDlg::IDD, pParent)
	//cloudClient("192.168.111.101", "4211")
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//faceImage = cvCreateImage(cvSize(75,75),8,1);
	//faceDetectThread = new ProcessThread(1,this,faceImage);
	
}

void CfaceDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CfaceDlgDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_Enroll, &CfaceDlgDlg::OnBnClickedEnroll)
	ON_BN_CLICKED(IDC_Match, &CfaceDlgDlg::OnBnClickedMatch)
	ON_BN_CLICKED(IDC_Capture, &CfaceDlgDlg::OnBnClickedCapture)

END_MESSAGE_MAP()


// CfaceDlgDlg message handlers

BOOL CfaceDlgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	HWND	m_hWnd = GetDlgItem(IDC_IMG_RENDER)->GetSafeHwnd();
	_content = new ContentRendering(m_hWnd);
	_content->_edit_box = (CEdit*)GetDlgItem(IDC_EDIT_PERSON);
	CFont *myFont = new CFont();
	myFont->CreateFont(40, 0, 0, 0, FW_HEAVY, true, false,
		0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FIXED_PITCH | FF_MODERN, _T("Courier New"));

	_content->_edit_box->SetFont(myFont);
	_content->_edit_box->SetWindowText("TEST\r\n\r\nTEST");
	_content->_edit_box->Invalidate(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CfaceDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CfaceDlgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
		
			
			


	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CfaceDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CfaceDlgDlg::OnBnClickedEnroll()
{
	// TODO: Add your control notification handler code here

	
}

void CfaceDlgDlg::OnBnClickedMatch()
{
	// TODO: Add your control notification handler code here

	
}

void CfaceDlgDlg::OnBnClickedCapture()
{
	_content->CaptureImage();
}




