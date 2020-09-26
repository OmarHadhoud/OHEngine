#ifndef GUI_SYSTEM_H
#define GUI_SYSTEM_H

#include <vector>
#include <GLFW/glfw3.h>

#include "game/ecs/Systems/System.h"
#include "game/ecs/Events/Event.h"

class GUISystem : public System
{
public:
	GUISystem();
	~GUISystem();
	void Update();
private:
	void ProcessEvent(Event*);
};

#endif // !GUI_SYSTEM_H