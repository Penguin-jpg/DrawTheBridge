#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "Object.hpp"

// this class is in charge of simulation
class Solver
{
public:
	Solver() = default;

	void update();
	void applyGravity();
	void updateObjects(float dt);

	Object& addObject(sf::Vector2f position, float radius);
	const std::vector<Object>& getObjects();
	const int getNumObjects();

	void setConstraint(sf::Vector2f center, float radius);
	const sf::Vector3f getConstraint();
	void applyConstraint();

	void checkCollisions(float dt);

	const float getElapsedTime();
	void setFrameDt(const int framerate);

	void setSubSteps(const int _subSteps);
	const float getStepDt();

private:
	sf::Vector2f gravity{ 0.0f, 1000.0f };
	std::vector<Object> objects;
	// constraint to hold objecst inside
	sf::Vector2f constraintCenter;
	float constraintRadius = 100.0f;
	// timer
	float elapsedTime = 0.0f;
	float frameDt = 0.0f;
	// sub-steps (steps to do per frame) for more precise simulation
	int subSteps = 1;
	float stepDt = 0.0f;
};