#include "TeamDontHaveBallCondition.h"
#include "Player.h"
#include "RugbyScene.h"

bool TeamDontHaveBallConditon::Test(Player* player)
{
	if (player->GetScene<RugbyScene>()->GetBall()->GetOwner() == nullptr)
		return false;

	return !(player->GetScene<RugbyScene>()->IsBallInTeam(player->GetTag()));
}
