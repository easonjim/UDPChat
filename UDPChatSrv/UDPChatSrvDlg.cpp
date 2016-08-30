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
CLIENTDATA CUDPChatSrvDlg::m_ClientData;//����ò�������������ã��������ᱨ��Ӧ���Ǻ��߳��й�
CArray<CLIENTDATA,CLIENTDATA&> CUDPChatSrvDlg::ClientData;//ͬ��


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
	ON_MESSAGE(WM_RECVDATA,OnRecvData)//��Ϣ���������Զ������Ϣ
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
	m_socket = socket(AF_INET,SOCK_DGRAM,0);//��ʼ���׽���
	if (INVALID_SOCKET == m_socket)//�����ʼ��ʧ��
	{
		MessageBox("�����׽���ʧ�ܣ�");
		return FALSE;
	}

	UpdateData(TRUE);//��ȡ�ؼ���������Ҫ�ǻ�ȡ�˿ڵı������˴��ù���������Ŀ����Ϊ�˼�
	SOCKADDR_IN addSock;//����������ַ�ṹ
	addSock.sin_family = AF_INET;
	addSock.sin_port = htons(m_port);//m_port�ǿؼ��ϵı���
	addSock.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	int retval;
	retval = bind(m_socket,(SOCKADDR*)&addSock,sizeof(SOCKADDR));//���׽������ĸ��˿ں�IP��
	if (SOCKET_ERROR == retval)//�����ʧ��
	{
		MessageBox("�׽��ְ�ʧ�ܣ�");
		return FALSE;
	}

	return TRUE;
}

DWORD WINAPI CUDPChatSrvDlg::RecvProc(LPVOID lpParameter)//�̺߳�����ʵ��
{
	SOCKET sock = ((RECVPARAM*)lpParameter)->sock;//������ת��
	HWND hwnd = ((RECVPARAM*)lpParameter)->hwnd;
	delete lpParameter;//�ͷ��ڴ棬�������ν̳�
	
	SOCKADDR_IN addFrom;//������յ�ַ
	int len = sizeof(SOCKADDR);//��ȡ���ȣ���Ҫ���ں����recvfrom��sendto��

	char recvBuf[200];//������յ��ַ���
	char tempBuf[300];//������ʱ�ַ���
	int k=0;//�����ڵ�һ��д����̬����ʱҪ�õ��ı�����������ʵ��


	while (TRUE)//ѭ���������Կͻ�������Ϣ
	{
		BOOL Falg=FALSE;//������ӿͻ�����Ϣд����̬����ʱ�õ��ı��������ΪFALSE����ʾ��������û����ͬ�Ŀͻ�����Ϣ������Ϣ��ӽ�����
		if (SOCKET_ERROR == recvfrom(sock,recvBuf,200,0,(SOCKADDR*)&addFrom,&len))//���ܿͻ�����������Ϣ
		{
			break;//����д��������ѭ�������߳�
		}	

		for (int j=0;j<ClientData.GetSize();j++)//ѭ��ƥ����������Ŀͻ�����Ϣ�������û��ͬ�ģ��Ͳ���ӽ�ȥ����һ̨�ͻ��������������У���Ϊ��������Ϊ0�����Բ�ƥ�䣬ֱ������FORѭ��
		{
			if (addFrom.sin_port == ClientData.GetAt(j).sockadd.sin_port && 
				addFrom.sin_addr.S_un.S_addr == ClientData.GetAt(j).sockadd.sin_addr.S_un.S_addr)//���￴�ͻ�����Ϣ�Ƿ���ͬ�����õ��ͻ����Ķ˿ںţ�ÿ�����ӵĶ˿ںŶ���ͬ��IP��ַ���ò���һ���������ӣ��˿ںžͻ�̶�������Ϊ������Ӷ�̬�����ṩ׼��
			{
				Falg=TRUE;//�������ͬ�Ŀͻ�����Ϣ��FalgΪTRUE������Ͳ���ִ�а�����д����̬������
			}

		}
		if (Falg == FALSE && k!=0)//���û�пͻ�����Ϣ��ͬ���Ҳ��ǵ�һ�����ӵĿͻ���������ǵ�2̨������ʱ�Ż��õ����ǾͰ���Ϣ��ӽ���̬����
		{
			
			m_ClientData.sockadd.sin_addr.S_un.S_addr=addFrom.sin_addr.S_un.S_addr;//Ϊ�ṹ�������ֵ�������Ҳ�Ƕ����ַ�����ĸ�ֵ
			m_ClientData.sockadd.sin_family=AF_INET;
			m_ClientData.sockadd.sin_port=addFrom.sin_port;
			ClientData.Add(m_ClientData);//����Ϣд����̬����
		}
			
		if (k == 0)//��������һ�οͻ�������ʱ����������ӽ���̬����
		{
			m_ClientData.sockadd.sin_addr.S_un.S_addr=addFrom.sin_addr.S_un.S_addr;
			m_ClientData.sockadd.sin_family=AF_INET;
			m_ClientData.sockadd.sin_port=addFrom.sin_port;
			ClientData.Add(m_ClientData);//����Ϣд����̬����
			k++;//k�Լӣ���2̨�Ժ����ӵĿͻ����������������ӣ����������FORѭ������ӣ����д��FOR�����棬��������棬����Ӵ����߼�����
		}
		
		CString str;//���嵱�ͻ����˳�ʱ����һ���ַ����ı�����������Ϊ��UDP���˳�ʱû���κ���Ϣ�����ԣ����������Э�飬��ȷ���ͻ������˳�
		str.Format("%s",recvBuf);//ת���ַ���
		if (str == "close_by Eason")//���Ϊ�ͻ����˳����ַ���
		{
			for (int l=0;l<ClientData.GetSize();l++)//ѭ��������������̨���͵��˳��ַ����Ŀͻ�����Ϣ
			{
				if (addFrom.sin_port == ClientData.GetAt(l).sockadd.sin_port)//���ƥ����ô�ڶ�̬������ɾ���ͻ���Ԫ��
				{
					ClientData.RemoveAt(l);//��̬����ɾ��Ԫ��
				}
			}
		}
		else//��������˳��ĵ��ַ�����ôִ�н���Ϣ���͵������ͻ����У�������Ϣͨ����Ϣģʽ���ݵ�������Ŀؼ���
		{
			sprintf(tempBuf,"%s ˵��%s",inet_ntoa(addFrom.sin_addr),recvBuf);//��ʽ���ַ���
			for (int i=0;i<ClientData.GetSize();i++)//ѭ������
			{
				sendto(sock,tempBuf,strlen(tempBuf)+1,0,(SOCKADDR*)&(ClientData.GetAt(i).sockadd),len);//ѭ��������Ϣ�������ͻ���
			}
			
			::PostMessage(hwnd,WM_RECVDATA,0,(LPARAM)tempBuf);//������Ϣ��m_hWnd����Ҫ����ʾ��Ϣ��������
		}
	}
	return 0;//�̱߳�����һ������ֵ
}

void CUDPChatSrvDlg::OnRecvData(WPARAM wParam,LPARAM lParam)//��Ϣ��������ʵ��
{
	CString str = (char*)lParam;//ת���ַ���
	CString tempstr;//������ʱ�ַ���
	GetDlgItemText(IDC_EDIT_RECV,tempstr);//��ȡ�ؼ�����
	str += "\r\n";//Ϊ�ַ�����һ������
	str += tempstr;
	SetDlgItemText(IDC_EDIT_RECV,str);//Ϊ�ؼ�д�ַ���
}


void CUDPChatSrvDlg::OnButtonStart() 
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_BUTTON_START))->EnableWindow(FALSE);//����������ť
	((CEdit*)GetDlgItem(IDC_EDIT_PORT))->EnableWindow(FALSE);

	InitSocket();//��ʼ���׽��ֺ���
	RECVPARAM *pRecvParam = new RECVPARAM;//�����̲߳����Ľṹ��
	pRecvParam->sock = m_socket;//���ṹ��ֵ
	pRecvParam->hwnd = m_hWnd;//�����m_hWnd��Ҫ��Ϊ�˷�����Ϣʱ���ݵ�������
	HANDLE hThread = CreateThread(NULL,0,RecvProc,(LPVOID)pRecvParam,0,NULL);//����recvfrom���̣߳���Ϊrecvfromһ�����ͻ���������������̣߳����߳�������ʹ��������������ȥ
	CloseHandle(hThread);//�ر��߳̾��

	CString srvIP,srvPort;
	srvIP = GetHostIP();//��ȡ����IP
	srvPort.Format("%d",m_port);//ת���˿ںţ���Ϊֻ֧��CString���ͣ���ȡ��m_port��int��,��������Ҫת��
	((CEdit*)GetDlgItem(IDC_EDIT_RECV))->SetWindowText("������IP:"+srvIP+"  �˿�:"+srvPort+"\r\n""�����������ɹ���");//����������Ϣ
}

void CUDPChatSrvDlg::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	((CButton*)GetDlgItem(IDC_BUTTON_START))->EnableWindow(TRUE);//����������ť
	((CEdit*)GetDlgItem(IDC_EDIT_PORT))->EnableWindow(TRUE);
	closesocket(m_socket);//ȡ�����׽���

	((CEdit*)GetDlgItem(IDC_EDIT_RECV))->SetWindowText("�������ѹرգ�");//���ùر���Ϣ
}



CString CUDPChatSrvDlg::GetHostIP()//��ȡIP����
{
	char Name[255];//�������ڴ�Ż�õ��������ı��� 
	char *IP;//����IP��ַ���� 
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
