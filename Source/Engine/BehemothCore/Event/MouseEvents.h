#pragma once
#include "Event.h"
#include "Input/InputCodes.h"

namespace Behemoth
{
	class MouseEvent : public Event
	{
	public:
		MouseEvent(int x, int y) : xPos(x), yPos(y) {}

		virtual EventType GetEventType() = 0;
		virtual uint16_t GetEventFlags() = 0;
		virtual const char* GetEventName() = 0;
		std::pair<float, float> GetMousePos() { return { xPos, yPos }; }

	protected:
		float xPos;
		float yPos;
	};

	class MouseMoveEvent : public MouseEvent
	{
	public:
		MouseMoveEvent(int x, int y) : MouseEvent(x, y) {}

		static EventType GetStaticEventType() { return EventType::MouseMove; }
		virtual EventType GetEventType() override { return EventType::MouseMove; }
		virtual uint16_t GetEventFlags() override { return EventFlags::Input | EventFlags::Mouse | EventFlags::MouseMove; }
		virtual const char* GetEventName() { return "Mouse Move Event"; }
	private:

	};

	class MouseDragEvent : public MouseEvent 
	{
	public:
		MouseDragEvent(int x, int y) : MouseEvent(x, y) {}

		static EventType GetStaticEventType() { return EventType::MouseDrag; }
		virtual EventType GetEventType() override { return EventType::MouseDrag; }
		virtual uint16_t GetEventFlags() override { return EventFlags::Input | EventFlags::Mouse | EventFlags::MouseDrag; }
		virtual const char* GetEventName() { return "Mouse Drag Event"; }
	};

	class MouseDownEvent : public MouseEvent
	{
	public:
		MouseDownEvent(MouseCode code, int x, int y) : MouseEvent(x, y), code(code) {}

		static EventType GetStaticEventType() { return EventType::MouseDown; }
		virtual EventType GetEventType() override { return EventType::MouseDown; }
		virtual uint16_t GetEventFlags() override { return EventFlags::Input | EventFlags::Mouse | EventFlags::MouseClick; }
		virtual const char* GetEventName() { return "Mouse Down Event"; }

		MouseCode GetMouseCode() { return code; }
	private:
		MouseCode code;
	};

	class MouseUpEvent : public MouseEvent
	{
	public:
		MouseUpEvent(MouseCode code, int x, int y) : MouseEvent(x, y), code(code) {}

		static EventType GetStaticEventType() { return EventType::MouseUp; }
		virtual EventType GetEventType() override { return EventType::MouseUp; }
		virtual uint16_t GetEventFlags() override { return EventFlags::Input | EventFlags::Mouse | EventFlags::MouseClick; }
		virtual const char* GetEventName() { return "Mouse Up Event"; }

		MouseCode GetMouseCode() { return code; }
	private:
		MouseCode code;
	};


}