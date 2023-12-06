#pragma once
#include <SFML/Graphics.hpp>

static int objectCounter = 0;

// basic object
struct Particle
{
	// use an id to identify an object
	int id = 0;
	sf::Vector2f currentPosition;
	sf::Vector2f prevPosition;
	float mass = 1.0f;
	sf::Vector2f velocity;
	sf::Vector2f acceleration;
	float radius = 1.0f;
	// whether the particle can move or not
	bool pinned = false;

	Particle() = default;

	Particle(sf::Vector2f position, float radius, bool pinned = false)
		:currentPosition(position), prevPosition(position), radius(radius), pinned(pinned)
	{
		id = objectCounter;
		objectCounter++;
	}

	void initVelocity(const sf::Vector2f& v, float dt)
	{
		//rb.velocity = v;
		prevPosition = currentPosition - (v * dt);
	}

	// let Verlet integration to deal with dynamic motion
	void update(float dt)
	{
		// don't need to calculate for static particle
		if (pinned)
			return;

		// the formula is x_n = x_{n-1} + v_{n-1} * dt + a_n * dt^2

		// v_{n-1} * dt = displacement traveled from previous pos to current pos
		sf::Vector2f displacement = currentPosition - prevPosition;
		//sf::Vector2f displacement = rb.velocity * dt;
		sf::Vector2f newPosition = currentPosition + displacement + acceleration * dt * dt;

		// update position
		prevPosition = currentPosition;
		currentPosition = newPosition;
		// update velocity
		//rb.velocity = (currentPosition - prevPosition) / dt;

		// reset acceleration for next calculation
		acceleration = { 0.0f, 0.0f };
	}

	void applyForce(const sf::Vector2f& force)
	{
		// F = m * a -> a = F / m
		sf::Vector2f a = force / mass;
		acceleration += a;
	}
};