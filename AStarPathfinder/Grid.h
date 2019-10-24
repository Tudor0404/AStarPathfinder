#pragma once
#include <vector>
#include <array>
#include "Node.h"

class Grid {
public:
	int xLength;
	int yLength;
	Node* startNode = nullptr;
	Node* endNode = nullptr;
	Grid(int _xLength, int _yLength);
	std::vector<std::vector<Node>> nodeGrid;
	std::vector<Node*> openNodes;
	std::vector<Node*> closedNodes;
	std::vector<std::array<int, 2>> path;

	void resetGrid(bool onlyCosts = false);
	std::vector<Node*> getNeighbours(Node* node);
	int getDistance(Node* nodeA, Node* nodeB);
	void search();
	void generatePath();
	void alterNode(int x, int y, int action); // 0 = add obstacle, 1 = set start, 2 = set end, 3 = walkable 
};