#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "ConstantIndexVector/index_vector.hpp"
#include "Particle.hpp"
#include "Link.hpp"
#include "CollisionGrid2.hpp"

// this class is in charge of simulation
class Solver
{
public:
	Solver(sf::Vector2f size, float margin);

	void update();
	void applyGravity();
	void updateParticles(float dt);
	void updateLinks(float dt);

	void applyForce(float radius, sf::Vector2f position);

	civ::Ref<Particle> addParticle(sf::Vector2f position, float radius, bool pinned = false);
	const civ::IndexVector<Particle>& getParticles();
	const int getNumParticles();

	civ::Ref<Link> addLink(civ::Ref<Particle> p1, civ::Ref<Particle> p2);
	const civ::IndexVector<Link>& getLinks();
	const int getNumLinks();

	void setConstraint(sf::Vector2f center, float radius);
	const sf::Vector3f getConstraint();
	void applyConstraint();

	const sf::Vector3f getWorld();
	void solveCollisionWithWorld(Particle& particle);

	void solveCollisions();
	void fillCollisionGrid();
	void solveGridCollision();
	void solveCellCollision(CollisionCell& cell1, CollisionCell& cell2);
	void solveParticleCollision(Particle* p1, Particle* p2);

	const float getElapsedTime();
	void setFrameDt(const int framerate);

	void setSubSteps(const int _subSteps);
	const float getStepDt();

private:
	sf::Vector2f gravity{ 0.0f, 1000.0f };
	civ::IndexVector<Particle> particles;
	civ::IndexVector<Link> links;
	//std::vector<Particle> particles;
	//std::vector<Link> links;
	// constraint to hold objecst inside
	sf::Vector2f constraintCenter;
	float constraintRadius = 100.0f;
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