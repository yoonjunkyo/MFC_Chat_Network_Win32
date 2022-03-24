
// yoonj_0207_Winsocket_clientDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "yoonj_0207_Winsocket_client.h"
#include "yoonj_0207_Winsocket_clientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define X_COUNT     19 //가로, 세로 줄이 19개인 바둑판
#define Y_COUNT     19

#define START_X     50 //가로여백
#define START_Y     50 //세로여백

#define INTERVAL    26 //줄사이 간격26, 돌의 지름과 동일
#define HALF_INTERVAL INTERVAL/2

#define XPOS(x)     (START_X+(x)*INTERVAL) //돌이 놓일 수 있는 위치
#define YPOS(y)     (START_Y+(y)*INTERVAL)

unsigned int g_dol[Y_COUNT][X_COUNT];
unsigned int g_step;

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

// Cyoonj0207WinsocketclientDlg 대화 상자

Cyoonj0207WinsocketclientDlg::Cyoonj0207WinsocketclientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_YOONJ_0207_WINSOCKET_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


}

void Cyoonj0207WinsocketclientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_event_list);
}

BEGIN_MESSAGE_MAP(Cyoonj0207WinsocketclientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEND_BTN, &Cyoonj0207WinsocketclientDlg::OnBnClickedSendBtn)
	ON_BN_CLICKED(IDOK, &Cyoonj0207WinsocketclientDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// Cyoonj0207WinsocketclientDlg 메시지 처리기

void Cyoonj0207WinsocketclientDlg::AddEventString(CString parm_string)//자신의 listbox에 message출력
{
	int index = m_event_list.InsertString(-1, parm_string);
	m_event_list.SetCurSel(index);//마지막에 추가된 항목으로 커서가 설정
	//클라이언트가 자신의 상태를 출력하고자 할 때
}

BOOL Cyoonj0207WinsocketclientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		

	mh_socket = socket(AF_INET, SOCK_STREAM, 0); //소켓생성
	sockaddr_in srv_addr;  //소켓에 주소정보를 등록
	//SOCKADDR_IN srv_addr;
	srv_addr.sin_family = AF_INET; //IPv4 주소체계
	srv_addr.sin_addr.s_addr = inet_addr("172.30.1.89"); //서버의 IP주소
	srv_addr.sin_port = htons(20001); //port

	WSAAsyncSelect(mh_socket, m_hWnd, 27001, FD_CONNECT);//connect가 성공하면 윈도우핸들에 27001번 메시지가 발생 -> WinProc에서 처리
	connect(mh_socket, (sockaddr*)&srv_addr, sizeof(srv_addr)); //접속시도
	//bind함수를 호출 하지 않았을 때 connect함수호출 시 소켓에 자동으로 IP와 포트번호가 할당 ***
	//서버가 정상적이라면 connect실행하지만 아니라면 connect함수에서 최대 28초동안 대기함

	return TRUE;  
}

void Cyoonj0207WinsocketclientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void Cyoonj0207WinsocketclientDlg::OnPaint()
{
	PAINTSTRUCT ps;
	HDC h_dc = ::BeginPaint(m_hWnd, &ps);
	HBRUSH h_bk_brush = CreateSolidBrush(RGB(244, 176, 27));
	HGDIOBJ h_old_brush = SelectObject(h_dc, h_bk_brush);
	Rectangle(h_dc, 40, 40, 530, 530);

	SelectObject(h_dc, GetStockObject(BLACK_PEN));
	for (int i = 0; i < X_COUNT; i++) {
		MoveToEx(h_dc, XPOS(i), YPOS(0), NULL);
		LineTo(h_dc, XPOS(i), YPOS(Y_COUNT - 1));
	}
	for (int i = 0; i < Y_COUNT; i++) {
		MoveToEx(h_dc, XPOS(0), YPOS(i), NULL);
		LineTo(h_dc, XPOS(X_COUNT - 1), YPOS(i));
	}
	for (int y = 0; y < Y_COUNT; y++) {
		for (int x = 0; x < X_COUNT; x++) {
			if (g_dol[y][x] > 0) {
				if (g_dol[y][x] == 1) SelectObject(h_dc, GetStockObject(BLACK_BRUSH));
				else if (g_dol[y][x] == 2) SelectObject(h_dc, GetStockObject(WHITE_BRUSH));

				Ellipse(h_dc, XPOS(x) - HALF_INTERVAL, YPOS(y) - HALF_INTERVAL, XPOS(x) + HALF_INTERVAL, YPOS(y) + HALF_INTERVAL);
			}
		}
	}
	::EndPaint(m_hWnd, &ps);

}
void Cyoonj0207WinsocketclientDlg::OnLButtonDown(HWND ah_wnd, int a_x, int a_y) {
	if (a_x > (XPOS(0) - HALF_INTERVAL) && a_y > (YPOS(0) - HALF_INTERVAL)
		&& a_x < (XPOS(X_COUNT - 1) + HALF_INTERVAL)
		&& a_y < (YPOS(Y_COUNT - 1) + HALF_INTERVAL)) {
		int x = (a_x - START_X + HALF_INTERVAL) / INTERVAL;
		int y = (a_y - START_Y + HALF_INTERVAL) / INTERVAL;
		if (g_dol[y][x] == 0) {
			g_dol[y][x] = g_step + 1;
			g_step = !g_step;
			// 화면을 갱신해서 돌 놓은것을 적용
			::InvalidateRect(m_hWnd, NULL, TRUE);
		}
	}
}
// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR Cyoonj0207WinsocketclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Cyoonj0207WinsocketclientDlg::ConnectProcess(LPARAM lParam)//error만 넘어오니까 lParam
{
	if (WSAGETSELECTERROR(lParam) == 0) { //에러가 없다
		//서버에서 데이터를 주는지 혹은 끊었는지 확인 비동기 바꿔줌
		WSAAsyncSelect(mh_socket, m_hWnd, 27002, FD_READ | FD_CLOSE);
		AddEventString(L"서버에 접속했습니다.");
	}
	else {//실패
		closesocket(mh_socket);
		mh_socket = INVALID_SOCKET;
		AddEventString(L"서버에 접속할 수 없습니다.");
	}
}
void Cyoonj0207WinsocketclientDlg::DestroySocket()
{
	LINGER temp_linger = { TRUE,0 };
	setsockopt(mh_socket, SOL_SOCKET, SO_LINGER, (char*)&temp_linger, sizeof(temp_linger));
	closesocket(mh_socket);
	mh_socket = INVALID_SOCKET;
}
void Cyoonj0207WinsocketclientDlg::ReceiveData(SOCKET parm_h_socket, char* parm_p_buffer, int parm_size) {
	int current_size, total_size = 0, retry_count = 0;

	while (total_size < parm_size) {
		current_size = recv(parm_h_socket, parm_p_buffer + total_size, parm_size - total_size,0); 

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
void Cyoonj0207WinsocketclientDlg::ReadFrameData()
{
	char key, network_message_id;
	recv(mh_socket, &key, 1, 0);
	if (key == 27) {
		unsigned short int body_size;
		recv(mh_socket, (char*)&body_size, 2, 0); //char -> 8bit
		recv(mh_socket, &network_message_id, 1, 0);
		if (body_size > 0) {
			char* p_body_data = new char[body_size];

			ReceiveData(mh_socket, p_body_data, body_size);  //받은 데이터 크기가 2바이트가 될 때까지 받음

			if (network_message_id == 1) {
				CString temp;
				temp.Format(L"%s", p_body_data);
				AddEventString((wchar_t*)p_body_data);

			}
			else if (network_message_id == 2) {
				CString temp;
				temp.Format(L"%s", p_body_data);
				if (temp.Find('@') != -1) {
					CString str_omok;
					str_omok.Format(L"%s", p_body_data);
					//AddEventString(str_omok); // 지울것
					int index = str_omok.Find('@');
					CString x = str_omok.Left(index);
					CString y = str_omok.Right(str_omok.GetLength() - index - 1);
					int dx = _ttoi(x);
					int dy = _ttoi(y);
					OnLButtonDown(m_hWnd, dx, dy);
				}
			}
			delete[] p_body_data;
		}

	}
	else {//잘못된 데이터이니 잘라내야함
		DestroySocket();
		AddEventString(L"잘못된 프로토콜입니다.");
	}
}

LRESULT Cyoonj0207WinsocketclientDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == 27001) { //FD_CONNECT
		ConnectProcess(lParam);
	}
	else if (message == 27002)
	{
		if (WSAGETSELECTEVENT(lParam) == FD_READ) { //lParam에서 event part만 잘라서
			//서버로부터 데이터를 받음
			WSAAsyncSelect(mh_socket, m_hWnd, 27002, FD_CLOSE);
			//FD_READ가 무한정 발생하는것을 막기위해비동기를 다시 걸어줌
			ReadFrameData();
			//매개변수가 없는 이유 -> 클라이언트는 소켓이 하나이기 때문에 wParam을 처리할 필요가 x 바로 mh_socket으로 처리
			if(mh_socket != INVALID_SOCKET)	WSAAsyncSelect(mh_socket, m_hWnd, 27002, FD_CLOSE | FD_READ);
		}
		else {//연결이 끊김
			DestroySocket();
			AddEventString(L"서버에서 연결을 해제하였습니다.");
		}
	}
	else if (message == WM_LBUTTONDOWN) {
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (x > (XPOS(0) - HALF_INTERVAL) && y > (YPOS(0) - HALF_INTERVAL)
			&& x < (XPOS(X_COUNT - 1) + HALF_INTERVAL)
			&& y < (YPOS(Y_COUNT - 1) + HALF_INTERVAL)) {
			CString str;
			CString str_x;
			CString str_y;
			str_x.Format(_T("%d"), x);
			str_y.Format(_T("%d"), y);
			CString str_xy;
			str_xy = str_x + "@" + str_y;
			SendFrameData(mh_socket, 2, (const wchar_t*)str_xy, (str_xy.GetLength() + 1) * 2);
		}
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}

void Cyoonj0207WinsocketclientDlg::SendFrameData(SOCKET parm_h_socket,unsigned char parm_id, const void* parm_p_data, int parm_size)
{
	char* p_send_data = new char[parm_size + 4];

	*p_send_data = 27;
	*(unsigned short int*)(p_send_data+1) = parm_size;
	*(p_send_data+3) = parm_id;

	memcpy(p_send_data + 4, parm_p_data, parm_size);

	send(parm_h_socket, p_send_data, parm_size + 4, 0);

	delete[] p_send_data;
}

void Cyoonj0207WinsocketclientDlg::OnBnClickedSendBtn()
{
	CString str;
	GetDlgItemText(IDC_EDIT1,str);
	SendFrameData(mh_socket, 1,(const wchar_t*)str,(str.GetLength() + 1)*2);
	SetDlgItemText(IDC_EDIT1, L"");
}


void Cyoonj0207WinsocketclientDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
}
