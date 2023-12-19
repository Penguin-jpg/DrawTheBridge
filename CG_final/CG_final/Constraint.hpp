#pragma once
#include "Particle.hpp"
#include "Math.hpp"
#include "ConstantIndexVector/index_vector.hpp"

// constraint to hold two particles together
struct Constraint
{
	civ::Ref<Particle> p1, p2;
	// length of the constraint (also the max distance that two particles can separate)
	float length = 1.0f;

	Constraint() = default;
	Constraint(civ::Ref<Particle> p1, civ::Ref<Particle> p2, float length)
		:p1(p1), p2(p2), length(length) {}


	bool isValid()
	{
		// constraint is only valid when two particle pointers are not nullptr
		return p1 && p2;
	}

	void update(float dt)
	{
		if (!isValid())
			return;

		sf::Vector2f direction = p1->currentPosition - p2->currentPosition;
		float distance = Math::getLength(direction);
		// if the distance between two particles is further than length, put them back
		sf::Vector2f unit = direction / distance;
		const float delta = 0.5f * (length - distance);
		// only update distance if particles are not pinned
		if (!p1->pinned)
			p1->move(unit * delta);
		if (!p2->pinned)
			p2->move(-unit * delta);
	}
};