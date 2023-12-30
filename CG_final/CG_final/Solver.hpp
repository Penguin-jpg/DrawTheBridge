#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "ConstantIndexVector/index_vector.hpp"
#include "Particle.hpp"
#include "Constraint.hpp"
#include "Wind.hpp"
#include "CollisionGrid.hpp"
#include "Obstacle.hpp"

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
	void updateObstacles(float dt);

	// creation and getters
	civ::Ref<Particle> addParticle(const sf::Vector2f& position, bool pinned = false);
	const civ::IndexVector<Particle>& getParticles();
	const int getNumParticles();

	civ::Ref<Constraint> addConstraint(civ::Ref<Particle> p1, civ::Ref<Particle> p2, float distance = -1.0f, float strength = 1.0f);
	const civ::IndexVector<Constraint>& getConstraints();
	const int getNumLinks();

	civ::Ref<Wind> addWind(const sf::Vector2f& position, const sf::Vector2f& size, float speed, float strength);
	const civ::IndexVector<Wind>& getWinds();

	civ::Ref<Obstacle> addObstacle(int direction, bool moveHorizontal, const std::pair<int, int>& particleIndexRange,
		const std::pair<float, float>& horizontalBound, const std::pair<float, float>& verticalBound, float movingSpeed);
	const civ::IndexVector<Obstacle>& getObstacles();

	const sf::Vector3f getWorld();
	CollisionGrid& getGrid();

	// collision functions
	void fillCollisionGrid();
	void solveCollisions();
	void solveGridCollision();
	void solveCellCollision(CollisionCell& cell1, CollisionCell& cell2);
	void solveParticleCollision(Particle* p1, Particle* p2);
	void solveObstacleCollision(Obstacle& obstacle);
	void solveCollisionWithWorld(Particle& particle);

	// additional physics effect functions
	void applyWind();
	void applyForce(float radius, const sf::Vector2f& position);

	// utility functions
	bool isValidPosition(const sf::Vector2f& position);
	civ::Ref<Particle> getClickedParticle(const sf::Vector2f& clickedPosition);
	civ::Ref<Particle> getNearestParticle(const sf::Vector2f& position);
	void addCube(const sf::Vector2f& position, float stiffness = 1.0f, bool pinned = false);
	//void addChain(const sf::Vector2f& position, float chainLength);
	void addChain(civ::Ref<Particle> p1, civ::Ref<Particle> p2);
	//std::pair<int, int> addCircle(const sf::Vector2f& poisition, float radius, int numParticles, float stiffness = 1.0f, bool pinCenter = false, bool pinOuter = false);

	//std::pair<int, int> addCircle(const sf::Vector2f& poisition, float radius, int numParticles, ParticleColor color, float stiffness = 1.0f, bool pinCenter = true, bool pinOuter = true);

	std::pair<int, int> addRectangle(const sf::Vector2f& position, bool pinned, int width, int height, float particleRadius, ParticleColor color);
	// std::pair<int, int> addCircle(const sf::Vector2f& position, float radius, int numParticles);

	std::pair<int, int> addTriangle(const sf::Vector2f& position, bool pinned, int baseWidth, float specParticleRadius, ParticleColor color);

	std::pair<int, int> addRightTriangle(const sf::Vector2f& position, bool pinned, int height, float specParticleRadius, ParticleColor color);

	std::pair<int, int> addCircle(const sf::Vector2f& poisition, float radius, int numParticles, sf::Vector2f initVelocity, float stiffness = 1.0f, bool pinCenter = false, bool pinOuter = false);

	std::pair<int, int> addCircle(const sf::Vector2f& position, float radius, int numParticles, ParticleColor color);

	bool IsBallReachDestination(const std::pair<int, int>& ballIndexRange, const sf::Vector2f& destinationPos);
	bool IsBallCollideObstacle(const std::pair<int, int>& ballIndexRange);
	// timing functions
	const float getElapsedTime();
	void setFrameDt(const int framerate);
	void setSubSteps(const int subSteps);
	const float getStepDt();

	void clearScene();

private:
	sf::Vector2f gravity{ 0.0f, 1000.0f };
	civ::IndexVector<Particle> particles;
	civ::IndexVector<Constraint> constraints;
	civ::IndexVector<Wind> winds;
	civ::IndexVector<Obstacle> obstacles;
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