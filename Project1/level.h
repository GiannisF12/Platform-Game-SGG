#pragma once
#include "gameobject.h"
#include "sgg/graphics.h"
#include <list>
#include "box.h"
#include "enemy.h"

class Level : public GameObject
{
	graphics::Brush m_brush_background;

	std::vector<GameObject*> m_static_objects;
	std::list<GameObject*> m_dynamic_objects;

	std::vector<Box> m_blocks;
	std::vector<Box> coins;
	Box finish;
	std::string finishName;
	std::vector<Box> health;
	std::vector<Enemy> enemys;
	std::vector<std::string> healthNames;
	std::vector<std::string> coinsNames;
	std::vector<std::string> m_block_names;

	int MaxCoins=0;
	int coinsCollected=0;

	const float m_block_size = 1.0f;
	graphics::Brush m_block_brush;
	graphics::Brush m_block_brush_debug; //diaforetika oria sxediashs kai collision

	float m_center_x = 5.0f;
	float m_center_y = 5.0f;

	float initEnemiesStatus = 0;

	void drawBlock(int i, const std::string& str);

	void checkCollisions();
	void EnemiesUpdate(float dt);
	void initEnemies();


public:
	int Finish = 0;
	void update(float dt) override;
	void init() override;
	void draw() override;

	Level(const std::string& name = "Level0");
	~Level() override;									//Destructor
};