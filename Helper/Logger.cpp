#include "Logger.hpp"
#include "CheatManager.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <Windows.h>
#include <string>

void WukongLogger::init(){
    TCHAR pathW[MAX_PATH];
    GetModuleFileName(GetModuleHandle(L"Helper.dll"), pathW, MAX_PATH);
    MessageBoxW(NULL, pathW,L"2", MB_OK);
    this->path= std::filesystem::path(path);
    MessageBoxW(NULL, path.c_str(), L"2", MB_OK);
    


    isInited = true;
}

void WukongLogger::addLog(const char* message, ...) noexcept {
    //if (!isInited)
    //    init();
    va_list args;
    va_start(args, message);
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    char timeStr[100];
    struct tm tm_time;
    localtime_s(&tm_time, &time);
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &tm_time);
    std::ofstream logfile("C:\\log.txt", std::ios_base::app);
    char buffer[256];
    
    vsnprintf(buffer, sizeof(buffer), message, args);
    {
        // 写入时间和消息到日志文件
        logfile << "[" << timeStr << "] " << buffer << std::endl;
        logfile.close();
    }
    va_end(args);
}