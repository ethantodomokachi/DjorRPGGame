#include "RugbyScene.h"
#include "Ball.h"
#include "Debug.h"
#include "Player.h"
#include <iostream>

#define SPEED 100.f
#define PLAYER_RADIUS 30.f

Ball* RugbyScene::GetBall()
{
	return m_ball;
}

bool RugbyScene::IsBallInTeam(int tag)
{
	if (m_ball->GetOwner() == nullptr)
		return false;

	return tag == m_ball->GetOwner()->GetTag();
}

Player** RugbyScene::GetPlayers()
{
	return m_players;
}

void RugbyScene::OnInitialize()
{

	m_width = GetWindowWidth();
	m_height = GetWindowHeight();
	m_laneHeight = m_height / 2;;
	m_rightGoal = m_width - GOALPLACEMENT;
	m_leftGoal = GOALPLACEMENT;

	sf::Vector2f playerPlacement[PLAYERCOUNT] =
	{
		{ PLAYER1XY, PLAYER1XY},
		{ m_rightGoal + PLAYER1XY, PLAYER1XY},
		{ PLAYER2X * 2, PLAYER2Y},
		{ m_rightGoal - PLAYER2X, PLAYER2Y},
		{ (m_width / 2) - PLAYER3X, m_height / 2},
		{ (m_width / 2) + PLAYER3X, m_height / 2},
		{ PLAYER4X, PLAYER4Y},
		{ m_width - PLAYER4X, PLAYER4Y},
		{ PLAYER5X, PLAYER5Y},
		{ m_rightGoal + PLAYER5X, PLAYER5Y},
	};

	for (int i = 0; i < PLAYERCOUNT; i++) {

		if (i % 2 == 0) {
	
			m_players[i] = CreateEntity<Player>(PLAYER_RADIUS, sf::Color::Green);
			m_players[i]->SetTag((int)RugbyScene::Tag::GreenPlayer);
			m_players[i]->SetPosition(playerPlacement[i].x, playerPlacement[i].y);
		}

		else {
			m_players[i] = CreateEntity<Player>(PLAYER_RADIUS, sf::Color::Red);
			m_players[i]->SetTag((int)RugbyScene::Tag::RedPlayer);
			m_players[i]->SetPosition(playerPlacement[i].x, playerPlacement[i].y);

		}

		m_players[i]->SetSpeed(SPEED);
	}

	m_ball = CreateEntity<Ball>(15, sf::Color::Yellow);
	m_ball->SetPosition(m_width / 2, m_height / 2);
	m_ball->SetOwner(m_players[4]);
}

void RugbyScene::OnEvent(const sf::Event& event)
{
	if (event.mouseButton.button == sf::Mouse::Button::Right)
	{
		for (int i = 0; i < PLAYERCOUNT; i++)
		{
			TrySetSelectedEntity(m_players[i], event.mouseButton.x, event.mouseButton.y);
		}
	}

	if (event.mouseButton.button == sf::Mouse::Button::Left)
	{
		if (pEntitySelected != nullptr)
			pEntitySelected->GoToPosition(event.mouseButton.x, event.mouseButton.y, 100.f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if (m_pauseTimer <= 0) {
			if (m_paused == false) {
				m_paused = true;

				m_pauseTimer = 0.5f;

				for (int i = 0; i < PLAYERCOUNT; i++)
				{
					m_players[i]->SetSpeed(0);
				}
			}
			else {
 				m_paused = false;
				m_pauseTimer = 0.5f;

				for (int i = 0; i < PLAYERCOUNT; i++)
				{
					m_players[i]->SetSpeed(SPEED);
				}
			}
		}		
	}
}

void RugbyScene::OnUpdate()
{
	Debug::DrawRectangle(0.f, 0.f, m_width, m_laneHeight, sf::Color::Green);
	Debug::DrawRectangle(0.f, m_laneHeight, m_width, m_laneHeight, sf::Color::Green);
	Debug::DrawRectangle(0.f, m_laneHeight / 2, m_width, m_laneHeight, sf::Color::Yellow);

	Debug::DrawLine(m_leftGoal, 0.f, m_leftGoal, m_height, sf::Color::White);
	Debug::DrawLine(m_rightGoal, 0.f, m_rightGoal, m_height, sf::Color::White);

	Debug::DrawText(700, 0, "Red: " + std::to_string(m_pointRed), sf::Color::White);
	Debug::DrawText(500, 0, "Green: " + std::to_string(m_pointGreen), sf::Color::White);

	CountPoint();

	StayInsideWindow();

	if(m_pauseTimer > 0)
		m_pauseTimer -= GetDeltaTime();
}

void RugbyScene::TrySetSelectedEntity(Entity* Entity, int x, int y)
{
	if (Entity->IsInside(x, y) == false)
		return;

	pEntitySelected = Entity;
}

void RugbyScene::CountPoint()
{
	sf::Vector2f pos = m_ball->GetPosition();

	if (pos.x < m_leftGoal)
	{
		m_pointRed++;
		ResetScene();
	}

	else if (pos.x > m_rightGoal)
	{
		m_pointGreen++;
		ResetScene();
	}
}

void RugbyScene::ResetScene()
{
	sf::Vector2f playerPlacement[PLAYERCOUNT] =
	{
		{ PLAYER1XY, PLAYER1XY},
		{ m_rightGoal + PLAYER1XY, PLAYER1XY},
		{ PLAYER2X * 2, PLAYER2Y},
		{ m_rightGoal - PLAYER2X, PLAYER2Y},
		{ (m_width / 2) - PLAYER3X, m_height / 2},
		{ (m_width / 2) + PLAYER3X, m_height / 2},
		{ PLAYER4X, PLAYER4Y},
		{ m_width - PLAYER4X, PLAYER4Y},
		{ PLAYER5X, PLAYER5Y},
		{ m_rightGoal + PLAYER5X, PLAYER5Y},
	};

	for (int i = 0; i < PLAYERCOUNT; i++) {
		m_players[i]->SetPosition(playerPlacement[i].x, playerPlacement[i].y);
		m_players[i]->ResetState();
	}

	m_ball->SetOwner(m_players[4]);
}

void RugbyScene::StayInsideWindow()
{
	for (int i = 0; i < PLAYERCOUNT; i++)
	{
		sf::Vector2f pos = m_players[i]->GetPosition();

		if (pos.y <= PLAYER_RADIUS)
		{
			m_players[i]->SetPosition(pos.x, PLAYER_RADIUS);
		}

		if (pos.y >= m_height - PLAYER_RADIUS)
		{
			m_players[i]->SetPosition(pos.x, m_height - PLAYER_RADIUS);
		}
	}
}
