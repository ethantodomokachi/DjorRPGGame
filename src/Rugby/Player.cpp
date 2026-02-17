#include "Player.h"
#include "RugbyScene.h"
#include "Ball.h"
#include "IdlePlayerState.h"
#include "HasBallPlayerState.h"
#include "GivingPlayerState.h"
#include "ReceivingPlayerState.h"
#include "DefendingPlayerState.h"
#include "AttackingPlayerState.h"
#include "HasBallCondition.h"
#include "TeamHasBallCondition.h"
#include "TeamDontHaveBallCondition.h"
#include "Debug.h"
#include "Utils.h"
#include <iostream>

#define ENNEMYDETECTDISTANCE 700.f
#define PROJECTIONLINEOFFSET 50.f
#define PROJECTIONLINEHEIGHT 300.f
#define PROJECTIONLINESEGMENTLENGTH 240.f
#define AVOIDANCEPOINTDIST 200.f
#define AVOIDANCEINTENSITY 250.f

void Player::Pass(Player* _playerToPass)
{
	Ball* ball = GetScene<RugbyScene>()->GetBall();
	ball->SetOwner(nullptr);

	ball->GoToPosition(_playerToPass->GetPosition().x, _playerToPass->GetPosition().y);

	this->TryTransitionTo((int)State::Giving);
	_playerToPass->TryTransitionTo((int)State::Receiving);
}
	
std::vector<Player*> Player::FindPlayerToPass()
{
	Player** players = GetScene<RugbyScene>()->GetPlayers();
	Ball* ball = GetScene<RugbyScene>()->GetBall();

	std::vector<Player*> passPossible;

	float posBallOwnerX = GetPosition().x;
	float posBallOwnerY = GetPosition().y;

	for (int i = 0; i < PLAYERCOUNT; i++)
	{
		if (players[i] == this)
			continue;

		if (players[i]->IsTag(GetTag()) == false)
			continue;

		float posTeammateX = players[i]->GetPosition().x;
		float posTeammateY = players[i]->GetPosition().y;

		double distToTeammate = Utils::GetDistance(posBallOwnerX, posBallOwnerY, posTeammateX, posTeammateY);

		sf::Vector2f toTeammateVector(posTeammateX - posBallOwnerX, posTeammateY - posBallOwnerY);

		if (distToTeammate <= m_passMaxDist)
		{
			if (distToTeammate <= m_passMinDist)
				continue;

			bool isBlocked = false;

			for (int j = 0; j < PLAYERCOUNT; j++)
			{

				if (players[j]->IsTag(GetTag()) == false)
				{
					float posOpponentX = players[j]->GetPosition().x;
					float posOpponentY = players[j]->GetPosition().y;

					sf::Vector2f toOpponentVector(posOpponentX - posBallOwnerX, posOpponentY - posBallOwnerY);

					float scal = ((toTeammateVector.x * toOpponentVector.x) + (toTeammateVector.y * toOpponentVector.y));

					float ratio = scal / (distToTeammate * distToTeammate);

					if (ratio > 0 && ratio < 1)
					{
						float opponentProjectionX = posBallOwnerX + ratio * toTeammateVector.x;
						float opponentProjectionY = posBallOwnerY + ratio * toTeammateVector.y;

						double opponentToProjectionDist = Utils::GetDistance(opponentProjectionX, opponentProjectionY, posOpponentX, posOpponentY);

						if (opponentToProjectionDist <= GetRadius())
						{
							isBlocked = true;
							break;
						}
					}
				}
			}

			if (isBlocked == true)
			{
				Debug::DrawLine(posBallOwnerX, posBallOwnerY, posTeammateX, posTeammateY, sf::Color::Red);
			}
			else
			{
				passPossible.push_back(players[i]);
				Debug::DrawLine(posBallOwnerX, posBallOwnerY, posTeammateX, posTeammateY, sf::Color::Green);
			}
		}
	}
	return passPossible;
}

Player* Player::SelectBestToPass(std::vector<Player*> _choices)
{
	Player* bestChoice = _choices[0];
	int bestChoiceDangerCount = bestChoice->CountDangerPoints();

	for (int i = 1; i < _choices.size(); i++) 
	{
		
		int currentChoiceDangerCount = _choices[i]->CountDangerPoints();
		
		if (bestChoiceDangerCount > currentChoiceDangerCount) 
		{
			bestChoice = _choices[i];
			bestChoiceDangerCount = currentChoiceDangerCount;
		}
	}
	return bestChoice;
}

int Player::CountDangerPoints()
{
	Player** players = GetScene<RugbyScene>()->GetPlayers();
	int dangerCount = 0;

	for (int i = 0; i < PLAYERCOUNT; i++) 
	{
		if (players[i]->IsTag(GetTag())) 
			continue;

		sf::Vector2f position = GetPosition();

		float dx = players[i]->GetPosition().x - position.x;
		float dy = players[i]->GetPosition().y - position.y;

		float radius = m_dangerRadius;
		float radius2 = m_extremeDangerRadius;
		
		if ((dx * dx + dy * dy) < (radius2 * radius2)) {
			dangerCount += 3;
		}
		else if ((dx * dx + dy * dy) < (radius * radius)) {
			dangerCount++;
		}
	}

	return dangerCount;
	
}

bool Player::IsInvulnerable()
{	
	return m_invulTimer > 0;
}

void Player::ResetState()
{
	m_stateMachine.TryTransitionTo(this, (int)State::Idle);
}

void Player::MakeInvulnerable()
{
	m_invulTimer = m_invulDelay;
}

void Player::TryTransitionTo(int _nextState)
{
	m_stateMachine.TryTransitionTo(this, _nextState);
}

void Player::AvoidEnemies()
{
	Player** players = GetScene<RugbyScene>()->GetPlayers();

	int countUp = 0;
	int countMid = 0;
	int countDown = 0;

	float team = (IsTag((int)RugbyScene::Tag::RedPlayer)) ? -1.f : 1.f;

	float posBallOwnerX = GetPosition().x;
	float posBallOwnerY = GetPosition().y;
	
	for (int i = 0; i < PLAYERCOUNT; i++)
	{
		if (players[i]->IsTag(GetTag()) == true)
			continue;

		float posOpponentX = players[i]->GetPosition().x;
		float posOpponentY = players[i]->GetPosition().y;

		float distToOpponent = Utils::GetDistance(posBallOwnerX, posBallOwnerY, posOpponentX, posOpponentY);
	
		float projectionLineX = posBallOwnerX + (PROJECTIONLINEOFFSET * team);
			
		if (distToOpponent <= ENNEMYDETECTDISTANCE)
		{
			Debug::DrawLine(projectionLineX, posBallOwnerY - (PROJECTIONLINEHEIGHT * team), projectionLineX, posBallOwnerY + (PROJECTIONLINEHEIGHT * team), sf::Color::Magenta);
			Debug::DrawLine(posOpponentX, posOpponentY, projectionLineX, posOpponentY, sf::Color::Magenta);

			if (posOpponentY < posBallOwnerY && posOpponentY > posBallOwnerY - PROJECTIONLINESEGMENTLENGTH)
			{
				countUp++;
			}

			if (team > 0) {
				if (posOpponentX > posBallOwnerX && posOpponentX < posBallOwnerX + PROJECTIONLINESEGMENTLENGTH)
				{
					countMid++;
				}
			}
			else {
				if (posBallOwnerX > posOpponentX && posBallOwnerX < posOpponentX + PROJECTIONLINESEGMENTLENGTH)
				{
					countMid++;
				}
			}

			if (posOpponentY > posBallOwnerY && posOpponentY < posBallOwnerY + PROJECTIONLINESEGMENTLENGTH)
			{
				countDown++;
			}
		}
	}

	if (countMid > 0)
	{
		if (countUp <= countDown)
		{
			GoToPosition(posBallOwnerX + (AVOIDANCEPOINTDIST * team), posBallOwnerY - AVOIDANCEINTENSITY);
		}

		else
		{
			GoToPosition(posBallOwnerX + (AVOIDANCEPOINTDIST * team), posBallOwnerY + AVOIDANCEINTENSITY);
		}
	}
}

void Player::OnCollision(Entity* entity)
{
	if (entity->IsTag((int)RugbyScene::Tag::Ball))
	{
		if (GetScene<RugbyScene>()->GetBall()->GetOwner() == nullptr && this->m_stateMachine.GetState() != (int)State::Giving) {
			GetScene<RugbyScene>()->GetBall()->SetOwner(this);
			MakeInvulnerable();
		}
		return;
	}

	if (entity->IsTag(GetTag())) 
		return;

	Ball* ball = GetScene<RugbyScene>()->GetBall();

	if (ball->GetOwner() != this)
		return;

	if (this->IsInvulnerable()) {
		return;
	}

	ball->SetOwner(entity);
	Player* newOwner = (Player*)entity;
	newOwner->MakeInvulnerable();
}

void Player::OnInitialize()
{
	m_passMaxDist = 500.f;
	m_passMinDist = 60.f;
	m_dangerRadius = GetRadius() * 5;
	m_extremeDangerRadius = GetRadius() * 3;
	SetSpeed(100.f);
	SetRigidBody(true);

	// Idle
	{
		m_stateMachine.AddState(new IdlePlayerState());
		{
			//->HasBall
			Transition<Player>* t = m_stateMachine.AddTransition((int)State::Idle, (int)State::HasBall);
			t->AddCondition(new HasBallCondition());
		}

		{
			//->Attacking
			Transition<Player>* t = m_stateMachine.AddTransition((int)State::Idle, (int)State::Attacking);
			t->AddCondition(new TeamHasBallCondition());
		}

		{
			//->Defending
			Transition<Player>* t = m_stateMachine.AddTransition((int)State::Idle, (int)State::Defending);
			t->AddCondition(new TeamDontHaveBallConditon());
		}
	}

	// HasBall
	{
		m_stateMachine.AddState(new HasBallPlayerState());
		{
			//->Defending
			Transition<Player>* t = m_stateMachine.AddTransition((int)State::HasBall, (int)State::Defending);
			t->AddCondition(new TeamDontHaveBallConditon());
		}
	}

	// Giving
	{
		m_stateMachine.AddState(new GivingPlayerState);

		{
			//->Defending
			Transition<Player>* t = m_stateMachine.AddTransition((int)State::Giving, (int)State::Defending);
			t->AddCondition(new TeamDontHaveBallConditon());
		}

		{
			//->Attacking
			Transition<Player>* t = m_stateMachine.AddTransition((int)State::Giving, (int)State::Attacking);
			t->AddCondition(new TeamHasBallCondition());
		}
	}

	// Receiving
	{
		m_stateMachine.AddState(new ReceivingPlayerState);

		{
			//->Defending
			Transition<Player>* t = m_stateMachine.AddTransition((int)State::Receiving, (int)State::Defending);
			t->AddCondition(new TeamDontHaveBallConditon());
		}

		{
			//->HasBall
			Transition<Player>* t = m_stateMachine.AddTransition((int)State::Receiving, (int)State::HasBall);
			t->AddCondition(new HasBallCondition());
		}

		{
			//->Attacking
			Transition<Player>* t = m_stateMachine.AddTransition((int)State::Receiving, (int)State::Attacking);
			t->AddCondition(new TeamHasBallCondition());
		}
	}

	// Defending
	{
		m_stateMachine.AddState(new DefendingPlayerState);
		
		{
			//->HasBall
			Transition<Player>* t = m_stateMachine.AddTransition((int)State::Defending, (int)State::HasBall);
			t->AddCondition(new HasBallCondition());
		}

		{
			//->Attacking
			Transition<Player>* t = m_stateMachine.AddTransition((int)State::Defending, (int)State::Attacking);
			t->AddCondition(new TeamHasBallCondition());
		}
	}

	// Attacking
	{
		m_stateMachine.AddState(new AttackingPlayerState);

		{
			//->Defending
			Transition<Player>* t = m_stateMachine.AddTransition((int)State::Attacking, (int)State::Defending);
			t->AddCondition(new TeamDontHaveBallConditon());
		}

		{
			//->HasBall
			Transition<Player>* t = m_stateMachine.AddTransition((int)State::Attacking, (int)State::HasBall);
			t->AddCondition(new HasBallCondition());
		}
	}
}

void Player::OnUpdate()
{
	// debug
	Debug::DrawText(GetPosition().x, GetPosition().y, "Danger : " + std::to_string(CountDangerPoints()), sf::Color::White);
	Debug::DrawText(GetPosition().x, GetPosition().y - 20, StateToStr(), sf::Color::White);
	
	if (GetScene<RugbyScene>()->m_paused) {
		Debug::DrawCircle(GetPosition().x, GetPosition().y, m_dangerRadius, sf::Color::Cyan);
		Debug::DrawCircle(GetPosition().x, GetPosition().y, m_extremeDangerRadius, sf::Color::Yellow);
	}

	if (IsInvulnerable())
		Debug::DrawFilledCircle(GetPosition().x, GetPosition().y, GetRadius(), sf::Color::Blue);

	m_stateMachine.Update(this, GetDeltaTime());
}

const char* Player::StateToStr() const
{
	switch ((State)m_stateMachine.GetState())
	{
	case State::Idle :
		return "Idle";
	case State::HasBall:
		return "HasBall";
	case State::Receiving:
		return "Receiving";
	case State::Giving:
		return "Giving";
	case State::Attacking:
		return "Attacking";
	case State::Defending:
		return "Defending";
	}
}
