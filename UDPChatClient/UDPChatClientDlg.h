// UDPChatClientDlg.h : header file
//

#if !defined(AFX_UDPCHATCLIENTDLG_H__17269251_568A_4921_8F7F_170A5AE23714__INCLUDED_)
#define AFX_UDPCHATCLIENTDLG_H__17269251_568A_4921_8F7F_170A5AE23714__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CUDPChatClientDlg dialog
#define WM_RECVDATA WM_USER+1
struct RECVPARAM//�����������ݲ������̵߳Ľṹ��
{
	SOCKET sock;//�׽���
	HWND hwnd;//���ڽ��������ڵľ��
};

class CUDPChatClientDlg : public CDialog
{
// Construction
public:
	static DWORD WINAPI RecvProc(LPVOID lpParameter);
	BOOL InitSocket();
	CUDPChatClientDlg(CWnd* pParent = NULL);	// standard constructor
	SOCKADDR_IN addrto;//����������ַ�ṹ

// Dialog Data
	//{{AFX_DATA(CUDPChatClientDlg)
	enum { IDD = IDD_UDPCHATCLIENT_DIALOG };
	int		m_port;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUDPChatClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CUDPChatClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonLink();
	afx_msg void OnButtonCancel();
	afx_msg void OnButtonSend();
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg void OnRecvData(WPARAM wParam,LPARAM lParam);//������Ϣ����������Ҫ���ڽ��ܵ�����Ϣ��ʾ����������
	DECLARE_MESSAGE_MAP()
private:
	SOCKET m_socket;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UDPCHATCLIENTDLG_H__17269251_568A_4921_8F7F_170A5AE23714__INCLUDED_)
