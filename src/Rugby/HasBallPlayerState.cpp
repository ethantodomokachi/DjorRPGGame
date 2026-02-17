#include "HasBallPlayerState.h"
#include "Player.h"
#include "RugbyScene.h"

#define GOALX 95.f

void HasBallPlayerState::Start(Player* type)
{
	type->SetSpeed(150.f);
}

void HasBallPlayerState::Update(Player* type, float dt)
{
	//debug
	if (type->GetScene<RugbyScene>()->m_paused == true)
		return;

	if (type->m_invulTimer > 0)
		type->m_invulTimer -= dt;

	if (type->GetTag() == (int)RugbyScene::Tag::GreenPlayer) {
		type->GoToDirection(type->GetScene<RugbyScene>()->GetWindowWidth() - GOALX, type->GetPosition().y);
	}
	else {
		type->GoToDirection(GOALX, type->GetPosition().y);
	}

	type->AvoidEnemies();

	if (type->IsInvulnerable() == false) {
		type->SetSpeed(100.f);

		if (type->CountDangerPoints() >= 3) {
			std::vector<Player*> passPossible = type->FindPlayerToPass();

			if (passPossible.size() == 0)
				return;

			Player* toPass = type->SelectBestToPass(passPossible);
			type->Pass(toPass);
		}
	}
}
