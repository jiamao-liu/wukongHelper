﻿#pragma once

#include <iostream>
#include "imgui/imgui.h"

class GUI {
public:
	void render() noexcept;

	void reset();

	float fontSize_real = 1.0f;
	float fontSize_temp = 1.0f;
	void setFontSize() noexcept;

	bool rainbowFont_real = false;
	bool rainbowFont_temp = false;
	void setRainbowFont() noexcept;

	float alpha_real = 0.2f;
	float alpha_temp = 0.2f;
	void setAlpha() noexcept;

	bool setting = false;
	bool showWindow = true;
	bool noMouse = false;

	ImFont* font;

	ImVec4 color_temp = ImVec4(0.45f, 0.55f, 0.60f, 0.50f);
	ImVec4 color_real = ImVec4(0.45f, 0.55f, 0.60f, 0.50f);
private:
};
