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
    ImGui::Begin("WuKong", nullptr, noMouse ? ImGuiWindowFlags_NoMouseInputs :0);
	{
        //ImGui::Text("attack:%.1f", *(float*)cheatManager.memory->attack);
        ImGui::Text("will:\t%d", 9999);//*(int*)cheatManager.memory->will);
        ImGui::Text("asdasdas:\t%d", 12312);//*(int*)cheatManager.memory->will);
        ImGui::Text("todo!!!:\t%d", 11);//*(int*)cheatManager.memory->will);
        ImGui::Text("asdasd:\t%d", 1.151);//*(int*)cheatManager.memory->will);
        ImGui::Text("asdad:\t%d", 15);//*(int*)cheatManager.memory->will);
        ImGui::Text("zx:\t%d", 18474);//*(int*)cheatManager.memory->will);
	}

    cheatManager.logger->addLog("render successful!");
	ImGui::End();

    if (this->setting) {
        ImGui::Begin("Setting", nullptr);
        cheatManager.utils->hotkey("setting hotkey", cheatManager.config->settingMenu);
        ImGui::SliderFloat("font size", &this->fontSize_temp, 0.5f, 2.0f);
        ImGui::SliderFloat("alpha", &this->alpha_temp, 0.0f, 1.0f);
        ImGui::ColorEdit4("page color", (float*)&this->color_real, ImGuiColorEditFlags_AlphaBar); // Edit 3 floats representing a color
        ImGui::Checkbox("no mouse?", &this->noMouse);
        ImGui::End();
    }
}

void GUI::reset(){
    if (this->fontSize_real != this->fontSize_temp)
        setFontSize();
    if (this->rainbowFont_real != this->rainbowFont_temp)
        setRainbowFont();
    if (this->alpha_real != this->alpha_temp)
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
    ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = alpha_temp;
}




