#pragma once

#include "gameobject.h"
#include "box.h"
#include <sgg/graphics.h>

class Enemy : public GameObject, public Box
{
	graphics::Brush m_brush_enemy;
	graphics::Brush m_brush_enemy_debug;

	std::vector<std::string> m_sprites_left;
	std::vector<std::string> m_sprites_right;

	const float gravity = 10.0f;
	const float horAccelaration = 25.0f;
	const float maxVelocity = 6.0f;
	float startX = 0.0f;
	float startY = 0.0f;


	void movePlayer(float dt);
	void fillVector(std::vector<std::string>& vector, std::string asset_folder);

public:
	float velX = 0.0f;
	float velY = 0.0f;
	float Health = 100.0f;
	float direction = 1.0f;

	void update(float dt) override;
	void init() override;
	void draw() override;
	Enemy(std::string name,float posX,float posY) : GameObject(name), startX(posX), startY(posY) {}

};