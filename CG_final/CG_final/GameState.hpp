#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Item.hpp"
#include "RenderContext.hpp"

enum class StateID {
	StartMenu,
	LevelSelection,
	Level1,
	Level2,
	Level3,
	Refresh,
	LevelGamePlay
};

class GameState {
public:
	std::vector<StateID> levelStates = { StateID::Level1, StateID::Level2, StateID::Level3 };
	virtual bool handleInput(const sf::Vector2f& mousePos, StateID& currentStateID, bool isWin, bool isLose) = 0;
	virtual void update(float dt) = 0;
	virtual void render(RenderContext& context) = 0;
	virtual void btnIsHovered(const sf::Vector2f& mousePos) = 0;
	virtual void flgIsPressed(bool chaining, bool pinned, bool showGrid, bool useWind, bool grabbing, bool pause) = 0;
	virtual void btnSetColor(Item& item, const sf::Vector2f& mousePos) {
		if (item.mSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
			item.mSprite.setColor(sf::Color(150, 150, 150));
		}
		else {
			item.mSprite.setColor(sf::Color(255, 255, 255));
		}
	};
	virtual void flgSetColor(Item& item, bool isPressed)
	{
		if (isPressed) {
			item.mSprite.setColor(sf::Color(255, 255, 255));

		}
		else {
			item.mSprite.setColor(sf::Color(150, 150, 150));
		}
	}

	virtual void renderWinWindow(RenderContext& context) {}
	virtual void renderLoseWindow(RenderContext& context) {}

	virtual ~GameState() {}

};

class StartMenuState : public GameState {
	Item startButton;
	Item background;
	Item logo;

public:

	StartMenuState();
	StartMenuState(sf::RenderWindow& window);

	bool handleInput(const sf::Vector2f& mousePos, StateID& currentStateID, bool isWin, bool isLose) override;
	void btnIsHovered(const sf::Vector2f& mousePos) override;
	void update(float dt) override;
	void render(RenderContext& context) override;
	void flgIsPressed(bool chaining, bool pinned, bool showGrid, bool useWind, bool grabbing, bool pause) {

	}
};

class LevelSelectionState : public GameState {

public:
	Item startButton;
	Item background;
	Item titleBtn;
	Item backBtn;

	const int NUM_LEVELS = 3;
	std::vector<Item> levelButton;



	LevelSelectionState();

	LevelSelectionState(sf::RenderWindow& window);
	bool handleInput(const sf::Vector2f& mousePos, StateID& currentStateID, bool isWin, bool isLose) override;
	void btnIsHovered(const sf::Vector2f& mousePos) override;
	void update(float dt) override;
	void render(RenderContext& context) override;
	void flgIsPressed(bool chaining, bool pinned, bool showGrid, bool useWind, bool grabbing, bool pause) {

	}
};


class LevelGameState : public GameState {

public:
	Item background;
	Item backBtn;
	Item destinationFlag;
	Item refreshBtn;
	Item chainingFlg;
	Item pinnedFlg;
	Item showGridFlg;
	Item windFlg;
	Item grabbingFlg;
	Item pauseFlg;
	Item playBtn;
	Item pauseBtn;
	Item win;
	Item win_reload;
	Item win_continue;
	Item game_over;
	Item game_over_list;

	LevelGameState()
		: background("assets/UI/background2.jpg"), destinationFlag("assets/UI/red-flag.png")
		, chainingFlg("assets/UI/chaining.png"), pinnedFlg("assets/UI/pinned.png")
		, showGridFlg("assets/UI/show_grid.png"), windFlg("assets/UI/wind.png")
		, grabbingFlg("assets/UI/grabbing.png"), pauseFlg("assets/UI/pause.png")
		, playBtn("assets/UI/play.png"), pauseBtn("assets/UI/pause-button.png")
		, win("assets/UI/win2.png"), win_reload("assets/UI/win_reload.png")
		, win_continue("assets/UI/win_continue.png"), game_over("assets/UI/game_over.png")
		, game_over_list("assets/UI/list.png") {}

	LevelGameState(sf::RenderWindow& window);

	bool handleInput(const sf::Vector2f& mousePos, StateID& currentStateID, bool isWin, bool isLose) override;

	void btnIsHovered(const sf::Vector2f& mousePos) override;
	void flgIsPressed(bool chaining, bool pinned, bool showGrid, bool useWind, bool grabbing, bool pause) override;
	void update(float dt) override;
	void render(RenderContext& context) override;
	void renderWinWindow(RenderContext& context) override;
	void renderLoseWindow(RenderContext& context) override;

};

class Level1 : public LevelGameState {

public:

	Level1() = default;

	Level1(sf::RenderWindow& window);

	//void render(RenderContext& context) override;
};

class Level2 : public LevelGameState {

public:
	Item obstacle1;
	Item obstacle2;


	Level2() = default;
	Level2(sf::RenderWindow& window);
};

class Level3 : public LevelGameState {

public:

	Level3() = default;
	Level3(sf::RenderWindow& window);
};