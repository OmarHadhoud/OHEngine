#ifndef SYSTEM_H
#define SYSTEM_H

#include "game/ecs/Events/EventsController.h"
#include "game/ecs/ECSManager.h"

class System
{
public:
	System();
	virtual ~System() = 0;
	void SetEventsController(EventsController* eventsController);
	void SetECSManager(ECSManager* manager);
	void SetCurrentWindow(GLFWwindow *currentWindow, int width, int height);
	virtual void Update() = 0;
protected:
	EventsController* m_EventsController;
	ECSManager* m_ECSManager;
	int m_EventIndex;
	//Window variables
	GLFWwindow *m_CurrentWindow;
	int m_WindowWidth;
	int m_WindowHeight;
	void ProcessEvents();
	virtual void ProcessEvent(Event*) = 0;
};
#endif // !SYSTEM_H

