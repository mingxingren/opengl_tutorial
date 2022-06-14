#ifndef COMMON_FUNC_H
#define COMMON_FUNC_H

#include <iostream>

constexpr float G_PI = 3.14159265358979323846f;

static void log_cout(const std::string& _msg) {
    std::cout << _msg << std::endl;
}

#endif // COMMON_FUNC_H