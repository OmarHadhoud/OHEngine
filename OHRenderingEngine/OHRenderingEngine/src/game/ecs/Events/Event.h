#ifndef EVENT_H
#define EVENT_H

enum class EventType
{
	//Input events
	kExitGame,
	kPauseGame,
	GL_TEXTURE_WRAP_ResumeGame,
	GL_TEXTURE_WRAP_ToggleDayTime,
	kMovePlayer,
	GL_TEXTURE_WRAP_RotatePlayer,
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
	GL_TEXTURE_WRAP_Right,
	kLeft,
	GL_BACKward
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

struct UpdatePostProcessingParams : Event
{
	float m_GammaCorrection;
	float m_Exposure;
};

#endif // !EVENT_H