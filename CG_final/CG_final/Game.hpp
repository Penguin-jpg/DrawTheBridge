#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "EventManager/event_manager.hpp"
#include "RenderContext.hpp"
#include "Item.hpp"
#include "GameState.hpp"
#include "Solver.hpp"
#include "Renderer.hpp"
#include "Obstacle.hpp"

class Game
{
public:
	Game(int width, int height, const std::string& title, int windowStyle);

	const int FRAMERATE = 60;
	const int NUM_SUB_STEPS = 8;
	const int NUM_NUM_OBJECTS = 2000;
	const float OBJECT_RADIUS = 5.0f;
	const int CELL_SIZE = 2 * OBJECT_RADIUS;
	const float PARTICLE_SPAWN_TIME = 0.08f;
	const float CUBE_SPAWN_TIME = 0.2f;
	const float CIRCLE_SPAWN_TIME = 0.2f;
	const sf::Vector2f WORLD_SIZE = { 500.0f, 300.0f };

	sf::RenderWindow& getWindow();
	sfev::EventManager& getEventManager();
	RenderContext& getRenderContext();

	// functions for main loop
	bool isRunning();
	void handleEvents();
	void clear(sf::Color clearColor = sf::Color::Black);
	void display();

	void setFramerate(int framerate);
	void addBasicEvents();
	void addAdditionalEvents();

	void click();
	void drag();
	void release();
	void undrag();
	const sf::Vector2f getScreenMousePosition();
	const sf::Vector2f getWorldMousePosition();
	const sf::Vector2f screenToWorldPosition(const sf::Vector2f& screenPosition);

	// store position for some checkings
	sf::Vector2f clickPosition;
	sf::Vector2f dragPosition;
	sf::Vector2f prevDragPosition;



	Solver solver;
	Renderer renderer;

	std::vector<civ::Ref<Particle>> chainedParitlces;
	std::vector<civ::Ref<Particle>> connected;
	civ::ID lastClicked = 0;


	void run();

private:
	int windowWidth;
	int windowHeight;

	sf::RenderWindow window;
	sfev::EventManager eventManager;
	RenderContext context;


	bool useForce = false; // apply force on objects
	bool isBuilding = false; // build or not
	int buildMode = 0; // 0: particle, 1: cube
	bool pinned = false; // pin objects or not
	bool chaining = false; // chain the drawn particles together
	bool showGrid = false; // show collision grid or not
	bool useWind = false; // use wind or not
	bool grabbing = false; // grab clicked object
	bool pause = true; // pause game or not


	sf::Vector2f ballPos;

	float destinationFlg_Radius = 10.0f;

	StateID currentStateID;
	GameState* currentGameState;
	GameState* startMenuState;
	GameState* levelSelectionState;
	GameState* level1;
	GameState* level2;
	GameState* level3;
	bool isMouseLeftPressed;
	bool isMouseLeftPressAndReleased;
	bool isInLevelGameState();
	void clearLevelGameScene();
	void createLevelOneScene();
	void createLevelTwoScene();
	void createLevelThreeScene();

	std::vector<std::pair<int, int>> circleIndex;

	std::pair<int, int> ballIndex;
	bool ballReachDestination;
	sf::Vector2f destinationPos;
	sf::Vector2f ballInitVelocity = { 0.0f, 0.0f };
	/*void addObstacle(int direction, bool moveHorizontal, const std::pair<int, int>& particleIndexRange,
		const std::pair<float, float>& horizontalBound, const std::pair<float, float>& verticalBound, float movingSpeed);*/

	bool isGameOver;
	bool testText = false;
};