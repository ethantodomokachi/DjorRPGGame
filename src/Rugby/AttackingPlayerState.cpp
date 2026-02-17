#include "AttackingPlayerState.h"
#include "Player.h"
#include "RugbyScene.h"

void AttackingPlayerState::Update(Player* type, float dt)
{
	//debug
	if (type->GetScene<RugbyScene>()->m_paused == true)
		return;
	Entity* ballOwner = type->GetScene<RugbyScene>()->GetBall()->GetOwner();
	if (ballOwner == nullptr)
		return;

	type->GoToDirection(ballOwner->GetPosition().x - 50.f, type->GetPosition().y);
}
