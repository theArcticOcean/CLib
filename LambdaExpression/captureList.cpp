#include <algorithm>
#include <iostream>
using namespace std;

int main()
{
	int a = 12;
	// // reference capture, it can capture changing but can't change variable.
	// auto ref_fun = [&a]() -> int {  a += 100; return a; };
	// cout << ref_fun() << ", " << a << endl;		// 112, 12
	// a = 120;
	// cout << ref_fun() << ", " << a << endl;		// 220, 120

	// // value capture, the parameter value will not change once variable is transported into param. 
	// auto val_fun = [a]() { cout << a << endl; };
	// val_fun();		// 220
	// a = 100;
	// val_fun();		// 220

	// Following ways use invisible capture.

	// reference capture, it can capture changing but can't change variable.
	auto ref_fun = [&]() -> int {  a += 100; return a; };
	cout << ref_fun() << ", " << a << endl;		// 112, 12
	a = 120;
	cout << ref_fun() << ", " << a << endl;		// 220, 120

	// value capture, the parameter value will not change once variable is transported into param. 
	auto val_fun = [=]() { cout << a << endl; };
	val_fun();		// 220
	a = 100;
	val_fun();		// 220

	return 0;
}
