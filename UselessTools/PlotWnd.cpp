#include "PlotWnd.h"

CPlotWnd* CPlotWnd::pWnd = 0;

CPlotWnd::CPlotWnd()
{
}

void CPlotWnd::Show(HINSTANCE hInst, LPCSTR pchTitle, int w, int h, Func func, double Xm, double Ym, double Xs, double Ys)
{
	m_hInst = hInst;
	m_Func = func;
	m_Xm = Xm, m_Ym = Ym;
	m_Xs = Xs, m_Ys = Ys;

	// Register the window class.
	const char CLASS_NAME[] = "UselesTools.PlotWnd";
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInst;
	wc.lpszClassName = CLASS_NAME;
	RegisterClass(&wc);

	// Create the window.
	m_hWnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		pchTitle,                       // Window text
		WS_OVERLAPPEDWINDOW,            // Window style
										// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, w, h,
		NULL,       // Parent window
		NULL,       // Menu
		hInst,      // Instance handle
		NULL        // Additional application data
	);

	if (m_hWnd == NULL)
	{
		return;
	}

	CPlotWnd::pWnd = this;

	ShowWindow(m_hWnd, SW_SHOW);

	// Run the message loop.

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	CPlotWnd::pWnd = 0;
}

CPlotWnd::~CPlotWnd()
{
}

void CPlotWnd::DoPaint(HDC hDC)
{
	RECT rect;
	GetClientRect(m_hWnd, &rect);
	FillRect(hDC, &rect, (HBRUSH)(COLOR_WINDOW + 1));

	LPCSTR kkk = "Построение графика";
	TextOut(hDC, 20, 20, kkk, strlen(kkk));

	LONG w = rect.right - rect.left;
	LONG h = rect.bottom - rect.top;

	//AXES
	LONG w_mid = w / 2;
	LONG h_mid = h / 2;
	MoveToEx(hDC, w_mid, 0, NULL);
	LineTo(hDC, w_mid, h);
	TextOut(hDC, w_mid + 4, 4, "Y", 1);
	MoveToEx(hDC, 0, h_mid, NULL);
	LineTo(hDC, w, h_mid);
	TextOut(hDC, w - 15, h_mid + 4, "X", 1);

	//GRID


	//FUNC
	double y = m_Func(0);
}

LRESULT CPlotWnd::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_PAINT:
			{
				if (CPlotWnd::pWnd != 0)
				{
					PAINTSTRUCT ps;
					HDC hDC = BeginPaint(hWnd, &ps);
					pWnd->DoPaint(hDC);
					EndPaint(hWnd, &ps);
				}
			}
			return 0;

		/*case WM_ERASEBKGND:
			return 1;*/

		case WM_SIZE:
			InvalidateRect(hWnd, NULL, false);
			break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
