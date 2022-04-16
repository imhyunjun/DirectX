﻿#include "../header/Window.h"
#include "../header/WindowMessageMap.h"

/// <param name="hinstance"></param> 인스턴스 : 아이디
/// <param name="hPrevInstance"></param> 항상 0
/// <param name="lpCmdLine"></param> 프로그램 외부에서 내부러 값 전달
/// <param name="nCmdShow"></param> 윈도우 출력 형태에 관한 값
/// <returns></returns>
int CALLBACK WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Window wnd(800, 300, "HyunJun First Window");
	//UpdateWindow(hWnd);

	//메세지 루프
	MSG msg;
	BOOL gResult;

	//getmessage -> 메세지를 가져오는 역할, WM_QUIT가 발생할 경우에만 FALSE를 리턴
	while ((gResult = GetMessage(&msg, NULL, 0, 0)) > 0)
	{
		//문자 키 또는 입력에 대해 메세지 반환
		//WM_KEYDOWN -> WM_CHAR
		TranslateMessage(&msg);

		//WndProc 함수 호출
		//WndProc가 종료될때까지 대기
		//WndProc에 msg 전달
		DispatchMessageW(&msg);
	}

	if (gResult == -1)
	{
		return -1;
	}

	return msg.wParam;
}


