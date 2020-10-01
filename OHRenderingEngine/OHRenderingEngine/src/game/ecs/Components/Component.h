#ifndef COMPONENT_H
#define COMPONENT_H

#include "game/ecs/ECSCount.h"

struct Component
{
	bool m_Enabled;
	int m_EntityID;
};

#endif // !COMPONENT_H