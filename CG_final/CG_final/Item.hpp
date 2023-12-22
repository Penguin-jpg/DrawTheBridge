#pragma once
#include <SFML/Graphics.hpp>

class Item {

public:

	Item();
	Item(std::string imagePath);
	std::string mImagePath;
	sf::Texture mTexture;
	sf::Sprite mSprite;

	void setTexure(std::string imagePath);
	void setPosition(float positionX, float positionY);
	void setScale(bool scaleToWindow, sf::RenderWindow& window, float scaleX, float scaleY);
	bool isMouseOver(sf::RenderWindow& window);
};