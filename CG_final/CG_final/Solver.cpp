#include "Solver.hpp"
#include "Math.hpp"
#include <iostream>

Solver::Solver(sf::Vector2f size, float particleRadius, int cellSize)
	:worldSize(size), particleRadius(particleRadius), grid(size.x, size.y, cellSize) {}

void Solver::update()
{
	elapsedTime += frameDt;

	for (int i = 0; i < numSubSteps; i++)
	{
		applyGravity();
		fillCollisionGrid();
		solveGridCollision();
		//solveCollisions();
		updateParticles(stepDt);
		updateConstraints(stepDt);
	}
}

void Solver::applyGravity()
{
	for (Particle& particle : particles)
	{
		particle.applyForce(gravity);
	}
}

void Solver::applyWind()
{
	for (Wind& wind : winds)
	{
		for (Particle& particle : particles)
		{
			wind.apply(worldSize.x);
			if (wind.insideWind(particle))
			{
				particle.applyForce({ wind.strength , 0.0f });
			}
		}
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

void Solver::updateConstraints(float dt)
{
	for (Constraint& constraint : constraints)
	{
		constraint.update(dt);
	}
}

void Solver::applyForce(float radius, const sf::Vector2f& position)
{
	for (Particle& particle : particles)
	{
		sf::Vector2f direction = particle.currentPosition - position;
		float distance = Math::getLength(direction);
		if (distance < radius)
		{
			particle.applyForce(1.f * (radius - distance) * direction);
		}
	}
}

civ::Ref<Particle> Solver::addParticle(const sf::Vector2f& position, bool pinned)
{
	civ::ID id = particles.emplace_back(position, particleRadius, pinned);
	particles[id].id = id;
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

civ::Ref<Particle> Solver::getClickedParticle(const sf::Vector2f& clickedPosition)
{
	for (Particle& particle : particles)
	{
		if (Math::getDistance(particle.currentPosition, clickedPosition) <= particle.radius)
		{
			return particles.createRef(particle.id);
		}
	}
	// if none of particles are near, return empty Ref
	return civ::Ref<Particle>();
}

civ::Ref<Particle> Solver::getNearestParticle(const sf::Vector2f& position)
{
	float minDistance = 9999.9f;
	civ::Ref<Particle> nearest;
	for (Particle& particle : particles)
	{
		float distance = Math::getDistance(particle.currentPosition, position);
		if (distance < minDistance)
		{
			minDistance = distance;
			nearest = particles.createRef(particle.id);
		}
	}
	return nearest;
}

civ::Ref<Constraint> Solver::addConstraint(civ::Ref<Particle> p1, civ::Ref<Particle> p2, float distance, float strength)
{
	civ::ID id = 0;
	if (distance < 0.0f)
		id = constraints.emplace_back(p1, p2, Math::getLength(p1->currentPosition - p2->currentPosition), strength);
	else
		id = constraints.emplace_back(p1, p2, distance, strength);
	return constraints.createRef(id);
}

const civ::IndexVector<Constraint>& Solver::getConstraints()
{
	return constraints;
}

const int Solver::getNumLinks()
{
	return constraints.size();
}

civ::Ref<Wind> Solver::addWind(const sf::Vector2f& position, const sf::Vector2f& size, float speed, float strength)
{
	civ::ID id = winds.emplace_back(position, size, speed, strength);
	return winds.createRef(id);
}

const civ::IndexVector<Wind>& Solver::getWinds()
{
	return winds;
}


void Solver::addCube(const sf::Vector2f& position, float stiffness, bool pinned)
{
	// use offset so that cube will be drawn at exactly that position
	float offset = 2 * particleRadius;

	// a cube is composed of 9 particles and constraints at every edge and diagonal
	// I use more particles in order to simulate more dynamic motion
	civ::Ref<Particle> p1 = addParticle({ position.x - offset, position.y - offset }, pinned);
	civ::Ref<Particle> p2 = addParticle({ position.x, position.y - offset }, pinned);
	civ::Ref<Particle> p3 = addParticle({ position.x + offset, position.y - offset }, pinned);
	civ::Ref<Particle> p4 = addParticle({ position.x - offset, position.y }, pinned);
	civ::Ref<Particle> p5 = addParticle({ position.x , position.y }, pinned);
	civ::Ref<Particle> p6 = addParticle({ position.x + offset, position.y }, pinned);
	civ::Ref<Particle> p7 = addParticle({ position.x - offset, position.y + offset }, pinned);
	civ::Ref<Particle> p8 = addParticle({ position.x, position.y + offset }, pinned);
	civ::Ref<Particle> p9 = addParticle({ position.x + offset, position.y + offset }, pinned);
	// edges
	addConstraint(p1, p2, -1.0f, stiffness);
	addConstraint(p2, p3, -1.0f, stiffness);
	addConstraint(p1, p4, -1.0f, stiffness);
	addConstraint(p2, p5, -1.0f, stiffness);
	addConstraint(p3, p6, -1.0f, stiffness);
	addConstraint(p4, p5, -1.0f, stiffness);
	addConstraint(p5, p6, -1.0f, stiffness);
	addConstraint(p4, p7, -1.0f, stiffness);
	addConstraint(p5, p8, -1.0f, stiffness);
	addConstraint(p6, p9, -1.0f, stiffness);
	addConstraint(p7, p8, -1.0f, stiffness);
	addConstraint(p8, p9, -1.0f, stiffness);
	// main diagonal
	addConstraint(p1, p5, -1.0, stiffness);
	addConstraint(p2, p6, -1.0, stiffness);
	addConstraint(p4, p8, -1.0, stiffness);
	addConstraint(p5, p9, -1.0, stiffness);
}

void Solver::addChain(const sf::Vector2f& position, float chainLength)
{
	// calculate how much particles needed
	int numParticles = chainLength / (2 * particleRadius);
	float offset = 2 * particleRadius;
	sf::Vector2f chainPosition(position.x - offset, position.y - offset);

	std::vector<civ::Ref<Particle>> ps(numParticles);
	// manually create the first one
	ps[0] = addParticle(chainPosition, true);
	// create rest ones along the y axis
	for (int i = 1; i < numParticles; i++)
	{
		chainPosition += sf::Vector2f(5.0f, offset);
		ps[i] = addParticle(chainPosition);
	}
	// add constraints
	for (int i = 1; i < numParticles; i++)
	{
		addConstraint(ps[i - 1], ps[i]);
	}
}

void Solver::addCircle(const sf::Vector2f& position, float radius, int numParticles, float stiffness, bool pinCenter, bool pinOuter)
{
	// small angle for every particle (2 * pi / numParticles)
	float delta = 2.0f * Math::PI / float(numParticles);
	// center of circle
	civ::Ref<Particle> center = addParticle(position, pinCenter);
	// store outer particles
	std::vector<civ::Ref<Particle>> outerParticles(numParticles);

	for (int i = 0; i < numParticles; i++)
	{
		float x = radius * std::cos(i * delta);
		float y = radius * std::sin(i * delta);
		civ::Ref<Particle> particle = addParticle({ position.x + x, position.y + y }, pinOuter);
		outerParticles[i] = particle;
		// add constraint to center
		addConstraint(particle, center, -1.0f, stiffness);
	}

	// connect every outer particle with their adjacent particles
	for (int i = 0; i < numParticles; i++)
	{
		// connect adjacent and a further particle to make sure the structure is strong
		// reference: https://github.com/subprotocol/verlet-js/blob/master/lib/objects.js#L102
		int adjacent = (i + 1) % numParticles;
		int far = (i + 5) % numParticles;
		addConstraint(outerParticles[i], outerParticles[adjacent], -1.0f, stiffness);
		addConstraint(outerParticles[i], outerParticles[far], -1.0f, stiffness);
	}
}

bool Solver::isValidPosition(const sf::Vector2f& position)
{
	sf::Vector2i coord = grid.getGridCoordinate(position, particleRadius);
	CollisionCell& cell = grid.getCell(coord.x, coord.y);
	for (int i = 0; i < cell.numObjects; i++)
	{
		if (Math::getDistance(position, cell.objects[i]->currentPosition) < 2 * particleRadius - 2.0f)
		{
			return false;
		}
	}
	return true;
}

const sf::Vector3f Solver::getWorld()
{
	return { worldSize.x, worldSize.y, particleRadius };
}

void Solver::solveCollisionWithWorld(Particle& particle)
{
	// if particle out of world, put it back
	if (particle.currentPosition.x > worldSize.x - particleRadius)
	{
		particle.currentPosition.x = worldSize.x - particleRadius;
	}
	else if (particle.currentPosition.x < particleRadius)
	{
		particle.currentPosition.x = particleRadius;
	}
	if (particle.currentPosition.y > worldSize.y - particleRadius)
	{
		particle.currentPosition.y = worldSize.y - particleRadius;
	}
	else if (particle.currentPosition.y < particleRadius)
	{
		particle.currentPosition.y = particleRadius;
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
			// the min distance to not overlap is the sum of radius
			const float minDistance = p1.radius + p2.radius;

			if (distance < minDistance)
			{
				sf::Vector2f unit = direction / distance;
				// use radius as ratio of bouncing
				float total = p1.radius + p2.radius;
				float ratio1 = p1.radius / total;
				float ratio2 = p2.radius / total;

				// distance to push to separate two particles
				// minDistance - distance = overlappinig distance between two particles
				// times 0.5 because each particles only need to move away half of that distance
				float delta = responseStrength * 0.5f * (minDistance - distance);

				p1.move(unit * (ratio2 * delta));
				p2.move(-unit * (ratio1 * delta));
			}
		}
	}
}

CollisionGrid& Solver::getGrid()
{
	return grid;
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
	for (int row = 0; row < grid.numRows; row++)
	{
		for (int col = 0; col < grid.numCols; col++)
		{
			CollisionCell& currentCell = grid.getCell(row, col);
			// check its neighbors
			for (int i = -1; i <= 1; i++)
			{
				for (int j = -1; j <= 1; j++)
				{
					// if neighbor out of bound, check collision inside the current cell
					if (row + i < 0 || row + i >= grid.numRows || col + j < 0 || col + j >= grid.numCols)
					{
						solveCellCollision(currentCell, currentCell);
					}
					else
					{
						CollisionCell& neighborCell = grid.getCell(row + i, col + j);
						solveCellCollision(currentCell, neighborCell);
					}
				}
			}
		}
	}
}

void Solver::solveCellCollision(CollisionCell& cell1, CollisionCell& cell2)
{
	for (int i = 0; i < cell1.numObjects; i++)
	{
		Particle* p1 = cell1.objects[i];
		for (int j = 0; j < cell2.numObjects; j++)
		{
			Particle* p2 = cell2.objects[j];

			if (p1->id != p2->id)
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
	// the min distance to not overlap is the sum of radius
	const float minDistance = p1->radius + p2->radius;

	if (distance < minDistance)
	{
		sf::Vector2f unit = direction / distance;
		// use radius as ratio of bouncing
		float total = p1->radius + p2->radius;
		float ratio1 = p1->radius / total;
		float ratio2 = p2->radius / total;

		// distance to push to separate two particles
		// minDistance - distance = overlappinig distance between two particles
		// times 0.5 because each particles only need to move away half of that distance
		float delta = responseCoef * 0.5f * (minDistance - distance);

		p1->move(unit * (ratio2 * delta));
		p2->move(-unit * (ratio1 * delta));
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

void Solver::setSubSteps(const int subSteps)
{
	numSubSteps = subSteps;
	// calculate time for each sub-step
	stepDt = frameDt / (float)numSubSteps;
}

const float Solver::getStepDt()
{
	return stepDt;
}