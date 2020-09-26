#ifndef EVENTS_CONTROLLER_H
#define EVENTS_CONTROLLER_H

#include <vector>
#include "Event.h"

const int MAX_EVENTS_COUNT = 100;

class EventsController
{
public:
	EventsController();
	~EventsController();
	int GetLastEventIndex() const;
	void ClearEvents();
	void AddEvent(Event* eventToBeAdded);
	const std::vector<Event*> &GetEvents() const;
private:
	std::vector<Event*> m_EventsBuffer;
	int m_EventsCounter;
};
#endif // !EVENTS_CONTROLLER_H

