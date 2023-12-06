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
		applyGravity();
		//fillCollisionGrid();
		//solveGridCollision();
		solveCollisions();
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

civ::Ref<Particle> Solver::addParticle(sf::Vector2f position, float radius, bool pinned)
{
	civ::ID id = particles.emplace_back(position, radius, pinned);
	return particles.createRef(id);
}

const civ::IndexVector<Particle>& Solver::getParticles()
{
	return particles;
}

const int Solver::getNumParticles()
{
	return particles.size();
}

civ::Ref<Link> Solver::addLink(civ::Ref<Particle> p1, civ::Ref<Particle> p2)
{
	civ::ID id = links.emplace_back(p1, p2, Math::getLength(p1->currentPosition - p2->currentPosition));
	return links.createRef(id);
}

const civ::IndexVector<Link>& Solver::getLinks()
{
	return links;
}

const int Solver::getNumLinks()
{
	return links.size();
}

void Solver::addCube(sf::Vector2f position, bool soft, bool pinned)
{
	// use offset so that cube will be drawn at exactly that position
	float offset = 10.0f;

	// a cube is composed of 9 particles and 12 links
	// I use more particles in order to simulate more dynamic motion
	civ::Ref<Particle> p1 = addParticle({ position.x - offset, position.y - offset }, 5.0f, pinned);
	civ::Ref<Particle> p2 = addParticle({ position.x, position.y - offset }, 5.0f, pinned);
	civ::Ref<Particle> p3 = addParticle({ position.x + offset, position.y - offset }, 5.0f, pinned);
	civ::Ref<Particle> p4 = addParticle({ position.x - offset, position.y }, 5.0f, pinned);
	civ::Ref<Particle> p5 = addParticle({ position.x , position.y }, 5.0f, pinned);
	civ::Ref<Particle> p6 = addParticle({ position.x + offset, position.y }, 5.0f, pinned);
	civ::Ref<Particle> p7 = addParticle({ position.x - offset, position.y + offset }, 5.0f, pinned);
	civ::Ref<Particle> p8 = addParticle({ position.x, position.y + offset }, 5.0f, pinned);
	civ::Ref<Particle> p9 = addParticle({ position.x + offset, position.y + offset }, 5.0f, pinned);
	addLink(p1, p2);
	addLink(p2, p3);
	addLink(p1, p4);
	addLink(p2, p5);
	addLink(p3, p6);
	addLink(p4, p5);
	addLink(p5, p6);
	addLink(p4, p7);
	addLink(p5, p8);
	addLink(p6, p9);
	addLink(p7, p8);
	addLink(p8, p9);
	// if this is not a soft box, add links for every diagonal to strengthen it
	if (!soft)
	{
		addLink(p1, p5);
		addLink(p5, p9);
		addLink(p3, p5);
		addLink(p5, p7);
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
