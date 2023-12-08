#include "Game.hpp"
#include <iostream>

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