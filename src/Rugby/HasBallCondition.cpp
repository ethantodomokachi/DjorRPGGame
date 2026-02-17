#include "HasBallCondition.h"
#include "Player.h"
#include "RugbyScene.h"

bool HasBallCondition::Test(Player* player)
{
	return player == player->GetScene<RugbyScene>()->GetBall()->GetOwner();
}
