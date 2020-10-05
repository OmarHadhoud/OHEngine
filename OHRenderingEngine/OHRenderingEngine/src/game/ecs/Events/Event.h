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
	kPlayerSelects,
	//Game logic events
	kPlayerWins,
	kPlayerDies,
	kPlayerHit,
	kEnemyHit,

	//AI events
	kEnemyMove,
	kEnemyShoots,

	//Renderer events


	//GUI events
	kUpdatePostProcessingParams

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
	double m_LastMouseXPos;
	double m_LastMouseYPos;
};

struct PlayerMouseClickEvent : Event
{
	double m_MouseXPos;
	double m_MouseYPos;
};

struct UpdatePostProcessingParams : Event
{
	float m_GammaCorrection;
	float m_Exposure;
};

#endif // !EVENT_H