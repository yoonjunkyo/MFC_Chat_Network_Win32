
// yoonj_0207_Winsocket_serverDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "yoonj_0207_Winsocket_server.h"
#include "yoonj_0207_Winsocket_serverDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cyoonj0207WinsocketserverDlg 대화 상자



Cyoonj0207WinsocketserverDlg::Cyoonj0207WinsocketserverDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_YOONJ_0207_WINSOCKET_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_client_count = 0; //아직 접속한 클라이언트 0명
}

void Cyoonj0207WinsocketserverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_event_list);
}

BEGIN_MESSAGE_MAP(Cyoonj0207WinsocketserverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_LBN_SELCHANGE(IDC_LIST1, &Cyoonj0207WinsocketserverDlg::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// Cyoonj0207WinsocketserverDlg 메시지 처리기

BOOL Cyoonj0207WinsocketserverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//리슨소켓핸들에 소켓생성 (AF,TYPE,protocol)
	//AF_INET : IPv4 /SOCK_STREAM : 연결지향형 / 프로토콜 : TCP
	mh_listen_socket = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);
	//소켓등록 - 소켓구조체 생성

	SOCKADDR_IN srv_addr; 
	srv_addr.sin_family = AF_INET; //주소체계 - IPv4
	srv_addr.sin_addr.s_addr = inet_addr("172.30.1.89"); //ipconfig로 ip가져옴
	srv_addr.sin_port = htons(20001); //포트번호 
	//listem socket에 소켓에 대한 명세(srv_addr)를 등록
	//srv_addr 를 long pointer형태로 변환해서 등록
	bind(mh_listen_socket, (LPSOCKADDR)&srv_addr,sizeof(srv_addr)); 
	//클라이언트의 접속을 받아 줄 수 있는 기능
	listen(mh_listen_socket, 1); 

	AddEventString(L"클라이언트의 접속을 허락합니다....");
	//WSAAsyncSelect(socket,HWND,message,Event)
	//socket에서 FD_ACCEPT라는 이벤트가 발생하면 27001 message를 m_hWnd에 넘겨줌
	//m_hWnd가 받은 messgae에 따라 WindowProc 실행 
	//클라이언트의 접속을 확인하는 기능 but 클라이언트가 접속할때까지 이 함수를
	//빠져나오지 못함 -> 1.동기방식(스레드프로그래밍) 2.비동기방식 (WSAAsyncSelect)
	WSAAsyncSelect(mh_listen_socket,m_hWnd, 27001, FD_ACCEPT); 

	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	return TRUE;  
}

void Cyoonj0207WinsocketserverDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}


void Cyoonj0207WinsocketserverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR Cyoonj0207WinsocketserverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Cyoonj0207WinsocketserverDlg::AcceptProcess(SOCKET parm_h_socket)
{
	if (MAX_CIIENT_COUNT > m_client_count) {
		//복제된 소켓을 사용 다음 사용자가 listen_socket을 사용하기 위해
		struct sockaddr_in client_addr;
		//클라이언트의 주소를 받을 구조체 생성
		int sockaddr_in_size = sizeof(client_addr);

		mh_client_list[m_client_count] = accept(parm_h_socket, (SOCKADDR*)&client_addr, &sockaddr_in_size);
		//클라이언트로부터 받은 소켓을 허용, 전달받은 소켓을 소켓구조체에 주소와 사이즈로 저장
		//다음 사용자가 listen해야 하기 때문에 - 여러 사용자를 받기 위해
		WSAAsyncSelect(mh_client_list[m_client_count], m_hWnd, 27002, FD_READ | FD_CLOSE);
		
		CString ip_address;
		ip_address = inet_ntoa(client_addr.sin_addr);
		wcscpy(m_client_ip[m_client_count], ip_address);

		AddEventString(L"새로운 클라이언트 접속 : "+ ip_address);
		//MessageBox(ip_address, L"새로운 클라이언트 접속", MB_OK);
		m_client_count++;
		//h_temp_socket 변수가 없어지면 통신 불가능 -> 헤더에서 맴버변수로 변환
	}
	else {

	}
}

void Cyoonj0207WinsocketserverDlg::AddEventString(CString parm_string)
{
	int index = m_event_list.InsertString(-1, parm_string);
	m_event_list.SetCurSel(index);
}

void Cyoonj0207WinsocketserverDlg::ClientCloseProcess(SOCKET parm_h_socket,char parm_force_flag)
{
	if (parm_force_flag == 1) {
		LINGER temp_linger = { TRUE,0 };
		setsockopt(parm_h_socket, SOL_SOCKET, SO_LINGER, (char*)&temp_linger, sizeof(temp_linger));
	}
	closesocket(parm_h_socket);

	for (int i = 0; i < m_client_count; i++) {
		if (mh_client_list[i] == parm_h_socket) {
			m_client_count--;
			if (i != m_client_count) {
				mh_client_list[i] = mh_client_list[m_client_count];
				wcscpy(m_client_ip[i], m_client_ip[m_client_count]);
			}
		}
	}
}

void Cyoonj0207WinsocketserverDlg::ReceiveData(SOCKET parm_h_socket,char *parm_p_buffer,int parm_size) {
	int current_size, total_size = 0, retry_count = 0;
	
	while (total_size < parm_size) {
		current_size = recv(parm_h_socket, parm_p_buffer + total_size, parm_size - total_size, 0);

		if (current_size == SOCKET_ERROR) {
			retry_count++;
			Sleep(50);
			if (retry_count > 5) {
				break;
			}
			else {
				retry_count = 0;
				total_size = total_size + current_size;
			}
		}
		else {
			total_size = total_size + current_size;
		}
	}
}

void Cyoonj0207WinsocketserverDlg::SendFrameData(SOCKET parm_h_socket, unsigned char parm_id, const void* parm_p_data, int parm_size)
{
	char* p_send_data = new char[parm_size + 4];

	*p_send_data = 27; //key int형 4bit
	*(unsigned short int*)(p_send_data + 1) = parm_size; 
	*(p_send_data + 3) = parm_id; //network_id

	memcpy(p_send_data + 4, parm_p_data, parm_size);

	send(parm_h_socket, p_send_data, parm_size + 4, 0);

	delete[] p_send_data;
}

LRESULT Cyoonj0207WinsocketserverDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
//FD_ACCEPT 가 발생하면 27001이 넘어옴
if (27001 == message) {
	AcceptProcess((SOCKET)wParam);
	//(SOCKET)wParam 으로 소켓 전달
}
else if (27002 == message) { //FD_READ | FD_CLOSE 
	SOCKET h_socket = (SOCKET)wParam;
	//(SOCKET)wParam을 복사해서 사용
	if (WSAGETSELECTEVENT(lParam) == FD_READ) {
		WSAAsyncSelect(h_socket, m_hWnd, 27002, FD_CLOSE);
		char key, network_message_id;
		recv(h_socket, &key, 1, 0); //구별자 들어온 데이터를 처리해도되는지 구별
		//h_socket에 받은 데이터를 key에 저장(주소로 전달)

		if (key == 27) {
			int current_size, total_size = 0, retry_count = 0;
			unsigned short int body_size;
			recv(h_socket, (char*)&body_size, sizeof(body_size), 0);
			recv(h_socket, &network_message_id, 1, 0);

			if (body_size > 0) {
				char* p_body_data = new char[body_size];

				ReceiveData(h_socket, p_body_data, body_size);

				if (network_message_id == 1) {
					// 실제로 클라이언트가 보내준 데이터를 처리(p_body_data안에 있음)
					CString temp;
					temp.Format(L"%s", p_body_data);
					CString str;
					for (int i = 0; i < m_client_count; i++) {
						if (h_socket == mh_client_list[i]) {
							str.Format(L"%s : %s", m_client_ip[i], p_body_data);
							break;
						}
					}
					AddEventString(str);
					//모든 클라이언트한테 다시 전송하는 기능
					for (int i = 0; i < m_client_count; i++) {
						//SendFrameData(mh_client_list[i], 1, p_body_data, body_size);
						SendFrameData(mh_client_list[i], 1, (const wchar_t*)str, ((str.GetLength() + 1) * 2));
						//채팅데이터타입 1번,클라이언트로부터 받은 데이터,size
					}
				}
				else if (network_message_id == 2) {
					//바둑판 좌표를 받을 경우
					CString temp;
					temp.Format(L"%s", p_body_data);

					if (temp.Find('@') != -1) {
						CString str_omok;
						str_omok.Format(L"%s", p_body_data);
						AddEventString(temp);
						for (int i = 0; i < m_client_count; i++) {
							SendFrameData(mh_client_list[i], 2, (const wchar_t*)str_omok, ((str_omok.GetLength() + 1) * 2));
						}
					}
				}
				delete[] p_body_data;
			}
			WSAAsyncSelect(h_socket, m_hWnd, 27002, FD_CLOSE | FD_READ);
		}
		else { // FD_CLOSE
			ClientCloseProcess(h_socket, 1); //클라이언트가 데이터를 줘도 자르겠다
		}
	}

	else {
		ClientCloseProcess(h_socket, 0);
		AddEventString(L"클라이언트가 접속을 해제했습니다.");
	}
}

return CDialogEx::WindowProc(message, wParam, lParam);
}

void Cyoonj0207WinsocketserverDlg::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
