#pragma once

#include <concepts>
#include <mutex>
#include <random>
#include <string>

#include "imgui/imgui.h"

class Utils {
public:
    void rainbowText() noexcept;
    void msg_uintptr(uintptr_t p, const char* title);
    void msg_int(int p, const char* title);
};
