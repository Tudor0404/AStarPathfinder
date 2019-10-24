#include <vector>
#include <algorithm>
#include <iostream>
#include "Grid.h"
#include "Node.h"

// Grid constructor
Grid::Grid(int _xLength, int _yLength) {
	xLength = _xLength;
	yLength = _yLength;
}

// Resets the grid by resizing it to the x/y length
// Reset the Nodes and initializes them
// Clears the open and closed vectors
void Grid::resetGrid(bool onlyCosts) {
	if (!onlyCosts) {
		nodeGrid.clear();
		nodeGrid.resize(yLength, std::vector<Node>(xLength));

		for (int y = 0; y < nodeGrid.size(); y++) {
			for (int x = 0; x < nodeGrid[y].size(); x++) {
				nodeGrid[y][x].x = x;
				nodeGrid[y][x].y = y;
			}
		}

	} else {
		for (int y = 0; y < nodeGrid.size(); y++) {
			for (int x = 0; x < nodeGrid[y].size(); x++) {
				nodeGrid[y][x].g = 0;
				nodeGrid[y][x].h = 0;
			}
		}
	}
	openNodes.clear();
	closedNodes.clear();
	path.clear();
}

// Gets the neighbours of a Node by getting all the walkable Nodes around it
// using an offset
std::vector<Node*> Grid::getNeighbours(Node* node) {
	std::vector<Node*> neighbours;
	int x = node->x;
	int y = node->y;

	for (int yOffset = -1; yOffset <= 1; yOffset++) {
		for (int xOffset = -1; xOffset <= 1; xOffset++) {

			if (xOffset == 0 && yOffset == 0)
				continue;

			int newX = x + xOffset;
			int newY = y + yOffset;
			if (newY >= 0 && newY < yLength && newX >= 0 && newX < xLength)
				neighbours.push_back(&nodeGrid[newY][newX]);

		}
	}

	return neighbours;
}

// Get distance between two nodes by first getting the difference in x/y
// then multiply the smallest by 14 and the difference between biggest and smallest 
// by 10, then add them together
int Grid::getDistance(Node* nodeA, Node* nodeB) {
	int distX = abs(nodeA->x - nodeB->x);
	int distY = abs(nodeA->y - nodeB->y);

	if (distX > distY)
		return 14 * distY + 10 * (distX - distY);
	return 14 * distX + 10 * (distY - distX);
}

void Grid::search() {
	Node* currentNode = nullptr;

	// Set costs for start nodes
	startNode->h = getDistance(startNode, endNode);
	startNode->g = 0;

	// Push start node in the open node vector
	openNodes.push_back(startNode);


	while (openNodes.size() > 0) {
		// Get current node by one with smallest f cost
		currentNode = openNodes[0];
		for (Node* node : openNodes) {
			if (node->f() < currentNode->f() || node->f() == currentNode->f()) {
				if (node->h < currentNode->h)
					currentNode = node;
			}
		}

		openNodes.erase(std::remove(openNodes.begin(), openNodes.end(), currentNode), openNodes.end());
		closedNodes.push_back(currentNode);

		// Check if current node is the end node
		if (currentNode == endNode) {
			generatePath();
			return;
		}

		// Go through all the neighbours
		for (Node* node : getNeighbours(currentNode)) {
			// If the node is in the closed set or is impassable, continue
			if (node->state == 1 || std::find(closedNodes.begin(), closedNodes.end(), node) != closedNodes.end())
				continue;

			// Calculate the new f cost of the new node
			int newGCost = node->g + getDistance(currentNode, node);
			if (newGCost < node->g || std::find(openNodes.begin(), openNodes.end(), node) == openNodes.end()) {
				node->g = newGCost;
				node->h = getDistance(node, endNode);
				node->parent = currentNode;

				if (std::find(openNodes.begin(), openNodes.end(), node) == openNodes.end())
					openNodes.push_back(node);
			}
		}
	}
}

void Grid::generatePath() {
	std::vector<std::array<int, 2>> _path;
	Node* currentNode = endNode;

	while (currentNode != startNode) {
		_path.push_back({ currentNode->y, currentNode->x });
		currentNode = currentNode->parent;
	}

	std::reverse(_path.begin(), _path.end());

	path = _path;
}

void Grid::alterNode(int x, int y, int action) {
	Node* selectedNode = &nodeGrid[y][x];

	selectedNode->state = 0;

	switch (action) {
		// Create obstacle
		case 0:
		if (selectedNode == startNode)
			startNode = nullptr;
		if (selectedNode == endNode)
			startNode = nullptr;
		selectedNode->state = 1;
		break;

		// Set start Node
		case 1:
		if (selectedNode == endNode)
			endNode = nullptr;
		startNode = selectedNode;
		break;

		// Set end Node
		case 2:
		if (selectedNode == startNode)
			startNode = nullptr;
		endNode = selectedNode;
		break;

		// walkable Node
		case 3:
		if (selectedNode == startNode)
			startNode = nullptr;
		if (selectedNode == endNode)
			endNode = nullptr;
		selectedNode->state = 0;
		break;
	}

}
