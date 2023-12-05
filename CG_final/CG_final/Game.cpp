#include "Game.hpp"

Game::Game(int width, int height, const std::string& title, int windowStyle = sf::Style::Default)
	: windowWidth(width), windowHeight(height), window(sf::VideoMode(width, height), title, windowStyle),
	eventManager(window, true), context(window)
{
	addBasicEvents();
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
	eventManager.addEventCallback(sf::Event::EventType::Closed, [&](const sf::Event& event) { window.close(); });
	eventManager.addKeyPressedCallback(sf::Keyboard::Escape, [&](const sf::Event& event) { window.close(); });
}

const sf::Vector2f Game::getMousePosition()
{
	return eventManager.getFloatMousePosition();
}
