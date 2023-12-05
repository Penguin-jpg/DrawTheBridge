#include "StateManager.hpp"

StateManager::StateManager(sf::Vector2f windowSize) :state(windowSize)
{
	state.update();
}

const sf::Transform& StateManager::getTransform()
{
	return state.transform;
}

void StateManager::setFocus(sf::Vector2f focus)
{
	state.offset = focus;
	state.update();
}

void StateManager::setZoom(float zoom)
{
	state.zoom = zoom;
	state.update();
}

void StateManager::adjustOffset(sf::Vector2f amount)
{
	// need to consider zoom
	state.offset += amount / state.zoom;
	state.update();
}

void StateManager::click(sf::Vector2f clickedMousePosition)
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

void StateManager::updateMousePosition(sf::Vector2f newMousePosition)
{
	if (state.clicked)
	{
		// adjust offset so the click position will be correct
		adjustOffset(state.screenMousePosition - newMousePosition);
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
