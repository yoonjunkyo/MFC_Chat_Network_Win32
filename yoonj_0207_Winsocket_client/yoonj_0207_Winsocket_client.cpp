
// yoonj_0207_Winsocket_client.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "yoonj_0207_Winsocket_client.h"
#include "yoonj_0207_Winsocket_clientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cyoonj0207WinsocketclientApp

BEGIN_MESSAGE_MAP(Cyoonj0207WinsocketclientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// Cyoonj0207WinsocketclientApp 생성

Cyoonj0207WinsocketclientApp::Cyoonj0207WinsocketclientApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 Cyoonj0207WinsocketclientApp 개체입니다.

Cyoonj0207WinsocketclientApp theApp;


// Cyoonj0207WinsocketclientApp 초기화

BOOL Cyoonj0207WinsocketclientApp::InitInstance()
{
	CWinApp::InitInstance();//인스턴스 호출 코드

	//2.2버전 WSA 소켓사용 요청 
	WSADATA temp;
	WSAStartup(0x0202,&temp);

	Cyoonj0207WinsocketclientDlg dlg; 
	m_pMainWnd = &dlg;
	dlg.DoModal();//다이얼로그 실행코드
	
	WSACleanup();
	
	return FALSE; //리턴 false
}

