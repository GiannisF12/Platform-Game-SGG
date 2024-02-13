#include "gameobject.h"

GameObject::GameObject(const std::string& name)
	: m_name(name), m_id(m_next_id++), m_state(GameState::getInstance())
{

}
int GameObject::m_next_id = 1;