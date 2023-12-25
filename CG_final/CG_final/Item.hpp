#pragma once
#include <SFML/Graphics.hpp>

struct Item
{
	std::string mImagePath;
	sf::Texture mTexture;
	sf::Sprite mSprite;

	Item() = default;

	Item(std::string imagePath) : mImagePath(imagePath)
	{
		if (!mTexture.loadFromFile(mImagePath)) {
			// handle error
		}

		mSprite.setTexture(mTexture);
	}

	void setTexure(const std::string& imagePath) {
		mImagePath = imagePath;
		if (!mTexture.loadFromFile(mImagePath)) {
			// handle error
		}

		mSprite.setTexture(mTexture);
	}

	void setPosition(float positionX, float positionY) {
		mSprite.setPosition(positionX, positionY); // Adjust for scaled size
	}

	void setScale(bool scaleToWindow, sf::RenderWindow& window, float scaleX, float scaleY) {
		if (scaleToWindow) {
			sf::Vector2u textureSize = mTexture.getSize();
			sf::Vector2u windowSize = window.getSize();
			scaleX = static_cast<float>(windowSize.x) / textureSize.x;
			scaleY = static_cast<float>(windowSize.y) / textureSize.y;
		}

		mSprite.setScale(scaleX, scaleY);

	}

	bool isMouseOver(sf::RenderWindow& window) {
		/*
		float mouseX = sf::Mouse::getPosition(window).x;
		float mouseY = sf::Mouse::getPosition(window).y;

		float btnPosX = shape.getPosition().x;
		float btnPosY = shape.getPosition().y;

		float btnxPosWidth = shape.getPosition().x + shape.getLocalBounds().width;
		float btnyPosHeight = shape.getPosition().y + shape.getLocalBounds().height;

		if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY) {
			return true;
		}

		*/
		return false;
	}
};