// GraphicalInterface1.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "GraphicalInterface1.h"
#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst; // текущий экземпляр

WCHAR szTitle[MAX_LOADSTRING]; // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING]; // имя класса главного окна

HWND hWnd1;
HWND hWnd2;

COLORREF color = RGB(255, 0, 0);
RGBTRIPLE rgbColor = { 0 , 255 , 0 };

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_GRAPHICALINTERFACE1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GRAPHICALINTERFACE1));

	MSG msg;

	// Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GRAPHICALINTERFACE1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_GRAPHICALINTERFACE1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	hWnd1 = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	hWnd2 = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd1 || !hWnd2)
	{
		return FALSE;
	}

	ShowWindow(hWnd1, nCmdShow);
	UpdateWindow(hWnd1);

	ShowWindow(hWnd2, nCmdShow);
	UpdateWindow(hWnd2);

	return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Разобрать выбор в меню:
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
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		RECT rect;

		GetClientRect(hWnd, &rect);

		POINT p1;
		p1.x = rect.left;
		p1.y = rect.top;

		POINT p2;
		p2.x = rect.right;
		p2.y = rect.bottom;

		DrawLine(hdc, p1, p2);

		p1.y = rect.bottom;
		p2.y = rect.top;

		DrawLine(hdc, p1, p2);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		rgbColor = { rgbColor.rgbtBlue, rgbColor.rgbtRed, rgbColor.rgbtGreen };

		color = RGB(rgbColor.rgbtRed, rgbColor.rgbtGreen, rgbColor.rgbtBlue);
		InvalidateRect(hWnd, NULL, TRUE);
	}
	break;
	case WM_MOUSEMOVE:
	{
		HDC hdc = GetDC(hWnd2);
		HDC hdc1 = GetDC(hWnd1);
		RECT clientRect;
		RECT lol;
		lol.left = 0;
		lol.bottom = 0;
		lol.top = 1;
		lol.right = 1;
		
		GetClientRect(hWnd,&clientRect);
		
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(hWnd, &p);
		
		std::wstring textone = L"X:" + to_wstring(p.x) + L" Y:" + to_wstring(p.y) + L"     ";
		//std::wstring textone = L"X:" + to_wstring(int(LOWORD(lParam))) + L" Y:" + to_wstring(int(HIWORD(lParam))) + L"     ";
		DrawText(hdc, textone.c_str(), wcslen(textone.c_str()), &clientRect, DT_LEFT);
		DrawText(hdc1, textone.c_str(), wcslen(textone.c_str()), &clientRect, DT_LEFT);
		ReleaseDC(hWnd1, hdc);
		InvalidateRect(hWnd1, &lol, TRUE);
		

		/*DrawText(hdc, textone.c_str(), wcslen(textone.c_str()), &clientRect, DT_LEFT);
		ReleaseDC(hWnd, hdc);
		InvalidateRect(hWnd, &lol, TRUE);*/
	}
	break;
	case WM_DESTROY:

		if (hWnd == hWnd2)
		{
			DestroyWindow(hWnd2);
			hWnd2 = nullptr;
		}
		else if (hWnd == hWnd1)
		{
			DestroyWindow(hWnd1);
			hWnd1 = nullptr;
		}

		if (hWnd1 == nullptr and hWnd2 == nullptr)
		{
			PostQuitMessage(0);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

inline void DrawLine(HDC hdc, POINT p1, POINT p2)
{
	HPEN pen = CreatePen(PS_SOLID, 5, color);

	SelectObject(hdc, pen);
	MoveToEx(hdc, p1.x, p1.y, NULL);
	LineTo(hdc, p2.x, p2.y);
	DeleteObject(pen);
}

// Обработчик сообщений для окна "О программе".
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
