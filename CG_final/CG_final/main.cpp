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
	const int FRAMERATE = 60;
	const int NUM_SUB_STEPS = 8;
	const int MAX_NUM_OBJECTS = 2000;
	const float OBJECT_RADIUS = 5.0f;
	const int CELL_SIZE = 2 * OBJECT_RADIUS;
	const float WORLD_BOX_MARGIN = OBJECT_RADIUS;
	const float OBJECT_MIN_RADIUS = 10.0f;
	const float OBJECT_MAX_RADIUS = 20.0f;
	const float OBJECT_SPAWN_DELAY = 0.025f;
	const float OBJECT_SPPED = 1200.0f;
	const sf::Vector2f SPAWN_LOCATION = { 100.0f, 100.0f };
	const sf::Vector2f WORLD_SIZE = { 300.0f, 300.0f };

	// control varaibles
	bool useForce = false; // apply force on objects
	bool isBuilding = false; // build or not
	int buildMode = 0; // 0: particle, 1: cube
	bool pinned = false; // pin objects or not
	bool pause = false; // pause game or not

	Game game(WINDOW_WIDTH, WINDOW_HEIGHT, "SFML Game", sf::Style::Fullscreen);
	RenderContext& context = game.getRenderContext();

	// settings
	game.setFramerate(FRAMERATE);
	const float margin = 20.0f;
	const float zoom = (float)(WINDOW_HEIGHT - margin) / WORLD_SIZE.y;
	context.setZoom(zoom);
	context.setFocus({ WORLD_SIZE.x * 0.5f, WORLD_SIZE.y * 0.5f });

	// timer for game
	sf::Clock timer;

	Solver solver(WORLD_SIZE, WORLD_BOX_MARGIN, CELL_SIZE);
	Renderer renderer(solver);
	RNG rng;

	solver.setFrameDt(FRAMERATE);
	solver.setSubSteps(NUM_SUB_STEPS);

	// add additional events
	sfev::EventManager& eventManager = game.getEventManager();
	eventManager.addMousePressedCallback(sf::Mouse::Left, [&](const sf::Event& event) {
		game.click();
		isBuilding = true;
		/*const sf::Vector2f screenMousePosition = getScreenMousePosition();
		const sf::Vector2f worldMousePosition = getWorldMousePosition();
		std::cout << "screen mouse: (" << screenMousePosition.x << ", " << screenMousePosition.y << ")\n";
		std::cout << "world mouse: (" << worldMousePosition.x << ", " << worldMousePosition.y << ")\n";*/
		});
	eventManager.addMouseReleasedCallback(sf::Mouse::Left, [&](const sf::Event& event) {
		game.release();
		isBuilding = false;
		});
	eventManager.addMousePressedCallback(sf::Mouse::Right, [&](const sf::Event& event) {
		useForce = true;
		});
	eventManager.addMouseReleasedCallback(sf::Mouse::Right, [&](const sf::Event& event) {
		useForce = false;
		});
	eventManager.addMousePressedCallback(sf::Mouse::Middle, [&](const sf::Event& event) {
		buildMode = (buildMode + 1) % 2;
		});
	eventManager.addKeyPressedCallback(sf::Keyboard::P, [&](const sf::Event& event) {
		pinned = !pinned;
		});
	eventManager.addKeyPressedCallback(sf::Keyboard::Space, [&](const sf::Event& event) {
		pause = !pause;
		});

	while (game.isRunning())
	{
		game.handleEvents();

		/*if (solver.getNumParticles() < MAX_NUM_OBJECTS && timer.getElapsedTime().asSeconds() >= OBJECT_SPAWN_DELAY) {
			timer.restart();
			civ::Ref<Particle> particle = solver.addParticle(SPAWN_LOCATION, OBJECT_RADIUS);
			const float elapsedTime = solver.getElapsedTime();
			const float angle = sin(elapsedTime) + Math::PI * 0.5f;
			particle->initVelocity(OBJECT_SPPED * sf::Vector2f(cos(angle), sin(angle)), solver.getStepDt());
		}*/

		if (isBuilding && timer.getElapsedTime().asSeconds() >= 0.3f)
		{
			timer.restart();
			const sf::Vector2f mousePosition = game.getWorldMousePosition();
			switch (buildMode)
			{
			case 0:
				solver.addParticle(mousePosition, OBJECT_RADIUS, pinned);
				break;
			case 1:
				solver.addCube(mousePosition, false, pinned);
				break;
			}
		}

		if (useForce)
			solver.applyForce(150.0f, game.getWorldMousePosition());

		if (!pause)
			solver.update();

		game.clear();
		renderer.render(context, game.getWorldMousePosition(), buildMode);
		game.display();
	}

	return 0;
}