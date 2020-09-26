#ifndef EVENT_H
#define EVENT_H

enum class EventType
{
	//Input events
	kExitGame,
	kPauseGame,
	kResumeGame,
	kToggleDayTime,
	kMovePlayer,
	kRotatePlayer,
	kPlayerShoots,
	kPlayerSelects
	//Game logic events


	//AI events


	//Renderer events


	//GUI events


	//Sound events
};


struct Event
{
	EventType m_EventType;
	virtual ~Event() {};
};

enum class MovementDirection
{
	kForward,
	kRight,
	kLeft,
	kBackward
};

struct MovePlayerEvent : Event
{
	MovementDirection m_MovementDirection;
};

struct RotatePlayerEvent : Event
{
	double m_MouseXPos;
	double m_MouseYPos;
};

#endif // !EVENT_H