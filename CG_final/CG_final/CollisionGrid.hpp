#pragma once
#include <vector>
#include "Object.hpp"
#include "Math.hpp"
#include <iostream>
#include <cmath>

struct CollisionCell
{
	std::vector<Object> objects;
};

struct CollisionGrid
{
	// size of cell, used to calculate the number of rows and columns
	float cellSize;
	int numRows;
	int numCols;
	// basically a 2D array
	std::vector<std::vector<CollisionCell>> cells;

	CollisionGrid(sf::Vector2f size, float cellSize) :cellSize(cellSize)
	{
		numRows = ceil((float)size.y / cellSize);
		numCols = ceil((float)size.x / cellSize);
		std::cout << numRows << ", " << numCols << std::endl;
		// resize to reserve space so we can use index to access
		/*cells.resize(numRows);
		for (int i = 0; i < numRows; i++)
		{
			cells[i].resize(numCols);
		}*/
		clearGrid();
	}

	void addObject(const Object& object)
	{
		const sf::Vector2f position = object.currentPosition;
		// find the corresponding row and column of the object
		int row = int(position.y / cellSize);
		int col = int(position.x / cellSize);
		std::cout << row << ", " << col << std::endl;
		cells[row][col].objects.push_back(object);
	}

	void clearGrid()
	{
		cells.clear();
	}

	void checkCellCollisions()
	{
		// we will check every neighbor so start from 1 to prevent out of bound
		for (int row = 1; row < numRows - 1; row++)
		{
			for (int col = 1; col < numCols - 1; col++)
			{
				// current cell
				CollisionCell& currentCell = cells[row][col];

				// check its neighbors
				for (int i = -1; i <= 1; i++)
				{
					for (int j = -1; j <= 1; j++)
					{
						CollisionCell& neighborCell = cells[row + i][col + j];
						solveCellCollision(currentCell, neighborCell);
					}
				}
			}
		}
	}

	void solveCellCollision(CollisionCell& cell1, CollisionCell& cell2)
	{
		for (Object& object1 : cell1.objects)
		{
			for (Object& object2 : cell2.objects)
			{
				if (object1.id != object2.id)
				{
					solveObjectCollision(object1, object2);
				}
			}
		}
	}

	void solveObjectCollision(Object& object1, Object& object2)
	{
		// strength of bouncing response when colliding
		constexpr float responseCoef = 0.75f;

		sf::Vector2f direction = object1.currentPosition - object2.currentPosition;
		float distance = Math::getLength(direction);
		// the min distance to not collide is the sum of radius
		const float minDistance = object1.radius + object2.radius;

		if (distance < minDistance)
		{
			sf::Vector2f unit = direction / distance;
			// use radius as ratio of bouncing
			float total = object1.radius + object2.radius;
			float ratio1 = object1.radius / total;
			float ratio2 = object2.radius / total;

			// distance to push to separate two objects
			// distance - minDistance = overlappinig distance between two objects
			// times 0.5 because each objects only need to move away half of that distance
			float delta = responseCoef * 0.5f * (distance - minDistance);

			object1.currentPosition -= unit * (ratio2 * delta);
			object2.currentPosition += unit * (ratio1 * delta);
		}
	}
};