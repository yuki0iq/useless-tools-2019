#pragma once

#include <windows.h>

//y = Func(x)
typedef double(*Func)(double);

class CPlotWnd
{
	HWND      m_hWnd;
	HINSTANCE m_hInst;
	Func      m_Func;
	double    m_Xm, m_Ym;
	double    m_Xs, m_Ys;

public:
	CPlotWnd();
	void Show(HINSTANCE hInst, LPCSTR pchTitle, int w, int h, Func func, double Xm, double Ym, double Xs, double Ys);
	~CPlotWnd();

	void DoPaint(HDC hDC);

	static CPlotWnd* pWnd;

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

