#pragma once
#include "Node.h"

#include <vector>
#include <unordered_map>

class AStarPathFinder {
public:
	static void PathFinder(std::vector<Node> Graph, std::unordered_map<Node, Node> &came_from, std::unordered_map<Node, double> &cost_so_far);
	static std::vector<Node> Reconstruct_Path(const std::vector<Node> &Graph, std::unordered_map<Node,Node> &came_from);
	static std::vector<Node> neighbor(const std::vector<Node> &Graph, const Node &node);
	static inline double heuristic(Node a, Node b);


private:
};
