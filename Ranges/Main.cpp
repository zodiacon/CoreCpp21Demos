// Ranges.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define __cpp_lib_ranges

#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <ranges>
#include <Windows.h>
#include <wtsapi32.h>

#pragma comment(lib, "wtsapi32")

#define __cpp_lib_format
#include <format>

using namespace std;

struct ProcessInfo {
	std::wstring ProcessName;
	uint32_t Threads;
	uint32_t Id;
	uint32_t Session;
	uint32_t Handles;
	int64_t CpuTime;
};

struct SimpleProcess {
	uint32_t Id;
	std::wstring ProcessName;
	uint32_t Threads;
};

std::wostream& operator<<(std::wostream& out, ProcessInfo const& pi) {
	out << std::format(L"Name: {} PID: {} Threads: {} Session: {} Handles: {}",
		pi.ProcessName, pi.Id, pi.Threads, pi.Session, pi.Handles);
	return out;
}

std::wostream& operator<<(std::wostream& out, SimpleProcess const& pi) {
	out << std::format(L"Name: {} PID: {} Threads: {}",
		pi.ProcessName, pi.Id, pi.Threads);
	return out;
}

std::vector<ProcessInfo> EnumProcesses() {
	DWORD level = 1;
	WTS_PROCESS_INFO_EX* info;
	DWORD count;
	std::vector<ProcessInfo> processes;
	if (::WTSEnumerateProcessesEx(WTS_CURRENT_SERVER_HANDLE, &level, WTS_ANY_SESSION, (LPTSTR*)&info, &count)) {
		processes.reserve(count);
		for (DWORD i = 0; i < count; i++) {
			auto& p = info[i];
			ProcessInfo pi;
			pi.ProcessName = p.pProcessName;
			pi.Id = p.ProcessId;
			pi.Threads = p.NumberOfThreads;
			pi.Session = p.SessionId;
			pi.Handles = p.HandleCount;
			pi.CpuTime = p.UserTime.QuadPart + p.KernelTime.QuadPart;
			processes.push_back(std::move(pi));
		}
		::WTSFreeMemoryEx(WTSTypeProcessInfoLevel1, info, count);
	}
	return processes;
}

void Display(std::string_view text, std::ranges::range auto& data) {
	cout << text << endl;
	for (const auto& value : data)
		cout << value << " ";
	cout << endl;
}

void Display(std::wstring_view text, std::ranges::range auto& data) {
	wcout << text << endl;
	for (const auto& value : data)
		wcout << value << endl;
	wcout << endl;
}

void ProcessesDemo() {
	auto processes = EnumProcesses();
	Display(L"All processes", processes);

	// a filtered view
	auto sessionNonZero = processes | views::filter(
		[](auto const& pi) { return pi.Session > 0; });
	Display(L"Session non zero processes", sessionNonZero);

	// create container based on a view
	auto sessionNonZeroProcesses = vector(begin(sessionNonZero), end(sessionNonZero));
	
	// sort by process name
	ranges::sort(sessionNonZeroProcesses, {}, [](auto const& p) { return p.ProcessName; });
	Display(L"Session non-zero processes - Sorted", sessionNonZeroProcesses);

	auto result = processes
		| views::filter([](auto const& pi) { return pi.Threads > 20; })
		| views::transform([](auto const& p) { return SimpleProcess{ p.Id, p.ProcessName, p.Threads }; });

	auto sp = vector(begin(result), end(result));
	ranges::sort(sp, {}, [](auto const& p) { return p.Threads; });
	auto result2 = sp | views::reverse | views::take(10);
	Display(L"Top processes with most threads (at least 21)", result2);
}

//auto EvenNumbers(std::ranges::view auto const& numbers) {
//	return numbers.filter([](auto n) { return n % 2 == 0; });
//}

auto EvenNumbers(auto const& numbers) {
	return numbers | std::views::filter([](auto n) { return n % 2 == 0; });
}

void SyntacticSugarDemo() {
	vector numbers{ 12, 33, 8, 55, 101, 88, 6, 16, 87, 34 };
	ranges::sort(numbers);

	auto result = numbers 
		| views::filter([](int n) { return n % 2 == 1; })
		| views::take_while([](int n) { return n < 100; });
	Display("With operator | chaining", result);

	auto result2 = views::take_while(
		views::filter(numbers, 
			[](int n) { return n % 2 == 1; }), 
				[](int n) { return n < 100; });
	Display("With function calls", result2);

}

void NumbersDemo() {
	vector numbers{ 12, 33, 8, 55, 101, 88, 6, 16, 87, 34 };

	Display("Original", numbers);
	ranges::sort(numbers);
	Display("Sorted", numbers);

	auto result = numbers | views::filter([](auto n) { return n % 2 == 0; });
	Display("Even numbers", result);

	auto result2 = result | views::transform([](const auto& value) { return value * value; }) | views::reverse;
	Display("Even numbers, squared and reversed", result2);
}

void iotaDemo() {
	auto result = views::iota(1, 100)		// lazily generates numbers from 1 to 100
		| views::transform([](int n) { return n * 2; })
		| views::filter([](int n) { return n % 3 == 0; });
	Display("Result", result);
}

int main() {
	// ProcessesDemo();
	// NumbersDemo
	//SyntacticSugarDemo();
	iotaDemo();

	return 0;
}
