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
    ImGui::Begin("猿神", nullptr, noMouse ? ImGuiWindowFlags_NoMouseInputs :0);
    {
        //ImGui::Text("attack:%.1f", *(float*)cheatManager.memory->attack);
        ImGui::Text("灵蕴:\t%d", 99999);//*(int*)cheatManager.memory->will);
        ImGui::Text("攻击力:\t%d", 132);//*(int*)cheatManager.memory->will);
        ImGui::Text("暴击几率!!!:\t%d", 30);//*(int*)cheatManager.memory->will);
        ImGui::Text("暴击伤害:\t%d", 151);//*(int*)cheatManager.memory->will);
        ImGui::Text("增伤:\t%d", 15);//*(int*)cheatManager.memory->will);
        ImGui::Text("减伤:\t%d", 0);//*(int*)cheatManager.memory->will);
	}
	ImGui::End();

    if (this->setting) {
        ImGui::Begin("设置", nullptr);
        cheatManager.utils->hotkey("面板热键", cheatManager.config->settingMenu);
        ImGui::SliderFloat("字体大小",&this->fontSize_temp,0.5f,2.0f);
        ImGui::SliderFloat("透明度", &this->alpha_temp, 0.0f, 1.0f);
        ImGui::ColorEdit4("颜色", (float*)&this->color_real, ImGuiColorEditFlags_AlphaBar); // Edit 3 floats representing a color
        ImGui::Checkbox("穿透", &this->noMouse);
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
    ImGui::GetStyle().Colors[ImGuiCol_FrameBg].w = alpha_temp;
    ImGui::GetStyle().Colors[ImGuiCol_TitleBg].w = alpha_temp;
}




