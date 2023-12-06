#pragma once
#include <SFML/Graphics.hpp>
#include "StateManager.hpp"

// this idea is from https://github.com/johnBuffer/VerletSFML-Multithread

// this struct is the real one that doing drawing and transformation
struct RenderContext
{
	sf::RenderWindow& window;
	StateManager stateManager;

	RenderContext(sf::RenderWindow& window)
		:window(window), stateManager((sf::Vector2f)window.getSize()) {}

	void draw(sf::Drawable& drawable, sf::RenderStates states)
	{
		// apply transform
		states.transform = stateManager.getTransform();
		window.draw(drawable, states);
	}

	void setFocus(const sf::Vector2f& focus)
	{
		stateManager.setFocus(focus);
	}

	void setZoom(float zoom)
	{
		stateManager.setZoom(zoom);
	}
};