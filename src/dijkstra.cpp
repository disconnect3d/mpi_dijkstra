#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <stdexcept>
#include <limits>

#include "dijkstra.h"
#include "map.h"

#ifdef DEBUG_DIJKSTRA

#define LOG(s) std::cout << "Dijkstra: " << s << std::endl

#define LOGv(v) do { \
    std::cout << #v << " = [ "; \
    for(auto i=0u; i<v.size(); ++i) \
        std::cout << v[i] << " "; \
    std::cout << "]" << std::endl; \
} while(0)

#else

#define LOG(s)
#define LOGv(v)

#endif // DEBUG_DIJKSTRA

static const auto INF = std::numeric_limits<int>::max();

void dijkstra(const Map& m, const std::string& initialNodeName, const std::string& goalNodeName) {
    const auto& weights = m.getWeights();
    const auto& nodesNames = m.getNodesNames();
    auto nodesCount = nodesNames.size();

    std::vector<int> nodes(nodesCount);
    std::vector<int> distances(nodesCount);
    std::vector<int> prevNodes(nodesCount);

    std::set<int> unvisited;
    for(auto node=0u; node<nodesCount; ++node) {
        unvisited.insert(node);
        nodes[node] = node;

        distances[node] = INF;
        prevNodes[node] = INF;
    }
    distances[0] = 0;

    auto indexOf = [&] (auto nodeName) { return std::find(nodesNames.begin(), nodesNames.end(), nodeName) - nodesNames.begin(); };
    auto isVisited = [&] (auto node) { return unvisited.find(node) == unvisited.end(); };
    auto isNeighbour = [&] (auto currentNode, auto node) { return weights[currentNode][node] != -1; };

    auto initialNode = indexOf(initialNodeName);
    auto currentNode = initialNode;
    auto goalNode = indexOf(goalNodeName);

    while (1) {
        for(auto node=0u; node<nodesCount; ++node) {
            if (isVisited(node)) {
                LOG("Node " << node << " already visited - skipping.");
                continue;
            }

            if (isNeighbour(currentNode, node)) {
                auto nodeDistance = weights[currentNode][node];
                auto totalCostToNode = distances[currentNode] + nodeDistance;

                LOG("Node " << node << " is neighbour of " << currentNode << " (distance: " << nodeDistance << ", totalCostToNode: " << totalCostToNode << ")");
                if (totalCostToNode < distances[node]) {
                    distances[node] = totalCostToNode;
                    prevNodes[node] = currentNode;
                    LOG("New total cost is less then the old, replacing");
                }
            }
        }

        if (currentNode == goalNode) {
            std::cout << "Goal node found" << std::endl;

            LOGv(distances);
            LOGv(prevNodes);

            std::vector<int> stack;
            while(currentNode != initialNode) {
                stack.push_back(currentNode);

                auto prev = prevNodes[currentNode];
                currentNode = prev;
            }

            LOGv(stack);
            std::cout << "Total cost: " << distances[goalNode] << std::endl;
            std::cout << "Path: " << nodesNames[currentNode];



            for(auto it=stack.rbegin(); it != stack.rend(); ++it) {
                auto nextNodeName = nodesNames[*it];
                std::cout << " -> " << nextNodeName;
            }
            std::cout << std::endl;

            break;
        }

        LOG("Visited " << currentNode);
        unvisited.erase(currentNode);

        auto minCost = std::numeric_limits<int>::max(); 
        auto nextNode = -1;

        for(auto node=0u; node<nodesCount; ++node) {
            auto totalCost = distances[node];

            if (!isVisited(node) && totalCost < minCost) {
                minCost = totalCost;
                nextNode = node;
            }
        }
        
        currentNode = nextNode;
        LOG("Next currentNode = " << currentNode);

        if (currentNode == -1) {
            std::cout << "Path not found" << std::endl;
            return;
        }
    }
}
