#include <iostream>

#include "map.cpp"

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "Usage: " << argv[0] << "<testcase file>" << std::endl;
        return -1;
    }
    Map::fromFile(argv[1]).printWeights();
    return 0;
}
