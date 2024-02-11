#pragma once

#include "gameobject.h"
#include "box.h"
#include <sgg/graphics.h>

class Player : public GameObject, public Box
{
	graphics::Brush m_brush_player;
	graphics::Brush m_brush_player_debug;

	std::vector<std::string> m_sprites_left;
	std::vector<std::string> m_sprites_right;
	
	const float gravity = 10.0f;
	const float vertAccelaration = 2.5f;
	const float horAccelaration = 40.0f;
	const float maxVelocity = 5.0f;

	void movePlayer(float dt);
	void fillVector(std::vector<std::string>& vector, std::string asset_folder);
	
public:
	float velX = 0.0f;
	float velY = 0.0f;
	float Score = 0.0f;
	float Health = 100.0f;

	void update(float dt) override;
	void init() override;
	void draw() override;
	Player(std::string name) : GameObject(name) {}
};