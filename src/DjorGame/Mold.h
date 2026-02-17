#pragma once

#include "Entity.h"

class Material;

class Mold : public Entity
{
	int m_Level;
	std::vector<Material*> m_materialsUsed;
};
