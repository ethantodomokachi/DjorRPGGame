#include "GivingPlayerState.h"
#include "Player.h"

void GivingPlayerState::Start(Player* type)
{
	type->SetSpeed(0.f);
}

void GivingPlayerState::End(Player* type)
{
	type->SetSpeed(100.f);
}
