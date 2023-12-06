#include "StateManager.hpp"

StateManager::StateManager(sf::Vector2f windowSize) :state(windowSize)
{
	updateTransform();
}

const sf::Transform& StateManager::getTransform()
{
	return state.transform;
}

void StateManager::updateTransform()
{
	// to make object center on the screen, we need to:
	// 1. translate a distance half of the object's width and height
	//    (this step is to make sure that object starting drawing from the correct position)
	// 2. (optinoal) zoom in so that the object can fill the screen
	// 3. translate it back to the center of window
	state.transform = sf::Transform::Identity;
	state.transform.translate(state.center);
	state.transform.scale({ state.zoom, state.zoom });
	state.transform.translate(-state.offset);
}

void StateManager::setFocus(const sf::Vector2f& focus)
{
	state.offset = focus;
	updateTransform();
}

void StateManager::setZoom(float zoom)
{
	state.zoom = zoom;
	updateTransform();
}

void StateManager::adjustOffset(const sf::Vector2f& amount)
{
	// need to consider zoom
	state.offset += amount / state.zoom;
	updateTransform();
}

void StateManager::click(const sf::Vector2f& clickedMousePosition)
{
	state.clicked = true;
	state.screenMousePosition = clickedMousePosition;
}

void StateManager::release()
{
	state.clicked = false;
}

bool StateManager::isClicked()
{
	return state.clicked;
}

void StateManager::updateMousePosition(const sf::Vector2f& newMousePosition)
{
	if (state.clicked)
	{
		// adjust offset so the click position will be correct
		//adjustOffset(state.screenMousePosition - newMousePosition);
	}

	state.screenMousePosition = newMousePosition;
	// since the position on the screen is not the real world position, we need to use inverse
	// transform to take it back to world coordinate
	state.worldMousePosition = state.offset + (newMousePosition - state.center) / state.zoom;
}

const sf::Vector2f StateManager::getScreenMousePosition()
{
	return state.screenMousePosition;
}

const sf::Vector2f StateManager::getWorldMousePosition()
{
	return state.worldMousePosition;
}