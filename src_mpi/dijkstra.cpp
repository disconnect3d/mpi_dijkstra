#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <stdexcept>
#include <limits>

#include <mpi.h>

#include "debug.h"
#include "dijkstra.h"
#include "map.h"

static const auto INF = std::numeric_limits<int>::max();
const int mpiRootId = 0;

std::pair<int, int> getMpiWorkerNodeRanges(int nodesCount, int mpiNodesCount, int mpiNodeId) {
    mpiNodesCount -= 1; // counting only worker nodes
    auto fromNode = (nodesCount / mpiNodesCount) * (mpiNodeId - 1);
    auto toNode = (nodesCount / mpiNodesCount) * mpiNodeId - 1;

    auto restNodes = nodesCount % mpiNodesCount;

    if (mpiNodeId - 1 < restNodes) {
        fromNode += mpiNodeId - 1;
        toNode += mpiNodeId;
    }
    else {
        fromNode += restNodes;
        toNode += restNodes;
    }

    return std::pair<int, int>(fromNode, toNode);
}

void dijkstra(const Map& m, const std::string& initialNodeName, const std::string& goalNodeName, const int mpiNodesCount) {
    const auto& weights = m.getWeights();
    const auto& nodesNames = m.getNodesNames();
    auto nodesCount = nodesNames.size();

    std::vector<int> distances(nodesCount);
    std::vector<int> prevNodes(nodesCount);

    std::set<int> visited;
    for(auto node=0u; node<nodesCount; ++node) {
        distances[node] = INF;
        prevNodes[node] = INF;
    }

    auto indexOf = [&] (auto nodeName) { return std::find(nodesNames.begin(), nodesNames.end(), nodeName) - nodesNames.begin(); };
    auto isVisited = [&] (auto node) { return visited.find(node) != visited.end(); };
    auto isNeighbour = [&] (auto currentNode, auto node) { return weights[currentNode][node] != -1; };

    auto initialNode = static_cast<int>(indexOf(initialNodeName));
    auto currentNode = initialNode;
    auto goalNode = indexOf(goalNodeName);

    int workerNodes = mpiNodesCount - 1;
    int nodesStep = nodesCount / workerNodes;

    std::cout << "Sending initial data to workers..." << std::endl;
    int data[3] = {nodesCount, initialNode, goalNode};
    MPI_Bcast(&data, 3, MPI_INT, mpiRootId, MPI_COMM_WORLD);

    for(auto i=0u; i<nodesCount; ++i)
        MPI_Bcast((int*)&weights[i][0], nodesCount, MPI_INT, mpiRootId, MPI_COMM_WORLD);

    distances[initialNode] = 0;

    while (1) {
        std::cout << "Sending currNode=" << currentNode << " distance=" << distances[currentNode] << std::endl;
        int data[2] = {currentNode, distances[currentNode]};
        MPI_Bcast(&data, 2, MPI_INT, mpiRootId, MPI_COMM_WORLD);

        // receive distances calculated by workers
        for(auto mpiNodeId=1; mpiNodeId<mpiNodesCount; ++mpiNodeId) {
            const auto nodeRanges = getMpiWorkerNodeRanges(nodesCount, mpiNodesCount, mpiNodeId);
            const auto fromNode = nodeRanges.first;
            const auto toNode = nodeRanges.second;
            MPI_Recv(&distances[fromNode], toNode - fromNode + 1, MPI_INT, mpiNodeId, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Recv from " << mpiNodeId << std::endl;
        }

        // test for goal
        if (currentNode == goalNode) {
            std::cout << "Goal node found" << std::endl;
            for(auto mpiNodeId=1; mpiNodeId<mpiNodesCount; ++mpiNodeId) {
                const auto nodeRanges = getMpiWorkerNodeRanges(nodesCount, mpiNodesCount, mpiNodeId);
                const auto fromNode = nodeRanges.first;
                const auto toNode = nodeRanges.second;
                MPI_Recv(&prevNodes[fromNode], toNode - fromNode + 1, MPI_INT, mpiNodeId, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
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
        visited.insert(currentNode);

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


void dijkstraWorker(int mpiNodeId, int mpiNodesCount) {
    int data[3];
    MPI_Bcast(&data, 3, MPI_INT, 0, MPI_COMM_WORLD);

    int nodesCount = data[0];
    int initialNode = data[1];
    int goalNode = data[2];

    std::cout << "mpiID=" << mpiNodeId << " bcast recv nodesCount=" << nodesCount << " initialNode=" << initialNode << " goalNode=" << goalNode << std::endl;

    std::vector<std::vector<int>> weights(nodesCount);
    std::vector<int> distances(nodesCount, INF);
    std::vector<int> prevNodes(nodesCount, INF);
    std::set<int> visited;

    auto isVisited = [&] (auto node) { return visited.find(node) != visited.end(); };
    auto isNeighbour = [&] (auto currentNode, auto node) { return weights[currentNode][node] != -1; };

    for(auto i=0u; i<nodesCount; ++i) {
        weights[i].resize(nodesCount);
        MPI_Bcast((int*)&weights[i][0], nodesCount, MPI_INT, mpiRootId, MPI_COMM_WORLD);
    }

    const auto nodeRanges = getMpiWorkerNodeRanges(nodesCount, mpiNodesCount, mpiNodeId);
    const auto fromNode = nodeRanges.first;
    const auto toNode = nodeRanges.second;
    std::cout << "mpiID=" << mpiNodeId << " from=" << fromNode << " to=" << toNode << std::endl;

    // real work
    while (1) {
        std::cout << "~~~~" << std::endl;
        MPI_Bcast(&data, 2, MPI_INT, mpiRootId, MPI_COMM_WORLD);

        int currentNode = data[0];
        distances[currentNode] = data[1];
        std::cout << "mpiId=" << mpiNodeId << " bcast recv currNode=" << currentNode << " dist=" << distances[currentNode] << std::endl;

        // goal node not found
        if (currentNode == -1)
            return;

        for (auto node=fromNode; node<=toNode; ++node) {
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

        visited.insert(currentNode);
        MPI_Send(&distances[fromNode], toNode - fromNode + 1, MPI_INT, mpiRootId, 0, MPI_COMM_WORLD);

        // goal node found
        if (currentNode == goalNode) {
            MPI_Send(&prevNodes[fromNode], toNode - fromNode + 1, MPI_INT, mpiRootId, 0, MPI_COMM_WORLD);
            return;
        }
        std::cout << "======" << std::endl;
    }
}
