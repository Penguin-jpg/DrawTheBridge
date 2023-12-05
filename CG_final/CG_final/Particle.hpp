#pragma once
#include <SFML/Graphics.hpp>
#include "RigidBody.hpp"

static int objectCounter = 0;

// basic object
struct Particle
{
	// use an id to identify an object
	int id = 0;
	sf::Vector2f currentPosition;
	sf::Vector2f prevPosition;
	float radius = 1.0f;
	RigidBody rb;

	Particle() = default;

	Particle(sf::Vector2f position, float radius)
		:currentPosition(position), prevPosition(position), radius(radius)
	{
		id = objectCounter;
		objectCounter++;
	}

	void initVelocity(sf::Vector2f v, float dt)
	{
		//rb.velocity = v;
		prevPosition = currentPosition - (v * dt);
	}

	// let Verlet integration to deal with dynamic motion
	void update(float dt)
	{
		// the formula is x_n = x_{n-1} + v_{n-1} * dt + a_n * dt^2

		// v_{n-1} * dt = displacement traveled from previous pos to current pos
		sf::Vector2f displacement = currentPosition - prevPosition;
		//sf::Vector2f displacement = rb.velocity * dt;
		sf::Vector2f newPosition = currentPosition + displacement + rb.acceleration * dt * dt;

		// update position
		prevPosition = currentPosition;
		currentPosition = newPosition;
		// update velocity
		//rb.velocity = (currentPosition - prevPosition) / dt;

		// reset acceleration for next calculation
		rb.resetAcceleration();
	}
};