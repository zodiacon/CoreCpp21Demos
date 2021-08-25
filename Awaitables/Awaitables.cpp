// Awaitables.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <coroutine>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#define __cpp_lib_format
#include <format>

using namespace std;
using namespace std::chrono_literals;

template<typename T>
struct promise {
	auto get_return_object() {
		return std::coroutine_handle<promise<T>>::from_promise(*this);
	}

	auto initial_suspend() {
		return std::suspend_never();
	}

	auto final_suspend() noexcept {
		return std::suspend_always();
	}

	void unhandled_exception() {}
	void return_void() {}
};


struct task {
	using promise_type = promise<void>;
	task() = default;
	task(std::coroutine_handle<promise<void>> handle) {}
};

struct resume_new_thread : std::suspend_always {
	void await_suspend(std::coroutine_handle<> handle) {
		thread([handle]() { handle(); }).detach();
	}
};

struct DelayAwaiter : std::suspend_always {
	DelayAwaiter(chrono::milliseconds ms) : _ms(ms) {}

	void await_suspend(std::coroutine_handle<> coro) {
		thread t([coro](auto ms) {
			std::this_thread::sleep_for(ms);
			coro();
			}, _ms);
		t.detach();
	}

	chrono::milliseconds _ms;
};

struct Delay {
	Delay(chrono::milliseconds ms) : _ms(ms) {}

	auto operator co_await() {
		return DelayAwaiter(_ms);
	}

	chrono::milliseconds _ms;
};


string GetTime() {
	return format("{}", chrono::system_clock::to_time_t(chrono::system_clock::now()));
}

task TestDelay() {
	cout << "Before waiting... " << GetTime() << "TID: " << this_thread::get_id() << endl;
	cout << "TID: " << this_thread::get_id() << endl;
	co_await resume_new_thread();
	cout << "TID: " << this_thread::get_id() << endl;

	co_await resume_new_thread();
	cout << "TID: " << this_thread::get_id() << endl;

	co_await Delay(2s);
	cout << "After 2 sec delay... " << GetTime() << endl;
	co_await Delay(3s);
	cout << "After 3 sec delay... " << GetTime() << endl;
	co_await Delay(2s);
	cout << "After 2 sec delay... " << GetTime() << endl;
}

int main() {
	TestDelay();
	cout << "TestDelay returns\n";

	this_thread::sleep_for(5s);
	cout << "Main thread does something\n";

	this_thread::sleep_for(5s);
	cout << "Main thread does another something\n";

	this_thread::sleep_for(5s);

	return 0;
}

