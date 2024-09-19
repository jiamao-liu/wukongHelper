#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>

#include <ranges>

#include "imgui/imgui.h"
#include <iostream>
#include "gui.hpp"
#include "Utils.hpp"
#include "CheatManager.hpp"

void GUI::render() noexcept{

    this->reset();
    
	ImGui::Begin("WuKong", nullptr);
	{
		if (ImGui::BeginTabBar("MyTabBar")) {
            if (ImGui::BeginTabItem("Monitor"))
            {
                ImGui::Text("attack:%.1f", *(float*)cheatManager.memory->attack);
                ImGui::Text("will:%d", *(int*)cheatManager.memory->will);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Setting"))
            {
                ImGui::SliderFloat("font size", &this->fontSize_temp, 0.5f, 2.0f);
                ImGui::SliderFloat("alpha", &this->alpha_temp, 0.0f, 1.0f);
                ImGui::ColorEdit4("page color", (float*)&this->color_real, ImGuiColorEditFlags_AlphaBar); // Edit 3 floats representing a color

                ImGui::Checkbox("Rainbow font", &this->rainbowFont_temp);
                ImGui::EndTabItem();
            }

            // 结束选项卡栏
            ImGui::EndTabBar();
		}
	}
    cheatManager.logger->addLog("render successful!");
	ImGui::End();
}

void GUI::reset(){
    if (this->fontSize_real != this->fontSize_temp)
        setFontSize();
    if (this->rainbowFont_real != this->rainbowFont_temp)
        setRainbowFont();
    if (this->alpha_real != this->alpha_real)
        setAlpha();
}

void GUI::setFontSize() noexcept{
    this->fontSize_real = this->fontSize_temp;
    ImGui::GetIO().FontGlobalScale = this->fontSize_real;
}

void GUI::setRainbowFont() noexcept{
    this->rainbowFont_real = this->rainbowFont_temp;
    //ImGui::rainbowText();
}

void GUI::setAlpha() noexcept{
    this->alpha_real = this->alpha_temp;
}




