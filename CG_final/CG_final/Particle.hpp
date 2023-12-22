#pragma once
#include <SFML/Graphics.hpp>
#include "ConstantIndexVector/index_vector.hpp"

enum class ParticleColor {
	Deg1,
	Deg2,
	Deg3,
	Deg4,
	Deg5,
	Deg6,
	Deg7,
	Deg8,
	Deg9,
	Deg10,
	Deg11,
	Deg12,
	Deg13,
	Deg14,
	Deg15,
	Deg16,
};

// basic object
struct Particle
{
	// use an id to identify an object
	civ::ID id = 0;
	sf::Vector2f currentPosition;
	sf::Vector2f prevPosition;
	float mass = 1.0f;
	sf::Vector2f velocity;
	sf::Vector2f acceleration;
	float radius = 1.0f;
	// whether the particle can move or not
	bool pinned = false;

	ParticleColor color;
	Particle() = default;

	Particle(sf::Vector2f position, float radius, bool pinned = false)
		:currentPosition(position), prevPosition(position), radius(radius), pinned(pinned), color(ParticleColor::Deg5) {}

	void initVelocity(const sf::Vector2f& v, float dt)
	{
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
		// velocity = (currentPosition - prevPosition) / dt;

		// reset acceleration for next calculation
		acceleration = { 0.0f, 0.0f };
	}

	void applyForce(const sf::Vector2f& force)
	{
		// F = m * a -> a = F / m
		sf::Vector2f a = force / mass;
		acceleration += a;
	}

	void move(const sf::Vector2f& amount)
	{
		if (!pinned)
			currentPosition += amount;
	}
};