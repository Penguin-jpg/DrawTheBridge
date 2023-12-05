#include "RenderContext.hpp"

RenderContext::RenderContext(sf::RenderWindow& window)
	:window(window), transformManager((sf::Vector2f)window.getSize()) {}

void RenderContext::draw(sf::Drawable& drawable, sf::RenderStates states)
{
	// apply transform
	states.transform = transformManager.getTransform();
	window.draw(drawable, states);
}

void RenderContext::setFocus(sf::Vector2f focus)
{
	transformManager.setFocus(focus);
}

void RenderContext::setZoom(float zoom)
{
	transformManager.setZoom(zoom);
}
