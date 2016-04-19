#pragma once
#include <vector>

struct Map {
private:
    Map(int verticesCount);

    std::vector<std::vector<int>> weights;

public:
    static Map fromFile(const std::string& str, const char delimiter=',');
    static Map fromFile(std::ifstream&& istream, const char delimiter=',');

    decltype(weights.size()) getSize() const { return weights.size(); }
    const decltype(weights) getWeights() const { return weights; }

    void printWeights() const;
};
