#pragma once
#include <SFML/Graphics.hpp>
#include "Particle.hpp"
#include <vector>
#include <unordered_map>
#include <array>
#include <iostream>

constexpr int CELL_CAPACITY = 10;

struct CollisionCell
{
	int numObjects = 0;
	//Particle* objects[CELL_CAPACITY] = {};
	std::array<Particle*, CELL_CAPACITY> objects = { nullptr };
	//std::vector<Particle*> objects;

	void addObject(Particle* object)
	{
		objects[numObjects] = object;
		//objects.push_back(object);
		numObjects = numObjects + 1 >= CELL_CAPACITY ? numObjects : numObjects + 1;
	}

	void clear()
	{
		//objects.clear();
		numObjects = 0;
	}
};

struct CollisionGrid
{
	int width, height, numRows, numCols, cellSize;
	std::vector<CollisionCell> grid;

	CollisionGrid(int width, int height, int cellSize)
		:width(width), height(height),
		numRows(ceil((float)height / cellSize)), numCols(ceil((float)width / cellSize)), cellSize(cellSize)
	{
		grid.resize(numRows * numCols);
	}

	void clearGrid()
	{
		for (CollisionCell& cell : grid)
		{
			cell.clear();
		}
	}

	CollisionCell& getCell(int row, int col)
	{
		int pos = row * numCols + col;
		return grid[pos];
	}

	sf::Vector2i getGridCoordinate(const sf::Vector2f& position, float radius)
	{
		float x = position.x + radius < 0.0f ? 0.0f : position.x > width ? width : position.x;
		float y = position.y + radius < 0.0f ? 0.0f : position.y > height ? height : position.y;
		int row = y / cellSize;
		int col = x / cellSize;
		row = row < 0 ? 0 : row >= numRows ? numRows - 1 : row;
		col = col < 0 ? 0 : col >= numCols ? numCols - 1 : col;
		return { row, col };
	}

	void addObject(Particle& object)
	{
		const sf::Vector2f position = object.currentPosition;
		// top-left side of the object
		//const sf::Vector2f minPosition = position - sf::Vector2f(object.radius, object.radius);
		// bottom-right side of the object
		//const sf::Vector2f maxPosition = position + sf::Vector2f(object.radius, object.radius);

		sf::Vector2i coord = getGridCoordinate(position, object.radius);
		getCell(coord.x, coord.y).addObject(&object);
	}
};