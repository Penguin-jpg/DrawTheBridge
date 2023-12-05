#include "Solver.hpp"
#include "Math.hpp"
#include <iostream>

Solver::Solver(sf::Vector2f size, float margin)
	:worldSize(size), margin(margin), grid(size.x, size.y) {}

void Solver::update()
{
	elapsedTime += frameDt;

	for (int i = 0; i < subSteps; i++)
	{
		//applyGravity();
		//fillCollisionGrid();
		//solveGridCollision();
		solveCollisions();
		//applyConstraint();
		updateParticles(stepDt);
		updateLinks(stepDt);
	}
}

void Solver::applyGravity()
{
	for (Particle& particle : particles)
	{
		particle.rb.applyForce(gravity);
	}
}

void Solver::updateParticles(float dt)
{
	for (Particle& particle : particles)
	{
		particle.update(dt);
		solveCollisionWithWorld(particle);
	}
}

void Solver::updateLinks(float dt)
{
	for (Link& link : links)
	{
		link.update(dt);
	}
}

void Solver::applyForce(float radius, sf::Vector2f position)
{
	for (Particle& particle : particles)
	{
		sf::Vector2f direction = particle.currentPosition - position;
		float distance = Math::getLength(direction);
		std::cout << distance << std::endl;
		if (distance < radius)
		{
			particle.rb.applyForce(1.f * (radius - distance) * direction);
		}
	}
}

Particle& Solver::addParticle(sf::Vector2f position, float radius)
{
	// in c++17, insertion return a reference of the appended particle
	// note that another insertion will invalidate the reference
	return particles.emplace_back(position, radius);
}

std::vector<Particle>& Solver::getParticles()
{
	return particles;
}

const int Solver::getNumParticles()
{
	return particles.size();
}

Link& Solver::addLink(Particle* p1, Particle* p2)
{
	return links.emplace_back(p1, p2, Math::getLength(p1->currentPosition - p2->currentPosition));
}

const std::vector<Link>& Solver::getLinks()
{
	return links;
}

const int Solver::getNumLinks()
{
	return links.size();
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
	for (Particle& particle : particles)
	{
		// direction from current position of particle to center of constraint
		sf::Vector2f direction = constraintCenter - particle.currentPosition;
		float distance = Math::getLength(direction);
		// if particle is outside of contraint, put it back along the unit direction vector
		if (distance > (constraintRadius - particle.radius))
		{
			std::cout << "!\n";
			sf::Vector2f unit = direction / distance;
			// constraintRadius - particle.radius = how far the particle out of the constraint
			particle.currentPosition = constraintCenter - unit * (constraintRadius - particle.radius);
		}
	}
}

const sf::Vector3f Solver::getWorld()
{
	return { worldSize.x, worldSize.y, margin };
}

void Solver::solveCollisionWithWorld(Particle& particle)
{
	// if particle out of world, put it back
	if (particle.currentPosition.x > worldSize.x - margin)
	{
		particle.currentPosition.x = worldSize.x - margin;
	}
	else if (particle.currentPosition.x < margin)
	{
		particle.currentPosition.x = margin;
	}
	if (particle.currentPosition.y > worldSize.y - margin)
	{
		particle.currentPosition.y = worldSize.y - margin;
	}
	else if (particle.currentPosition.y < margin)
	{
		particle.currentPosition.y = margin;
	}
}

void Solver::solveCollisions()
{
	// strength of bouncing response when colliding
	const float responseStrength = 0.75f;

	// naive O(n^2) method
	for (int i = 0; i < particles.size(); i++)
	{
		Particle& p1 = particles[i];

		for (int j = i + 1; j < particles.size(); j++)
		{
			Particle& p2 = particles[j];

			sf::Vector2f direction = p1.currentPosition - p2.currentPosition;
			float distance = Math::getLength(direction);
			// the min distance to not collide is the sum of radius
			const float minDistance = p1.radius + p2.radius;

			if (distance < minDistance)
			{
				sf::Vector2f unit = direction / distance;
				// use radius as ratio of bouncing
				float total = p1.radius + p2.radius;
				float ratio1 = p1.radius / total;
				float ratio2 = p2.radius / total;

				// distance to push to separate two particles
				// distance - minDistance = overlappinig distance between two particles
				// times 0.5 because each particles only need to move away half of that distance
				float delta = responseStrength * 0.5f * (distance - minDistance);

				p1.currentPosition -= unit * (ratio2 * delta);
				p2.currentPosition += unit * (ratio1 * delta);
			}
		}
	}
}

void Solver::fillCollisionGrid()
{
	// initialize the grid
	grid.clearGrid();

	for (Particle& particle : particles)
	{
		grid.addObject(particle);
	}
}

void Solver::solveGridCollision()
{
	// we will check every neighbor so start from 1 to prevent out of bound
	for (int row = 1; row < grid.numRows - 1; row++)
	{
		for (int col = 1; col < grid.numCols - 1; col++)
		{
			// current cell
			CollisionCell& currentCell = grid.getCell(row, col);

			// check its neighbors
			for (int i = -1; i <= 1; i++)
			{
				for (int j = -1; j <= 1; j++)
				{
					CollisionCell& neighborCell = grid.getCell(row + i, col + j);
					solveCellCollision(currentCell, neighborCell);
				}
			}
		}
	}
}

void Solver::solveCellCollision(CollisionCell& cell1, CollisionCell& cell2)
{
	for (Particle* p1 : cell1.objects)
	{
		for (Particle* p2 : cell2.objects)
		{
			if (p1 != nullptr && p2 != nullptr && p1->id != p2->id)
			{
				solveParticleCollision(p1, p2);
			}
		}
	}
}

void Solver::solveParticleCollision(Particle* p1, Particle* p2)
{
	// strength of bouncing response when colliding
	constexpr float responseCoef = 0.75f;

	sf::Vector2f direction = p1->currentPosition - p2->currentPosition;
	float distance = Math::getLength(direction);
	// the min distance to not collide is the sum of radius
	const float minDistance = p1->radius + p2->radius;

	if (distance < minDistance)
	{
		sf::Vector2f unit = direction / distance;
		// use radius as ratio of bouncing
		float total = p1->radius + p2->radius;
		float ratio1 = p1->radius / total;
		float ratio2 = p2->radius / total;

		// distance to push to separate two particles
		// distance - minDistance = overlappinig distance between two particles
		// times 0.5 because each particles only need to move away half of that distance
		float delta = responseCoef * 0.5f * (distance - minDistance);

		p1->currentPosition -= unit * (ratio2 * delta);
		p2->currentPosition += unit * (ratio1 * delta);
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
