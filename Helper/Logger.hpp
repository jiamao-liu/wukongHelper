#pragma once

#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <filesystem>

class WukongLogger {
public:
    WukongLogger() { this->clear(); }

    void clear() noexcept
    {
    }

    void addLog(const char* message, ...) noexcept
    {
        va_list args;
        va_start(args,message);
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

private:
};
