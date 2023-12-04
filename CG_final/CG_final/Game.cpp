#include <SFML/Graphics.hpp>
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
	const float OBJECT_MIN_RADIUS = 10.0f;
	const float OBJECT_MAX_RADIUS = 50.0f;
	const float OBJECT_SPAWN_DELAY = 0.025f;
	const float OBJECT_SPPED = 1200.0f;
	const sf::Vector2f SPAWN_LOCATION = { 500.0f, 200.0f };

	// control
	bool dragging = false;

	// opengl context
	sf::ContextSettings settings;
	settings.antialiasingLevel = ANTI_ALIASING_LEVEL;
	// main window
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Game", sf::Style::Default, settings);
	// set framerate
	window.setFramerateLimit(FRAMERATE);
	// timer for game
	sf::Clock timer;

	// event manager
	sfev::EventManager eventManager(window, true);

	Solver solver;
	Renderer renderer(window, solver);
	RNG rng;

	//solver.setConstraint({ WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f }, 450.0f);
	solver.setWorldSize({ WINDOW_WIDTH - 100.0f, WINDOW_HEIGHT - 100.0f }, 100.0f);
	solver.setFrameDt(FRAMERATE);
	solver.setSubSteps(NUM_SUB_STEPS);

	// add event
	eventManager.addEventCallback(sf::Event::EventType::Closed, [&](const sf::Event& event) { window.close(); });
	eventManager.addKeyPressedCallback(sf::Keyboard::Escape, [&](const sf::Event& event) { window.close(); });
	eventManager.addMousePressedCallback(sf::Mouse::Left, [&](const sf::Event& event) { dragging = true; });
	eventManager.addMouseReleasedCallback(sf::Mouse::Left, [&](const sf::Event& event) {dragging = false; });

	while (window.isOpen())
	{
		eventManager.processEvents();

		/*if (solver.getNumObjects() < MAX_NUM_OBJECTS && timer.getElapsedTime().asSeconds() >= OBJECT_SPAWN_DELAY) {
			timer.restart();
			Object& object = solver.addObject(SPAWN_LOCATION, rng.sampleFromRange(OBJECT_MIN_RADIUS, OBJECT_MAX_RADIUS));
			const float elapsedTime = solver.getElapsedTime();
			const float angle = sin(elapsedTime) + Math::PI * 0.5f;
			object.initVelocity(OBJECT_SPPED * sf::Vector2f(cos(angle), sin(angle)), solver.getStepDt());
		}*/
		if (dragging && timer.getElapsedTime().asSeconds() >= OBJECT_SPAWN_DELAY)
		{
			timer.restart();
			Object& object = solver.addObject(eventManager.getFloatMousePosition(), rng.sampleFromRange(OBJECT_MIN_RADIUS, OBJECT_MAX_RADIUS));
		}

		if (!dragging)
			solver.update();
		window.clear(sf::Color::Black);
		renderer.render();
		window.display();
	}

	return 0;
}