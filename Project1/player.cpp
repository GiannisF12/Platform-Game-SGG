#include"player.h"
#include <sgg/graphics.h>
#include "gamestate.h"
#include "util.h"
#include <filesystem>
#include <iostream>

void Player::movePlayer(float dt)
{
	float delta_time = dt / 1000.0f;

	float move = 0.0f;
	//horizontal
	if (graphics::getKeyState(graphics::SCANCODE_A)) {
		move -= 1.0f;
	}
	if (graphics::getKeyState(graphics::SCANCODE_D)) {
		move += 1.0f;
	}
	velX = std::min(maxVelocity, velX + delta_time * move * horAccelaration);
	velX = std::max(-maxVelocity, velX);
	velX -= 0.2f * velX/(0.1f + fabs(velX));
	if (fabs(velX) < 0.01f) {
		velX = 0.0f;
	}
	m_pos_x += delta_time * velX;

	//vertical
	if (graphics::getKeyState(graphics::SCANCODE_W)) {
		velY -= vertAccelaration;
	}
	else {
		velY -= 0.0f;
	}
	if (velY < -3) {
		velY = -3;
	}

	velY += delta_time * gravity;

	m_pos_y += velY * delta_time;
}

void Player::update(float dt)
{
	movePlayer(dt);

	m_state->m_global_offset_x = m_state -> getCanvasWidth() / 2.0f - m_pos_x;
	m_state->m_global_offset_y = m_state -> getCanvasHeight() / 2.0f - m_pos_y;

	GameObject::update(dt);
}

void Player::init()
{
	Score = 0.0f;
	Health = 100.0f;
	m_pos_x = 5.0f;
	m_pos_y = -2.0f;
	m_width /= 3.0f;
	m_height /= 2.2f;
	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;

	m_brush_player.fill_opacity = 1.0f;
	m_brush_player.outline_opacity = 0.0f;
	m_brush_player.texture = m_state->getFullAssetPath("./idle_left/right01.png");

	m_brush_player_debug.fill_opacity = 0.1f;
	SETCOLOR(m_brush_player_debug.fill_color, 1.0f, 0.0f, 0.0f);
	SETCOLOR(m_brush_player_debug.outline_color, 1.0f, 0.0f, 0.0f);

	fillVector(m_sprites_left, "assets/idle_left/");
	fillVector(m_sprites_right, "assets/idle_right/");

}

void Player::fillVector(std::vector<std::string>& vector, std::string asset_folder) {


	try {
		for (const auto& entry : std::filesystem::directory_iterator(asset_folder)) {
			std::string filename = entry.path().string();
			std::cout << "Pushing to animation vector: " + filename << std::endl;
			vector.push_back(filename);
		}
	}

	catch (const std::filesystem::filesystem_error& ex) {
		std::cerr << "Error accessing the directory: " << ex.what() << std::endl;
	}

}

void Player::draw()
{
	if (graphics::getKeyState(graphics::SCANCODE_A)) {
		int s = (int)fmodf(1000.0f - m_pos_x * 10.0f, m_sprites_left.size());
		m_brush_player.texture = m_sprites_left[s];
	}
	else if (graphics::getKeyState(graphics::SCANCODE_D)) {
		int s = (int)fmodf(1000.0f - m_pos_x * 10.0f, m_sprites_right.size());
		m_brush_player.texture = m_sprites_right[s];
	}


	graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_state->getCanvasHeight() * 0.5f, 0.4f, 0.5f, m_brush_player);
	if (m_state->m_debugging)
	{
		graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_state->getCanvasHeight() * 0.5f, m_width, m_height, m_brush_player_debug);
	}

	//UI
	graphics::Brush br;
	char healthUi[40];
	sprintf_s(healthUi, "Health : %g", Health);
	graphics::drawText(m_state->getCanvasWidth()-0.8f, 0.2f, 0.15f, healthUi, br);
}
