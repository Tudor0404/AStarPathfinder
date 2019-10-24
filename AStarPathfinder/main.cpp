#include <iostream>
#include <SFML\Graphics.hpp>
#include <vector>
#include <cmath>
#include "Grid.h"
#include "Node.h"

int main() {
	Grid grid(40, 40);
	grid.resetGrid();

	// Options for SFML
	int cellSize = 30;
	bool displaygrid = true;
	std::vector<sf::RectangleShape> cellsToDisplay;

	int currentMouseCell[2] = { 0,0 }; // x, y
	bool changed = true;

	sf::RenderWindow window(sf::VideoMode(grid.xLength * cellSize, grid.yLength * cellSize), "A* search algorithm");

	// main SFML loop
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			// Get current mouse cell position
			else if (event.type == sf::Event::MouseMoved) {
				currentMouseCell[0] = std::round(sf::Mouse::getPosition(window).x / cellSize);
				currentMouseCell[1] = std::round(sf::Mouse::getPosition(window).y / cellSize);
			}

		}
		window.clear(sf::Color(255, 255, 255));

		// Generate path
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			if (grid.startNode != nullptr && grid.endNode != nullptr && changed) {
				grid.resetGrid(true);
				grid.search();
				changed = false;
			}
		}

		// Check if mouse is within boundaries
		else if (currentMouseCell[0] >= 0 && currentMouseCell[0] < grid.xLength && currentMouseCell[1] >= 0 && currentMouseCell[1] < grid.yLength) {
			// Place obstacle
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				grid.alterNode(currentMouseCell[0], currentMouseCell[1], 0);
				changed = true;
				grid.resetGrid(true);
			}

			// Set start Node
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				grid.alterNode(currentMouseCell[0], currentMouseCell[1], 1);
				changed = true;
				grid.resetGrid(true);
			}

			// Set end Node
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
				grid.alterNode(currentMouseCell[0], currentMouseCell[1], 2);
				changed = true;
				grid.resetGrid(true);
			}

			// Walkable Node
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				grid.alterNode(currentMouseCell[0], currentMouseCell[1], 3);
				changed = true;
				grid.resetGrid(true);
			}
		}

		// ---- Display Node Grid ----

		// Path in yellow
		if (grid.path.size() != 0) {
			for (auto pos : grid.path) {
				sf::RectangleShape gridTile(sf::Vector2f((float)cellSize, (float)cellSize));
				gridTile.setFillColor(sf::Color(200, 200, 0));
				gridTile.setPosition(sf::Vector2f((float)(pos[1] * cellSize), (float)(pos[0] * cellSize)));
				window.draw(gridTile);
			}
		}

		// Obstacles in black, start Node in green, end Node in red
		for (auto nodes : grid.nodeGrid) {
			for (auto node : nodes) {
				if (node.state == 1) {
					sf::RectangleShape obstacle(sf::Vector2f((float)cellSize, (float)cellSize));
					obstacle.setFillColor(sf::Color(0, 0, 0));
					obstacle.setPosition(sf::Vector2f((float)(node.x * cellSize), (float)(node.y * cellSize)));
					window.draw(obstacle);
				}
			}
		}

		if (grid.startNode != nullptr) {
			sf::RectangleShape start(sf::Vector2f((float)cellSize, (float)cellSize));
			start.setFillColor(sf::Color(0, 255, 0));
			start.setPosition(sf::Vector2f((float)(grid.startNode->x * cellSize), (float)(grid.startNode->y * cellSize)));
			window.draw(start);
		}

		if (grid.endNode != nullptr) {
			sf::RectangleShape end(sf::Vector2f((float)cellSize, (float)cellSize));
			end.setFillColor(sf::Color(255, 0, 0));
			end.setPosition(sf::Vector2f((float)(grid.endNode->x * cellSize), (float)(grid.endNode->y * cellSize)));
			window.draw(end);
		}

		// Display a tile grid
		if (displaygrid) {
			for (int x = 0; x < grid.yLength; x++) {
				for (int y = 0; y < grid.xLength; y++) {
					// displays a transparent overlay which alternates the alpha
					sf::RectangleShape gridTile(sf::Vector2f((float)cellSize, (float)cellSize));
					bool isBlack = false;
					if ((y % 2 == 0 && x % 2 == 0) || (y % 2 == 1 && x % 2 == 1))
						isBlack = true;
					if (isBlack)
						gridTile.setFillColor(sf::Color(0, 0, 0, 20));
					else
						gridTile.setFillColor(sf::Color(0, 0, 0, 30));
					gridTile.setPosition(sf::Vector2f((float)(x * cellSize), (float)(y * cellSize)));
					window.draw(gridTile);
				}
			}
		}


		// Display current selected tile
		sf::RectangleShape cursor(sf::Vector2f((float)cellSize, (float)cellSize));
		cursor.setFillColor(sf::Color(0, 0, 0, 50));
		cursor.setPosition(sf::Vector2f((float)(currentMouseCell[0] * cellSize), (float)(currentMouseCell[1] * cellSize)));
		window.draw(cursor);

		window.display();
	}
	return 0;
}