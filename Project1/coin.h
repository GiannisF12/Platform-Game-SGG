#pragma once

#include "gameobject.h"
#include "box.h"
#include <sgg/graphics.h>

class Coin : public GameObject, public Box
{
	graphics::Brush coinBrush;
	graphics::Brush coinBrushDebug;

	std::vector<std::string> m_sprites;

	void fillVector(std::vector<std::string>& vector, std::string asset_folder);


public:
	float velX = 0.0f;
	float velY = 0.0f;

	void update(float dt) override;
	void init() override;
	void draw() override;
	Coin(std::string name) : GameObject(name) {};
};