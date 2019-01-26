#pragma once

#include <windows.h>

//y = Func(x)
typedef double(*Func)(double);

#define PLOT_UNIT 20
#define PLOT_ZASECKA 5

class CPlotWnd
{
	HWND      m_hWnd = 0;
	HINSTANCE m_hInst = 0;
	Func      m_Func = 0;
	double    m_Xm = 0, m_Ym = 0;
	double    m_Xs = 0, m_Ys = 0;
	int       m_wndW = 0, m_wndH = 0;
	char      *m_pInfo = 0;

public:
	CPlotWnd();
	void Show(HINSTANCE hInst, LPCSTR pchTitle, int w, int h, Func func, double Xm, double Ym, double Xs, double Ys, char *pInfo);
	~CPlotWnd();

	void DoPaint(HDC hDC);
	void DoSize();

	static CPlotWnd* pWnd;

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	int REALtoPX_X(double x);
	int REALtoPX_Y(double y);
	double PXtoREAL_X(int x);
};

