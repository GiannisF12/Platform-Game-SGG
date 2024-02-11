#include"coin.h"
#include <sgg/graphics.h>
#include "gamestate.h"
#include "util.h"
#include <filesystem>
#include <iostream>


void Coin::update(float dt)
{
	//float delta_time = dt / 1000.0f;

	//const float velocity = 10.0f;
	//if (graphics::getKeyState(graphics::SCANCODE_A)){
	//	m_pos_x -= delta_time * velocity;
	//}
	//if (graphics::getKeyState(graphics::SCANCODE_D)) {
	//	m_pos_x += delta_time * velocity;
	//}
	//if (graphics::getKeyState(graphics::SCANCODE_W)) {
	//	m_pos_y -= delta_time * velocity;
	//}
	//if (graphics::getKeyState(graphics::SCANCODE_S)) {
	//	m_pos_y += delta_time * velocity;
	//}


	GameObject::update(dt);
}

void Coin::init()
{
	m_pos_x = 5.0f;
	m_pos_y = 3.0f;
	m_width /= 3.0f;
	m_height /= 1.8f;

	coinBrush.fill_opacity = 1.0f;
	coinBrush.outline_opacity = 0.0f;
	coinBrush.texture = m_state->getFullAssetPath("coin/coin.png");

	coinBrushDebug.fill_opacity = 0.1f;
	SETCOLOR(coinBrushDebug.fill_color, 1.0f, 0.0f, 0.0f);
	SETCOLOR(coinBrushDebug.outline_color, 1.0f, 0.0f, 0.0f);

	fillVector(m_sprites, "assets/coin/");

}

void Coin::fillVector(std::vector<std::string>& vector, std::string asset_folder) {


	try {
		for (const auto& entry : std::filesystem::directory_iterator(asset_folder)) {
			// Convert std::filesystem::path to std::string
			std::string filename = entry.path().string();
			std::cout << "Pushing to animation vector: " + filename << std::endl;
			vector.push_back(filename);
		}
	}

	catch (const std::filesystem::filesystem_error& ex) {
		std::cerr << "Error accessing the directory: " << ex.what() << std::endl;
	}

}

void Coin::draw()
{
	int s = (int)fmodf(1000.0f - m_pos_x * 10.0f, m_sprites.size());
	coinBrush.texture = m_sprites[s];


	graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_state->getCanvasHeight() * 0.5f, 0.8f, 0.8f, coinBrush);
	if (m_state->m_debugging)
	{
		graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_state->getCanvasHeight() * 0.5f, m_width, m_height, coinBrushDebug);
	}
}
