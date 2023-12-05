#include "Game.hpp"
#include <iostream>

Game::Game(int width, int height, const std::string& title, int windowStyle = sf::Style::Default)
	: windowWidth(width), windowHeight(height), window(sf::VideoMode(width, height), title, windowStyle),
	eventManager(window, true), context(window)
{
	addEvents();
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

void Game::addEvents()
{
	// close window
	eventManager.addEventCallback(sf::Event::EventType::Closed, [&](const sf::Event& event) { window.close(); });
	eventManager.addKeyPressedCallback(sf::Keyboard::Escape, [&](const sf::Event& event) { window.close(); });

	// update mouse position
	eventManager.addEventCallback(sf::Event::MouseMoved, [&](const sf::Event& event) {
		context.stateManager.updateMousePosition(eventManager.getFloatMousePosition());
		});
	eventManager.addMousePressedCallback(sf::Mouse::Left, [&](const sf::Event& event) {
		context.stateManager.click(eventManager.getFloatMousePosition());
		const sf::Vector2f screenMousePosition = getScreenMousePosition();
		const sf::Vector2f worldMousePosition = getWorldMousePosition();
		std::cout << "screen mouse: (" << screenMousePosition.x << ", " << screenMousePosition.y << ")\n";
		std::cout << "world mouse: (" << worldMousePosition.x << ", " << worldMousePosition.y << ")\n";
		});
	eventManager.addMouseReleasedCallback(sf::Mouse::Left, [&](const sf::Event& event) {
		context.stateManager.release();
		});
}

const sf::Vector2f Game::getScreenMousePosition()
{
	return context.stateManager.getScreenMousePosition();
}

const sf::Vector2f Game::getWorldMousePosition()
{
	return context.stateManager.getWorldMousePosition();
}