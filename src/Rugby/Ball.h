#pragma once
#include "Entity.h"

class Player;

class Ball : public Entity
{
private:

	Entity* m_owner = nullptr;

public:

	Entity* GetOwner();
	void SetOwner(Entity* _owner);

protected:

	void OnInitialize() override;
	void OnUpdate() override;	
};
