#pragma once

#include <functional>

namespace Behemoth
{
	enum class EventType
	{
		None = 0,
		WindowResize, WindowClose, WindowMove,
		KeyPressed, KeyReleased, KeyHeld, // possibly add special key event types?
		MouseClick, MouseMove, MouseScrollWheel, MouseDrag,
		ControllerPressed, ControllerMove, ControllerTriggerPressed
	};

	enum EventFlags : uint16_t 
	{
		None			= 0x0000,
		WindowEvent		= 0x0001, 
		InputEvent		= 0x0002,
		KeyboardEvent	= 0x0004,
		MouseEvent		= 0x0008,
		ControllerEvent = 0x0010,
		MouseMoveEvent	= 0x0020,
		MouseClickEvent = 0x0040,
		MouseDragEvent	= 0x0080
	};


	class Event
	{
	public:
		virtual ~Event() = default;
		virtual EventType GetEventType() = 0;
		virtual uint16_t GetEventFlags() = 0;
		virtual const char* GetEventName() = 0;
	};
}

