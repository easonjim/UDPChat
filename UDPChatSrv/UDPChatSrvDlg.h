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
struct RECVPARAM//�����������ݲ������̵߳Ľṹ��
{
	SOCKET sock;//�׽���
	HWND hwnd;//���ڽ��������ڵľ��
};

struct CLIENTDATA//���������������ӵĿͻ����ĵ�ַ�Ľṹ�壬��Ҫ���ڶ�̬����CArray
{
	SOCKADDR_IN sockadd;//�����ַ�����������кܶ����Ҫ��ֵ
};

class CUDPChatSrvDlg : public CDialog
{
// Construction
public:
	CString GetHostIP();
	static DWORD WINAPI RecvProc(LPVOID lpParameter );//�����̺߳��������������static����Ϊ���̺߳����������ĳ�Ա������ȫ�ֲ���static���������ν̳�
	BOOL InitSocket();//������ʼ���׽��ֺ���
	CUDPChatSrvDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CUDPChatSrvDlg)
	enum { IDD = IDD_UDPCHATSRV_DIALOG };
	int		m_port;//�˱����ǹ����ؼ�����
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
	afx_msg void OnRecvData(WPARAM wParam,LPARAM lParam);//������Ϣ����������Ҫ���ڽ��ܵ�����Ϣ��ʾ����������
	DECLARE_MESSAGE_MAP()
private:
	SOCKET m_socket;//��������ȫ���׽���
	static CArray<CLIENTDATA,CLIENTDATA&> ClientData;//���嶯̬���飬��Ϊ���̲߳�������������static
	static CLIENTDATA m_ClientData;//���������������ӵĿͻ����ĵ�ַ�Ľṹ�������Ҫ���ڶ�̬����CArray
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UDPCHATSRVDLG_H__7432E0D5_C6CF_4703_BB6D_1C7785DFD4FD__INCLUDED_)
