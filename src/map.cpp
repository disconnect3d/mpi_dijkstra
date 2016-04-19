#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

struct Map {
private:
    Map(int verticesCount) : weights(verticesCount) {
        for(int i=0; i<verticesCount; ++i)
            weights[i].reserve(verticesCount);
    }
    std::vector<std::vector<int>> weights;

public:
    static Map fromFile(const std::string& str, const char delimiter=',') {
        return fromFile(std::ifstream(str.c_str()), delimiter);
    }

    static Map fromFile(std::ifstream&& istream, const char delimiter=',') {
        std::string str;

        if (std::getline(istream, str, ':') && str == "vertices" && std::getline(istream, str)) {
            int verticesCount = std::stoi(str);
            
            Map m(verticesCount);        

            for(auto i=0; i<verticesCount; ++i)
                for(auto j=0; j<verticesCount; ++j) {
                    std::getline(istream, str, delimiter);
                    m.weights[i][j] = std::stoi(str);
                }

            return m;
        }

        throw std::runtime_error("Wrong map file format");
    }

    decltype(weights.size()) getSize() const { return weights.size(); }
    const decltype(weights) getWeights() const { return weights; }

    void printWeights() const {
        for(auto i=0u; i<getSize(); ++i) {
            for(auto j=0u; j<getSize(); ++j)
                std::cout << std::setw(4) << weights[i][j] << " ";

            std::cout << std::endl;
        }
    }
};
