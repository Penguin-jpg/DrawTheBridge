#pragma once
#include <SFML/Graphics.hpp>
#include "TransformManager.hpp"

// this idea is from https://github.com/johnBuffer/VerletSFML-Multithread

// this class is the real one that doing drawing and transformation
class RenderContext
{
public:
	RenderContext(sf::RenderWindow& window);

	void draw(sf::Drawable& drawable, sf::RenderStates states = {});

	void setFocus(sf::Vector2f focus);
	void setZoom(float zoom);

private:
	sf::RenderWindow& window;
	TransformManager transformManager;
};