
// yoonj_0207_Winsocket_serverDlg.h: 헤더 파일
//

#pragma once

#define MAX_CIIENT_COUNT	10
//최대 클라이언트 수 지정

// Cyoonj0207WinsocketserverDlg 대화 상자
class Cyoonj0207WinsocketserverDlg : public CDialogEx
{
// 생성입니다.
private:
	SOCKET mh_listen_socket; //소켓핸들선언
	SOCKET mh_client_list[MAX_CIIENT_COUNT]; //클라이언트를 최대 10까지 받음
	wchar_t m_client_ip[MAX_CIIENT_COUNT][24]; //ip관리 핸들
	int m_client_count;

public:
	Cyoonj0207WinsocketserverDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	void AcceptProcess(SOCKET parm_h_socket);
	void ClientCloseProcess(SOCKET parm_h_socket, char parm_force_flag);
	void ReceiveData(SOCKET parm_h_socket, char* parm_p_buffer, int parm_size);
	void AddEventString(CString parm_string);
	void SendFrameData(SOCKET parm_h_socket, unsigned char parm_id, const void* parm_p_data, int parm_size);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_YOONJ_0207_WINSOCKET_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
private:
	CListBox m_event_list;
public:
	afx_msg void OnLbnSelchangeList1();
};