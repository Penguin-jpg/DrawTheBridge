#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "EventManager/event_manager.hpp"
#include "RenderContext.hpp"

class Game
{
public:
	Game(int width, int height, const std::string& title, int windowStyle);

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

	void click();
	void release();
	const sf::Vector2f getScreenMousePosition();
	const sf::Vector2f getWorldMousePosition();

private:
	int windowWidth;
	int windowHeight;

	sf::RenderWindow window;
	sfev::EventManager eventManager;
	RenderContext context;
};