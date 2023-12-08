#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "ConstantIndexVector/index_vector.hpp"
#include "Particle.hpp"
#include "Constraint.hpp"
#include "CollisionGrid.hpp"

// this class is in charge of physics
class Solver
{
public:
	Solver(sf::Vector2f size, float margin, int cellSize);

	void update();
	void applyGravity();
	void updateParticles(float dt);
	void updateConstraints(float dt);

	void applyForce(float radius, const sf::Vector2f& position);

	civ::Ref<Particle> addParticle(const sf::Vector2f& position, float radius, bool pinned = false);
	const civ::IndexVector<Particle>& getParticles();
	const int getNumParticles();

	civ::Ref<Constraint> addConstraint(civ::Ref<Particle> p1, civ::Ref<Particle> p2);
	const civ::IndexVector<Constraint>& getConstraints();
	const int getNumLinks();

	void addCube(const sf::Vector2f& position, bool soft = true, bool pinned = false);

	const sf::Vector3f getWorld();
	void solveCollisionWithWorld(Particle& particle);

	void fillCollisionGrid();
	void solveCollisions();
	void solveGridCollision();
	void solveCellCollision(CollisionCell& cell1, CollisionCell& cell2);
	void solveParticleCollision(Particle* p1, Particle* p2);

	const float getElapsedTime();
	void setFrameDt(const int framerate);

	void setSubSteps(const int _subSteps);
	const float getStepDt();

	const CollisionGrid& getGrid()
	{
		return grid;
	}

private:
	sf::Vector2f gravity{ 0.0f, 1000.0f };
	civ::IndexVector<Particle> particles;
	civ::IndexVector<Constraint> constraints;
	// world box
	sf::Vector2f worldSize;
	// collision grid
	CollisionGrid grid;
	float margin = 1.0f;
	// timer
	float elapsedTime = 0.0f;
	float frameDt = 0.0f;
	// sub-steps (steps to do per frame) for more precise simulation
	int subSteps = 1;
	float stepDt = 0.0f;
};