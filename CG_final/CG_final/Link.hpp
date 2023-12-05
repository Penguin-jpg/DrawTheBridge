#pragma once
#include "Particle.hpp"
#include "Math.hpp"

struct Link
{
	Particle* p1 = nullptr;
	Particle* p2 = nullptr;
	// length of the link (also the max distance that two particles can separate)
	float length;

	Link(Particle* p1, Particle* p2, float length) :p1(p1), p2(p2), length(length) {}

	void update(float dt)
	{
		sf::Vector2f direction = p1->currentPosition - p2->currentPosition;
		float distance = Math::getLength(direction);
		// if the distance between two particles is further than length, put them back
		sf::Vector2f unit = direction / distance;
		const float delta = length - distance;
		p1->currentPosition += 0.5f * delta * unit;
		p2->currentPosition -= 0.5f * delta * unit;
	}
};