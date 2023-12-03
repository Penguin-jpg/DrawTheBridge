#pragma once
#include <SFML/Graphics.hpp>

// to apply forces and velcoity
struct RigidBody
{
	float mass = 1.0f;
	sf::Vector2f velocity;
	sf::Vector2f acceleration;

	RigidBody() :mass(1.0f), velocity({ 0.0f, 0.0f }), acceleration({ 0.0f, 0.0f }) {};
	RigidBody(float mass, sf::Vector2f velocity) :mass(mass), velocity(velocity), acceleration({ 0.0f, 0.0f }) {}

	void applyForce(sf::Vector2f force)
	{
		// F = m * a -> a = F / m
		sf::Vector2f a = force / mass;
		acceleration += a;
	}

	void resetAcceleration()
	{
		acceleration = { 0.0f, 0.0f };
	}
};