// UDPChatClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UDPChatClient.h"
#include "UDPChatClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUDPChatClientDlg dialog

CUDPChatClientDlg::CUDPChatClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUDPChatClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUDPChatClientDlg)
	m_port = 6000;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUDPChatClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUDPChatClientDlg)
	DDX_Text(pDX, IDC_EDIT_SRVPORT, m_port);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUDPChatClientDlg, CDialog)
	//{{AFX_MSG_MAP(CUDPChatClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LINK, OnButtonLink)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_RECVDATA,OnRecvData)//消息处理函数，自定义的消息
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUDPChatClientDlg message handlers

BOOL CUDPChatClientDlg::OnInitDialog()
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
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_SRVIP))->SetWindowText("10.116.46.254");
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUDPChatClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUDPChatClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUDPChatClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CUDPChatClientDlg::OnButtonLink() 
{
	// TODO: Add your control notification handler code here
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_SRVIP))->EnableWindow(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_SRVPORT))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_LINK))->EnableWindow(FALSE);

	InitSocket();//初始化套接字
	sendto(m_socket,"上线",sizeof("上线")+1,0,
		(SOCKADDR*)&addrto,sizeof(SOCKADDR));//发送上线通知

	RECVPARAM *pRecvParam = new RECVPARAM;//传递线程参数的结构体
	pRecvParam->sock = m_socket;//给结构赋值
	pRecvParam->hwnd = m_hWnd;//定义的m_hWnd主要是为了发送消息时传递到主窗口
	HANDLE hThread = CreateThread(NULL,0,RecvProc,(LPVOID)pRecvParam,0,NULL);//创建recvfrom的线程，因为recvfrom一启动就会阻塞，这里调用线程，让线程阻塞，使主界面能运行下去
	CloseHandle(hThread);//关闭线程句柄

}

void CUDPChatClientDlg::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_SRVIP))->EnableWindow(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT_SRVPORT))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_BUTTON_LINK))->EnableWindow(TRUE);

	sendto(m_socket,"下线",sizeof("下线")+1,0,
		(SOCKADDR*)&addrto,sizeof(SOCKADDR));//发送下线通知
	sendto(m_socket,"close_by Eason",sizeof("close_by Eason")+1,0,
 		(SOCKADDR*)&addrto,sizeof(SOCKADDR));//发送关闭通知

}

BOOL CUDPChatClientDlg::InitSocket()//初始化套接字函数实现
{
	m_socket = socket(AF_INET,SOCK_DGRAM,0);//初始化套接字
	if (INVALID_SOCKET == m_socket)//如果初始化失败
	{
		MessageBox("创建套接字失败！");
		return FALSE;
	}
	
	UpdateData(TRUE);//获取控件变量，主要是获取端口的变量，此处用关联变量，目的是为了简单
	DWORD dwIP;
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_SRVIP))->GetAddress(dwIP);//获取控件IP
	addrto.sin_family = AF_INET;

	addrto.sin_port = htons(m_port);//m_port是控件上的变量
	addrto.sin_addr.S_un.S_addr = htonl(dwIP);
	
	return TRUE;
}

void CUDPChatClientDlg::OnButtonSend() //发送信息函数
{
	// TODO: Add your control notification handler code here
	CString sendBuf;
	GetDlgItemText(IDC_EDIT_SEND,sendBuf);
	sendto(m_socket,sendBuf,sendBuf.GetLength()+1,0,
		(SOCKADDR*)&addrto,sizeof(SOCKADDR));
	SetDlgItemText(IDC_EDIT_SEND,"");
}

DWORD WINAPI CUDPChatClientDlg::RecvProc(LPVOID lpParameter)
{
	SOCKET sock = ((RECVPARAM*)lpParameter)->sock;//参数的转换
	HWND hwnd = ((RECVPARAM*)lpParameter)->hwnd;
	delete lpParameter;//释放内存，出自孙鑫教程

	SOCKADDR_IN addFrom;//定义接收地址
	int len = sizeof(SOCKADDR);//换取长度，主要用于后面的recvfrom和sendto中
	
	char recvBuf[200];//定义接收的字符串

	while (TRUE)
	{
		recvfrom(sock,recvBuf,200,0,(SOCKADDR*)&addFrom,&len);//接受发来的信息,这里不能用break，这样会退出线程	
		::PostMessage(hwnd,WM_RECVDATA,0,(LPARAM)recvBuf);//发送消息给m_hWnd，主要是显示信息到主窗口
	}

	return 0;
}

void CUDPChatClientDlg::OnRecvData(WPARAM wParam,LPARAM lParam)//消息处理函数的实现
{
	CString str = (char*)lParam;//转换字符串
	CString tempstr;//定义临时字符串
	GetDlgItemText(IDC_EDIT_RECV,tempstr);//获取控件变量
	str += "\r\n";//为字符串加一个换行
	str += tempstr;
	SetDlgItemText(IDC_EDIT_RECV,str);//为控件写字符串
}

void CUDPChatClientDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
	sendto(m_socket,"下线",sizeof("下线")+1,0,
		(SOCKADDR*)&addrto,sizeof(SOCKADDR));//发送下线通知
 	sendto(m_socket,"close_by Eason",sizeof("close_by Eason")+1,0,
 		(SOCKADDR*)&addrto,sizeof(SOCKADDR));//发送关闭通知

}
