
#include <Windows.h>
#include <chrono>
#include <cstdint>
#include <string>
#include <thread>
#include <vector>
#include "CheatManager.hpp"
#include "Memory.hpp"
#include "Offsets.hpp"
#include "Utils.hpp"

[[nodiscard]] static std::uint8_t* find_signature(const wchar_t* szModule, const char* szSignature) noexcept
{
	const auto module{ ::GetModuleHandleW(szModule) };

	using bytes_t = std::vector<std::int32_t>;

	static const auto pattern_to_byte = [](const char* pattern) noexcept -> bytes_t{
			bytes_t bytes{};
			const auto start{ const_cast<char*>(pattern) }; // const_cast 可以将常量字符串指针转为非常量指针
			const auto end{ const_cast<char*>(pattern) + strlen(pattern) };

			for (auto current{ start }; current < end; ++current) {
				if (*current == '?') {
					++current;
					if (*current == '?')
						++current;
					bytes.push_back(-1);
				}
				else {
					bytes.push_back(strtoul(current, &current, 16));
				}
			}

			return bytes;
		};

	const auto dosHeader{ reinterpret_cast<PIMAGE_DOS_HEADER>(module) };
	const auto ntHeaders{ reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module) + dosHeader->e_lfanew) };
	const auto textSection{ IMAGE_FIRST_SECTION(ntHeaders) };

	const auto sizeOfImage{ textSection->SizeOfRawData };
	const auto patternBytes{ pattern_to_byte(szSignature) };
	const auto scanBytes{ reinterpret_cast<std::uint8_t*>(module) + textSection->VirtualAddress };

	const auto s{ patternBytes.size() };
	const auto d{ patternBytes.data() };

	MEMORY_BASIC_INFORMATION mbi{ nullptr };
	const std::uint8_t* next_check_address{ nullptr };

	for (auto i{ 0ul }; i < sizeOfImage - s; ++i) {
		bool found{ true };
		for (auto j{ 0ul }; j < s; ++j) {
			const auto current_address{ scanBytes + i + j };
			if (current_address >= next_check_address) {
				if (!::VirtualQuery(current_address, &mbi, sizeof(mbi)))
					break;

				if (mbi.Protect == PAGE_NOACCESS) {
					i += reinterpret_cast<std::uintptr_t>(mbi.BaseAddress) + mbi.RegionSize - (reinterpret_cast<std::uintptr_t>(scanBytes) + i);
					i--;
					found = false;
					break;
				}
				else {
					next_check_address = static_cast<std::uint8_t*>(mbi.BaseAddress) + mbi.RegionSize;
				}
			}

			if (scanBytes[i + j] != d[j] && d[j] != -1) {
				found = false;
				break;
			}
		}

		if (found)
			return &scanBytes[i];
	}

	return nullptr;

}
void Memory::Init() noexcept {
	this->base = reinterpret_cast<std::uintptr_t>(::GetModuleHandle(nullptr));
	this->readValue();

	/*
	try {
		const auto& signatureToSearch{ this->sigs };
		for (const auto& sig : signatureToSearch)
			*sig.offset = 0;
		while (true) {
			bool missing_offset{ false };
			for (auto& sig : signatureToSearch) {

				if (*sig.offset != 0)
					continue;

				for (auto& pattern : sig.pattern) {
					auto address{ find_signature(nullptr, pattern.c_str()) };

					if (!address) {
						::MessageBoxA(nullptr, ("Failed to find pattern: " + pattern).c_str(), "R3nzSkin", MB_OK | MB_ICONWARNING);
						// cheatManager.logger->addLog("Not found: %s\n", pattern.c_str());
						continue;
					}

					if (sig.read)
						address = *reinterpret_cast<std::uint8_t**>(address + (pattern.find_first_of('?') / 3));
					else if (sig.relative)
						address = address + *reinterpret_cast<std::uint32_t*>(address + 3) + 7;
					else if (address[0] == 0xE8)// E8 貌似是汇编的call，后面跟的是一个跳转地址
						address = address + *reinterpret_cast<std::uint32_t*>(address + 1) + 5;

					if (sig.sub_base)
						address -= this->base;

					address += sig.additional;

					*sig.offset = reinterpret_cast<std::uint32_t>(address);
					// cheatManager.logger->addLog("Found: %s\n\tAddress: 0x%X\n", pattern.c_str(), *sig.offset);
					break;
				}

				if (!*sig.offset) {
					missing_offset = true;
					break;
				}
			}

			if (!missing_offset)
				break;

			//std::this_thread::sleep_for(2500ms);
		}
		this->Update();
	}
	catch (const std::exception& e) {
		::MessageBoxA(nullptr, e.what(), "R3nzSkin", MB_OK | MB_ICONWARNING);
	}
	*/
}

void Memory::readValue() {
	for (auto value : this->values)
	{
		uintptr_t baseAddress = this->base + value.base;
		for (auto offset : value.pattern) {
			baseAddress = (uintptr_t) * (uintptr_t*)baseAddress + offset;
		}
		cheatManager.utils->msg_uintptr(baseAddress, "addr");

		*value.target = baseAddress;
	}
}

void Memory::Update() noexcept {

}