#include "TeamHasBallCondition.h"
#include "Player.h"
#include "RugbyScene.h"

bool TeamHasBallCondition::Test(Player* player)
{
	return player->GetScene<RugbyScene>()->IsBallInTeam(player->GetTag());
}
