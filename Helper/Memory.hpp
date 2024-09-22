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
	void Init() noexcept;
	void Update() noexcept;
	void readValue();
	std::uintptr_t base{0};
	std::uintptr_t will{0};
	std::uintptr_t attack{0};
	std::vector<offset_reader>  values
	{

	};
private:


};
