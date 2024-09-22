#pragma once

#include <filesystem>

class WukongLogger {
public:
    std::filesystem::path path;

    WukongLogger() {
        isInited = false;
    }

    void init();

    void addLog(const char* message, ...) noexcept;


private:
    bool isInited;
};
