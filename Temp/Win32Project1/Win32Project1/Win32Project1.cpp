// Win32Project1.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Win32Project1.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;								// 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING];					// 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 여기에 코드를 입력합니다.
	MSG msg;
	HACCEL hAccelTable;

	// 전역 문자열을 초기화합니다.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

	// 기본 메시지 루프입니다.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WIN32PROJECT1);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//

int g_width(0);
int g_height(0);
BYTE* g_pBuffer = nullptr;

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

  

   // 여기서 파일을 로드한다.
   // 실제 파일을 연다.
   HANDLE hFileHandle = CreateFile(TEXT("C:\\sdh\\trunk\\Temp\\Win32Project1\\Debug\\second.hiy"), GENERIC_READ, 0, NULL, OPEN_ALWAYS, 0, NULL);

   // 파일의 데이터를 버퍼에 읽는다.
   DWORD resultSize = 0;
   
   BYTE header[11] = {0,};
   if (ReadFile(hFileHandle, &header[0], 11, &resultSize, NULL) == true)
   {
	   char tag[3];
	   ::CopyMemory((void*)&tag, &header[0], 3);

	   // 성공적으로 파일을 읽었어.
	   ::CopyMemory((void*)&g_width, &header[3], 4);
	   ::CopyMemory((void*)&g_height, &header[7], 4);
	   
	   int unitSize = sizeof(int);	// 최소 단위
	   int channelCount = 3;		// 사용하는 채널 개수 (R,G,B)
	   int bufferSize = unitSize * channelCount * g_width * g_height;
	   
	   g_pBuffer = new BYTE[bufferSize];
	   ::ZeroMemory(g_pBuffer, bufferSize);	// 버퍼를 깨끗하게 초기화

	   if (ReadFile(hFileHandle, g_pBuffer, bufferSize, &resultSize, NULL) == true)
	   {

		   // 성공
	   }
   }

   // 다 쓴 파일 핸들은 반드시 닫아 줘야 한다.
   CloseHandle(hFileHandle);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 메뉴 선택을 구문 분석합니다.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
	{
					 hdc = BeginPaint(hWnd, &ps);
					 // TODO: 여기에 그리기 코드를 추가합니다.
					 
					 int* pBuffer = reinterpret_cast<int*>(g_pBuffer);

					 for (int y = 0; y < g_height; ++y)
					 {
						 // g_width == 200 => 픽셀이 가로로 200개
						 // 1개 픽셀은 3개의 RGB 채널을 갖고 있다.
						 // 버퍼는 [픽셀*채널]
						 for (int x = 0; x < g_width; ++x)
						 {
							 int channelCount = 3;
							 int pitch = g_width * channelCount;
							 
							 int bufferStartX = pitch * y; // = > 99, 199, 299, 399;
							 bufferStartX += x * 3;

							 int r = pBuffer[bufferStartX];
							 int g = pBuffer[bufferStartX + 1];
							 int b = pBuffer[bufferStartX + 2];

							 DWORD color = RGB(r, g, b);
							 ::SetPixel(hdc, x, y, color);
						 }
					 }
					 
					 EndPaint(hWnd, &ps);
	}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		delete[]g_pBuffer;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
