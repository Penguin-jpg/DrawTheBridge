#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "Solver.hpp"
#include "Renderer.hpp"
#include "Random.hpp"
#include "Math.hpp"
#include <iostream>


int main()
{
	// constants
	const int WINDOW_WIDTH = 1920;
	const int WINDOW_HEIGHT = 1080;
	const int ANTI_ALIASING_LEVEL = 4;
	const int FRAMERATE = 60;
	const int NUM_SUB_STEPS = 8;
	//const int MAX_NUM_OBJECTS = 100;
	const int MAX_NUM_OBJECTS = 10;
	const float OBJECT_MIN_RADIUS = 10.0f;
	const float OBJECT_MAX_RADIUS = 20.0f;
	const float OBJECT_SPAWN_DELAY = 0.1f;
	const float OBJECT_SPPED = 1200.0f;
	const sf::Vector2f SPAWN_LOCATION = { 100.0f, 100.0f };
	//const sf::Vector2f WORLD_SIZE = { 1000.0f, 1000.0f };
	const sf::Vector2f WORLD_SIZE = { 300.0f, 300.0f };

	bool force = false;

	Game game(WINDOW_WIDTH, WINDOW_HEIGHT, "SFML Game", sf::Style::Default);
	RenderContext& context = game.getRenderContext();

	// settings
	game.setFramerate(FRAMERATE);
	const float margin = 20.0f;
	const float zoom = (float)(WINDOW_HEIGHT - margin) / WORLD_SIZE.y;
	context.setZoom(zoom);
	context.setFocus({ WORLD_SIZE.x * 0.5f, WORLD_SIZE.y * 0.5f });

	// timer for game
	sf::Clock timer;

	Solver solver(WORLD_SIZE, OBJECT_MAX_RADIUS);
	Renderer renderer(solver);
	RNG rng;

	//solver.setConstraint({ WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f }, 450.0f);
	solver.setFrameDt(FRAMERATE);
	solver.setSubSteps(NUM_SUB_STEPS);

	//if (context.stateManager.isClicked() && timer.getElapsedTime().asSeconds() >= OBJECT_SPAWN_DELAY)
	//{
	//	sf::Vector2f pos = game.getWorldMousePosition();
	//	Particle& p1 = solver.addParticle(pos + sf::Vector2f(0.0f, 0.0f), 10.0f);
	//	Particle& p2 = solver.addParticle(pos + sf::Vector2f(40.0f, 0.0f), 10.0f);
	//	//Link& link = solver.addLink(p1, p2);
	//	std::cout << pos.x << " " << pos.y << std::endl;
	//	std::cout << p1.currentPosition.x << " " << p1.currentPosition.y << std::endl;
	//	std::cout << p2.currentPosition.x << " " << p2.currentPosition.y << std::endl;
	//}
	solver.addParticle(SPAWN_LOCATION, 10.0f);
	solver.addParticle(SPAWN_LOCATION + sf::Vector2f(40.0f, 0.0f), 10.0f);
	std::vector<Particle>& particles = solver.getParticles();
	Particle* p1 = &particles[0];
	Particle* p2 = &particles[1];
	std::cout << p1->currentPosition.x << " " << p1->currentPosition.y << std::endl;
	std::cout << p2->currentPosition.x << " " << p2->currentPosition.y << std::endl;
	Link& link = solver.addLink(p1, p2);

	game.getEventManager().addMousePressedCallback(sf::Mouse::Right, [&](const sf::Event& event) {
		force = true;
		});

	while (game.isRunning())
	{
		game.handleEvents();

		/*if (solver.getNumParticles() < MAX_NUM_OBJECTS && timer.getElapsedTime().asSeconds() >= OBJECT_SPAWN_DELAY) {
			timer.restart();
			Particle& particle = solver.addParticle(SPAWN_LOCATION, rng.sampleFromRange(OBJECT_MIN_RADIUS, OBJECT_MAX_RADIUS));
			const float elapsedTime = solver.getElapsedTime();
			const float angle = sin(elapsedTime) + Math::PI * 0.5f;
			particle.initVelocity(OBJECT_SPPED * sf::Vector2f(cos(angle), sin(angle)), solver.getStepDt());
		}*/

		//if (!dragging)
		if (force)
			solver.applyForce(150.0f, game.getWorldMousePosition());
		solver.update();
		game.clear(sf::Color::Black);
		renderer.render(context);
		game.display();
	}

	return 0;
}