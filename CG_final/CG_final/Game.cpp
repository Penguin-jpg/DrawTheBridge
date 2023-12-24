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
}

void Game::createLevelOneScene() {

	clearLevelGameScene();
	sf::Vector2f recPos = { 90,240 };
	solver.addRectangle(recPos, true, 13, 30, 5.0f, ParticleColor::Deg2);
	recPos = { 400, 230 };
	solver.addRectangle(recPos, true, 10, 15, 5.0f, ParticleColor::Deg2);
	destinationPos = { 408.774, 124.811 };
	ballPos = { 151.5, 28.3 };
	ballIndex = solver.addCircle(ballPos, 10.0f, 6);
}

void Game::createLevelTwoScene() {

	clearLevelGameScene();
	sf::Vector2f recPos = { 65, 100 };
	solver.addRectangle(recPos, true, 12, 3, 5.0f, ParticleColor::Deg14);
	recPos = { 200, 300 };
	solver.addRectangle(recPos, true, 60, 1, 5.0f, ParticleColor::Deg14);

	sf::Vector2f circlePos = { 220, 80 };
	std::pair<int, int> particleIndexRange = solver.addCircle(circlePos, 30, 20, ParticleColor::Deg15);
	addObstacle(particleIndexRange, 1, 1, std::pair(150, 450), std::pair(0, 0), 120.0f);
	circlePos = { 380, 170 };
	particleIndexRange = solver.addCircle(circlePos, 30, 20, ParticleColor::Deg15);
	addObstacle(particleIndexRange, 0, 1, std::pair(150, 450), std::pair(0, 0), 120.0f);

	destinationPos = { 432, 261.226 };
	ballPos = { 141.5, 27.3 };
	ballIndex = solver.addCircle(ballPos, 10.0f, 6);
	// std::cout << "ballIndex : " << ballIndex.first << " " << ballIndex.second << std::endl;

}

void Game::addObstacle(std::pair<int, int> particleIndexRange, int isFowrad, int moveHorizontal, std::pair<int, int> horizontalBound, std::pair<int, int> verticalBound, float movingSpeed) {

	Obstacle anObstacle;
	anObstacle.particleIndexRange = particleIndexRange;
	anObstacle.isFowrad = isFowrad;
	anObstacle.moveHorizontal = moveHorizontal;
	anObstacle.horizontalBound = horizontalBound;
	anObstacle.verticalBound = verticalBound;
	anObstacle.movingSpeed = movingSpeed;
	obstacles.push_back(anObstacle);
	return;
}

void Game::createLevelThreeScene() {

	clearLevelGameScene();
	sf::Vector2f recPos = { 200, 150 };
	solver.addRectangle(recPos, true, 14, 3, 5.0f, ParticleColor::Deg14);

	recPos = { 380, 200 };
	solver.addRectangle(recPos, true, 12, 3, 5.0f, ParticleColor::Deg14);

	recPos = { 200, 300 };
	solver.addRectangle(recPos, true, 60, 1, 5.0f, ParticleColor::Deg14);


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
		

		const sf::Vector2f mousePosition = getWorldMousePosition();
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		const sf::Vector2f gridCoord = (sf::Vector2f)solver.getGrid().getGridCoordinate(mousePosition, OBJECT_RADIUS);
		sf::Vector2f objectPosition(gridCoord.y * CELL_SIZE + OBJECT_RADIUS, gridCoord.x * CELL_SIZE + OBJECT_RADIUS);



		if (isMouseLeftPressAndReleased) {
			isMouseLeftPressAndReleased = false;
			StateID preStateID = currentStateID;
			currentGameState->handleInput(window, mousePos, currentStateID, ballReachDestination);


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
				solver.addCube(mousePosition, 1.0f, pinned);
			}
			else if (buildMode == 2 && spawnTimer.getElapsedTime().asSeconds() >= CIRCLE_SPAWN_TIME)
			{
				spawnTimer.restart();
				std::cout << "mousePosition : " << mousePosition.x << " " << mousePosition.y << std::endl;
				sf::Vector2f initVelocity = { -1.0, 0.3 };
				solver.addCircle(mousePosition, 10.0f, 6, initVelocity);
			}

		}

		if (grabbing)
		{
			civ::Ref<Particle> grabbed = solver.getNearestParticle(mousePosition);
			if (grabbed)
			{
				sf::Vector2f direction = mousePosition - grabbed->currentPosition;
				float distance = Math::getLength(direction);
				if (distance > 0.3f)
				{
					sf::Vector2f unit = direction / distance;
					grabbed->move(unit);
				}
			}
		}

		currentGameState->btnIsHovered(window, mousePos);
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
				solver.updateObstacle(obstacles[i].moveHorizontal, obstacles[i].isFowrad, obstacles[i].particleIndexRange, obstacles[i].horizontalBound, obstacles[i].verticalBound, obstacles[i].movingSpeed, dt);
			}
		}
		
		
		if (isInLevelGameState() && !ballReachDestination) {
			ballReachDestination = solver.IsBallReachDestination(ballIndex, destinationPos);
		}
		
		clear();
		
		currentGameState->render(window);

		if (isInLevelGameState()) {


			renderer.render(context, getWorldMousePosition(), buildMode, showGrid);

			// if ( )
		}
		if (ballReachDestination) {
			currentGameState->renderWinWindow(window);
		}


		// currentGameState->renderWinWindow(window);
		//window.draw(text); // Draw the text
		//game.getWindow().draw(spline);
		display();


		//testText = false;
	}

}