#include "PlotWnd.h"
#include <stdio.h>
#include <math.h>

void Marker(LONG x, LONG y, HDC hDC)
{
	MoveToEx(hDC, (int)x - 10, (int)y, (LPPOINT)NULL);
	LineTo(hDC, (int)x + 10, (int)y);
	MoveToEx(hDC, (int)x, (int)y - 10, (LPPOINT)NULL);
	LineTo(hDC, (int)x, (int)y + 10);
}

CPlotWnd* CPlotWnd::pWnd = 0;

CPlotWnd::CPlotWnd()
{
}

void CPlotWnd::Show(HINSTANCE hInst, LPCSTR pchTitle, int w, int h, Func func, double Xm, double Ym, double Xs, double Ys, char *pInfo)
{
	m_hInst = hInst;
	m_Func = func;
	m_Xm = Xm, m_Ym = Ym;
	m_Xs = Xs, m_Ys = Ys;
	m_pInfo = pInfo;

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

	LONG w = rect.right - rect.left;
	LONG h = rect.bottom - rect.top;

	LONG w_mid = w / 2;
	LONG h_mid = h / 2;

	//GRID
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(192, 192, 192));
	HGDIOBJ hOldPen = SelectObject(hDC, hPen);
	{
		int hor = 0;
		while (hor < h_mid)
		{
			hor += PLOT_UNIT;
			MoveToEx(hDC, 0, h_mid + hor, NULL);
			LineTo(hDC, m_wndW, h_mid + hor);
			MoveToEx(hDC, 0, h_mid - hor, NULL);
			LineTo(hDC, m_wndW, h_mid - hor);
		}
		int ver = 0;
		while (ver < w_mid)
		{
			ver += PLOT_UNIT;
			MoveToEx(hDC, w_mid + ver, 0, NULL);
			LineTo(hDC, w_mid + ver, m_wndH);
			MoveToEx(hDC, w_mid - ver, 0, NULL);
			LineTo(hDC, w_mid - ver, m_wndH);
		}
	}
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);

	//AXES
	hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	hOldPen = SelectObject(hDC, hPen);
	{
		MoveToEx(hDC, w_mid, 0, NULL);
		LineTo(hDC, w_mid, h);
		TextOut(hDC, w_mid + 4, 4, "Y", 1);

		MoveToEx(hDC, 0, h_mid, NULL);
		LineTo(hDC, w, h_mid);
		TextOut(hDC, w - 15, h_mid + 4, "X", 1);

		MoveToEx(hDC, w_mid + 20, h_mid + 5, NULL);
		LineTo(hDC, w_mid + PLOT_UNIT, h_mid - PLOT_ZASECKA);

		MoveToEx(hDC, w_mid - 5, h_mid - 20, NULL);
		LineTo(hDC, w_mid + PLOT_ZASECKA, h_mid - PLOT_UNIT);
	}
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);

	//NULL-POINT
	hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
	hOldPen = SelectObject(hDC, hPen);
	{
		Marker(REALtoPX_X(0), REALtoPX_Y(0), hDC);
	}
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);

	//FUNC
	hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	hOldPen = SelectObject(hDC, hPen);
	{
		MoveToEx(hDC, -m_wndW, h_mid, NULL);
		for (int x_p = 0; x_p < m_wndW; ++x_p)
		{
			double x_u = PXtoREAL_X(x_p);
			double y_u = m_Func(x_u);
			int y_p = REALtoPX_Y(y_u);
			LineTo(hDC, x_p, y_p);
			MoveToEx(hDC, x_p, y_p, NULL);
		}
	}
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);

	//FUNC-DATA
	char kkk[1000] = { "\0" };
	sprintf(kkk, "O(%g;%g), X=%g, Y=%g", m_Xm, m_Ym, m_Xs, m_Ys);
	TextOut(hDC, 10, 10, kkk, strlen(kkk));
	TextOut(hDC, 10, 25, m_pInfo, strlen(m_pInfo));
}

void CPlotWnd::DoSize()
{
	RECT aaa;
	GetClientRect(m_hWnd, &aaa);
	m_wndH = aaa.bottom - aaa.top;
	m_wndW = aaa.right - aaa.left;
	InvalidateRect(m_hWnd, NULL, false);
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

		case WM_SIZE:
			if (CPlotWnd::pWnd != 0)
			{
				pWnd->DoSize();
			}
			break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int CPlotWnd::REALtoPX_X(double x)
{
	int pos = (int)round(m_wndW * (x - m_Xm) / (m_Xs * m_wndW / (double)PLOT_UNIT)) + m_wndW/2;
	return pos;
}

int CPlotWnd::REALtoPX_Y(double y)
{
	double dpixel = (double)m_wndH * (y - m_Ym) / (m_Ys * m_wndH / (double)PLOT_UNIT);
	int pos = m_wndH / 2 - (int)round(dpixel);
	return pos;
}

double CPlotWnd::PXtoREAL_X(int x)
{
	double dxu = ((double)x - m_wndW/2.) * m_Xs / (double)PLOT_UNIT + m_Xm;
	return dxu;
}
