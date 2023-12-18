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
	const float OBJECT_MIN_RADIUS = 10.0f;
	const float OBJECT_MAX_RADIUS = 20.0f;
	const float OBJECT_SPAWN_TIME = 0.07f;
	//const float OBJECT_SPAWN_TIME = 0.15f;
	//const float OBJECT_SPAWN_TIME = 0.025f;
	const float OBJECT_SPPED = 500.0f;
	const sf::Vector2f WORLD_SIZE = { 500.0f, 300.0f };
	const sf::Vector2f SPAWN_LOCATION = { OBJECT_RADIUS, OBJECT_RADIUS + 3.0f };

	// control varaibles
	bool useForce = false; // apply force on objects
	bool isBuilding = false; // build or not
	int buildMode = 0; // 0: particle, 1: cube
	bool pinned = false; // pin objects or not
	bool chaining = false; // chain the drawn particles together
	bool showGrid = false; // show collision grid or not
	bool pause = false; // pause game or not

	Game game(WINDOW_WIDTH, WINDOW_HEIGHT, "SFML Game", sf::Style::Fullscreen);
	RenderContext& context = game.getRenderContext();

	// settings
	game.setFramerate(FRAMERATE);
	const float margin = 20.0f;
	const float zoom = (float)(WINDOW_HEIGHT - margin) / WORLD_SIZE.y;
	context.setZoom(zoom);
	context.setFocus({ WORLD_SIZE.x * 0.5f, WORLD_SIZE.y * 0.5f });

	// timer to track spawning time
	sf::Clock spawnTimer;

	Solver solver(WORLD_SIZE, OBJECT_RADIUS, CELL_SIZE);
	Renderer renderer(solver);
	RNG rng;

	solver.setFrameDt(FRAMERATE);
	solver.setSubSteps(NUM_SUB_STEPS);

	std::vector<civ::Ref<Particle>> chainedParitlces;

	// add additional events
	sfev::EventManager& eventManager = game.getEventManager();
	eventManager.addMousePressedCallback(sf::Mouse::Left, [&](const sf::Event& event) {
		game.click();
		game.drag();
		isBuilding = true;
		/*const sf::Vector2f screenMousePosition = getScreenMousePosition();
		const sf::Vector2f worldMousePosition = getWorldMousePosition();
		std::cout << "screen mouse: (" << screenMousePosition.x << ", " << screenMousePosition.y << ")\n";
		std::cout << "world mouse: (" << worldMousePosition.x << ", " << worldMousePosition.y << ")\n";*/
		});
	eventManager.addMouseReleasedCallback(sf::Mouse::Left, [&](const sf::Event& event) {
		game.release();
		game.undrag();
		isBuilding = false;
		if (chaining)
		{
			for (int i = 1; i < chainedParitlces.size(); i++)
			{
				solver.addConstraint(chainedParitlces[i - 1], chainedParitlces[i], 10.0f);
			}
			chainedParitlces.clear();
		}
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
	eventManager.addKeyPressedCallback(sf::Keyboard::C, [&](const sf::Event& event) {
		chaining = !chaining;
		std::cout << "is chaining: " << chaining << std::endl;
		});
	eventManager.addKeyPressedCallback(sf::Keyboard::P, [&](const sf::Event& event) {
		pinned = !pinned;
		});
	eventManager.addKeyPressedCallback(sf::Keyboard::G, [&](const sf::Event& event) {
		showGrid = !showGrid;
		});
	eventManager.addKeyPressedCallback(sf::Keyboard::Space, [&](const sf::Event& event) {
		pause = !pause;
		});


	while (game.isRunning())
	{
		game.handleEvents();

		//if (solver.getNumParticles() < MAX_NUM_OBJECTS && spawnTimer.getElapsedTime().asSeconds() >= OBJECT_SPAWN_TIME) {
		//	spawnTimer.restart();
		//	civ::Ref<Particle> particle = solver.addParticle(SPAWN_LOCATION, OBJECT_RADIUS);
		//	const float elapsedTime = solver.getElapsedTime();
		//	const float angle = sin(elapsedTime) + Math::PI * 0.5f;
		//	//particle->initVelocity(OBJECT_SPPED * sf::Vector2f(cos(angle), sin(angle)), solver.getStepDt());
		//	particle->initVelocity(OBJECT_SPPED * sf::Vector2f(1.0f, 0.0f), solver.getStepDt());
		//	if (chaining)
		//		chainedParitlces.push_back(particle);
		//}

		const sf::Vector2f mousePosition = game.getWorldMousePosition();
		if (isBuilding && spawnTimer.getElapsedTime().asSeconds() >= OBJECT_SPAWN_TIME)
		{
			spawnTimer.restart();;
			switch (buildMode)
			{
			case 0:
			{
				//if (!chaining)
				//{
				sf::Vector2f gridCoord = (sf::Vector2f)solver.getGrid().getGridCoordinate(mousePosition, OBJECT_RADIUS);
				//civ::Ref<Particle> particle = solver.addParticle(mousePosition, OBJECT_RADIUS, pinned);
				civ::Ref<Particle> particle = solver.addParticle(
					{ gridCoord.y * CELL_SIZE, gridCoord.x * CELL_SIZE },
					OBJECT_RADIUS, pinned
				);
				if (chaining)
					chainedParitlces.push_back(particle);
				//}
				//else
				//{
				//	solver.addChain(mousePosition, 50.0f);
				//}
				break;
			}
			case 1:
				solver.addCube(mousePosition, pinned);
				break;
			}
		}

		if (useForce)
			solver.applyForce(150.0f, game.getWorldMousePosition());

		if (!pause)
			solver.update();

		game.clear();
		renderer.render(context, game.getWorldMousePosition(), buildMode, showGrid);
		game.display();
	}

	return 0;
}