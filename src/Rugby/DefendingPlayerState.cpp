#include "DefendingPlayerState.h"
#include "Player.h"
#include "RugbyScene.h"

void DefendingPlayerState::Update(Player* type, float dt)
{
	//debug
	if (type->GetScene<RugbyScene>()->m_paused == true)
		return;

	Entity* target = type->GetScene<RugbyScene>()->GetBall()->GetOwner();

	if (target == nullptr) 
	{
		target = type->GetScene<RugbyScene>()->GetBall();
	}

	type->GoToPosition(target->GetPosition().x, target->GetPosition().y);
}
