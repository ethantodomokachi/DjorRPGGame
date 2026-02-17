#pragma once
#include "StateMachine.h"

class Player;

class HasBallCondition : public Condition<Player>
{
public:
	bool Test(Player* player) override;
};

