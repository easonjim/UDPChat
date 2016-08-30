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
	ON_MESSAGE(WM_RECVDATA,OnRecvData)//��Ϣ���������Զ������Ϣ
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

	InitSocket();//��ʼ���׽���
	sendto(m_socket,"����",sizeof("����")+1,0,
		(SOCKADDR*)&addrto,sizeof(SOCKADDR));//��������֪ͨ

	RECVPARAM *pRecvParam = new RECVPARAM;//�����̲߳����Ľṹ��
	pRecvParam->sock = m_socket;//���ṹ��ֵ
	pRecvParam->hwnd = m_hWnd;//�����m_hWnd��Ҫ��Ϊ�˷�����Ϣʱ���ݵ�������
	HANDLE hThread = CreateThread(NULL,0,RecvProc,(LPVOID)pRecvParam,0,NULL);//����recvfrom���̣߳���Ϊrecvfromһ�����ͻ���������������̣߳����߳�������ʹ��������������ȥ
	CloseHandle(hThread);//�ر��߳̾��

}

void CUDPChatClientDlg::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_SRVIP))->EnableWindow(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT_SRVPORT))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_BUTTON_LINK))->EnableWindow(TRUE);

	sendto(m_socket,"����",sizeof("����")+1,0,
		(SOCKADDR*)&addrto,sizeof(SOCKADDR));//��������֪ͨ
	sendto(m_socket,"close_by Eason",sizeof("close_by Eason")+1,0,
 		(SOCKADDR*)&addrto,sizeof(SOCKADDR));//���͹ر�֪ͨ

}

BOOL CUDPChatClientDlg::InitSocket()//��ʼ���׽��ֺ���ʵ��
{
	m_socket = socket(AF_INET,SOCK_DGRAM,0);//��ʼ���׽���
	if (INVALID_SOCKET == m_socket)//�����ʼ��ʧ��
	{
		MessageBox("�����׽���ʧ�ܣ�");
		return FALSE;
	}
	
	UpdateData(TRUE);//��ȡ�ؼ���������Ҫ�ǻ�ȡ�˿ڵı������˴��ù���������Ŀ����Ϊ�˼�
	DWORD dwIP;
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_SRVIP))->GetAddress(dwIP);//��ȡ�ؼ�IP
	addrto.sin_family = AF_INET;

	addrto.sin_port = htons(m_port);//m_port�ǿؼ��ϵı���
	addrto.sin_addr.S_un.S_addr = htonl(dwIP);
	
	return TRUE;
}

void CUDPChatClientDlg::OnButtonSend() //������Ϣ����
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
	SOCKET sock = ((RECVPARAM*)lpParameter)->sock;//������ת��
	HWND hwnd = ((RECVPARAM*)lpParameter)->hwnd;
	delete lpParameter;//�ͷ��ڴ棬�������ν̳�

	SOCKADDR_IN addFrom;//������յ�ַ
	int len = sizeof(SOCKADDR);//��ȡ���ȣ���Ҫ���ں����recvfrom��sendto��
	
	char recvBuf[200];//������յ��ַ���

	while (TRUE)
	{
		recvfrom(sock,recvBuf,200,0,(SOCKADDR*)&addFrom,&len);//���ܷ�������Ϣ,���ﲻ����break���������˳��߳�	
		::PostMessage(hwnd,WM_RECVDATA,0,(LPARAM)recvBuf);//������Ϣ��m_hWnd����Ҫ����ʾ��Ϣ��������
	}

	return 0;
}

void CUDPChatClientDlg::OnRecvData(WPARAM wParam,LPARAM lParam)//��Ϣ��������ʵ��
{
	CString str = (char*)lParam;//ת���ַ���
	CString tempstr;//������ʱ�ַ���
	GetDlgItemText(IDC_EDIT_RECV,tempstr);//��ȡ�ؼ�����
	str += "\r\n";//Ϊ�ַ�����һ������
	str += tempstr;
	SetDlgItemText(IDC_EDIT_RECV,str);//Ϊ�ؼ�д�ַ���
}

void CUDPChatClientDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
	sendto(m_socket,"����",sizeof("����")+1,0,
		(SOCKADDR*)&addrto,sizeof(SOCKADDR));//��������֪ͨ
 	sendto(m_socket,"close_by Eason",sizeof("close_by Eason")+1,0,
 		(SOCKADDR*)&addrto,sizeof(SOCKADDR));//���͹ر�֪ͨ

}
