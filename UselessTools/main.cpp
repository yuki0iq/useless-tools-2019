#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <iostream>
#include "../../../modules/console_as_gui/console.h"
using namespace std;

#define getch _getch

void Algebra();
void Algebra_Linear();
void Algebra_Linear_Solve();
void Algebra_Linear_Plot();
void Algebra_Square();
void Algebra_Cubic();
void Algebra_Basic();

void Geometry();
void Geometry_Point();
void Geometry_Line();
void Geometry_Triangle();
void Geometry_Rectangle();
void Geometry_Circle();
void Geometry_Parallelogram();
void Geometry_Angle();

void Calculus();
void Physics();
void Statistics();

int find_approx_d_l(double arr[], int size, double what);

CConsole con;

int main()
{
	const char *pchMenuLocal =
		"Useless Tools (SBU aka Sbornik Bespoleznikh Utilit) main menu\n"
		"[(1) Algebra   ]\n"
		"[(2) Geometry  ]\n"
		"[(3) Calculus  ]\n"
		"[(4) Physics   ]\n"
		"[(5) Statistics]\n"
		"[( ) Exit      ]\n";

	int ch;
	while (true)
	{
		con.Clear();
		cout << pchMenuLocal;
		ch = tolower(getch());

		switch (ch)
		{
			case '1': Algebra(); break;
			case '2': Geometry(); break;
			case '3': Calculus(); break;
			case '4': Physics(); break;
			case '5': Statistics(); break;
			default: goto end;
		}
	}
end:

	return 0;
}

void Algebra()
{
	const char *pchMenuLocal =
		"ALGEBRA\n"
		"[(1) Linear equations  ]\n"
		"[(2) Square equations  ]\n"
		"[(3) Cubic equations   ]\n"
		"[(4) Basic calculations]\n"
		"[( ) Return            ]\n";
	con.Clear();
	cout << pchMenuLocal;
	int ch = tolower(getch());

	switch (ch)
	{
		case '1': Algebra_Linear(); break;
		case '2': Algebra_Square(); break;
		case '3': Algebra_Cubic(); break;
		case '4': Algebra_Basic(); break;
		default: return;
	}
}

void Algebra_Linear()
{
	//Plot y=ax+b
	//Solve 0=ax+b

	const char *pchMenuLocal =
		"ALGEBRA:LINEAR\n"
		"[(1) Solve ]\n"
		"[(2) Plot  ]\n"
		"[( ) Return]\n";
	con.Clear();
	cout << pchMenuLocal;
	int ch = tolower(getch());

	switch (ch)
	{
		case '1': Algebra_Linear_Solve(); break;
		case '2': Algebra_Linear_Plot(); break;
		default: return;
	}
}

void Algebra_Linear_Solve()
{
	cout << "Solving ax+b=0\n";
	double a = 0; cout << "a="; cin >> a;
	double b = 0; cout << "b="; cin >> b;
	if (a == 0)
	{
		if (b == 0)
			cout << "x=ANY\n";
		else
			cout << "x=EMPTYSET\n";
	}
	else
		cout << "x=" << -b/a << "\n";
	getch();
}

void Algebra_Linear_Plot()
{
	//Plot data
	double Xm = 0, Ym = 0, Xs = 0, Ys = 0;
	cout << "Enter mid-point coordinates (Xm Ym): "; cin >> Xm >> Ym;
	cout << "Enter scale factor (Xs Ys): "; cin >> Xs >> Ys;
	if (Xs == 0 || Ys == 0)
	{
		cout << "Null scale factor is illegal!\n";
		return;
	}

	//Func data
	double a = 0, b = 0;
	cout << "Enter A and B for y=ax+b (A B): "; cin >> a >> b;
	con.Clear();

	//Plot base
	{
		//Borders
		for (int j = 0; j < 43; ++j)
		{
			con.SetCursorPos(j, 0);
			con.OutputColorText("#", COLOR::RED, COLOR::RED, true);
			con.SetCursorPos(j, 22);
			con.OutputColorText("#", COLOR::RED, COLOR::RED, true);
		}
		for (int i = 0; i < 23; ++i)
		{
			con.SetCursorPos(0, i);
			con.OutputColorText("#", COLOR::RED, COLOR::RED, true);
			con.SetCursorPos(1, i);
			con.OutputColorText("#", COLOR::RED, COLOR::RED, true);
			con.SetCursorPos(43, i);
			con.OutputColorText("#", COLOR::RED, COLOR::RED, true);
			con.SetCursorPos(44, i);
			con.OutputColorText("#", COLOR::RED, COLOR::RED, true);
		}

		//Plot grid
		/*
			^
			+
		+-+-+-+>
			+
			+
		*/
		{
			//Y
			con.SetCursorPos(22, 1);
			cout << "^";
			for (int i = 2; i < 22; ++i)
			{
				con.SetCursorPos(22, i);
				cout << "+";
			}

			//X
			for (int i = 2; i < 41; i += 2)
			{
				con.SetCursorPos(i, 11);
				cout << "+-";
			}
			con.SetCursorPos(41, 11);
			cout << ">";
		}
	}

	//Plot function
	{
		//Ranges
		double X[41], Y[21];
		X[20] = Xm, Y[10] = Ym;
		for (int i = 9; i >= 0; --i)
			Y[i] = Y[i + 1] + Ys;
		for (int i = 11; i < 21; ++i)
			Y[i] = Y[i - 1] - Ys;
		for (int i = 19; i >= 0; --i)
			X[i] = X[i + 1] - 0.5*Xs;
		for (int i = 21; i < 41; ++i)
			X[i] = X[i - 1] + 0.5*Xs;
		//Plot
		for (int i = 0; i < 41; ++i)
		{
			double y = a * X[i] + b;
			int iIndexY = find_approx_d_l(Y, 21, y);

			con.SetCursorPos(i + 2, iIndexY + 1);
			con.OutputColorText(" ", COLOR::GREEN, COLOR::GREEN, true);
		}
		//Data
		con.SetCursorPos(0, 24);
		printf("y=%gx+%g; M(%g,%g); S(%g,%g)", a, b, Xm, Ym, Xs, Ys);
	}

	getch();
}

void Algebra_Square()
{
	//Plot y=ax2+bx+c
	//Solve 0=ax2+bx+c
}

void Algebra_Cubic()
{
	//Plot y=ax3+bx2+cx+d
	//Solve 0=ax3+bx2+cx+d
}

void Algebra_Basic()
{
	//add sub mul div rem pow root GCD LCM divisors
}


void Geometry()
{
	const char *pchMenuLocal =
		"GEOMETRY\n"
		"[(1) Point        ]\n"
		"[(2) Line         ]\n"
		"[(3) Triangle     ]\n"
		"[(4) Rectangle    ]\n"
		"[(5) Circle       ]\n"
		"[(6) Parallelogram]\n"
		"[(7) Angle        ]\n"
		"[( ) Return       ]\n";
	con.Clear();
	cout << pchMenuLocal;
	int ch = tolower(getch());

	switch (ch)
	{
		case '1': Geometry_Point(); break;
		case '2': Geometry_Line(); break;
		case '3': Geometry_Triangle(); break;
		case '4': Geometry_Rectangle(); break;
		case '5': Geometry_Circle(); break;
		case '6': Geometry_Parallelogram(); break;
		case '7': Geometry_Angle(); break;
		default: return;
	}
}

void Geometry_Point()
{
}

void Geometry_Line()
{
}

void Geometry_Triangle()
{
}

void Geometry_Rectangle()
{
}

void Geometry_Circle()
{
}

void Geometry_Parallelogram()
{
}

void Geometry_Angle()
{
}


void Calculus()
{
}


void Physics()
{
}


void Statistics()
{
}

int find_approx_d_l(double arr[], int size, double what)
{
	if (size == 0)
		return -1; //No elements
	if (what > arr[0])
		return -2; //Too high
	if (what < arr[size - 1])
		return -3; //Too low

	int l = 0, r = size - 1;

	while (l + 1 < r)
	{
		int m = (l + r) / 2;
		if (what > arr[m])
			r = m;
		else
			l = m;
	}

	double dL = arr[l] - what;
	double dR = what - arr[r];

	if (dL >= dR) return r;
	else return l;
}
