#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "ConstantIndexVector/index_vector.hpp"
#include "Particle.hpp"
#include "Constraint.hpp"
#include "Wind.hpp"
#include "CollisionGrid.hpp"

// this class is in charge of physics
class Solver
{
public:
	Solver(sf::Vector2f size, float particleRadius, int cellSize);

	// functions for simulation
	void update();
	void applyGravity();
	void updateParticles(float dt);
	void updateConstraints(float dt);

	// creation and getters
	civ::Ref<Particle> addParticle(const sf::Vector2f& position, bool pinned = false);
	const civ::IndexVector<Particle>& getParticles();
	const int getNumParticles();

	civ::Ref<Constraint> addConstraint(civ::Ref<Particle> p1, civ::Ref<Particle> p2, float distance = -1.0f);
	const civ::IndexVector<Constraint>& getConstraints();
	const int getNumLinks();

	civ::Ref<Wind> addWind(const sf::Vector2f& position, const sf::Vector2f& size, float speed, float strength);
	const civ::IndexVector<Wind>& getWinds();

	const sf::Vector3f getWorld();
	CollisionGrid& getGrid();

	// collision functions
	void fillCollisionGrid();
	void solveCollisions();
	void solveGridCollision();
	void solveCellCollision(CollisionCell& cell1, CollisionCell& cell2);
	void solveParticleCollision(Particle* p1, Particle* p2);
	void solveCollisionWithWorld(Particle& particle);

	// additional physics effect functions
	void applyWind();
	void applyForce(float radius, const sf::Vector2f& position);

	// utility functions
	bool isValidPosition(const sf::Vector2f& position);
	civ::Ref<Particle> getClickedParticle(const sf::Vector2f& clickedPosition);
	void addCube(const sf::Vector2f& position, bool pinned = false);
	void addChain(const sf::Vector2f& position, float chainLength);
	void addCircle(const sf::Vector2f& poisition, float radius, int numParticles);

	// timing functions
	const float getElapsedTime();
	void setFrameDt(const int framerate);
	void setSubSteps(const int subSteps);
	const float getStepDt();


private:
	sf::Vector2f gravity{ 0.0f, 1000.0f };
	civ::IndexVector<Particle> particles;
	civ::IndexVector<Constraint> constraints;
	civ::IndexVector<Wind> winds;
	// world box
	sf::Vector2f worldSize;
	float particleRadius = 1.0f;
	// collision grid
	CollisionGrid grid;
	// timer
	float elapsedTime = 0.0f;
	float frameDt = 0.0f;
	// sub-steps (steps to do per frame) for more precise simulation
	int numSubSteps = 1;
	float stepDt = 0.0f;
};