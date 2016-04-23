#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

#include "map.h"
#include "debug.h"

Map::Map(int verticesCount) : weights(verticesCount) {
    for(int i=0; i<verticesCount; ++i)
        weights[i].reserve(verticesCount);
}

Map Map::fromFile(const std::string& str, const char delimiter) {
    return fromFile(std::ifstream(str.c_str()), delimiter);
}

Map Map::fromFile(std::ifstream&& istream, const char delimiter) {
    std::string header;

    if (std::getline(istream, header, ':') && header == "vertices" && std::getline(istream, header)) {
        int verticesCount = std::stoi(header);
        
        Map m(verticesCount);        

        for(auto i=0; i<verticesCount; ++i) {
            std::string line;
            std::getline(istream, line); 

            std::stringstream linestream(line);
            for(auto j=0; j<verticesCount; ++j) {
                std::string numstr;
                std::getline(linestream, numstr, delimiter);

                LOG(i);
                LOG(j);
                LOG(str);
                
                // the `i` node has no edge to `j`
                if (numstr.find("-") != std::string::npos)
                    m.weights[i][j] = NO_EDGE;
                    
                else
                    m.weights[i][j] = std::stoi(numstr);
            }
        }

        return m;
    }

    throw std::runtime_error("Wrong map file format");
}

void Map::printWeights() const {
    for(auto i=0u; i<getSize(); ++i) {
        for(auto j=0u; j<getSize(); ++j)
            if (weights[i][j] != -1)
                std::cout << std::setw(4) << weights[i][j] << " ";
            else
                std::cout << std::setw(4) << "-" << " ";

        std::cout << std::endl;
    }
}
