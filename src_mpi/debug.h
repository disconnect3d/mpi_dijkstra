#pragma once

#include <iostream>

#ifdef DEBUG

#warning "Compiling in DEBUG mode"

#define LOG(x) do { \
    std::cout << x << std::endl; \
} while(0)

#define LOGv(v) do { \
    std::cout << #v << " = [ "; \
    for(auto iiii=0u; iiii<v.size(); ++iiii) \
        std::cout << v[iiii] << " "; \
    std::cout << "]" << std::endl; \
} while(0)
#else

#define LOG(x)
#define LOGs(s)

#endif // DEBUG
