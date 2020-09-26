#include "EventsController.h"



EventsController::EventsController(): m_EventsBuffer(std::vector<Event*>(MAX_EVENTS_COUNT,nullptr)), m_EventsCounter(0)
{
}


EventsController::~EventsController()
{
	for (std::vector<Event*>::iterator i = m_EventsBuffer.begin(); i != m_EventsBuffer.end(); i++)
	{
		delete *i;
	}
}

int EventsController::GetLastEventIndex() const
{
	return m_EventsCounter;
}

void EventsController::ClearEvents()
{
	m_EventsCounter = 0;
}

void EventsController::AddEvent(Event* eventToBeAdded)
{
	delete m_EventsBuffer[m_EventsCounter];
	m_EventsBuffer[m_EventsCounter++] = eventToBeAdded;
	m_EventsCounter %= MAX_EVENTS_COUNT;
}

const std::vector<Event*> &EventsController::GetEvents() const
{
	return m_EventsBuffer;
}
