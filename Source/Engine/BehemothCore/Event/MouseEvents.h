#pragma once
#include "Event.h"
#include "Input/InputCodes.h"


namespace Behemoth
{
	using namespace Events;
	
	class MouseEvent : public Event
	{
	public:
		MouseEvent(int x, int y) : xPos(x), yPos(y) {}

		virtual EventType GetEventType() = 0;
		virtual uint16_t GetEventFlags() = 0;
		const std::pair<float, float> GetMousePos() const { return { xPos, yPos }; }

	protected:
		float xPos;
		float yPos;
	};

	class MouseMoveEvent : public MouseEvent
	{
	public:
		MouseMoveEvent(int x, int y) : MouseEvent(x, y) {}

		virtual EventType GetEventType() override { return EventType::MouseMove; }
		virtual uint16_t GetEventFlags() override { return EventFlags::Input | EventFlags::Mouse | EventFlags::MouseMove; }
	private:

	};

	class MouseDragEvent : public MouseEvent
	{
	public:
		MouseDragEvent(int x, int y) : MouseEvent(x, y) {}

		virtual EventType GetEventType() override { return EventType::MouseDrag; }
		virtual uint16_t GetEventFlags() override { return EventFlags::Input | EventFlags::Mouse | EventFlags::MouseDrag; }
	};

	class MouseDownEvent : public MouseEvent
	{
	public:
		MouseDownEvent(MouseCode code, int x, int y) : MouseEvent(x, y), code(code) {}

		virtual EventType GetEventType() override { return EventType::MouseDown; }
		virtual uint16_t GetEventFlags() override { return EventFlags::Input | EventFlags::Mouse | EventFlags::MouseClick; }

		const MouseCode GetMouseCode() const { return code; }
	private:
		MouseCode code;
	};

	class MouseUpEvent : public MouseEvent
	{
	public:
		MouseUpEvent(MouseCode code, int x, int y) : MouseEvent(x, y), code(code) {}

		virtual EventType GetEventType() override { return EventType::MouseUp; }
		virtual uint16_t GetEventFlags() override { return EventFlags::Input | EventFlags::Mouse | EventFlags::MouseClick; }

		const MouseCode GetMouseCode() const { return code; }
	private:
		MouseCode code;
	};

}