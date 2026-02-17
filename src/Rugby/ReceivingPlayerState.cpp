#include "ReceivingPlayerState.h"
#include "Player.h"

void ReceivingPlayerState::Start(Player* type)
{
	type->SetSpeed(0.f);
}

void ReceivingPlayerState::End(Player* type)
{
	type->SetSpeed(100.f);
}
