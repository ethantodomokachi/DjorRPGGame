#include "Ball.h"
#include "RugbyScene.h"

Entity* Ball::GetOwner()
{
	return m_owner;
}

void Ball::SetOwner(Entity* _owner)
{
	m_owner = _owner;
	if (m_owner == nullptr)
		return;

	sf::Vector2f pos = m_owner->GetPosition();
	GoToPosition(pos.x, pos.y);
}

void Ball::OnInitialize()
{
	SetTag((int)RugbyScene::Tag::Ball);
	SetSpeed(300.f);
}

void Ball::OnUpdate()
{
	if (m_owner != nullptr)
	{
		sf::Vector2f pos = m_owner->GetPosition();
		SetPosition(pos.x, pos.y);
	}
}
