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

	State(sf::Vector2f windowSize)
		:center(windowSize.x * 0.5f, windowSize.y * 0.5f), offset(center) {}

	// update transform
	void update()
	{
		// to make object center on the screen, we need to:
		// 1. translate a distance half of the object's width and height
		//    (this step is to make sure that object starting drawing from the correct position)
		// 2. (optinoal) zoom in so that the object can fill the screen
		// 3. translate it back to the center of window

		transform = sf::Transform::Identity;
		// translate to center of window
		transform.translate(center);
		// scale according to zoom
		transform.scale({ zoom, zoom });
		// translate according to offset
		transform.translate(-offset);
	}
};

class StateManager
{
public:
	StateManager(sf::Vector2f windowSize);

	const sf::Transform& getTransform();

	// set the focus location of camera
	void setFocus(sf::Vector2f focus);
	void setZoom(float zoom);

	void adjustOffset(sf::Vector2f amount);

	// mouse-related functions
	void click(sf::Vector2f clickedMousePosition);
	void release();
	bool isClicked();
	void updateMousePosition(sf::Vector2f newMousePosition);
	const sf::Vector2f getScreenMousePosition();
	const sf::Vector2f getWorldMousePosition();

private:
	State state;
};