#pragma once
#include <vector>

const int NO_EDGE = -1;

struct Map {
private:
    Map(int verticesCount);

    std::vector<std::string> nodesNames;
    std::vector<std::vector<int>> weights;

public:
    static Map fromFile(const std::string& str, const char delimiter=',');
    static Map fromFile(std::ifstream&& istream, const char delimiter=',');

    decltype(weights.size()) getSize() const { return weights.size(); }
    const decltype(weights) getWeights() const { return weights; }

    const decltype(nodesNames) getNodesNames() const { return nodesNames; }

    void printWeights() const;
};
