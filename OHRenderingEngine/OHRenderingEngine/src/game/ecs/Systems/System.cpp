#include "System.h"



System::System(): m_EventIndex(-1)
{
}

void System::SetEventsController(EventsController * eventsController)
{
	m_EventsController = eventsController;
}

void System::ProcessEvents()
{
	int lastEventIndex = m_EventsController->GetLastEventIndex();
	const std::vector<Event*> events = m_EventsController->GetEvents();
	while (m_EventIndex != lastEventIndex)
	{
		m_EventIndex = m_EventIndex == -1 ? 0 : m_EventIndex;
		ProcessEvent(events[m_EventIndex]);
		m_EventIndex = (m_EventIndex+1)%MAX_EVENTS_COUNT;
	}
}


System::~System()
{
}
