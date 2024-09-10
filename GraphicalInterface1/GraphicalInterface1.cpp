// GraphicalInterface1.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "GraphicalInterface1.h"
#include <unordered_map>

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst; // текущий экземпляр

WCHAR szTitle[MAX_LOADSTRING]; // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING]; // имя класса главного окна

std::unordered_map< HWND, COLORREF> windowsColor;
RGBTRIPLE rgbColor = { 0 , 255 , 0 };

///
double secondAngle = 180, minuteAngle = 180, hourAngle = 180;
HANDLE hTImer = 0;
HANDLE hTimerQueue = 0;
///
BOOL IsSingleWindow = TRUE;
RECT clientRect;

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
		if (msg.message == WM_HOTKEY)
		{
			IsSingleWindow = !IsSingleWindow;
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

void CALLBACK Timer(PVOID, BOOLEAN) {
	///
	if (secondAngle == -180) secondAngle = 180;
	secondAngle -= 180 / 60; // изменение угла секундной стрелки
	if (minuteAngle == -180) minuteAngle = 180;
	minuteAngle -= 180 / 3600.0; // изменение угла минутной стрелки
	if (hourAngle == -180) hourAngle = 180;
	hourAngle -= 180 / 216000.0; // изменение угла часовой стрелки
	for (const auto& elem : windowsColor) {
		InvalidateRect(elem.first, NULL, TRUE);
	}
	///
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	constexpr int windowsNum = 2;
	for (int i = 0; i < windowsNum; ++i) {
		const HWND& hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

		windowsColor.emplace(hWnd, RGB(255, 0, 0));
	}

	for (const auto& elem : windowsColor) {
		if (!elem.first) {
			return FALSE;
		}
	}

	///
	SYSTEMTIME st;
	GetLocalTime(&st);
	secondAngle -= st.wSecond * 6;
	minuteAngle -= st.wMinute * 6;
	hourAngle -= st.wHour * 30;

	auto i = 1;
	for (const auto& elem : windowsColor) {

		ShowWindow(elem.first, nCmdShow);
		UpdateWindow(elem.first);
	}

	hTimerQueue = CreateTimerQueue();
	CreateTimerQueueTimer(&hTImer, hTimerQueue, (WAITORTIMERCALLBACK)Timer, NULL, 1000, 1000, 0);
	///

	RegisterHotKey(NULL, CHANGE_VIEW_STATE, MOD_CONTROL, VK_SPACE);

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

		GetClientRect(hWnd, &clientRect);

		//DrawCross(hdc);
		DrawClock(hdc,windowsColor[hWnd]);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_TIMER:
	{
		///
		if (secondAngle == -180) secondAngle = 180;
		secondAngle -= 180 / 60; // изменение угла секундной стрелки
		if (minuteAngle == -180) minuteAngle = 180;
		minuteAngle -= 180 / 3600.0; // изменение угла минутной стрелки
		if (hourAngle == -180) hourAngle = 180;
		hourAngle -= 180 / 216000.0; // изменение угла часовой стрелки
		for (const auto& elem : windowsColor) {
			InvalidateRect(elem.first, NULL, TRUE);
		}

		///
	}
	break;
	case WM_LBUTTONDOWN:
	{
		windowsColor[hWnd] = RGB(rgbColor.rgbtRed, rgbColor.rgbtGreen, rgbColor.rgbtBlue);

		rgbColor = { rgbColor.rgbtBlue, rgbColor.rgbtRed, rgbColor.rgbtGreen };
		
		InvalidateRect(hWnd, NULL, TRUE);
	}
	break;
	case WM_MOUSEMOVE:
	{
		POINT p;

		GetCursorPos(&p);
		ScreenToClient(hWnd, &p);

		wstring text = L"X: " + to_wstring(p.x) + L" Y: " + to_wstring(p.y) + L"    ";
		const wchar_t* textC = text.c_str();

		if (IsSingleWindow)
		{
			HDC hdc = GetDC(hWnd);

			DrawText(hdc, textC, wcslen(textC), &clientRect, DT_LEFT);

			ReleaseDC(hWnd, hdc);
		}
		else
		{
			for (const auto& elem : windowsColor) {
				const HDC& hdc = GetDC(elem.first);
				DrawText(hdc, textC, wcslen(textC), &clientRect, DT_LEFT);
				ReleaseDC(elem.first, hdc);
			}			
		}
	}
	break;
	case WM_QUIT:
		break;
	case WM_DESTROY:

		windowsColor.erase(hWnd);
		DestroyWindow(hWnd);

		if (windowsColor.empty())
		{
			UnregisterHotKey(NULL, CHANGE_VIEW_STATE);

			PostQuitMessage(0);
			DeleteTimerQueueTimer(hTimerQueue, hTImer, NULL);
			DeleteTimerQueue(hTimerQueue);
		}
		
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return EXIT_SUCCESS;
}

inline void DrawLine(const HDC& hdc, const POINT& p1, const POINT& p2, const COLORREF& color)
{
	HPEN pen = CreatePen(PS_SOLID, 5, color);

	SelectObject(hdc, pen);
	MoveToEx(hdc, p1.x, p1.y, NULL);
	LineTo(hdc, p2.x, p2.y);
	DeleteObject(pen);
}

inline void DrawLine(const HDC& hdc, const POINT& p1, const POINT& p2, INT sizeLine, const COLORREF& color)
{
	HPEN pen = CreatePen(PS_SOLID, sizeLine, color);

	SelectObject(hdc, pen);
	MoveToEx(hdc, p1.x, p1.y, NULL);
	LineTo(hdc, p2.x, p2.y);
	DeleteObject(pen);
}

inline void DrawCross(const HDC& hdc, const COLORREF& color)
{
	POINT p1{};
	p1.x = clientRect.left;
	p1.y = clientRect.top;

	POINT p2{};
	p2.x = clientRect.right;
	p2.y = clientRect.bottom;

	DrawLine(hdc, p1, p2, color);

	p1.y = clientRect.bottom;
	p2.y = clientRect.top;

	DrawLine(hdc, p1, p2, color);
}

inline void DrawClock(const HDC& hdc, const COLORREF& color)
{
	HPEN pen = CreatePen(PS_SOLID, 5, color);

	SelectObject(hdc, pen);

	POINT center =
	{
		clientRect.right / 2,
		clientRect.bottom / 2
	};

	int radius = center.y / 1.25;

	Ellipse(hdc, center.x - radius, center.y + radius, center.x + radius, center.y - radius);

	POINT secondsEndPoint =
	{
		center.x + 0.7 * radius * sin(M_PI * secondAngle / 180),
		center.y + 0.7 * radius * cos(M_PI * secondAngle / 180),
	};

	POINT minutesEndPoint =
	{
		center.x + 0.6 * radius * sin(M_PI * minuteAngle / 180),
		center.y + 0.6 * radius * cos(M_PI * minuteAngle / 180),
	};

	POINT hoursEndPoint =
	{
		center.x + 0.4 * radius * sin(M_PI * hourAngle / 180),
		center.y + 0.4 * radius * cos(M_PI * hourAngle / 180),
	};

	DrawLine(hdc, center, secondsEndPoint, 3, color);
	DrawLine(hdc, center, minutesEndPoint, 4, color);
	DrawLine(hdc, center, hoursEndPoint, 5, color);

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
