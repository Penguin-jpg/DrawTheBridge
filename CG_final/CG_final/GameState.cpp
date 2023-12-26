#include "GameState.hpp"
#include "Solver.hpp"
#include "Renderer.hpp"

StartMenuState::StartMenuState() : background("assets/UI/background2.jpg") {
}



StartMenuState::StartMenuState(sf::RenderWindow& window) : startButton("assets/UI/start-button.png"), background("assets/UI/background2.jpg"), logo("assets/UI/logo.png") {
	startButton.setScale(false, window, 1.0f, 1.0f);
	startButton.setPosition(700, 350);
	logo.setScale(false, window, 0.3f, 0.3f);
	logo.setPosition(350, 50);
	background.setScale(true, window, 1.0f, 1.0f);
}

bool StartMenuState::handleInput(sf::RenderWindow& window, const sf::Vector2f& mousePos, StateID& currentStateID, bool isWin, bool isLose) {
	if (startButton.mSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
		std::cout << "Start" << std::endl;
		currentStateID = StateID::LevelSelection;
		return true;
	}

	return false;
}


void StartMenuState::btnIsHovered(sf::RenderWindow& window, const sf::Vector2f& mousePos)
{
	btnSetColor(startButton, mousePos);
}

void StartMenuState::update(float dt) {

	// Update animations or other elements
}

void StartMenuState::render(sf::RenderWindow& window) {

	window.draw(background.mSprite);
	window.draw(logo.mSprite);
	// window.draw(startButton);
	// Render other UI components if any
	window.draw(startButton.mSprite);

}


LevelSelectionState::LevelSelectionState() : background("assets/UI/background2.jpg") {

}


LevelSelectionState::LevelSelectionState(sf::RenderWindow& window) : levelButton(NUM_LEVELS), background("assets/UI/background2.jpg"), titleBtn("assets/UI/select_level.png"), backBtn("assets/UI/back.png") {
	std::string levelImage[3] = { "one", "two", "three" };
	titleBtn.setScale(false, window, 0.3f, 0.3f);
	titleBtn.setPosition(450, 50);
	backBtn.setScale(false, window, 0.15f, 0.15f);
	backBtn.setPosition(70, 40);
	for (int i = 0; i < NUM_LEVELS; i++) {
		std::string imagePath = "assets/UI/" + levelImage[i] + ".png";
		levelButton[i].setTexure(imagePath);
		levelButton[i].setScale(false, window, 0.5f, 0.5f);
		levelButton[i].setPosition(350 + i * 500, 400); // Adjust for scaled size
	}
	background.setScale(true, window, 1.0f, 1.0f);
}

bool LevelSelectionState::handleInput(sf::RenderWindow& window, const sf::Vector2f& mousePos, StateID& currentStateID, bool isWin, bool isLose) {
	for (int i = 0; i < NUM_LEVELS; ++i) {
		if (levelButton[i].mSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
			std::cout << "Level " << i + 1 << " pressed" << std::endl;
			currentStateID = levelStates[i];
			return true;
		}
	}

	if (backBtn.mSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
		currentStateID = StateID::StartMenu;
		return true;
	}
	return false;
}

void LevelSelectionState::btnIsHovered(sf::RenderWindow& window, const sf::Vector2f& mousePos)
{

	btnSetColor(backBtn, mousePos);
	for (auto& levelBtn : levelButton) {
		btnSetColor(levelBtn, mousePos);
	}

}

void LevelSelectionState::update(float dt) {

	// Update animations or other elements
}

void LevelSelectionState::render(sf::RenderWindow& window) {

	window.draw(background.mSprite);
	window.draw(backBtn.mSprite);
	window.draw(titleBtn.mSprite);
	for (const auto& levelBtn : levelButton) {
		window.draw(levelBtn.mSprite);
	}

}



LevelGameState::LevelGameState(sf::RenderWindow& window) :background("assets/UI/dark_background.jpg"), backBtn("assets/UI/back.png"), destinationFlag("assets/UI/red-flag.png"), refreshBtn("assets/UI/refresh.png"), chainingFlg("assets/UI/chaining.png"), pinnedFlg("assets/UI/pinned.png"), showGridFlg("assets/UI/show_grid.png"), windFlg("assets/UI/wind.png"), grabbingFlg("assets/UI/grabbing.png"), pauseFlg("assets/UI/pause.png"), playBtn("assets/UI/play.png"), pauseBtn("assets/UI/pause-button.png"), win("assets/UI/win2.png"), win_reload("assets/UI/win_reload.png"), win_continue("assets/UI/win_continue.png"), game_over("assets/UI/game_over.png"), game_over_list("assets/UI/list.png") {

	backBtn.setScale(false, window, 0.15f, 0.15f);
	backBtn.setPosition(70, 40);
	refreshBtn.setScale(false, window, 0.15f, 0.15f);
	refreshBtn.setPosition(170, 40);

	chainingFlg.setScale(false, window, 0.13f, 0.13f);
	chainingFlg.setPosition(1730, 30);
	pinnedFlg.setScale(false, window, 0.13f, 0.13f);
	pinnedFlg.setPosition(1730, 90);
	showGridFlg.setScale(false, window, 0.13f, 0.13f);
	showGridFlg.setPosition(1730, 150);
	windFlg.setScale(false, window, 0.13f, 0.13f);
	windFlg.setPosition(1730, 210);
	grabbingFlg.setScale(false, window, 0.13f, 0.13f);
	grabbingFlg.setPosition(1730, 270);
	pauseFlg.setScale(false, window, 0.13f, 0.13f);
	pauseFlg.setPosition(1730, 330);

	playBtn.setScale(false, window, 0.15f, 0.15f);
	playBtn.setPosition(270, 40);
	pauseBtn.setScale(false, window, 0.15f, 0.15f);
	pauseBtn.setPosition(370, 40);

	win.setScale(false, window, 0.25f, 0.25f);
	win.setPosition(550, 340);
	win_reload.setScale(false, window, 0.8f, 0.8f);
	win_reload.setPosition(765, 650);
	win_continue.setScale(false, window, 0.8f, 0.8f);
	win_continue.setPosition(965, 650);

	game_over.setScale(false, window, 0.25f, 0.25f);
	game_over.setPosition(550, 340);
	game_over_list.setScale(false, window, 0.23f, 0.23f);
	game_over_list.setPosition(965, 650);
}

bool LevelGameState::handleInput(sf::RenderWindow& window, const sf::Vector2f& mousePos, StateID& currentStateID, bool isWin, bool isLose) {

	if (backBtn.mSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
		currentStateID = StateID::LevelSelection;
		return true;
	}
	if (refreshBtn.mSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
		std::cout << "refresh !!!!1" << std::endl;
		currentStateID = StateID::Refresh;
		return true;
	}

	if (playBtn.mSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
		std::cout << "refresh !!!!1" << std::endl;
		currentStateID = StateID::LevelGamePlay;
		return true;
	}

	if (isWin) {
		if (win_continue.mSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
			std::cout << "win continue !!!!1" << std::endl;
			currentStateID = StateID::LevelSelection;
			return true;
		}

		if (win_reload.mSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
			std::cout << "win reload !!!!1" << std::endl;
			currentStateID = StateID::Refresh;
			return true;
		}
	}

	if (isLose) {
		if (win_continue.mSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
			std::cout << "win continue !!!!1" << std::endl;
			currentStateID = StateID::LevelSelection;
			return true;
		}

		if (win_reload.mSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
			std::cout << "win reload !!!!1" << std::endl;
			currentStateID = StateID::Refresh;
			return true;
		}
	}
	return false;
}

void LevelGameState::btnIsHovered(sf::RenderWindow& window, const sf::Vector2f& mousePos)
{
	btnSetColor(backBtn, mousePos);
	btnSetColor(refreshBtn, mousePos);
	btnSetColor(playBtn, mousePos);
	btnSetColor(playBtn, mousePos);
	btnSetColor(win_reload, mousePos);
	btnSetColor(win_continue, mousePos);
	btnSetColor(game_over_list, mousePos);
	//btnSetColor(pauseBtn, mousePos);
}




void LevelGameState::flgIsPressed(bool chaining, bool pinned, bool showGrid, bool useWind, bool grabbing, bool pause) {

	flgSetColor(chainingFlg, chaining);
	flgSetColor(pinnedFlg, pinned);
	flgSetColor(showGridFlg, showGrid);
	flgSetColor(windFlg, useWind);
	flgSetColor(grabbingFlg, grabbing);
	flgSetColor(pauseFlg, pause);
}

void LevelGameState::update(float dt) {

	// Update animations or other elements
}

void LevelGameState::render(sf::RenderWindow& window) {

	window.draw(background.mSprite);
	window.draw(backBtn.mSprite);
	window.draw(refreshBtn.mSprite);
	window.draw(destinationFlag.mSprite);
	window.draw(chainingFlg.mSprite);
	window.draw(pinnedFlg.mSprite);
	window.draw(showGridFlg.mSprite);
	window.draw(windFlg.mSprite);
	window.draw(grabbingFlg.mSprite);
	window.draw(pauseFlg.mSprite);
	window.draw(playBtn.mSprite);

	// window.draw(pauseBtn.mSprite);
}

void LevelGameState::renderWinWindow(sf::RenderWindow& window) {

	window.draw(win.mSprite);
	window.draw(win_reload.mSprite);
	window.draw(win_continue.mSprite);
}

void LevelGameState::renderLoseWindow(sf::RenderWindow& window) {

	window.draw(game_over.mSprite);
	window.draw(win_reload.mSprite);
	window.draw(game_over_list.mSprite);
}

Level1::Level1(sf::RenderWindow& window) : LevelGameState(window) {
	destinationFlag.setScale(false, window, 0.4f, 0.4f);
	destinationFlag.setPosition(1450, 370);
};

/*
void Level1::render(sf::RenderWindow& window) {

	window.draw(background.mSprite);
	window.draw(backBtn.mSprite);
	window.draw(refreshBtn.mSprite);
	window.draw(destinationFlag.mSprite);
	window.draw(chainingFlg.mSprite);
	window.draw(pinnedFlg.mSprite);
	window.draw(showGridFlg.mSprite);
	window.draw(windFlg.mSprite);
	window.draw(grabbingFlg.mSprite);
	window.draw(pauseFlg.mSprite);
}
*/

Level2::Level2(sf::RenderWindow& window) : LevelGameState(window) {
	destinationFlag.setScale(false, window, 0.4f, 0.4f);
	destinationFlag.setPosition(1550, 830);
};

/*
void Level2::render(sf::RenderWindow& window) {

	window.draw(background.mSprite);
	window.draw(backBtn.mSprite);
	window.draw(refreshBtn.mSprite);
	window.draw(destinationFlag.mSprite);

}
*/
Level3::Level3(sf::RenderWindow& window) : LevelGameState(window) {
	destinationFlag.setScale(false, window, 0.4f, 0.4f);
	destinationFlag.setPosition(250, 340);
};

/*
void Level3::render(sf::RenderWindow& window) {
	window.draw(background.mSprite);
	window.draw(backBtn.mSprite);
	window.draw(refreshBtn.mSprite);
	window.draw(destinationFlag.mSprite);
}
*/