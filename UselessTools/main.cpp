#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include "../../../modules/console_as_gui/console.h"
#include "PlotWnd.h"
#include "duktape.h"
using namespace std;

#define getch _getch

void Algebra();
void Algebra_Linear();
void Algebra_Linear_Solve();
void Algebra_Linear_Plot();
void Algebra_Square();
void Algebra_Square_Solve();
void Algebra_Square_Plot();
void Algebra_Cubic();
void Algebra_Cubic_Solve();
void Algebra_Cubic_Plot();
void Algebra_Basic();
void Algebra_Plot();

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
void Statistics_ArrayProp();
void Statistics_ArrayOp();

int find_approx_d_l(double arr[], int size, double what);

double y_linear_A = 0, y_linear_B = 0;
inline double y_linear(double x) { return y_linear_A * x + y_linear_B; }
double y_square_A = 0, y_square_B = 0, y_square_C = 0;
inline double y_square(double x) { return y_square_A * x * x + y_square_B * x + y_square_C; }
double y_cubic_A = 0, y_cubic_B = 0, y_cubic_C = 0, y_cubic_D = 0;
inline double y_cubic(double x) { return y_cubic_A * x * x * x + y_cubic_B * x * x + y_cubic_C * x + y_cubic_D; }

string y_js_function;
double y_js(double x);

vector<pair<double, double>> SolveLinear(double a, double b);
vector<pair<double, double>> SolveSquared(double a, double b, double c);
vector<pair<double, double>> SolveCubic(double a, double b, double c, double d);

UINT64 GCD(UINT64 a, UINT64 b);
UINT64 LCM(UINT64 a, UINT64 b);
string DVS(UINT64 a);
bool IPR(UINT64 a);

CConsole con;
CPlotWnd w;
duk_context *ctx;

int main()
{
	cout << setprecision(15);
	ctx = duk_create_heap_default();

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
	duk_destroy_heap(ctx);
	ctx = nullptr;

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
		"[(5) Plot JS equation  ]\n"
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
		case '5': Algebra_Plot(); break;
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
	SolveLinear(a, b);
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
	cout << "Enter A and B for y=ax+b (A B): "; cin >> y_linear_A >> y_linear_B;
	con.Clear();

	/*
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
		//    ^
		//    +
		//+-+-+-+->
		//    +
		//    +
		
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
	*/

	char ddd[1000] = { "\0" };
	sprintf(ddd, "y(x)=%gx+%g", y_linear_A, y_linear_B);
	w.Show(GetModuleHandle(0), "Plot window", 640, 400, y_linear, Xm, Ym, Xs, Ys, ddd);
	y_linear_A = y_linear_B = 0;
}

void Algebra_Square()
{
	//Plot y=ax2+bx+c
	//Solve 0=ax2+bx+c

	const char *pchMenuLocal =
		"ALGEBRA:SQUARE\n"
		"[(1) Solve ]\n"
		"[(2) Plot  ]\n"
		"[( ) Return]\n";
	con.Clear();
	cout << pchMenuLocal;
	int ch = tolower(getch());

	switch (ch)
	{
		case '1': Algebra_Square_Solve(); break;
		case '2': Algebra_Square_Plot(); break;
		default: return;
	}
}

void Algebra_Square_Solve()
{
	cout << "Solving ax2+bx+c=0\n";
	double a = 0; cout << "a="; cin >> a;
	double b = 0; cout << "b="; cin >> b;
	double c = 0; cout << "c="; cin >> c;
	SolveSquared(a, b, c);
	getch();
}

void Algebra_Square_Plot()
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
	cout << "Enter A and B for y=ax2+bx+c (A B C): "; cin >> y_square_A >> y_square_B >> y_square_C;
	con.Clear();

	//Plot
	char ddd[1000] = { "\0" };
	sprintf(ddd, "y(x)=%gx2+%gx+%g", y_square_A, y_square_B, y_square_C);
	w.Show(GetModuleHandle(0), "Plot window", 640, 400, y_square, Xm, Ym, Xs, Ys, ddd);
	y_square_A = y_square_B = y_square_C = 0;
}

void Algebra_Cubic()
{
	//Plot y=ax3+bx2+cx+d
	//Solve 0=ax3+bx2+cx+d

	const char *pchMenuLocal =
		"ALGEBRA:CUBIC\n"
		"[(1) Solve ]\n"
		"[(2) Plot  ]\n"
		"[( ) Return]\n";
	con.Clear();
	cout << pchMenuLocal;
	int ch = tolower(getch());

	switch (ch)
	{
		case '1': Algebra_Cubic_Solve(); break;
		case '2': Algebra_Cubic_Plot(); break;
		default: return;
	}
}

void Algebra_Cubic_Solve()
{
	cout << "Solving ax3+bx2+cx+d=0\n";
	double a = 0; cout << "a="; cin >> a;
	double b = 0; cout << "b="; cin >> b;
	double c = 0; cout << "c="; cin >> c;
	double d = 0; cout << "d="; cin >> d;
	SolveCubic(a, b, c, d);
	getch();
}

void Algebra_Cubic_Plot()
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
	cout << "Enter A and B for y=ax3+bx2+cx+d (A B C D): "; cin >> y_cubic_A >> y_cubic_B >> y_cubic_C >> y_cubic_D;
	con.Clear();

	//Plot
	char ddd[1000] = { "\0" };
	sprintf(ddd, "y(x)=%gx3+%gx2+%gx+%g", y_cubic_A, y_cubic_B, y_cubic_C, y_cubic_D);
	w.Show(GetModuleHandle(0), "Plot window", 640, 400, y_cubic, Xm, Ym, Xs, Ys, ddd);
	y_cubic_A = y_cubic_B = y_cubic_C = y_cubic_D = 0;
}

void Algebra_Basic()
{
	//add sub mul div rem pow root GCD LCM divisors
	const char *pchMenuLocal =
		"ALGEBRA:BASIC\n"
		"[(+)ADD][(%)REM] [(Q)GCD] [(A)ADDF][(Z)INVF]\n"
		"[(-)SUB][(^)POW] [(W)LCM] [(S)SUBF][(X)POWF]\n"
		"[(*)MUL][(P)ROT] [(E)DVS] [(D)MULF][(C)ROTF]\n"
		"[(/)DIV][(.)RND] [(R)IPR] [(F)DIVF][(V)SIMF]\n"
		"[( )RET]\n";
	con.Clear();
	cout << pchMenuLocal;
	int ch = tolower(getch());

	switch (ch)
	{
		case '+': { double a, b; cout << "a="; cin >> a; cout << "b="; cin >> b; cout << "a+b=" << a + b; getch(); } break;
		case '-': { double a, b; cout << "a="; cin >> a; cout << "b="; cin >> b; cout << "a-b=" << a - b; getch(); } break;
		case '*': { double a, b; cout << "a="; cin >> a; cout << "b="; cin >> b; cout << "a*b=" << a * b; getch(); } break;
		case '/': { double a, b; cout << "a="; cin >> a; cout << "b="; cin >> b; cout << "a/b=" << a / b; getch(); } break;
		case '%': { double a, b; cout << "a="; cin >> a; cout << "b="; cin >> b; cout << "a%b=" << fmod(a, b); getch(); } break;
		case '^': { double a, b; cout << "a="; cin >> a; cout << "b="; cin >> b; cout << "a^b=" << pow(a, b); getch(); } break;
		case 'p': { double a, b; cout << "a="; cin >> a; cout << "b="; cin >> b; cout << "aVb=" << pow(b, 1./a); getch(); } break;
		case '.': { double a; cout << "a="; cin >> a; cout << "a~" << (long)round(a); getch(); } break;
		case 'q': { UINT64 a, b; cout << "a="; cin >> a; cout << "b="; cin >> b; cout << "GCD = " << (long)GCD(a, b); getch(); } break;
		case 'w': { UINT64 a, b; cout << "a="; cin >> a; cout << "b="; cin >> b; cout << "LCM = " << (long)LCM(a, b); getch(); } break;
		case 'e': { UINT64 a; cout << "a="; cin >> a; cout << "Divisors = " << DVS(a); getch(); } break;
		case 'r': { UINT64 a; cout << "a="; cin >> a; cout << "Prime? " << IPR(a); getch(); } break;
		case 'a': {} break;
		case 's': {} break;
		case 'd': {} break;
		case 'f': {} break;
		case 'z': {} break;
		case 'x': {} break;
		case 'c': {} break;
		case 'v': {} break;
		default: return;
	}
}

void Algebra_Plot()
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
	cout << "y = ";
	con.GetLine(false, false);
	string aaa = con.GetTrimmedLine();
	y_js_function = "function y(x){return (" + aaa + ");}";
	con.Clear();

	//Plot
	char ddd[1000] = { "\0" };
	sprintf(ddd, "y(x)=%s", aaa.c_str());
	w.Show(GetModuleHandle(0), "Plot window", 640, 400, y_js, Xm, Ym, Xs, Ys, ddd);
	y_js_function = "";
}


void Geometry()
{
	const char *pchMenuLocal =
		"GEOMETRY\n"
		"[(1) Point        ]\n" //dist1d dist2d dist3d
		"[(2) Line         ]\n" //intersect2d intersect3d
		"[(3) Triangle     ]\n" //P A build
		"[(4) Rectangle    ]\n" //P A build
		"[(5) Circle       ]\n" //P A build
		"[(6) Parallelogram]\n" //P A build
		"[(7) Angle        ]\n" //sin cos tan cot asin acos atan acot toRad toDeg
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
	//logn, log10, loge, 10n, en, int_table, deriv_table
}


void Physics()
{
	//P=mg, p=(p)gh, g, c2,, (p)=m/V, s=vt
}


void Statistics()
{
	const char *pchMenuLocal =
		"STAT\n"
		"[(1) Array prop]\n" //Mean, Median, Typical, Range, Min, Max, Shift, ShiftSquared, S-2, S-1
		"[(2) Array ops ]\n" //ArrayAdd, ArrayInv
		"[( ) Return    ]\n";
	con.Clear();
	cout << pchMenuLocal;
	int ch = tolower(getch());

	switch (ch)
	{
		case '1': Statistics_ArrayProp(); break;
		case '2': Statistics_ArrayOp(); break;
		default: return;
	}
}

void Statistics_ArrayProp()
{
	cout << "Enter size: "; int n; cin >> n;
	if (n == 0) { cout << "Null size array is illegal!"; getch(); return; }
	cout << "Enter elements space separated: ";
	vector<double> arr, sh, sh2;
	for (int i = 0; i < n; ++i)
	{
		double a;
		cin >> a;
		arr.push_back(a);
	}

	double sum = 0; for (auto k : arr) sum += k;
	cout << "Sum:\t" << sum << endl;

	double mean = sum / (double)n;
	cout << "Mean:\t" << mean << endl;

	double median = 0;
	sort(arr.begin(), arr.end());
	cout << "Sorted:\t";
	for (auto k : arr) cout << k << " ";
	cout << endl;
	if (arr.size() % 2 == 0) median = (arr[arr.size() / 2] + arr[arr.size() / 2 - 1]) / 2;
	else median = arr[arr.size() / 2];
	cout << "Median:\t" << median << endl;

	cout << "Typical:";
	multiset<double> m;
	for (auto k : arr) m.insert(k);
	int max_cnt = 0;
	for (auto k : m) if ((int)m.count(k) > max_cnt) max_cnt = m.count(k);
	set<double> m2;
	for (auto k : m) if ((int)m.count(k) == max_cnt) m2.insert(k);
	for (auto k : m2) cout << k << " ";
	cout << endl;

	double min = DBL_MAX, max = DBL_MIN; for (auto k : arr) { if (k < min) min = k; if (k > max) max = k; }
	cout << "Min:\t" << min << endl;
	cout << "Max:\t" << max << endl;
	cout << "Range:\t" << max-min << endl;

	for (auto k : arr) sh.push_back(k - mean);
	cout << "Shift:\t";
	for (auto k : sh) cout << k << " ";
	cout << endl;
	
	for (auto k : sh) sh2.push_back(k*k);
	cout << "Shift2:\t";
	for (auto k : sh2) cout << k << " ";
	cout << endl;

	double sum2 = 0; for (auto k : sh2) sum2 += k;
	cout << "SumSH2:\t" << sum2 << endl;
	double s2 = sum2 / sh2.size();
	cout << "S^2:\t" << s2 << endl;

	double s = sqrt(s2);
	cout << "S:\t" << s << endl;

	getch();
}

void Statistics_ArrayOp()
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


double y_js(double x)
{
	if (duk_peval_string(ctx, y_js_function.c_str()) != 0)
	{
		printf("Error: %s\n", duk_safe_to_string(ctx, -1));
		return 0;
	}
	duk_pop(ctx);
	duk_push_global_object(ctx);
	duk_get_prop_string(ctx, -1 /*index*/, "y");
	duk_push_number(ctx, x);
	if (duk_pcall(ctx, 1) != 0)
	{
		printf("Error: %s\n", duk_safe_to_string(ctx, -1));
		return 0;
	}
	else
	{
		double val = (double)duk_get_number(ctx, -1);
		return val;
	}
	duk_pop(ctx);
}

vector<pair<double, double>> SolveLinear(double a, double b)
{
	vector<pair<double, double>> x;
	if (a == 0)
	{
		if (b == 0)
		{
			x.push_back({ INFINITY, INFINITY });
			cout << "x=ANY\n";
		}
		else
		{
			x.push_back({ INFINITY, -INFINITY });
			cout << "x=EMPTYSET\n";
		}
	}
	else
	{
		x.push_back({ -b / a, 0 });
		cout << "x=" << -b / a << "\n";
	}
	return x;
}

vector<pair<double, double>> SolveSquared(double a, double b, double c)
{
	vector<pair<double, double>> x;
	if (a == 0)
	{
		cout << "AS LINEAR bx+c=0\n";
		x = SolveLinear(b, c);
	}
	else
	{
		double d = b * b - 4. * a*c;
		cout << "d=" << d << "\n";
		if (d == 0)
		{
			x.push_back({ -b / (2.*a), 0 });
			cout << "x=" << -b / (2.*a) << "\n";
		}
		else if (d > 0)
		{
			x.push_back({ (b + sqrt(d)) / (-2.*a), 0 });
			x.push_back({ (b - sqrt(d)) / (-2.*a), 0 });
			cout << "x1=" << (b + sqrt(d)) / (-2.*a) << "\n";
			cout << "x2=" << (b - sqrt(d)) / (-2.*a) << "\n";
		}
		else
		{
			double kkk = sqrt(-d) / (2.*a);
			x.push_back({ b / (-2.*a), abs(kkk) });
			x.push_back({ b / (-2.*a), -abs(kkk) });
			cout << "x1=" << b / (-2.*a) << " + " << abs(kkk) << "i\n";
			cout << "x2=" << b / (-2.*a) << " - " << abs(kkk) << "i\n";
		}
	}
	return x;
}

vector<pair<double, double>> SolveCubic(double a, double b, double c, double d)
{
	vector<pair<double, double>> x(3);
	if (a == 0)
	{
		cout << "AS SQUARED bx2+cx+d=0\n";
		x = SolveSquared(b, c, d);
	}
	else
	{
		const double M_PI = atan(1) * 4;
		//-----
		double p = (3.0*a*c - b*b) / (3.0*a*a);
		double q = (2.0*b*b*b - 9.0*a*b*c + 27.0*a*a*d) / (27.0*a*a*a);
		double S = (q*q / 4.0) + (p*p*p / 27.0);
		//-----
		double F;
		if (q == 0)
			F = M_PI / 2.0;
		if (q < 0)
			F = atan(-2.0*sqrt(-S) / q);
		if (q > 0)
			F = atan(-2.0*sqrt(-S) / q) + M_PI;
		//-----
		for (int i = 0; i < 3; i++)
			x[i].first = x[i].second = 0;
		if (S < 0)
		{
			x[0].first = 2.0*sqrt(-p / 3.0)*cos(F / 3.0) - b / (3.0*a);
			x[1].first = 2.0*sqrt(-p / 3.0)*cos((F / 3.0) + 2.0*M_PI / 3.0) - b / (3.0*a);
			x[2].first = 2.0*sqrt(-p / 3.0)*cos((F / 3.0) + 4.0*M_PI / 3.0) - b / (3.0*a);
			cout << "x1=" << x[0].first << "\n";
			cout << "x2=" << x[1].first << "\n";
			cout << "x3=" << x[2].first << "\n";
		}
		if (S == 0)
		{
			x[0].first = 2.0*cbrt(-q / 2.0) - b / (3.0*a);
			x[1].first = -cbrt(-q / 2.0) - b / (3.0*a);
			x[2].first = -cbrt(-q / 2.0) - b / (3.0*a);
			cout << "x1=" << x[0].first << "\n";
			cout << "x2=" << x[1].first << "\n";
		}
		if (S > 0)
		{
			double temp1 = cbrt((-q / 2.0) + sqrt(S)) + cbrt((-q / 2.0) - sqrt(S));
			double temp2 = cbrt((-q / 2.0) + sqrt(S)) - cbrt((-q / 2.0) - sqrt(S));
			x[0].first = temp1 - b / (3.0*a);
			x[1].first = -temp1 / 2.0 - b / (3.0*a); x[1].second = sqrt(3)*temp2 / 2.0;
			x[2].first = -temp1 / 2.0 - b / (3.0*a); x[2].second = -sqrt(3)*temp2 / 2.0;
			cout << "x1=" << x[0].first << "\n";
			cout << "x2=" << x[1].first << "+" << abs(x[1].second) << "i\n";
			cout << "x3=" << x[2].first << "-" << abs(x[1].second) << "i\n";
		}
	}
	return x;
}


UINT64 GCD(UINT64 a, UINT64 b)
{
	if (a == 0) return b;
	if (b == 0) return a;
	if (a == b + 1 || a == b - 1) return 1;
	if (a == 1 || b == 1) return 1;

	return GCD(min(a, b), llabs((UINT64)(a - b)));
}

UINT64 LCM(UINT64 a, UINT64 b)
{
	return a * b / GCD(a, b);
}

string DVS(UINT64 a)
{
	string s;
	for (int i = 2; i < (a / 2) + 1; ++i)
		if (a%i == 0)
			s = s + (IPR(i)?"P":"") + to_string(i) + " ";
	return s;
}

bool IPR(UINT64 a)
{
	for (int i = 2; i < (a / 2) + 1; ++i)
		if (a%i == 0)
			return false;
	return true;
}
