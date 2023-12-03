#include <SFML/Graphics.hpp>
#include "Object.hpp"
#include "Solver.hpp"
#include "Renderer.hpp"
#include "Random.hpp"
#include "Math.hpp"
#include <iostream>

int main()
{
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

	// opengl context
	sf::ContextSettings settings;
	settings.antialiasingLevel = ANTI_ALIASING_LEVEL;
	// main window
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Game", sf::Style::Default, settings);
	// set framerate
	window.setFramerateLimit(FRAMERATE);
	// timer for game
	sf::Clock timer;

	Solver solver;
	Renderer renderer(window);
	RNG rng;

	solver.setConstraint({ WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f }, 450.0f);
	solver.setFrameDt(FRAMERATE);
	solver.setSubSteps(NUM_SUB_STEPS);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
		}

		if (solver.getNumObjects() < MAX_NUM_OBJECTS && timer.getElapsedTime().asSeconds() >= OBJECT_SPAWN_DELAY) {
			timer.restart();
			Object& object = solver.addObject(SPAWN_LOCATION, rng.sampleFromRange(OBJECT_MIN_RADIUS, OBJECT_MAX_RADIUS));
			const float elapsedTime = solver.getElapsedTime();
			const float angle = sin(elapsedTime) + Math::PI * 0.5f;
			object.initVelocity(OBJECT_SPPED * sf::Vector2f(cos(angle), sin(angle)), solver.getStepDt());
		}

		solver.update();
		window.clear(sf::Color::White);
		renderer.render(solver);
		window.display();
	}

	return 0;
}