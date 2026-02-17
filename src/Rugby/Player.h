#pragma once
#include "Entity.h"
#include "Ball.h"
#include "StateMachine.h"

class Player : public Entity
{
	enum class State {
		Idle,
		HasBall,
		Giving,
		Receiving,
		Defending,
		Attacking,

		Count
	};

	float m_dangerRadius;
	float m_extremeDangerRadius;
	
	float m_passMaxDist;
	float m_passMinDist;

	float m_invulDelay = 1.5f;
	float m_invulTimer;

	StateMachine<Player> m_stateMachine;

public :
	void Pass(Player* _playerToPass);
	std::vector<Player*> FindPlayerToPass();
	Player* SelectBestToPass(std::vector<Player*> _choices);
	int CountDangerPoints();
	bool IsInvulnerable();
	void ResetState();
	void MakeInvulnerable();
	void TryTransitionTo(int _nextState);
	void AvoidEnemies();

protected:
	void OnInitialize() override;
	void OnUpdate() override; 
	void OnCollision(Entity* entity) override;

private:
	const char* StateToStr() const;

	friend class IdlePlayerState;
	friend class HasBallPlayerState;
	friend class GivingPlayerState;
	friend class ReceivingPlayerState;
	friend class DefendingPlayerState;
	friend class AttackingPlayerState;

	friend class HasBallCondition;
	friend class TeamHasBallCondition;
	friend class TeamDontHaveBallCondition;
};
