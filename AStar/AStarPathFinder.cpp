#include "AStarPathFinder.h"

#include <glm/glm.hpp>

#include <queue>

// priority queue wrapper struct
struct PriorityQueue {
	typedef std::pair<double, Node> PQElement;

	std::priority_queue<
		PQElement, 
		std::vector<PQElement>, 
		std::greater<PQElement> // smallest element at top
	> elements;

	inline bool empty() const
	{
		return elements.empty();
	}

	inline void put(Node item, double priority)
	{
		elements.emplace(priority, item);
	}

	Node get()
	{
		Node best_item = elements.top().second;
		//std::cout << "Node got: " << best_item.ID << " " << "Cost: " << elements.top().first << std::endl;
		elements.pop();
		return best_item;
	}
};

void AStarPathFinder::PathFinder(std::vector<Node> Graph, std::unordered_map<Node, Node> &came_from, std::unordered_map<Node, double> &cost_so_far)
{
	PriorityQueue frontier;
	// find start node
	auto it1 = std::find_if(Graph.begin(), Graph.end(), [](const Node &neighbor) { return neighbor.Type == START; });
	Node start = *it1;

	// find goal node
	auto it2 = std::find_if(Graph.begin(), Graph.end(), [](const Node &neighbor) { return neighbor.Type == GOAL; });
	Node goal = *it2;

	frontier.put(start, 0);
	
	came_from[start] = start;
	cost_so_far[start] = 0;

	while (!frontier.empty()) {
		Node current = frontier.get();
		if(current.Type == GOAL)
			break;

		for (Node next : AStarPathFinder::neighbor(Graph, current)) {
			double new_cost = cost_so_far[current] + 1;
			// if node is not visited or less expensive to visit again
			if (cost_so_far.find(next) == cost_so_far.end()/* || new_cost < cost_so_far[next]*/) {
				cost_so_far[next] = new_cost;
				double priority = new_cost + heuristic(next, goal);
				frontier.put(next, priority);
				came_from[next] = current;
			}
		}
	}
}

std::vector<Node> AStarPathFinder::Reconstruct_Path(const std::vector<Node> &Graph, std::unordered_map<Node,Node> &came_from)
{
	std::vector<Node> path;
	std::pair<Node, Node> temp;

	// find start node
	auto it1 = std::find_if(Graph.begin(), Graph.end(), [](const Node &neighbor) { return neighbor.Type == START; });
	Node start = *it1;
	// find goal node
	auto it2 = std::find_if(Graph.begin(), Graph.end(), [](const Node &neighbor) { return neighbor.Type == GOAL; });
	Node goal = *it2;

	Node current = goal;
	while (current.ID != start.ID) {
		path.push_back(current);		
		current = came_from[current];
	}
	path.push_back(start);
	std::reverse(path.begin(), path.end());
	return path;
}

std::vector<Node> AStarPathFinder::neighbor(const std::vector<Node> &Graph, const Node &node)
{
	std::vector<Node> results;
	
	// this function finds neighboring nodes. 
	// right and lower bound calculated by (screen_width[800] or height - unit_width[20] or height of a node)

	// left bound
	if (node.Position.x - 20 >= 0 && node.Type != BLOCK) {
		// find the node object from its id		
		auto it = std::find_if(Graph.begin(), Graph.end(), [&](const Node &neighbor) { 
			return neighbor.Position.x == node.Position.x - 20 && neighbor.Position.y == node.Position.y; 
		});
		Node neigbor = *it;
		results.push_back(neigbor);
	}
	// right bound
	if (node.Position.x + 20 <= 780 && node.Type != BLOCK) {
		auto it = std::find_if(Graph.begin(), Graph.end(), [&](const Node &neighbor)
		{
			return neighbor.Position.x == node.Position.x + 20 && neighbor.Position.y == node.Position.y;
		});		
		Node neigbor = *it;
		results.push_back(neigbor);
	}
	// upper bound
	if (node.Position.y - 20 >= 0 && node.Type != BLOCK) {
		auto it = std::find_if(Graph.begin(), Graph.end(), [&](const Node &neighbor)
		{
			return neighbor.Position.x == node.Position.x && neighbor.Position.y == node.Position.y - 20;
		});		
		Node neigbor = *it;
		results.push_back(neigbor);
	}
	// lower bound
	if (node.Position.y + 20 <= 780 && node.Type != BLOCK) {
		auto it = std::find_if(Graph.begin(), Graph.end(), [&](const Node &neighbor)
		{
			return neighbor.Position.x == node.Position.x && neighbor.Position.y == node.Position.y + 20;
		});		
		Node neigbor = *it;
		results.push_back(neigbor);
	}

	return results;
}

inline double AStarPathFinder::heuristic(Node a, Node b)
{
	return glm::distance(a.Position, b.Position);
}
