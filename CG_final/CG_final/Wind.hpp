#pragma once
#include "Particle.hpp"
#include <iostream>

struct Wind
{
	sf::FloatRect area;
	float speed, strength;

	Wind(sf::Vector2f position, sf::Vector2f size, float speed, float strength)
		:area(position, size), speed(speed), strength(strength) {}

	void apply(float worldWidth)
	{
		// move the wind according to speed
		area.left += speed;
		// if move outside of world, reset the position
		if (area.left > worldWidth)
		{
			area.left = -area.width;
		}
	}

	bool insideWind(Particle& particle)
	{
		return area.contains(particle.currentPosition);
	}
};