#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>

// some utility functions and constants about math
struct Math
{
	static constexpr float PI = 3.14159265358979323846f;

	static float getDistance(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		float dx = a.x - b.x;
		float dy = a.y - b.y;
		return sqrt(dx * dx + dy * dy);
	}

	static float getLength(const sf::Vector2f& v)
	{
		return sqrt(v.x * v.x + v.y * v.y);
	}
};
