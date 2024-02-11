#include "gamestate.h"
#include "level.h"
#include "player.h"
#include <thread>
#include <chrono>

GameState::GameState()
{
}

bool GameState::init()
{


	graphics::preloadBitmaps(getAssetDir());
	graphics::setFont(m_asset_path + "OpenSans-Regular.ttf");

	return true;
}

void GameState::draw()
{
	if (!m_current_level) {
		return;
	}
	m_current_level->draw();
	if (MenuStatus == 1) {
		graphics::Brush br;
		char levelsUi[40];
		sprintf_s(levelsUi, "Level : %d", levelCount);
		graphics::drawText(getCanvasWidth() / 2, 0.2f, 0.15f, levelsUi, br);
	}
}

void GameState::update(float dt)
{
	if (dt > 500) {
		return;
	}
	if (MenuStatus == 0) { //menu
		if (graphics::getKeyState(graphics::SCANCODE_SPACE)) {
			graphics::playSound(getFullAssetPath("music.mp3"), 0.5f);
			MenuStatus = 1;
			m_current_level = new Level("1.lvl");
			m_current_level->init();
			levelCount++;

			m_player = new Player("Player");
			m_player->init();
		}
		else {
			graphics::Brush br;
			char levelsUi[40];
			sprintf_s(levelsUi, "Press Space to Start Game");
			graphics::drawText(getCanvasWidth() / 2-1.6f, getCanvasHeight()/2, 0.3f, levelsUi, br);
			return;
		}
	}
	else {
		if (m_player->Health <= 0) { //gameover
			m_current_level = nullptr;
			levelCount = 0;
			graphics::Brush br;
			char levelsUi[40];
			sprintf_s(levelsUi, "GameOver Press R to Restart");
			graphics::playSound(getFullAssetPath("gameover.wav"), 0.5f);
			graphics::drawText(getCanvasWidth() / 2 - 1.6f, getCanvasHeight() / 2, 0.3f, levelsUi, br);
			if (graphics::getKeyState(graphics::SCANCODE_R)) {
				MenuStatus = 1;
				m_current_level = new Level("1.lvl");
				m_current_level->init();
				levelCount++;
				m_player = nullptr;
				m_player = new Player("Player");
				m_player->init();
			}
		}
		else if (m_player->Health > 0 && m_current_level->Finish==1) { //next level generator
			graphics::playSound(getFullAssetPath("win.wav"), 0.5f);
			m_current_level = nullptr;
			m_current_level = new Level("1.lvl");
			m_current_level->init();
			levelCount++;
			m_player = nullptr;
			m_player = new Player("Player");
			m_player->init();
		}
		
	}

	float sleep_time = std::max(0.0f, 17.0f - dt);

	std::this_thread::sleep_for(std::chrono::duration<float,std::milli>(sleep_time));

	if (!m_current_level) {
		return;
	}
	m_current_level->update(dt);

	m_debugging = graphics::getKeyState(graphics::SCANCODE_0);
}

GameState* GameState::getInstance()
{
	if (m_unique_instance == nullptr)
	{
		m_unique_instance = new GameState();
	}
	return m_unique_instance;
}

GameState::~GameState()
{
	if (m_player)
	{
		delete m_player;
	}
	if (m_current_level)
	{
		delete m_current_level;
	}
}

std::string GameState::getAssetDir()
{
	return m_asset_path;
}

std::string GameState::getFullAssetPath(const std::string& asset)
{
	return m_asset_path + asset;
}

GameState* GameState::m_unique_instance = nullptr;
