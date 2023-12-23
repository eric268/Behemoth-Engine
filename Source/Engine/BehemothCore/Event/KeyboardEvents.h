#pragma once

#include "Event.h"
#include "InputMapping.h"

namespace Behemoth
{
	class KeyboardEvent : public Event
	{
	public:
		KeyboardEvent(KeyCode keyCode) :keyCode(keyCode) {}

		virtual EventType GetEventType() = 0;
		virtual uint16_t GetEventFlags() = 0;
		virtual const char* GetEventName() = 0;

	private:
		KeyCode keyCode;
	};


	class KeyDownEvent : public KeyboardEvent
	{
	public:
		KeyDownEvent(KeyCode keyCode) : KeyboardEvent(keyCode) {}

		virtual EventType GetEventType()   override { return EventType::KeyPressed; }
		virtual uint16_t GetEventFlags()   override { return EventFlags::InputEvent | EventFlags::KeyboardEvent; }
		virtual const char* GetEventName() override { return "KeyDownEvent"; }
	};

	class KeyReleasedEvent : public KeyboardEvent
	{
	public:
		KeyReleasedEvent(KeyCode keyCode) : KeyboardEvent(keyCode) {}

		virtual EventType GetEventType()   override { return EventType::KeyReleased; }
		virtual uint16_t GetEventFlags()   override { return EventFlags::InputEvent | EventFlags::KeyboardEvent; }
		virtual const char* GetEventName() override { return "KeyReleasedEvent"; }
	};

	class KeyHeldEvent : public KeyboardEvent
	{
	public:

	private:

	};
}