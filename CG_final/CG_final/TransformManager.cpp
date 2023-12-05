#include "TransformManager.hpp"

TransformManager::TransformManager(sf::Vector2f windowSize) :state(windowSize)
{
	state.update();
}

const sf::Transform& TransformManager::getTransform()
{
	return state.transform;
}

void TransformManager::setFocus(sf::Vector2f focus)
{
	state.offset = focus;
	state.update();
}

void TransformManager::setZoom(float zoom)
{
	state.zoom = zoom;
	state.update();
}
