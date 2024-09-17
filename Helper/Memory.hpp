#pragma once

#include <Windows.h>
#include <cstdint>
#include <d3d11.h>
#include <iostream>
#include <vector>
#include "Offsets.hpp"

class offset_signature {
public:
	std::vector<std::string> pattern;
	bool sub_base;  // 是否减去base
	bool read;  // 是否读取
	bool relative; // 是否为相对偏移
	std::int32_t additional; // 有没有额外偏移
	std::uint64_t* offset; //  偏移地址
};

class offset_reader {
public:
	std::vector<uintptr_t> pattern;
	uintptr_t base;
	uintptr_t* target;
};

class Memory {
public:
	void Init();
	void Update() noexcept;
	std::uintptr_t base;
	std::uintptr_t will;
	std::uintptr_t attack;
private:
	void readValue();
	bool needInit = true;

	std::vector<offset_reader> values
	{
		{
			{0x8,0x18,0x10,0x18,0x10,0x20,0x20},
			0x1D9E1358,
			&this->will
		},
		{
			{0x298,0x138,0x20,0xF0,0x48,0x60,0x284},
			0x1D909380,
			&this->attack
		}
	};
};
