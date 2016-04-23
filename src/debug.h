#pragma once

#include <iostream>

#ifdef DEBUG

#warning "Compiling in DEBUG mode"
#define LOG(x) do { \
    std::cout << __func__ << ": " << #x << " = '" << (x) << "'" << std::endl; \
} while(0)

#else

#define LOG(x)

#endif // DEBUG
