#include <iostream>

int main() {
    // 普通字符串
    const char* narrowStr = "Hello, World!";
    std::cout << narrowStr << std::endl;

    // 宽字符字符串
    const wchar_t* wideStr = L"Hello, World!";
    std::wcout << wideStr << std::endl;

    return 0;
}