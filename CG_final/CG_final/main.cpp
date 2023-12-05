#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "Object.hpp"
#include "Solver.hpp"
#include "Renderer.hpp"
#include "Random.hpp"
#include "Math.hpp"
#include "EventManager/event_manager.hpp"
#include <iostream>


int main()
{
	// constants
	const int WINDOW_WIDTH = 1920;
	const int WINDOW_HEIGHT = 1080;
	const int ANTI_ALIASING_LEVEL = 4;
	const int FRAMERATE = 60;
	const int NUM_SUB_STEPS = 8;
	const int MAX_NUM_OBJECTS = 100;
	//const int MAX_NUM_OBJECTS = 10;
	const float OBJECT_MIN_RADIUS = 10.0f;
	const float OBJECT_MAX_RADIUS = 20.0f;
	const float OBJECT_SPAWN_DELAY = 0.025f;
	const float OBJECT_SPPED = 1200.0f;
	const sf::Vector2f SPAWN_LOCATION = { 100.0f, 100.0f };
	//const sf::Vector2f WORLD_SIZE = { 1000.0f, 1000.0f };
	const sf::Vector2f WORLD_SIZE = { 300.0f, 300.0f };

	// control
	bool dragging = false;

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

	// add events
	game.getEventManager().addMousePressedCallback(sf::Mouse::Left, [&](const sf::Event& event) { dragging = true; });
	game.getEventManager().addMouseReleasedCallback(sf::Mouse::Left, [&](const sf::Event& event) {dragging = false; });

	while (game.isRunning())
	{
		game.handleEvents();

		if (solver.getNumObjects() < MAX_NUM_OBJECTS && timer.getElapsedTime().asSeconds() >= OBJECT_SPAWN_DELAY) {
			timer.restart();
			Object& object = solver.addObject(SPAWN_LOCATION, rng.sampleFromRange(OBJECT_MIN_RADIUS, OBJECT_MAX_RADIUS));
			const float elapsedTime = solver.getElapsedTime();
			const float angle = sin(elapsedTime) + Math::PI * 0.5f;
			object.initVelocity(OBJECT_SPPED * sf::Vector2f(cos(angle), sin(angle)), solver.getStepDt());
		}

		/*sf::Vector2f mousePosition = game.getMousePosition();
		if (dragging && timer.getElapsedTime().asSeconds() >= OBJECT_SPAWN_DELAY)
		{
			timer.restart();
			Object& object = solver.addObject(game.getMousePosition(), rng.sampleFromRange(OBJECT_MIN_RADIUS, OBJECT_MAX_RADIUS));
		}

		if (!dragging)*/
		solver.update();
		game.clear(sf::Color::Black);
		renderer.render(context);
		game.display();
	}

	return 0;
}