// 150829.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "150829.h"

using namespace std;
#include <stdio.h>

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;								// 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING];					// 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.

BYTE*	g_pImageBuffer;
BYTE*	g_pImage2Buffer;

int	g_Width, g_Height;
int	g_Width2, g_Height2;

int	g_nFileType;

struct COLOR_
{
	UINT r;
	UINT g;
	UINT b;

};

struct COLOR_F
{
	float r;
	float g;
	float b;

	COLOR_F(float or, float og, float ob)
	{
		r = or;
		g = og;
		b = ob;
	}

	void operator/=(const float f)
	{
		r /= f;
		g /= f;
		b /= f;
	}

	void operator*=(const float f)
	{
		r *= f;
		g *= f;
		b *= f;
	}

	void Clamp (const float fMin, const float fMax)
	{
		if (r < fMin)
		{
			r = fMin;
		}
		if (g < fMin)
		{
			g = fMin;
		}
		if (b < fMin)
		{
			b = fMin;
		}

		if (r > fMax)
		{
			r = fMax;
		}
		if (g > fMax)
		{
			g = fMax;
		}
		if (b > fMax)
		{
			b = fMax;
		}
	}
};

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

bool				ReadImage(LPCWSTR lpFileName);
bool				ReadAlpha(LPCWSTR lpFileName1, LPCWSTR lpFileName2);
void				ReadHly(LPCWSTR lpFileName1, BYTE** pSaveLocation,int& nWidth, int& nHeight);

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
	LoadString(hInstance, IDC_MY150829, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY150829));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY150829));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY150829);
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

  /* if (!ReadImage(L"data/second.hiy"))
   {
	   MessageBoxA(hWnd, "CAN'T LOAD", "FILE LOAD FAIL", MB_OK);
   }*/
   if (!ReadAlpha(L"data/img.hiy", L"data/second.hiy"))
   {
	   MessageBoxA(hWnd, "CAN'T LOAD", "FILE LOAD FAIL", MB_OK);
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   // ReadFile
   
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
		wmId = LOWORD(wParam);
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

		//auto test = RGB(255, 255, 255);
		//SetPixel(hdc,위치X,위치Y,색깔값)

		int nStartX = 100;
		int nStartY = 100;

		int nIdx;

		for (int y = 0; y < g_Height; ++y)
		{
			for (int x = 0; x < g_Width; ++x)
			{
				//nIdx = ((y * g_Width * 3) + x * 3);
				nIdx = ((y * g_Width * g_nFileType) + x * g_nFileType);

				auto pixel =
					RGB(0,0,0);


				if (g_nFileType == 6)
				{
					COLOR_F	color1(0, 0, 0);

					COLOR_F	color2(0, 0, 0);
	
					// COLOR = ( 0 ~ 1 )

					COLOR_F finalColor(0, 0, 0);

					/*// 알파 블렌딩
					{
						// FINAL COLOR = ( SOURCE COLOR * ALPHA ) + ( DESTINATION COLOR * (1.0F - ALPHA) )
						// 최종 색상 = 소스 색상 * 알파 + 대상 색상 * (1.0 - 알파)

						nIdx = ((y * g_Width * 3) + x * 3);

						float	fAlpha = 0.3f;

						if (nIdx > (g_Width * g_Height) * 3)
						{
							nIdx %= (g_Width * g_Height) * 3;
						}

						color1.r = g_pImageBuffer[nIdx + 0];
						color1.g = g_pImageBuffer[nIdx + 1];
						color1.b = g_pImageBuffer[nIdx + 2];

						color1 /= 255.0f;

						color1 *= fAlpha;

						nIdx = ((y * g_Width * 3) + x * 3);

						if (nIdx > (g_Width2 * g_Height2) * 3)
						{
							nIdx %= (g_Width2 * g_Height2) * 3;
						}

						color2.r = g_pImage2Buffer[nIdx + 0];
						color2.g = g_pImage2Buffer[nIdx + 1];
						color2.b = g_pImage2Buffer[nIdx + 2];

						color2 /= 255.0f;

						color2 *= (1.0f - fAlpha);

						finalColor.r = color1.r + color2.r;
						finalColor.g = color1.g + color2.g;
						finalColor.b = color1.b + color2.b;

					}*/

					/*// 리니어 닷지
					// FINAL COLOR = SOURCE COLOR + DESTINATION COLOR
					{
					nIdx = ((y * g_Width * 3) + x * 3);

					if (nIdx > (g_Width * g_Height) * 3)
					{
					nIdx %= (g_Width * g_Height) * 3;
					}

					color1.r = g_pImageBuffer[nIdx + 0];
					color1.g = g_pImageBuffer[nIdx + 1];
					color1.b = g_pImageBuffer[nIdx + 2];

					color1 /= 255.0f;

					nIdx = ((y * g_Width * 3) + x * 3);

					if (nIdx > (g_Width2 * g_Height2) * 3)
					{
					nIdx %= (g_Width2 * g_Height2) * 3;
					}

					color2.r = g_pImage2Buffer[nIdx + 0];
					color2.g = g_pImage2Buffer[nIdx + 1];
					color2.b = g_pImage2Buffer[nIdx + 2];

					color2 /= 255.0f;

					finalColor.r = color1.r + color2.r;
					finalColor.g = color1.g + color2.g;
					finalColor.b = color1.b + color2.b;

					}*/

					// 색상 닷지
					// FINAL COLOR = SOURCE COLOR / ( 1 -  DESTINATION COLOR )
					{
						nIdx = ((y * g_Width * 3) + x * 3);

						if (nIdx > (g_Width * g_Height) * 3)
						{
							nIdx %= (g_Width * g_Height) * 3;
						}

						color1.r = g_pImageBuffer[nIdx + 0];
						color1.g = g_pImageBuffer[nIdx + 1];
						color1.b = g_pImageBuffer[nIdx + 2];

						color1 /= 255.0f;

						nIdx = ((y * g_Width * 3) + x * 3);

						if (nIdx > (g_Width2 * g_Height2) * 3)
						{
							nIdx %= (g_Width2 * g_Height2) * 3;
						}

						color2.r = g_pImage2Buffer[nIdx + 0];
						color2.g = g_pImage2Buffer[nIdx + 1];
						color2.b = g_pImage2Buffer[nIdx + 2];

						color2 /= 255.0f;

						finalColor.r = color1.r / ( 1.0f - color2.r );
						finalColor.g = color1.g / ( 1.0f - color2.g );
						finalColor.b = color1.b / ( 1.0f - color2.b );
					}

					/*if (finalColor.r > 1.0f)
					{
						finalColor.r = 1.0f;
					}
					if (finalColor.g > 1.0f)
					{
						finalColor.g = 1.0f;
					}
					if (finalColor.b > 1.0f)
					{
						finalColor.b = 1.0f;
					}*/

					finalColor.Clamp(0.0f, 1.0f);

					pixel =
						RGB(
						(finalColor.r*255.0f),
						(finalColor.g*255.0f),
						(finalColor.b*255.0f));
				}
				else
				{
					pixel =
						RGB(
						g_pImageBuffer[nIdx + 0],
						g_pImageBuffer[nIdx + 1],
						g_pImageBuffer[nIdx + 2]);
				}
									
				SetPixel(hdc, nStartX+x, nStartY+y, pixel);
			}
			int n = 0;
		}
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		delete[]	g_pImageBuffer;
		PostQuitMessage(0);
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

bool	ReadImage(LPCWSTR lpFileName)
{
	g_nFileType = -1;

	// 확장자 분리

	char cFile[256];

	int nLen = (int)wcslen(lpFileName);
	wcstombs(cFile, lpFileName, nLen + 1);

	char	ext[16];
	_splitpath(cFile, NULL, NULL, NULL, ext);
	
	if (strcmp(ext, ".hiy") == 0)
	{
		g_nFileType = 3;
	}
	else if (strcmp(ext, ".bmp") == 0)
	{
		g_nFileType = 1;
	}
	else
	{
		return false;
	}

	HANDLE hRead = NULL;

	DWORD	nResultSize = 0;
	BYTE*	Temp; 

	switch (g_nFileType)
	{
	case 3:
	{
			 /* BYTE	header[11] = { 0, };

			  if (ReadFile(hRead, &header, 11, &nResultSize, NULL))
			  {
				  char tag[3];

				  CopyMemory((void*)&tag, &header[0], 3);

				  CopyMemory((void*)&g_Width, &header[3], 4);
				  CopyMemory((void*)&g_Height, &header[7], 4);

				  Temp = new	BYTE[sizeof(BYTE)* g_Width * g_Height * 12];
				  ZeroMemory(Temp, sizeof(BYTE)* g_Width * g_Height * 12);

				  if (ReadFile(hRead, Temp, g_Width * g_Height * 12, &nResultSize, NULL) == true)
				  {
					  int a = 0;
				  }
			  }

			  g_pImageBuffer = new	BYTE[sizeof(BYTE)* g_Width * g_Height * 3];
			  ZeroMemory(g_pImageBuffer, sizeof(BYTE)* g_Width * g_Height * 3);

			  nResultSize = nResultSize / 4;

			  for (int i = 0; i < nResultSize; ++i)
			  {
				  g_pImageBuffer[i] = Temp[i * 4];
			  }

			  delete[] Temp;*/

			  ReadHly(lpFileName, &g_pImageBuffer,g_Width,g_Height);

			  break;
	}

	case 1:
	{
			  hRead = CreateFile(lpFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

			  BYTE	header[40] = { 0, };

			  int nPixelData = 0;
			  int nHeaderSize = 0;

			  // READ HEADER INFO
			  if (ReadFile(hRead, &header, 14, &nResultSize, NULL))
			  {
				 /* CopyMemory((void*)&nPixelData, &header[10], 4);

				  CopyMemory((void*)&nHeaderSize, &header[14], 4);

				  CopyMemory((void*)&g_Width, &header[18], 4);

				  CopyMemory((void*)&g_Height, &header[22], 4);

				  if (ReadFile(hRead, &header, nPixelData - 25, &nResultSize, NULL))
				  {
					  int a = 0;
				  }
				  Temp = new	BYTE[sizeof(BYTE)* g_Width * g_Height * 3];
				  ZeroMemory(Temp, sizeof(BYTE)* g_Width * g_Height * 3);

				  if (ReadFile(hRead, Temp, g_Width * g_Height * 3, &nResultSize, NULL) == true)
				  {
					  int a = 0;
				  }*/

				  // READ HEADER
				  if (ReadFile(hRead, &header, 40, &nResultSize, NULL))
				  {
					  CopyMemory((void*)&nHeaderSize, &header[0], 4);

					  CopyMemory((void*)&g_Width, &header[4], 4);
					  CopyMemory((void*)&g_Height, &header[8], 4);
				  }

				  // COLOR OUT
				  if (ReadFile(hRead, &header, 4, &nResultSize, NULL))
				  {

				  }

				  Temp = new	BYTE[sizeof(BYTE)* g_Width * g_Height * 12];
				  ZeroMemory(Temp, sizeof(BYTE)* g_Width * g_Height * 12);

				  // READ PIXEL
				  if (ReadFile(hRead, Temp, g_Width * g_Height * 12, &nResultSize, NULL))
				  {
					  int a = 0;
				  }
			  }

			  g_pImageBuffer = new	BYTE[sizeof(BYTE)* g_Width * g_Height * 12];
			  ZeroMemory(g_pImageBuffer, sizeof(BYTE)* g_Width * g_Height * 12);

			  for (int i = 0; i < g_Width * g_Height; ++i)
			  {
				  /*RGBQUAD	color_info;
				  
				  CopyMemory((void*)&color_info, &Temp[i*4], 4);

				  COLOR_	color;
				  color.r = color_info.rgbRed;
				  color.g = color_info.rgbGreen;
				  color.b = color_info.rgbBlue;

				  memcpy(
					  &g_pImageBuffer,
					  &color,
					  sizeof(color));*/

				  g_pImageBuffer[i + 0] = Temp[i * 4 + 8];
				  g_pImageBuffer[i + 1] = Temp[i * 4 + 4];
				  g_pImageBuffer[i + 2] = Temp[i * 4 + 0];

			  }

			  delete[] Temp;

			  break;
	}

	}

	CloseHandle(hRead);

	return	true;
}

bool	ReadAlpha(LPCWSTR lpFileName1, LPCWSTR lpFileName2)
{
	g_nFileType = 6;

	ReadHly(lpFileName1, &g_pImageBuffer,g_Width,g_Height);
	ReadHly(lpFileName2, &g_pImage2Buffer, g_Width2, g_Height2);

	return	true;
}

void	ReadHly(LPCWSTR lpFileName, BYTE** pSaveLocation, int& nWidth, int& nHeight)
{
	DWORD	nResultSize = 0;
	BYTE*	Temp;

	HANDLE hRead = CreateFile(lpFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

	BYTE	header[11] = { 0, };

	if (ReadFile(hRead, &header, 11, &nResultSize, NULL))
	{
		char tag[3];

		CopyMemory((void*)&tag, &header[0], 3);

		CopyMemory((void*)&nWidth, &header[3], 4);
		CopyMemory((void*)&nHeight, &header[7], 4);

		Temp = new	BYTE[sizeof(BYTE)* nWidth * nHeight * 12];
		ZeroMemory(Temp, sizeof(BYTE)* nWidth * nHeight * 12);

		if (ReadFile(hRead, Temp, nWidth * nHeight * 12, &nResultSize, NULL) == true)
		{
			int a = 0;
		}
	}

	*pSaveLocation = new	BYTE[sizeof(BYTE)* nWidth * nHeight * 3];
	ZeroMemory(*pSaveLocation, sizeof(BYTE)* nWidth * nHeight * 3);

	nResultSize = nResultSize / 4;

	for (int i = 0; i < nResultSize; ++i)
	{
		(*pSaveLocation)[i] = Temp[i * 4];
	}

	delete[] Temp;

	CloseHandle(hRead);
}