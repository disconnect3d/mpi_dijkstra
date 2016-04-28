#pragma once

#include <string>

#include "map.h"

void dijkstra(const Map& m, const std::string& initialNodeName, const std::string& goalNodeName, const int mpiNodesCount);

void dijkstraWorker(int mpiNodeId, int mpiWorkerNodes);
