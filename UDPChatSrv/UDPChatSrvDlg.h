// UDPChatSrvDlg.h : header file
//

#if !defined(AFX_UDPCHATSRVDLG_H__7432E0D5_C6CF_4703_BB6D_1C7785DFD4FD__INCLUDED_)
#define AFX_UDPCHATSRVDLG_H__7432E0D5_C6CF_4703_BB6D_1C7785DFD4FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_RECVDATA WM_USER+1
/////////////////////////////////////////////////////////////////////////////
// CUDPChatSrvDlg dialog
struct RECVPARAM//定义用来传递参数给线程的结构体
{
	SOCKET sock;//套接字
	HWND hwnd;//用于接受主窗口的句柄
};

struct CLIENTDATA//定义用来储存连接的客户机的地址的结构体，主要用于动态数组CArray
{
	SOCKADDR_IN sockadd;//定义地址变量，其中有很多变量要赋值
};

class CUDPChatSrvDlg : public CDialog
{
// Construction
public:
	CString GetHostIP();
	static DWORD WINAPI RecvProc(LPVOID lpParameter );//声明线程函数，这里必须用static，因为该线程函数是这个类的成员函数，全局不用static，出自孙鑫教程
	BOOL InitSocket();//声明初始化套接字函数
	CUDPChatSrvDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CUDPChatSrvDlg)
	enum { IDD = IDD_UDPCHATSRV_DIALOG };
	int		m_port;//此变量是关联控件变量
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUDPChatSrvDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CUDPChatSrvDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonCancel();
	//}}AFX_MSG
	afx_msg void OnRecvData(WPARAM wParam,LPARAM lParam);//声明消息处理函数，主要用于接受到的信息显示在主窗口中
	DECLARE_MESSAGE_MAP()
private:
	SOCKET m_socket;//定义此类的全局套接字
	static CArray<CLIENTDATA,CLIENTDATA&> ClientData;//定义动态数组，因为是线程操作，这里用了static
	static CLIENTDATA m_ClientData;//定义用来储存连接的客户机的地址的结构体对象，主要用于动态数组CArray
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UDPCHATSRVDLG_H__7432E0D5_C6CF_4703_BB6D_1C7785DFD4FD__INCLUDED_)
