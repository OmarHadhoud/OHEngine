#include "System.h"



System::System(): m_EventIndex(-1)
{
}

void System::SetEventsController(EventsController * eventsController)
{
	m_EventsController = eventsController;
}

void System::SetECSManager(ECSManager * manager)
{
	m_ECSManager = manager;
}

void System::SetCurrentWindow(GLFWwindow * currentWindow, int width, int height)
{
	m_CurrentWindow = currentWindow;
	m_WindowWidth = width;
	m_WindowHeight = height;
}

void System::ProcessEvents()
{
	int lastEventIndex = m_EventsController->GetLastEventIndex();
	const std::vector<Event*> events = m_EventsController->GetEvents();
	while (m_EventIndex != lastEventIndex && events[0]!=nullptr)
	{
		m_EventIndex = m_EventIndex == -1 ? 0 : m_EventIndex;
		ProcessEvent(events[m_EventIndex]);
		m_EventIndex = (m_EventIndex+1)%MAX_EVENTS_COUNT;
	}
}


System::~System()
{
}
