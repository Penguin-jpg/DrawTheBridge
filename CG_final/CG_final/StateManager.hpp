#pragma once
#include <SFML/Graphics.hpp>

// general state tracker
struct State
{
	// center of the main window
	sf::Vector2f center;
	// offset to adjust the position of object to make it center
	sf::Vector2f offset;
	// how close to look at
	float zoom = 1.0f;
	sf::Transform transform;

	// position of mouse on the screen coordinate
	sf::Vector2f screenMousePosition;
	// position of mouse in the world coordinate (the correct one)
	sf::Vector2f worldMousePosition;
	// is mouse clicked
	bool clicked = false;
	// is mouse dragging
	bool dragging = false;

	State(sf::Vector2f windowSize)
		:center(windowSize.x * 0.5f, windowSize.y * 0.5f), offset(center) {}
};

class StateManager
{
public:
	StateManager(sf::Vector2f windowSize);

	const sf::Transform& getTransform();

	void updateTransform();

	// set the focus location of camera
	void setFocus(const sf::Vector2f& focus);
	void setZoom(float zoom);

	// mouse-related functions
	void click(const sf::Vector2f& clickedMousePosition);
	void drag(const sf::Vector2f& draggingMousePosition);
	void release();
	void undrag();
	bool isClicked();
	bool isDragging();
	const sf::Vector2f screenToWorldPosition(const sf::Vector2f& screenPosition);
	void updateMousePosition(const sf::Vector2f& newMousePosition);
	const sf::Vector2f getScreenMousePosition();
	const sf::Vector2f getWorldMousePosition();

private:
	State state;
};