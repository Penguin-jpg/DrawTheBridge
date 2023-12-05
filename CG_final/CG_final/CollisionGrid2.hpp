#pragma once
#include <SFML/Graphics.hpp>
#include "Object.hpp"
#include <vector>
#include <iostream>

constexpr int CELL_CAPACITY = 4;
constexpr float CELL_SIZE = 30.0f;

struct CollisionCell
{
	int numObjects = 0;
	Object* objects[CELL_CAPACITY] = {};

	void addObject(Object& object)
	{
		objects[numObjects] = &object;
		numObjects = numObjects + 1 >= CELL_CAPACITY ? numObjects : numObjects + 1;
	}

	void clear()
	{
		numObjects = 0;
	}
};

struct CollisionGrid
{
	int numRows, numCols;
	std::vector<CollisionCell> cells;

	CollisionGrid(int width, int height)
		:numRows(ceil((float)height / CELL_SIZE)), numCols(ceil((float)width / CELL_SIZE))
	{
		std::cout << numRows << ", " << numCols << std::endl;
		cells.resize(numRows * numCols);
	}

	void clearGrid()
	{
		for (CollisionCell& cell : cells)
		{
			cell.clear();
		}
	}

	CollisionCell& getCell(int row, int col)
	{
		//std::cout << "row: " << row << "; col: " << col << std::endl;
		int pos = row * numCols + col;
		//std::cout << "pos: " << pos << std::endl;
		return cells[pos];
	}

	void addObject(Object& object)
	{
		const sf::Vector2f position = object.currentPosition;
		int row = position.y / CELL_SIZE;
		int col = position.x / CELL_SIZE;
		CollisionCell& cell = getCell(row, col);
		cell.addObject(object);
	}
};