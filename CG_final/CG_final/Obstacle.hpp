#pragma once
#include <utility>
#include <SFML/Graphics.hpp>

struct Obstacle
{
	bool isForward = true, moveHorizontal = true;
	std::pair<int, int> particleIndexRange;
	std::pair<int, int> horizontalBound;
	std::pair<int, int> verticalBound;

	float movingSpeed = 100.0f;

	Obstacle() = default;
	Obstacle(bool isForward, bool moveHorizontal, std::pair<int, int> particleIndexRange,
		std::pair<int, int> horizontalBound, std::pair<int, int> verticalBound, float movingSpeed)
		:isForward(isForward), moveHorizontal(moveHorizontal), particleIndexRange(particleIndexRange),
		horizontalBound(horizontalBound), verticalBound(verticalBound), movingSpeed(movingSpeed) {}
};
