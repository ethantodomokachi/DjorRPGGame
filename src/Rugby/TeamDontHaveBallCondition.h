#pragma once
#include "StateMachine.h"

class Player;

class TeamDontHaveBallConditon : public Condition<Player>
{
public:
	bool Test(Player* player) override;
};
