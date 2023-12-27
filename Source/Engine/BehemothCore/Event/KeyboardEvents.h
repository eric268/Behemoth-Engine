#pragma once

#include "Event.h"
#include "Input/InputCodes.h"

namespace Behemoth
{
	class KeyboardEvent : public Event
	{
	public:
		KeyboardEvent(KeyCode keyCode) :keyCode(keyCode) {}

		virtual EventType GetEventType() = 0;
		virtual uint16_t GetEventFlags() = 0;
		virtual const char* GetEventName() = 0;

		const KeyCode GetKeyCode() const { return keyCode; }

	private:
		KeyCode keyCode;
	};


	class KeyDownEvent : public KeyboardEvent
	{
	public:
		KeyDownEvent(KeyCode keyCode) : KeyboardEvent(keyCode) {}
		static EventType GetStaticEventType() { return EventType::KeyPressed; }
		virtual EventType GetEventType()   override { return EventType::KeyPressed; }
		virtual uint16_t GetEventFlags()   override { return EventFlags::Input | EventFlags::Keyboard; }
		virtual const char* GetEventName() override { return "KeyDownEvent"; }
	};

	class KeyReleasedEvent : public KeyboardEvent
	{
	public:
		KeyReleasedEvent(KeyCode keyCode) : KeyboardEvent(keyCode) {}

		static EventType GetStaticEventType() { return EventType::KeyReleased; }
		virtual EventType GetEventType()   override { return EventType::KeyReleased; }
		virtual uint16_t GetEventFlags()   override { return EventFlags::Input | EventFlags::Keyboard; }
		virtual const char* GetEventName() override { return "KeyReleasedEvent"; }
	};

	class KeyHeldEvent : public KeyboardEvent
	{
	public:

	private:

	};
}