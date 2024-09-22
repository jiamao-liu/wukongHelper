#include <iostream>
#include <Windows.h>
#include "lazy_importer.hpp"
#include <tlhelp32.h>
#include <vector>
#include <array>
#include "xorstr.hpp"
#include <thread>
#include <fstream>

using proclist_t = std::vector<std::uint32_t>;
NTSTATUS NTAPI NtCreateThreadEx(PHANDLE, ACCESS_MASK, LPVOID, HANDLE, LPTHREAD_START_ROUTINE, LPVOID, BOOL, SIZE_T, SIZE_T, SIZE_T, LPVOID);
int injector();
int main() {
	injector();
	
	for (int i = 10;i > 0;i--) {
		std::cout << "exit "<<i<<"s "<<std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	::ExitProcess(0u);
}

int injector() {
	std::wstring name_value = L"b1-Win64-Shipping.exe";
	std::wstring dll = L"\\Helper.dll";
	std::wstring& name = name_value;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize = sizeof(fontInfo);
	GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);

	fontInfo.dwFontSize.X = 12; // 设置字体宽度
	fontInfo.dwFontSize.Y = 16; // 设置字体高度

	SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);


	proclist_t list;
	while (list.size() == 0) {
		auto process_snap{ LI_FN(CreateToolhelp32Snapshot)(TH32CS_SNAPPROCESS, 0) };
		if (process_snap == INVALID_HANDLE_VALUE)
		{
			MessageBoxW(NULL, L"无法获取系统进程快照", L"错误", MB_OK | MB_ICONINFORMATION);
			return -1;
		}

		PROCESSENTRY32W pe32{};
		pe32.dwSize = sizeof(PROCESSENTRY32W);

		if (LI_FN(Process32FirstW).get()(process_snap, &pe32)) {
			if (pe32.szExeFile == name)
				list.push_back(pe32.th32ProcessID);

			while (LI_FN(Process32NextW).get()(process_snap, &pe32)) {
				if (pe32.szExeFile == name)
					list.push_back(pe32.th32ProcessID);
			}
		}
		LI_FN(CloseHandle)(process_snap);
		std::cout << "not found game windows ,watting game client " << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}


	std::cout << "successful! found " << list.size() << "process ：" << " ";
	for (auto it = list.begin(); it != list.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	const std::uint32_t process_id = list.back();

	const auto handle{ LI_FN(OpenProcess)(PROCESS_ALL_ACCESS, false,process_id) };
	if (!handle || handle == INVALID_HANDLE_VALUE) {
		DWORD dwError = GetLastError();
		std::cerr << "Error opening process. Error code: " << dwError << std::endl;
	}
	std::cout << "successful! open game handle" << std::endl;


	FILETIME ft{};
	SYSTEMTIME st{};
	LI_FN(GetSystemTime)(&st);
	LI_FN(SystemTimeToFileTime)(&st, &ft);
	FILETIME create{}, exit{}, kernel{}, user{};
	LI_FN(GetProcessTimes)(handle, &create, &exit, &kernel, &user);

	const auto delta{ 10 - static_cast<std::int32_t>((*reinterpret_cast<std::uint64_t*>(&ft) - *reinterpret_cast<std::uint64_t*>(&create.dwLowDateTime)) / 10000000U) };
	std::cout << delta << std::endl;
	if (delta > 0)
		std::this_thread::sleep_for(std::chrono::seconds(delta));

	TCHAR current_dir[MAX_PATH];
	LI_FN(GetCurrentDirectoryW)(MAX_PATH, current_dir);
	const auto dll_path{ std::wstring(current_dir) + dll };
	if (const auto f{ std::ifstream(dll_path) }; !f.is_open()) {
		std::cout << "dll file could not be found. file path:" << std::endl;
		std::wcout << dll_path << std::endl;
		LI_FN(CloseHandle)(handle);
		return false;
	}
	const auto dll_path_remote{ LI_FN(VirtualAllocEx).get()(handle, nullptr, (dll_path.size() + 1) * sizeof(wchar_t), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE) };
	if (!dll_path_remote) {
		std::cout << "fail alloc memory!" << std::endl;
		DWORD dwError = GetLastError();
		std::cerr << "Error alloc memory. Error code: " << dwError << std::endl;
		LI_FN(CloseHandle)(handle);
	}

	if (!LI_FN(WriteProcessMemory).get()(handle, dll_path_remote, dll_path.data(), (dll_path.size() + 1) * sizeof(wchar_t), nullptr)) {
		std::cout << "fail write all path data!" << std::endl;
		LI_FN(VirtualFreeEx).get()(handle, dll_path_remote, 0u, MEM_RELEASE);
		LI_FN(CloseHandle)(handle);
		return false;
	}
	auto lpLoadLibraryA = reinterpret_cast<LPTHREAD_START_ROUTINE>(LI_FN(GetProcAddress).get()(LI_FN(GetModuleHandleW).get()(_XorStrW(L"kernel32.dll")), _XorStr("LoadLibraryW")));
	HANDLE thread{};
	LI_FN(NtCreateThreadEx).nt_cached()(
		&thread, 
		GENERIC_ALL, 
		nullptr,
		handle, 
		lpLoadLibraryA,
		dll_path_remote, FALSE, NULL, NULL, NULL, nullptr);


	if (!thread || thread == INVALID_HANDLE_VALUE) {
		std::cout << "fail start thead" << std::endl;
		LI_FN(VirtualFreeEx).get()(handle, dll_path_remote, 0u, MEM_RELEASE);
		LI_FN(CloseHandle)(handle);
		return false;
	}
	std::cout << "successful! create dll therd" << thread << std::endl;
	LI_FN(WaitForSingleObject)(thread, INFINITE);
	LI_FN(CloseHandle)(thread);
	LI_FN(VirtualFreeEx).get()(handle, dll_path_remote, 0u, MEM_RELEASE);
	LI_FN(CloseHandle)(handle);
	return 0;
}