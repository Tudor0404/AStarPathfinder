#pragma once
class Node {
public:
	int x{};
	int y{};
	int state = 0; // 0 = walkable, 1 = impassable
	Node* parent = nullptr;

	int g{};
	int h{};
	int f();
};