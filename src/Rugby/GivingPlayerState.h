#pragma once
#include "StateMachine.h"

class Player;

class GivingPlayerState : public StateBase<Player>
{
public:
	void Start(Player* type) override;
	void Update(Player* type, float dt) override {};
	void End(Player* type) override;
};
