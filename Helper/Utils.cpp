
#include <Windows.h>
#include <algorithm>
#include <array>
#include <string_view>
#include <iostream>
#include "imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"

#include "Utils.hpp"

void Utils::rainbowText() noexcept
{
	static float r{ 1.0f };
	static float g{ 0.f };
	static float b{ 0.f };

	if (true) {
		if (r == 1.f && g >= 0.f && b <= 0.f) {
			g += 0.005f;
			b = 0.f;
		}
		if (r <= 1.f && g >= 1.f && b == 0.f) {
			g = 1.f;
			r -= 0.005f;
		}
		if (r <= 0.f && g == 1.f && b >= 0.f) {
			r = 0.f;
			b += 0.005f;
		}
		if (r == 0.f && g <= 1.f && b >= 1.f) {
			b = 1.f;
			g -= 0.005f;
		}
		if (r >= 0.f && g <= 0.f && b == 1.f) {
			g = 0.f;
			r += 0.005f;
		}
		if (r >= 1.f && g >= 0.f && b <= 1.f) {
			r = 1.f;
			b -= 0.005f;
		}
		ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(r, g, b, 1.00f);
	}
	else {
		if (auto& clr{ ImGui::GetStyle().Colors[ImGuiCol_Text] }; clr.x != 0.92f && clr.y != 0.92f && clr.z != 0.92f)
			clr = ImVec4(0.92f, 0.92f, 0.92f, 0.92f);
	}
}

void Utils::msg_uintptr(uintptr_t p, const char* title) {

	char buffer[300];
	sprintf_s(buffer, "0x%p", p);
	MessageBoxA(NULL, buffer, title, MB_OK);
}

void Utils::msg_int(int p, const char* title) {
	char buffer[300];
	sprintf_s(buffer, "%d", p);
	MessageBoxA(NULL, buffer, title, MB_OK);
}