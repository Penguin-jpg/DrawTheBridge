#include "Game.hpp"
#include <iostream>

Game::Game(int width, int height, const std::string& title, int windowStyle = sf::Style::Default)
	: windowWidth(width), windowHeight(height), window(sf::VideoMode(width, height), title),
	eventManager(window, true), context(window), solver(WORLD_SIZE, OBJECT_RADIUS, CELL_SIZE), renderer(solver), connected(2)
{
	addBasicEvents();
	addAdditionalEvents();
	solver.setFrameDt(FRAMERATE);
	solver.setSubSteps(NUM_SUB_STEPS);
	// winds (there can be multiple winds)
	solver.addWind({ 0.0f, 0.0f }, { 100.0f, WORLD_SIZE.y }, 10.0f, 500.0f);

	currentStateID = StateID::StartMenu;
	startMenuState = new StartMenuState(window);
	levelSelectionState = new LevelSelectionState(window);
	level1 = new Level1(window);
	level2 = new Level2(window);
	level3 = new Level3(window);
	currentGameState = startMenuState;
	isMouseLeftPressed = false;
	isMouseLeftPressAndReleased = false;
	ballReachDestination = false;
	isGameOver = false;
}

sf::RenderWindow& Game::getWindow()
{
	return window;
}

sfev::EventManager& Game::getEventManager()
{
	return eventManager;
}

RenderContext& Game::getRenderContext()
{
	return context;
}

bool Game::isRunning()
{
	return window.isOpen();
}

void Game::handleEvents()
{
	eventManager.processEvents();
}

void Game::clear(sf::Color clearColor)
{
	window.clear(clearColor);
}

void Game::display()
{
	window.display();
}

void Game::setFramerate(int framerate)
{
	window.setFramerateLimit(framerate);
}

void Game::addBasicEvents()
{
	// close window
	eventManager.addEventCallback(sf::Event::EventType::Closed, [&](const sf::Event& event) { window.close(); });
	eventManager.addKeyPressedCallback(sf::Keyboard::Escape, [&](const sf::Event& event) { window.close(); });

	// update mouse position
	eventManager.addEventCallback(sf::Event::MouseMoved, [&](const sf::Event& event) {
		context.stateManager.updateMousePosition(eventManager.getFloatMousePosition());
		clickPosition = getWorldMousePosition();
		prevDragPosition = dragPosition;
		dragPosition = getWorldMousePosition();
		});
}


void Game::addAdditionalEvents()
{
	eventManager.addMousePressedCallback(sf::Mouse::Left, [&](const sf::Event& event) {
		click();
		drag();
		isBuilding = true;
		isMouseLeftPressed = true;
		if (chaining)
		{
			civ::Ref<Particle> particle = solver.getClickedParticle(getWorldMousePosition());
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
		}
		});
	eventManager.addMouseReleasedCallback(sf::Mouse::Left, [&](const sf::Event& event) {
		release();
		undrag();
		isBuilding = false;
		if (isMouseLeftPressed) {
			isMouseLeftPressAndReleased = true;
			isMouseLeftPressed = false;
		}

		// build a chain using two selected particles as pivots
		if (connected[0] && connected[1])
		{
			solver.addChain(connected[0], connected[1]);
			connected[0] = civ::Ref<Particle>();
			connected[1] = civ::Ref<Particle>();
		}
		});

	//if (connected[0] && connected[1])
	//{
	//	solver.addConstraint(
	//		connected[0],
	//		connected[1],
	//		Math::getDistance(connected[0]->currentPosition, connected[1]->currentPosition)
	//	);
	//	connected[0] = civ::Ref<Particle>();
	//	connected[1] = civ::Ref<Particle>();
	//}
	//});
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

	eventManager.addKeyPressedCallback(sf::Keyboard::V, [&](const sf::Event& event) {

		testText = true;
		});
}

void Game::click()
{
	context.stateManager.click(eventManager.getFloatMousePosition());
}

void Game::drag()
{
	context.stateManager.drag(eventManager.getFloatMousePosition());
}

void Game::release()
{
	context.stateManager.release();
}

void Game::undrag()
{
	context.stateManager.undrag();
}

const sf::Vector2f Game::getScreenMousePosition()
{
	return context.stateManager.getScreenMousePosition();
}

const sf::Vector2f Game::getWorldMousePosition()
{
	return context.stateManager.getWorldMousePosition();
}

const sf::Vector2f Game::screenToWorldPosition(const sf::Vector2f& screenPosition)
{
	return context.stateManager.screenToWorldPosition(screenPosition);
}

bool Game::isInLevelGameState() {
	if (currentStateID == StateID::Level1 || currentStateID == StateID::Level2 || currentStateID == StateID::Level3) {
		return true;
	}
	else
		return false;
}


void Game::clearLevelGameScene() {
	solver.removeAllParticles();
	obstacles.clear();
	ballReachDestination = false;
	isGameOver = false;
}

void Game::createLevelOneScene() {

	clearLevelGameScene();
	sf::Vector2f recPos = { 90,185 };
	solver.addRectangle(recPos, true, 13, 23, 5.0f, ParticleColor::Deg14);
	recPos = { 400, 230 };
	solver.addRectangle(recPos, true, 10, 14, 5.0f, ParticleColor::Deg14);
	destinationPos = { 408.774, 124.811 };
	ballPos = { 151.5, 38.3 };
	ballInitVelocity = { 0.0f, 0.0f };
	ballIndex = solver.addCircle(ballPos, 10.0f, 6, ballInitVelocity);
}

void Game::createLevelTwoScene() {

	clearLevelGameScene();
	sf::Vector2f recPos = { 65.0f, 100.0f };
	solver.addRectangle(recPos, true, 12, 3, 5.0f, ParticleColor::Deg14);
	recPos = { 250.0f, 290.0f };
	solver.addRectangle(recPos, true, 45, 1, 5.0f, ParticleColor::Deg14);

	recPos = { 355.0f, 265.0f };
	solver.addRectangle(recPos, true, 6, 4, 5.0f, ParticleColor::Deg14);

	sf::Vector2f circlePos = { 220.0f, 70.0f };
	std::pair<int, int> particleIndexRange = solver.addCircle(circlePos, 30, 20, ParticleColor::Deg15);
	addObstacle(true, true, particleIndexRange, std::pair(145, 450), std::pair(0, 0), 150.0f);
	circlePos = { 380.0f, 170.0f };
	particleIndexRange = solver.addCircle(circlePos, 30, 20, ParticleColor::Deg15);
	addObstacle(false, true, particleIndexRange, std::pair(145, 450), std::pair(0, 0), 150.0f);

	destinationPos = { 432.0f, 261.226f };
	ballPos = { 121.5f, 70.3f };
	ballInitVelocity = { 0.0f, 0.0f };
	ballIndex = solver.addCircle(ballPos, 10.0f, 6, ballInitVelocity);
	// std::cout << "ballIndex : " << ballIndex.first << " " << ballIndex.second << std::endl;

}

void Game::addObstacle(bool isForward, bool moveHorizontal, const std::pair<int, int>& particleIndexRange,
	const std::pair<int, int>& horizontalBound, const std::pair<int, int>& verticalBound, float movingSpeed)
{
	obstacles.emplace_back(isForward, moveHorizontal, particleIndexRange, horizontalBound, verticalBound, movingSpeed);
}

void Game::createLevelThreeScene() {

	clearLevelGameScene();
	sf::Vector2f recPos = { 200, 150 };
	/*
	solver.addRectangle(recPos, true, 14, 3, 5.0f, ParticleColor::Deg14);

	recPos = { 380, 200 };
	solver.addRectangle(recPos, true, 12, 3, 5.0f, ParticleColor::Deg14);
	*/

	recPos = { 380.0f, 225.0f };
	solver.addRectangle(recPos, true, 7, 12, 5.0f, ParticleColor::Deg14);

	recPos = { 230.0f, 180.0f };
	solver.addRectangle(recPos, true, 8, 18, 5.0f, ParticleColor::Deg14);

	recPos = { 90.0f, 160.0f };
	solver.addRectangle(recPos, true, 8, 3, 5.0f, ParticleColor::Deg14);

	recPos = { 250.0f, 290.0f };
	solver.addRectangle(recPos, true, 45, 1, 5.0f, ParticleColor::Deg14);



	recPos = { 150, 100 };
	std::pair<int, int> particleIndexRange = solver.addRectangle(recPos, true, 5, 2, 5.0f, ParticleColor::Deg15);
	addObstacle(false, false, particleIndexRange, std::pair(0, 0), std::pair(30, 100), 90.0f);


	recPos = { 150, 100 };
	particleIndexRange = solver.addRectangle(recPos, true, 5, 2, 5.0f, ParticleColor::Deg15);
	addObstacle(true, false, particleIndexRange, std::pair(0, 0), std::pair(100, 170), 90.0f);
	//addObstacle(particleIndexRange, 1, 0, std::pair(0, 0), std::pair(30, 120), 90.0f);

	destinationPos = { 72.54f, 119.43f };
	ballPos = { 368.585f, 148.019f };
	ballInitVelocity = { -1.0, -0.3 };
	ballIndex = solver.addCircle(ballPos, 10.0f, 6, ballInitVelocity);

	/*
	sf::Vector2f circlePos = { 330, 230 };
	// solver.addCircle(mousePosition, 50.0f, 10);
	std::pair<int, int> particleIndexRange = solver.addCircle(circlePos, 30, 20, ParticleColor::Deg15);
	addObstacle(particleIndexRange, 1, 1, std::pair(250, 450), std::pair(0, 0), 120.0f);


	recPos = { 30, 100 };
	particleIndexRange = solver.addRectangle(recPos, true, 3, 10, 5.0f, ParticleColor::Deg2);
	addObstacle(particleIndexRange, 1, 1, std::pair(30, 120), std::pair(0, 0), 120.0f);



	recPos = { 310, 100 };
	particleIndexRange = solver.addRectangle(recPos, true, 4, 3, 5.0f, ParticleColor::Deg2);
	addObstacle(particleIndexRange, 1, 0, std::pair(0, 0), std::pair(30, 120), 90.0f);
	*/

}

void Game::run() {
	sf::Clock spawnTimer;
	sf::Clock clock;

	//sf::Font font;
	//if (!font.loadFromFile("assets/Font/Arial.ttf")) {
	//	// return -1; // handle error
	//}

	//sf::Text text;
	//text.setFont(font);
	//text.setString("");
	//text.setCharacterSize(24);
	//text.setFillColor(sf::Color::Red);
	//text.setPosition(100, 300); // Adjust as needed



	while (isRunning())
	{

		handleEvents();
		sf::Time dt = clock.restart();


		const sf::Vector2f worldMousePosition = getWorldMousePosition();
		const sf::Vector2f screenMousePosition = getScreenMousePosition();
		const sf::Vector2f gridCoord = (sf::Vector2f)solver.getGrid().getGridCoordinate(worldMousePosition, OBJECT_RADIUS);
		sf::Vector2f objectPosition(gridCoord.y * CELL_SIZE + OBJECT_RADIUS, gridCoord.x * CELL_SIZE + OBJECT_RADIUS);



		if (isMouseLeftPressAndReleased) {
			isMouseLeftPressAndReleased = false;
			StateID preStateID = currentStateID;
			currentGameState->handleInput(window, screenMousePosition, currentStateID, ballReachDestination, isGameOver);


			if (currentStateID == StateID::LevelGamePlay) {
				pause = false;
				currentStateID = preStateID;
				isBuilding = false;
			}

			if (preStateID != currentStateID) {
				if (currentStateID == StateID::LevelSelection) {
					currentGameState = levelSelectionState;
				}
				else if (currentStateID == StateID::StartMenu) {
					currentGameState = startMenuState;
				}
				else if (currentStateID == StateID::Level1 || (preStateID == StateID::Level1 && currentStateID == StateID::Refresh)) {
					pause = true;
					currentStateID = StateID::Level1;
					currentGameState = level1;
					createLevelOneScene();

				}
				else if (currentStateID == StateID::Level2 || (preStateID == StateID::Level2 && currentStateID == StateID::Refresh)) {
					pause = true;
					currentStateID = StateID::Level2;
					currentGameState = level2;
					createLevelTwoScene();
				}
				else if (currentStateID == StateID::Level3 || (preStateID == StateID::Level3 && currentStateID == StateID::Refresh)) {
					pause = true;
					currentStateID = StateID::Level3;
					currentGameState = level3;
					createLevelThreeScene();
				}
			}
		}

		if (!chaining && !grabbing && isBuilding && solver.isValidPosition(objectPosition))
		{
			if (buildMode == 0 && spawnTimer.getElapsedTime().asSeconds() >= PARTICLE_SPAWN_TIME)
			{
				spawnTimer.restart();
				civ::Ref<Particle> particle = solver.addParticle(objectPosition, pinned);
			}
			else if (buildMode == 1 && spawnTimer.getElapsedTime().asSeconds() >= CUBE_SPAWN_TIME)
			{
				spawnTimer.restart();
				solver.addCube(worldMousePosition, 1.0f, pinned);
			}
			else if (buildMode == 2 && spawnTimer.getElapsedTime().asSeconds() >= CIRCLE_SPAWN_TIME)
			{
				spawnTimer.restart();
				std::cout << "mousePosition : " << worldMousePosition.x << " " << worldMousePosition.y << std::endl;
				sf::Vector2f initVelocity = { -1.0, 0.3 };
				solver.addCircle(worldMousePosition, 10.0f, 6, initVelocity);
			}

		}

		if (grabbing)
		{
			civ::Ref<Particle> grabbed = solver.getNearestParticle(worldMousePosition);
			if (grabbed)
			{
				sf::Vector2f direction = worldMousePosition - grabbed->currentPosition;
				float distance = Math::getLength(direction);
				if (distance > 0.3f)
				{
					sf::Vector2f unit = direction / distance;
					grabbed->move(unit);
				}
			}
		}

		currentGameState->btnIsHovered(window, screenMousePosition);
		currentGameState->flgIsPressed(chaining, pinned, showGrid, useWind, grabbing, pause);



		if (useWind)
			solver.applyWind();

		if (useForce)
			solver.applyForce(150.0f, getWorldMousePosition());

		if (!pause)
			solver.update();



		//spline.update();

		if (currentStateID == StateID::Level2 || currentStateID == StateID::Level3) {

			for (int i = 0; i < obstacles.size(); i++) {
				//solver.updateObstacle(obstacles[i].moveHorizontal, obstacles[i].isForward, obstacles[i].particleIndexRange, obstacles[i].horizontalBound, obstacles[i].verticalBound, obstacles[i].movingSpeed, dt);
				solver.updateObstacle2(obstacles[i], dt);
			}

			if (solver.IsBallCollideObstacle(ballIndex, obstacles) && !isGameOver) {
				isGameOver = true;
			}
		}


		if (isInLevelGameState() && !ballReachDestination && !isGameOver) {
			ballReachDestination = solver.IsBallReachDestination(ballIndex, destinationPos);
		}

		clear();

		currentGameState->render(window);

		if (isInLevelGameState()) {
			renderer.render(context, getWorldMousePosition(), buildMode, showGrid);

		}
		if (isGameOver) {
			currentGameState->renderLoseWindow(window);
		}
		else if (ballReachDestination) {
			currentGameState->renderWinWindow(window);
		}

		display();

	}

}