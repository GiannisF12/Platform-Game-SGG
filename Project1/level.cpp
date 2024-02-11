#include "level.h"
#include "gamestate.h"
#include "player.h"
#include "enemy.h"
#include "util.h"
#include <string>
#include <random>

void Level::drawBlock(int i, const std::string& str)
{
	if (str.compare("blocks")==0) {
		Box& box = m_blocks[i];
		float x = 0.0f;
		float y = 0.0f;
		box = m_blocks[i];
		x = box.m_pos_x + m_state->m_global_offset_x;
		y = box.m_pos_y + m_state->m_global_offset_y;
		m_block_brush.texture = m_state->getFullAssetPath(m_block_names[i]);
		graphics::drawRect(x, y, 1 * m_block_size, 1 * m_block_size, m_block_brush);

		if (m_state->m_debugging)
		{
			graphics::drawRect(x, y, m_block_size, m_block_size, m_block_brush_debug);
		}
	}
	else if (str.compare("coins") == 0) {
		Box& box = coins[i];
		float x = 0.0f;
		float y = 0.0f;
		box = coins[i];
		x = box.m_pos_x + m_state->m_global_offset_x;
		y = box.m_pos_y + m_state->m_global_offset_y;
		m_block_brush.texture = m_state->getFullAssetPath(coinsNames[i]);
		graphics::drawRect(x, y, box.m_width, box.m_height, m_block_brush);

		if (m_state->m_debugging)
		{
			graphics::drawRect(x, y, box.m_width, box.m_height, m_block_brush_debug);
		}
	}
	else if (str.compare("health") == 0) {
		Box& box = health[i];
		float x = 0.0f;
		float y = 0.0f;
		box = health[i];
		x = box.m_pos_x + m_state->m_global_offset_x;
		y = box.m_pos_y + m_state->m_global_offset_y;
		m_block_brush.texture = m_state->getFullAssetPath(healthNames[i]);
		graphics::drawRect(x, y, box.m_width, box.m_height, m_block_brush);

		if (m_state->m_debugging)
		{
			graphics::drawRect(x, y, box.m_width, box.m_height, m_block_brush_debug);
		}
	}

	else if (str.compare("enemy") == 0) {
		Enemy& enemy = enemys[i];
		enemy.draw();
	}
	else if (str.compare("finish") == 0) {
		float x = 0.0f;
		float y = 0.0f;
		x = finish.m_pos_x + m_state->m_global_offset_x;
		y = finish.m_pos_y + m_state->m_global_offset_y;
		m_block_brush.texture = m_state->getFullAssetPath(finishName);
		graphics::drawRect(x, y, finish.m_width, finish.m_height, m_block_brush);
	}
}

void Level::checkCollisions()
{
	for (auto& box : m_blocks)
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectDownPlayer(box)) {
			m_state->getPlayer()->m_pos_y += offset;
			if (m_state->getPlayer()->velY > 1.0f) {
				graphics::playSound(m_state->getFullAssetPath("metal2.wav"), 0.5f);
			}
			m_state->getPlayer()->velY = 0.0f;
			break;
		}
	}


	for (auto& box : m_blocks)
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectUp(box)) {
			m_state->getPlayer()->m_pos_y = box.m_pos_y + (box.m_height / 2.0f) + (m_state->getPlayer()->m_height / 2.0f);
			m_state->getPlayer()->velY = 0.0f;
		}
	}


	for (auto& box : m_blocks)
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectSideways(box)) {
			m_state->getPlayer()->m_pos_x += offset;
			m_state->getPlayer()->velX = 0.0f;
			break;
		}

	}

	for (int i = 0; i < coins.size(); i++) {

		if (m_state->getPlayer()->intersect(coins[i])) {
			coins.erase(coins.begin() + i);
			m_state->getPlayer()->Score += 10;
			coinsCollected++;
			graphics::playSound(m_state->getFullAssetPath("coin.wav"), 0.5f);
			break;
		}
	}

	for (int i = 0; i < health.size(); i++) {

		if (m_state->getPlayer()->intersect(health[i])) {
			health.erase(health.begin() + i);
			graphics::playSound(m_state->getFullAssetPath("health.wav"), 0.5f);
			if (m_state->getPlayer()->Health <= 80) {
				m_state->getPlayer()->Health += 20;
			}
			else {
				m_state->getPlayer()->Health = 100;
			}
			break;
		}
	}
	
	for (auto& box : m_blocks)
	{
		for (auto& enemy : enemys){
			float offset = 0.0f;
			if (offset= enemy.intersectSideways(box)) {
				enemy.direction *= -1.0f;
				enemy.m_pos_x += offset;
				enemy.velX = 0.0f;
			}
		}
	}

	for (auto& enemy : enemys)
	{
		float offset = 0.0f;
		if (m_state->getPlayer()->intersect(enemy)) {
			m_state->getPlayer()->Health -= 3;
			graphics::playSound(m_state->getFullAssetPath("damage.mp3"), 0.5f);
			break;
		}
	}

	if (m_state->getPlayer()->intersect(finish)) {
		if (coinsCollected == MaxCoins) {
			Finish = 1;
		}
	}
}

void Level::update(float dt)
{
	if (m_state->getPlayer()->isActive()) {
		m_state->getPlayer()->update(dt);
	}
	EnemiesUpdate(dt);

	checkCollisions();
	GameObject::update(dt);
}

void Level::init()
{
	m_brush_background.outline_opacity = 0.0f;
	m_brush_background.texture = m_state->getFullAssetPath("background.png");

	for (auto p_gob : m_static_objects) {
		if (p_gob) p_gob->init();
	}
	for (auto p_gob : m_dynamic_objects) {
		if (p_gob) p_gob->init();
	}

	//Level Generation

	srand(time(NULL));
	int maxWidth = 40;
	int height = rand() % (40 - 25 + 1) + 25;
	int width = rand() % (maxWidth - 25 + 1) + 25;
	int finalHole = 3;
	for (int i = 0; i < height; i++) {
		if (i == 0) {
			for (int j = 0; j < width; j++) {
				if (j!=finalHole) {
					m_blocks.push_back(Box(j * m_block_size, i * m_block_size, m_block_size, m_block_size));
					m_block_names.push_back("block.png");
				}
			}
		}
		else {
			int holeRandomizer = rand() % (width-width/4 - 0 + 1) + 0;
			int nextFinalHole = rand() % ((finalHole+ holeRandomizer) - (finalHole - holeRandomizer) + 1) + (finalHole - holeRandomizer);
			if (nextFinalHole > width - 2) {
				nextFinalHole = width - 2;
			}
			if (nextFinalHole < 2) {
				nextFinalHole = 2;
			}
			int max = -1;
			int min = maxWidth + 1;
			if (finalHole >= nextFinalHole) {
				max = finalHole;
				min = nextFinalHole;
			}
			else {
				min = finalHole;
				max = nextFinalHole;
			}
			printf("from MaxWidth:%d min: %d and max: %d\n", width, min, max);
			for (int j = 0; j < width; j++) {
				if (j == 0 || j == (width - 1)) {
					m_blocks.push_back(Box(j * m_block_size, i * m_block_size, m_block_size, m_block_size));
					m_block_names.push_back("block.png");
				}
				else if (j>max || j<min) {
					int spawnRatio = rand() % (1 - 0 + 1);
					if (spawnRatio == 1) {
						m_blocks.push_back(Box(j * m_block_size, i * m_block_size, m_block_size, m_block_size));
						m_block_names.push_back("block.png");
					}
					else {
						int spawnRatioEnemy = rand() % (60 - 0 + 1);
						if (spawnRatioEnemy == 51) {
							printf("SpawnedEnemy");
							enemys.push_back(Enemy("enemy", j, i));
						}
						int heartRatio = rand() % (50 - 0 + 1);
						if (heartRatio == 9) {
							health.push_back(Box(j * m_block_size, i * m_block_size, m_block_size / 2, m_block_size / 2));
							healthNames.push_back("heart.png");
						}
					}
				}
				else {
					if(i==(height-1) && j==nextFinalHole){
						finish = Box(j * m_block_size, i * m_block_size, m_block_size, m_block_size);
						finishName ="finish.png";
						for (int k = 0; k < width; k++) {
							m_blocks.push_back(Box(k * m_block_size, i+1 * m_block_size, m_block_size, m_block_size));
							m_block_names.push_back("block.png");
						}
					}
					int spawnRatio = rand() % (40 - 0 + 1);
					if (spawnRatio == 21 && (i != (height - 1) || j != nextFinalHole)) {
						
						enemys.push_back(Enemy("enemy", j, i));
					}
					int coinRatio = rand() % (2 - 0 + 1);
					if (coinRatio == 1 && (i != (height - 1) || j != nextFinalHole)) {
						coins.push_back(Box(j * m_block_size, i * m_block_size, m_block_size / 2, m_block_size / 2));
						coinsNames.push_back("coin/coin.png");
						MaxCoins++;
					}
				}
			}
			finalHole = nextFinalHole;
		}
	}
	for (int i = 1; i < 6; i++) {
		if (i == 6 - 1) {
			for (int k = 0; k < width; k++) {
				m_blocks.push_back(Box(k * m_block_size, -i + 1 * m_block_size, m_block_size, m_block_size));
				m_block_names.push_back("block.png");
			}
		}else{
			m_blocks.push_back(Box(0 * m_block_size, -i * m_block_size, m_block_size, m_block_size));
			m_block_names.push_back("block.png");
			m_blocks.push_back(Box(width - 1 * m_block_size, -i * m_block_size, m_block_size, m_block_size));
			m_block_names.push_back("block.png");
		}
	}
	//Finish Level Generation

	//Debug Level Brush

	m_block_brush.outline_opacity = 0.0f;
	m_block_brush_debug.fill_opacity = 0.1f;
	SETCOLOR(m_block_brush_debug.fill_color, 0.2f, 1.0f, 0.1f);
	SETCOLOR(m_block_brush_debug.outline_color, 0.3f, 1.0f, 0.2f);

	initEnemies();
}

void Level::EnemiesUpdate(float dt) {
	for (Enemy& enemy : enemys) {
		enemy.update(dt);
	}
}

void Level::initEnemies() {
	for (Enemy& enemy : enemys) {
		enemy.init();
	}
}

void Level::draw()
{
	float w = m_state->getCanvasWidth();
	float h = m_state->getCanvasHeight();

	float offset_x = m_state->m_global_offset_x + w / 2.0f;
	float offset_y = m_state->m_global_offset_y + h / 2.0f;

	//draw background
	graphics::drawRect(m_state->getCanvasWidth()/2, m_state->getCanvasHeight()/2, w,h, m_brush_background);

	if (m_state->getPlayer()->isActive()) {
		m_state->getPlayer()->draw();
	}


	for (int i = 0; i < m_blocks.size(); i++) {
		
		drawBlock(i, "blocks");
	}

	for (int i = 0; i < coins.size(); i++) {

		drawBlock(i,"coins");
	}

	for (int i = 0; i < health.size(); i++) {

		drawBlock(i, "health");
	}

	for (int i = 0; i < enemys.size(); i++) {

		drawBlock(i, "enemy");
	}

	drawBlock(0, "finish");

	graphics::Brush br;
	char coinsUi[40];
	sprintf_s(coinsUi, "Coins Collected : %d/%d", coinsCollected,MaxCoins);
	graphics::drawText(0.1f, 0.2f, 0.15f, coinsUi, br);
}

Level::Level(const std::string& name)
{
}

Level::~Level()
{
	for (auto p_gob : m_static_objects) {
		if (p_gob) delete p_gob;
	}
	for (auto p_gob : m_dynamic_objects) {
		if (p_gob) delete p_gob;
	}
}
