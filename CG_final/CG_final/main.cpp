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
	const float OBJECT_SPAWN_TIME = 0.1f;
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
	bool useWind = false; // use wind or not
	bool grabbing = false; // grab clicked object
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
	std::vector<civ::Ref<Particle>> connected(2);
	int counter = 0;
	civ::ID lastClicked = 0;

	// winds (there can be multiple winds)
	solver.addWind({ 0.0f, 0.0f }, { 100.0f, WORLD_SIZE.y }, 10.0f, 500.0f);

	// add additional events
	sfev::EventManager& eventManager = game.getEventManager();
	eventManager.addMousePressedCallback(sf::Mouse::Left, [&](const sf::Event& event) {
		game.click();
		game.drag();
		isBuilding = true;
		civ::Ref<Particle> particle = solver.getClickedParticle(game.getWorldMousePosition());
		if (particle)
		{
			// if already stored a particle and this one is a new particle
			if (connected[0] && particle->id != lastClicked)
			{
				connected[1] = particle;
			}
			else
			{
				connected[0] = particle;
				lastClicked = particle->id;
			}

		}
		});
	eventManager.addMouseReleasedCallback(sf::Mouse::Left, [&](const sf::Event& event) {
		game.release();
		game.undrag();
		isBuilding = false;
		if (chaining)
		{
			/*for (int i = 1; i < chainedParitlces.size(); i++)
			{
				solver.addConstraint(chainedParitlces[i - 1], chainedParitlces[i], 10.0f);
			}
			chainedParitlces.clear();*/

			// connect two particles with a constraint
			if (connected[0] && connected[1])
			{
				solver.addConstraint(
					connected[0],
					connected[1],
					Math::getDistance(connected[0]->currentPosition, connected[1]->currentPosition)
				);
				connected[0] = civ::Ref<Particle>();
				connected[1] = civ::Ref<Particle>();
			}
		}
		});
	eventManager.addMousePressedCallback(sf::Mouse::Right, [&](const sf::Event& event) {
		useForce = true;
		});
	eventManager.addMouseReleasedCallback(sf::Mouse::Right, [&](const sf::Event& event) {
		useForce = false;
		});
	eventManager.addMousePressedCallback(sf::Mouse::Middle, [&](const sf::Event& event) {
		buildMode = (buildMode + 1) % 3;
		});
	eventManager.addKeyPressedCallback(sf::Keyboard::C, [&](const sf::Event& event) {
		chaining = !chaining;
		});
	eventManager.addKeyPressedCallback(sf::Keyboard::P, [&](const sf::Event& event) {
		pinned = !pinned;
		});
	eventManager.addKeyPressedCallback(sf::Keyboard::G, [&](const sf::Event& event) {
		showGrid = !showGrid;
		});
	eventManager.addKeyPressedCallback(sf::Keyboard::W, [&](const sf::Event& event) {
		useWind = !useWind;
		});
	eventManager.addKeyPressedCallback(sf::Keyboard::X, [&](const sf::Event& event) {
		grabbing = !grabbing;
		});
	eventManager.addKeyPressedCallback(sf::Keyboard::Space, [&](const sf::Event& event) {
		pause = !pause;
		});

	while (game.isRunning())
	{
		game.handleEvents();

		//if (solver.getNumParticles() < MAX_NUM_OBJECTS && spawnTimer.getElapsedTime().asSeconds() >= OBJECT_SPAWN_TIME) {
		//	spawnTimer.restart();
		//	civ::Ref<Particle> particle = solver.addParticle(SPAWN_LOCATION);
		//	const float elapsedTime = solver.getElapsedTime();
		//	const float angle = sin(elapsedTime) + Math::PI * 0.5f;
		//	particle->initVelocity(OBJECT_SPPED * sf::Vector2f(1.0f, 0.0f), solver.getStepDt());
		//	if (chaining)
		//		chainedParitlces.push_back(particle);
		//}

		const sf::Vector2f mousePosition = game.getWorldMousePosition();
		if (!chaining && !grabbing && isBuilding && spawnTimer.getElapsedTime().asSeconds() >= OBJECT_SPAWN_TIME)
		{
			spawnTimer.restart();;
			switch (buildMode)
			{
			case 0:
			{
				sf::Vector2f gridCoord = (sf::Vector2f)solver.getGrid().getGridCoordinate(mousePosition, OBJECT_RADIUS);
				sf::Vector2f objectPosition(gridCoord.y * CELL_SIZE + OBJECT_RADIUS, gridCoord.x * CELL_SIZE + OBJECT_RADIUS);
				civ::Ref<Particle> particle = solver.addParticle(objectPosition, pinned);
				if (chaining)
					chainedParitlces.push_back(particle);
				break;
			}
			case 1:
				solver.addCube(mousePosition, pinned);
				break;
			case 2:
				solver.addCircle(mousePosition, 50.0f, 30);
			}
		}

		if (grabbing)
		{
			civ::Ref<Particle> grabbed = solver.getClickedParticle(mousePosition);
			if (grabbed)
			{
				sf::Vector2f direction = game.getScreenMousePosition() - grabbed->currentPosition;
				float distance = Math::getLength(direction);
				sf::Vector2f unit = direction / distance;
				grabbed->move(unit);
			}
		}

		if (useWind)
			solver.applyWind();

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