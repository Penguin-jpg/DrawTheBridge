#include "Solver.hpp"
#include "Math.hpp"
#include <iostream>

Solver::Solver(sf::Vector2f size, float margin, float cellSize)
	:worldSize(size), margin(margin), grid(size, cellSize) {}

void Solver::update()
{
	elapsedTime += frameDt;

	for (int i = 0; i < subSteps; i++)
	{
		applyGravity();
		solveObjectCollisions();
		//grid.clearGrid();
		//grid.checkCellCollisions();
		//applyConstraint();
		updateObjects(stepDt);
	}
}

void Solver::applyGravity()
{
	for (Object& object : objects)
	{
		object.rb.applyForce(gravity);
	}
}

void Solver::updateObjects(float dt)
{
	for (Object& object : objects)
	{
		object.update(dt);
		solveCollisionWithWorld(object);
	}
}

Object& Solver::addObject(sf::Vector2f position, float radius)
{
	// in c++17, emplace_back return a reference of the appended object
	Object& object = objects.emplace_back(position, radius);
	grid.addObject(object);
	return object;
}

const std::vector<Object>& Solver::getObjects()
{
	return objects;
}

const int Solver::getNumObjects()
{
	return objects.size();
}

void Solver::setConstraint(sf::Vector2f center, float radius)
{
	constraintCenter = center;
	constraintRadius = radius;
}

const sf::Vector3f Solver::getConstraint()
{
	return { constraintCenter.x, constraintCenter.y, constraintRadius };
}

void Solver::applyConstraint()
{
	for (Object& object : objects)
	{
		// direction from current position of object to center of constraint
		sf::Vector2f direction = constraintCenter - object.currentPosition;
		float distance = Math::getLength(direction);
		// if object is outside of contraint, put it back along the unit direction vector
		if (distance > (constraintRadius - object.radius))
		{
			std::cout << "!\n";
			sf::Vector2f unit = direction / distance;
			// constraintRadius - object.radius = how far the object out of the constraint
			object.currentPosition = constraintCenter - unit * (constraintRadius - object.radius);
		}
	}
}

const sf::Vector3f Solver::getWorld()
{
	return { worldSize.x, worldSize.y, margin };
}

void Solver::solveCollisionWithWorld(Object& object)
{
	// if object out of world, put it back
	if (object.currentPosition.x > worldSize.x - margin)
	{
		object.currentPosition.x = worldSize.x - margin;
	}
	else if (object.currentPosition.x < margin)
	{
		object.currentPosition.x = margin;
	}
	if (object.currentPosition.y > worldSize.y - margin)
	{
		object.currentPosition.y = worldSize.y - margin;
	}
	else if (object.currentPosition.y < margin)
	{
		object.currentPosition.y = margin;
	}
}

void Solver::solveObjectCollisions()
{
	// strength of bouncing response when colliding
	const float responseStrength = 0.75f;

	// naive O(n^2) method
	for (int i = 0; i < objects.size(); i++)
	{
		Object& object1 = objects[i];

		for (int j = i + 1; j < objects.size(); j++)
		{
			Object& object2 = objects[j];

			sf::Vector2f direction = object1.currentPosition - object2.currentPosition;
			float distance = Math::getLength(direction);
			// the min distance to not collide is the sum of radius
			const float minDistance = object1.radius + object2.radius;

			if (distance < minDistance)
			{
				sf::Vector2f unit = direction / distance;
				// use radius as ratio of bouncing
				float total = object1.radius + object2.radius;
				float ratio1 = object1.radius / total;
				float ratio2 = object2.radius / total;

				// distance to push to separate two objects
				// distance - minDistance = overlappinig distance between two objects
				// times 0.5 because each objects only need to move away half of that distance
				float delta = responseStrength * 0.5f * (distance - minDistance);

				object1.currentPosition -= unit * (ratio2 * delta);
				object2.currentPosition += unit * (ratio1 * delta);
			}
		}
	}
}

const float Solver::getElapsedTime()
{
	return elapsedTime;
}

void Solver::setFrameDt(const int framerate)
{
	frameDt = 1.0 / (float)framerate;
}

void Solver::setSubSteps(const int _subSteps)
{
	subSteps = _subSteps;
	// calculate time for each sub-step
	stepDt = frameDt / (float)subSteps;
}

const float Solver::getStepDt()
{
	return stepDt;
}
