#pragma once
#include "Particle.hpp"
#include "Math.hpp"
#include "ConstantIndexVector/index_vector.hpp"

struct Link
{
	civ::Ref<Particle> p1, p2;
	// length of the link (also the max distance that two particles can separate)
	float length;

	Link(civ::Ref<Particle> p1, civ::Ref<Particle> p2, float length) :p1(p1), p2(p2), length(length) {}

	void update(float dt)
	{
		sf::Vector2f direction = p1->currentPosition - p2->currentPosition;
		float distance = Math::getLength(direction);
		// if the distance between two particles is further than length, put them back
		sf::Vector2f unit = direction / distance;
		const float delta = length - distance;
		// only need to calculate when particle not pinned
		if (!p1->pinned)
			p1->currentPosition += 0.5f * delta * unit;
		if (!p2->pinned)
			p2->currentPosition -= 0.5f * delta * unit;
	}
};