#pragma once
#include "Scene.h"
#include "Ball.h"
#include <vector>

#define PLAYERCOUNT 10
#define GOALPLACEMENT 100.f

#define PLAYER1XY 50.f 

#define PLAYER2X 100.f
#define PLAYER2Y 150.f

#define PLAYER3X 350.f

#define PLAYER4X 200.f
#define PLAYER4Y 570.f

#define PLAYER5X 50.f
#define PLAYER5Y 670.f


class Ball;
class Player;

class RugbyScene : public Scene
{
public :
	enum class Tag {
		GreenPlayer,
		RedPlayer,
		Ball,

		Count
	};

	bool m_paused = false;

	bool IsBallInTeam(int tag);
	Ball* GetBall();
	Player** GetPlayers();

private:

	Ball* m_ball;
	Player* m_players[PLAYERCOUNT];

	float m_width;
	float m_height;
	float m_laneHeight;
	float m_rightGoal;
	float m_leftGoal;
	
	int m_pointRed = 0;
	int m_pointGreen = 0;

	Entity* pEntitySelected;

	float m_pauseTimer = 0;

protected : 
	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;
	void TrySetSelectedEntity(Entity* Entity, int x, int y);
	void CountPoint();
	void ResetScene();
	void StayInsideWindow();
	
};
