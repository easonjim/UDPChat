// UDPChatSrvDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UDPChatSrv.h"
#include "UDPChatSrvDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
CLIENTDATA CUDPChatSrvDlg::m_ClientData;//这里貌似是声明的作用，不声明会报错，应该是和线程有关
CArray<CLIENTDATA,CLIENTDATA&> CUDPChatSrvDlg::ClientData;//同上


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
// CUDPChatSrvDlg dialog

CUDPChatSrvDlg::CUDPChatSrvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUDPChatSrvDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUDPChatSrvDlg)
	m_port = 6000;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUDPChatSrvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUDPChatSrvDlg)
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUDPChatSrvDlg, CDialog)
	//{{AFX_MSG_MAP(CUDPChatSrvDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_RECVDATA,OnRecvData)//消息处理函数，自定义的消息
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUDPChatSrvDlg message handlers

BOOL CUDPChatSrvDlg::OnInitDialog()
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUDPChatSrvDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUDPChatSrvDlg::OnPaint() 
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
HCURSOR CUDPChatSrvDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CUDPChatSrvDlg::InitSocket()
{
	m_socket = socket(AF_INET,SOCK_DGRAM,0);//初始化套接字
	if (INVALID_SOCKET == m_socket)//如果初始化失败
	{
		MessageBox("创建套接字失败！");
		return FALSE;
	}

	UpdateData(TRUE);//获取控件变量，主要是获取端口的变量，此处用关联变量，目的是为了简单
	SOCKADDR_IN addSock;//定义主机地址结构
	addSock.sin_family = AF_INET;
	addSock.sin_port = htons(m_port);//m_port是控件上的变量
	addSock.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	int retval;
	retval = bind(m_socket,(SOCKADDR*)&addSock,sizeof(SOCKADDR));//绑定套接字在哪个端口和IP上
	if (SOCKET_ERROR == retval)//如果绑定失败
	{
		MessageBox("套接字绑定失败！");
		return FALSE;
	}

	return TRUE;
}

DWORD WINAPI CUDPChatSrvDlg::RecvProc(LPVOID lpParameter)//线程函数的实现
{
	SOCKET sock = ((RECVPARAM*)lpParameter)->sock;//参数的转换
	HWND hwnd = ((RECVPARAM*)lpParameter)->hwnd;
	delete lpParameter;//释放内存，出自孙鑫教程
	
	SOCKADDR_IN addFrom;//定义接收地址
	int len = sizeof(SOCKADDR);//换取长度，主要用于后面的recvfrom和sendto中

	char recvBuf[200];//定义接收的字符串
	char tempBuf[300];//定义临时字符串
	int k=0;//定义在第一次写进动态数组时要用到的变量，下面是实现


	while (TRUE)//循环接受来自客户机的信息
	{
		BOOL Falg=FALSE;//定义添加客户机信息写进动态数组时用到的变量，如果为FALSE，表示在数组中没有相同的客户机信息，把信息添加进数组
		if (SOCKET_ERROR == recvfrom(sock,recvBuf,200,0,(SOCKADDR*)&addFrom,&len))//接受客户机发来的信息
		{
			break;//如果有错误就跳出循环结束线程
		}	

		for (int j=0;j<ClientData.GetSize();j++)//循环匹配数组里面的客户机信息，如果有没相同的，就不添加进去，第一台客户机不会在这运行，因为本来就是为0，所以不匹配，直接跳出FOR循环
		{
			if (addFrom.sin_port == ClientData.GetAt(j).sockadd.sin_port && 
				addFrom.sin_addr.S_un.S_addr == ClientData.GetAt(j).sockadd.sin_addr.S_un.S_addr)//这里看客户机信息是否相同，是用到客户机的端口号，每次连接的端口号都不同，IP地址作用不大，一但建立连接，端口号就会固定下来，为下面添加动态数组提供准备
			{
				Falg=TRUE;//如果有相同的客户机信息，Falg为TRUE，下面就不会执行把数据写进动态数组中
			}

		}
		if (Falg == FALSE && k!=0)//如果没有客户机信息相同，且不是第一次连接的客户机，这个是第2台机连接时才会用到，那就把信息添加进动态数组
		{
			
			m_ClientData.sockadd.sin_addr.S_un.S_addr=addFrom.sin_addr.S_un.S_addr;//为结构体变量赋值，具体的也是定义地址那样的赋值
			m_ClientData.sockadd.sin_family=AF_INET;
			m_ClientData.sockadd.sin_port=addFrom.sin_port;
			ClientData.Add(m_ClientData);//将信息写进动态数组
		}
			
		if (k == 0)//这个定义第一次客户机连接时，把数据添加进动态数组
		{
			m_ClientData.sockadd.sin_addr.S_un.S_addr=addFrom.sin_addr.S_un.S_addr;
			m_ClientData.sockadd.sin_family=AF_INET;
			m_ClientData.sockadd.sin_port=addFrom.sin_port;
			ClientData.Add(m_ClientData);//将信息写进动态数组
			k++;//k自加，第2台以后连接的客户机将不用这个来添加，将用上面的FOR循环来添加，这个写在FOR的下面，如果在上面，会添加错误，逻辑问题
		}
		
		CString str;//定义当客户机退出时发送一个字符串的变量，这里因为是UDP，退出时没有任何消息，所以，定义了这个协议，来确定客户机的退出
		str.Format("%s",recvBuf);//转换字符串
		if (str == "close_by Eason")//如果为客户机退出的字符串
		{
			for (int l=0;l<ClientData.GetSize();l++)//循环在数组中找这台发送的退出字符串的客户机信息
			{
				if (addFrom.sin_port == ClientData.GetAt(l).sockadd.sin_port)//如果匹配那么在动态数组中删除客户机元素
				{
					ClientData.RemoveAt(l);//动态数组删除元素
				}
			}
		}
		else//如果不是退出的的字符串那么执行将信息发送到各个客户机中，并将信息通过消息模式传递到主界面的控件中
		{
			sprintf(tempBuf,"%s 说：%s",inet_ntoa(addFrom.sin_addr),recvBuf);//格式化字符串
			for (int i=0;i<ClientData.GetSize();i++)//循环数组
			{
				sendto(sock,tempBuf,strlen(tempBuf)+1,0,(SOCKADDR*)&(ClientData.GetAt(i).sockadd),len);//循环发送信息给各个客户机
			}
			
			::PostMessage(hwnd,WM_RECVDATA,0,(LPARAM)tempBuf);//发送消息给m_hWnd，主要是显示信息到主窗口
		}
	}
	return 0;//线程必须有一个返回值
}

void CUDPChatSrvDlg::OnRecvData(WPARAM wParam,LPARAM lParam)//消息处理函数的实现
{
	CString str = (char*)lParam;//转换字符串
	CString tempstr;//定义临时字符串
	GetDlgItemText(IDC_EDIT_RECV,tempstr);//获取控件变量
	str += "\r\n";//为字符串加一个换行
	str += tempstr;
	SetDlgItemText(IDC_EDIT_RECV,str);//为控件写字符串
}


void CUDPChatSrvDlg::OnButtonStart() 
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_BUTTON_START))->EnableWindow(FALSE);//禁用启动按钮
	((CEdit*)GetDlgItem(IDC_EDIT_PORT))->EnableWindow(FALSE);

	InitSocket();//初始化套接字函数
	RECVPARAM *pRecvParam = new RECVPARAM;//传递线程参数的结构体
	pRecvParam->sock = m_socket;//给结构赋值
	pRecvParam->hwnd = m_hWnd;//定义的m_hWnd主要是为了发送消息时传递到主窗口
	HANDLE hThread = CreateThread(NULL,0,RecvProc,(LPVOID)pRecvParam,0,NULL);//创建recvfrom的线程，因为recvfrom一启动就会阻塞，这里调用线程，让线程阻塞，使主界面能运行下去
	CloseHandle(hThread);//关闭线程句柄

	CString srvIP,srvPort;
	srvIP = GetHostIP();//获取本机IP
	srvPort.Format("%d",m_port);//转换端口号，因为只支持CString类型，获取的m_port是int型,所以这里要转换
	((CEdit*)GetDlgItem(IDC_EDIT_RECV))->SetWindowText("服务器IP:"+srvIP+"  端口:"+srvPort+"\r\n""服务器启动成功！");//设置启动信息
}

void CUDPChatSrvDlg::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_BUTTON_START))->EnableWindow(TRUE);//启用启动按钮
	((CEdit*)GetDlgItem(IDC_EDIT_PORT))->EnableWindow(TRUE);
	closesocket(m_socket);//取消绑定套接字

	((CEdit*)GetDlgItem(IDC_EDIT_RECV))->SetWindowText("服务器已关闭！");//设置关闭信息
}



CString CUDPChatSrvDlg::GetHostIP()//获取IP函数
{
	char Name[255];//定义用于存放获得的主机名的变量 
	char *IP;//定义IP地址变量 
	CString strIP="";
	LPHOSTENT hostinfo;
	if(gethostname (Name, sizeof(Name)) == 0)
	{ 
		if((hostinfo = gethostbyname(Name)) != NULL) 
		{ 
			IP = inet_ntoa (*(struct in_addr*)*hostinfo->h_addr_list);
			strIP.Format(IP);
		}
	}
	return strIP;
}
