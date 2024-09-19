#pragma warning(disable : 6387 4715)
#include <Windows.h>
#include <string>
#include <codecvt>
#include <iostream>
#include "gui.hpp"
#include <thread>
#include "CheatManager.hpp"

bool WINAPI HideThread(const HANDLE hThread) noexcept
{
	__try {
		using FnSetInformationThread = NTSTATUS(NTAPI*)(HANDLE ThreadHandle, UINT ThreadInformationClass, PVOID ThreadInformation, ULONG ThreadInformationLength);
		const auto NtSetInformationThread{ reinterpret_cast<FnSetInformationThread>(::GetProcAddress(::GetModuleHandleW(L"ntdll.dll"), "NtSetInformationThread")) };

		if (!NtSetInformationThread)
			return false;

		if (const auto status{ NtSetInformationThread(hThread, 0x11u, nullptr, 0ul) }; status == 0x00000000)
			return true;
	}
	__except (TRUE) {
		return false;
	}
	return false;
}


__declspec(safebuffers) static void WINAPI DllAttach([[maybe_unused]] LPVOID lp) noexcept
{
	using namespace std::chrono_literals;

	cheatManager.start();
	if (HideThread(::GetCurrentThread()))
		cheatManager.logger->addLog("Thread Hidden!\n");
	cheatManager.memory->Init();
	cheatManager.logger->addLog("memory init successful!\n");
	cheatManager.hooks->install();
	cheatManager.logger->addLog("hook install successful!\n");

	while (true)
		std::this_thread::sleep_for(2500ms);
	cheatManager.logger->addLog("ExitProcess\n");
	::ExitProcess(0u);
}


__declspec(safebuffers) BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	if (ul_reason_for_call != DLL_PROCESS_ATTACH) // 这里必须加上，dll、新线程创建和卸载时都会调这个函数。调用原因就是这个参数
		return FALSE;
	DisableThreadLibraryCalls(hModule);
	std::setlocale(LC_ALL, ".utf8");
	HideThread(hModule);
	::_beginthreadex(nullptr, 0u, reinterpret_cast<_beginthreadex_proc_type>(DllAttach), nullptr, 0u, nullptr);
	::CloseHandle(hModule);
}

