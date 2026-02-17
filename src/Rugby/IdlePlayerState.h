#pragma once
#include "StateMachine.h"

class Player;

class IdlePlayerState : public StateBase<Player>
{
public:
	void Start(Player* type) override {
		type->m_invulTimer = 0;
	}
	void Update(Player* type, float dt) override {}
	void End(Player* type) override {}
};
