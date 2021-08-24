// Coroutines.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <vector>
#include <cmath>
#include <iostream>
#include <experimental/generator>
#include <conio.h>

using namespace std::experimental;

bool IsPrime(int n) {
	int limit = (int)std::sqrt(n);
	for (int i = 2; i <= limit; i++)
		if (n % i == 0)
			return false;
	return true;
}

std::vector<int> CalcPrimes(int first, int last) {
	std::vector<int> primes;

	for (int i = first; i <= last; i++)
		if (IsPrime(i))
			primes.push_back(i);
	return primes;
}

generator<int> CalcPrimes2(int first, int last) {
	for (int i = first; i <= last; i++)
		if (IsPrime(i))
			co_yield i;
}

template<std::unsigned_integral T = unsigned>
generator<T> Fibonacci(unsigned count) {
	if (count == 0)
		co_return;

	co_yield 1;
	co_yield 1;

	T a = 1, b = 1;
	for (unsigned i = 0; i < count - 2; i++) {
		auto c = a + b;
		co_yield c;
		a = b;
		b = c;
	}
}

template<std::unsigned_integral T = unsigned>
void TestFib(unsigned count) {
	for (auto n : Fibonacci<T>(count)) {
		if (tolower(_getch()) == 'q')
			break;
		std::cout << n << std::endl;
	}
}

void TestPrimes() {
	for (auto n : CalcPrimes2(3, 100000000)) {
		std::cout << n << std::endl;
		if (n > 500)
			break;
	}
}

void TestFib2() {
	auto fib = Fibonacci(10);
	auto it = fib.begin();
	auto n = *it;
	it++;
	n = *it;
}

int main() {
	auto fib = Fibonacci(10);

	TestFib<unsigned long long>(30);

	return 0;
}

