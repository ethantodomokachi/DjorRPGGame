#pragma once
#include "StateMachine.h"

class Player;

class TeamHasBallCondition : public Condition<Player>
{
public:
	bool Test(Player* player) override;
};
