#ifndef SYSTEM_H
#define SYSTEM_H

#include "game/ecs/Events/EventsController.h"

class System
{
public:
	System();
	virtual ~System() = 0;
	void SetEventsController(EventsController* eventsController);
	virtual void Update() = 0;
protected:
	EventsController* m_EventsController;
	int m_EventIndex;
	void ProcessEvents();
	virtual void ProcessEvent(Event*) = 0;
};
#endif // !SYSTEM_H

