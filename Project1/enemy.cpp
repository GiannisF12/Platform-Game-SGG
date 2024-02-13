#include"enemy.h"
#include <sgg/graphics.h>
#include "gamestate.h"
#include "util.h"
#include <filesystem>
#include <iostream>

void Enemy::movePlayer(float dt)
{
	float delta_time = dt / 1000.0f;

	float move = 0.0f;

	if (direction==-1) {
		move -= 1.0f;
	}
	else {
		move += 1.0f;
	}
	velX = std::min(maxVelocity, velX + delta_time * move * horAccelaration);
	velX = std::max(-maxVelocity, velX);
	velX -= 0.2f * velX / (0.1f + fabs(velX));
	if (fabs(velX) < 0.01f) {
		velX = 0.0f;
	}
	m_pos_x += delta_time * velX;
}

void Enemy::update(float dt)
{
	movePlayer(dt);

	GameObject::update(dt);
}

void Enemy::init()
{
	Health = 0.0f;
	direction = 1.0f;
	m_pos_x = startX;
	m_pos_y = startY;
	m_width /= 3.0f;
	m_height /= 1.8f;

	m_brush_enemy.fill_opacity = 1.0f;
	m_brush_enemy.outline_opacity = 0.0f;
	m_brush_enemy.texture = m_state->getFullAssetPath("./Walking/left/left01.png");

	m_brush_enemy_debug.fill_opacity = 0.1f;
	SETCOLOR(m_brush_enemy_debug.fill_color, 0.0f, 0.0f, 1.0f);
	SETCOLOR(m_brush_enemy_debug.outline_color, 0.0f, 0.0f, 1.0f);

	fillVector(m_sprites_left, "assets/Walking/left/");
	fillVector(m_sprites_right, "assets/Walking/right/");

}

void Enemy::fillVector(std::vector<std::string>& vector, std::string asset_folder) {


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

void Enemy::draw()
{
	if (direction==-1) {
		int s = (int)fmodf(1000.0f - m_pos_x * 10.0f, m_sprites_left.size());
		m_brush_enemy.texture = m_sprites_left[s];
	}
	else{
		int s = (int)fmodf(1000.0f - m_pos_x * 10.0f, m_sprites_right.size());
		m_brush_enemy.texture = m_sprites_right[s];
	}

	graphics::drawRect(m_state->m_global_offset_x+m_pos_x, m_state->m_global_offset_y + m_pos_y, 0.5f, 0.5f, m_brush_enemy);
	if (m_state->m_debugging)
	{
		graphics::drawRect(m_state->m_global_offset_x + m_pos_x, m_state->m_global_offset_y + m_pos_y, m_width, m_height, m_brush_enemy_debug);
	}
}
