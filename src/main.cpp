#include <iostream>

#include "map.h"
#include "dijkstra.h"

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "Usage: " << argv[0] << " <testcase file>" << std::endl;
        return -1;
    }
    Map m = Map::fromFile(argv[1]);
    
    auto n = m.getNodesNames();
    auto initialNode = *n.begin();
    auto goalNode = *(n.end()-1);

    std::cout << "Dijkstra search algorithm" << std::endl;
    std::cout << "Starting at node: " << initialNode << std::endl;
    std::cout << "Ending at node: " << goalNode << std::endl;
    std::cout << "Consecutive nodes (A, B, ...) weights: " << std::endl;
    m.printWeights();

    std::cout << "Searching..." << std::endl;
    dijkstra(m, initialNode, goalNode);

    return 0;
}
