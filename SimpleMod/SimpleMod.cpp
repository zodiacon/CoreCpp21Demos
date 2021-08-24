// SimpleMod.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

import Math.Simple;

int main() {
	using std::cout, std::endl;

	using namespace Math;

	cout << Math::Add(10, 3) << endl;
	Point p1{ 3, 4 };
	Point p2{ 10, -2 };
	cout << Distance(p1, p2) << endl;

	return 0;
}

