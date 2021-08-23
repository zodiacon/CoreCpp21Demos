// AsyncIo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <coroutine>
#include <Windows.h>

using namespace std;

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
	void return_value(const T& value) {
		_value = value;
	}

	T _value;
};

template<typename T = void>
struct task {
	using promise_type = promise<T>;

	task(std::coroutine_handle<promise<T>> handle) : _handle(handle) {}
	T resume() {
		return _handle.resume();
	}
	T is_ready() const {
		return _handle.done();
	}

	T value() {
		return _handle.promise()._value;
	}

private:
	std::coroutine_handle<promise<T>> _handle;
};

template<>
struct task<void> {
	using promise_type = promise<void>;
	task() = default;
	task(std::coroutine_handle<promise<void>> handle) {}
};

struct AsyncIoAwaiter : suspend_always {
	AsyncIoAwaiter(HANDLE hFile, DWORD offset, void* buffer, DWORD size) : _hFile(hFile), _buffer(buffer), _size(size) {
		_ov.Offset = offset;
		_ov.hEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
	}

	void await_suspend(std::coroutine_handle<> handle) {
		::ReadFile(_hFile, _buffer, _size, nullptr, &_ov);
		::RegisterWaitForSingleObject(&_hReg, _ov.hEvent, [](auto param, auto) {
			auto coro = std::coroutine_handle<>::from_address(param);
			coro();
			}, handle.address(), INFINITE, WT_EXECUTEDEFAULT);
	};

	DWORD await_resume() noexcept {
		::UnregisterWait(_hReg);
		DWORD bytes;
		::GetOverlappedResult(_hFile, &_ov, &bytes, TRUE);
		::CloseHandle(_ov.hEvent);
		return bytes;
	}

	OVERLAPPED _ov{};
	HANDLE _hFile;
	void* _buffer;
	DWORD _size;
	HANDLE _hReg;
};

auto ReadFileAsync(HANDLE hFile, DWORD offset, void* buffer, DWORD size) {
	return AsyncIoAwaiter(hFile, offset, buffer, size);
}

task<DWORD> TestIo(PCWSTR path) {
	auto hFile = ::CreateFile(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr);
	if (hFile == INVALID_HANDLE_VALUE)
		co_return 0;

	auto size = ::GetFileSize(hFile, nullptr);
	cout << "File size: " << size << endl;
	auto buffer = std::make_unique<BYTE[]>(size);
	auto bytes = co_await ReadFileAsync(hFile, 0, buffer.get(), size);
	cout << "Bytes read: " << bytes << endl;
	co_return bytes;
}


int main() {
	TestIo(LR"(c:\temp\test2.reg)");
	Sleep(10000);
	return 0;
}

