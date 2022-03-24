
// yoonj_0207_Winsocket_server.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "yoonj_0207_Winsocket_server.h"
#include "yoonj_0207_Winsocket_serverDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cyoonj0207WinsocketserverApp

BEGIN_MESSAGE_MAP(Cyoonj0207WinsocketserverApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// Cyoonj0207WinsocketserverApp 생성

Cyoonj0207WinsocketserverApp::Cyoonj0207WinsocketserverApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 Cyoonj0207WinsocketserverApp 개체입니다.

Cyoonj0207WinsocketserverApp theApp;


// Cyoonj0207WinsocketserverApp 초기화

BOOL Cyoonj0207WinsocketserverApp::InitInstance()
{
	CWinApp::InitInstance();

	//WSA (windows socket API)

	WSADATA temp;
	WSAStartup(0x0202,&temp); //socket 네트워킹을 시작하겠다는 의미, 2.2버전

	Cyoonj0207WinsocketserverDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	WSACleanup();//socket 을 사용하지 않겠다

	return FALSE;
}

