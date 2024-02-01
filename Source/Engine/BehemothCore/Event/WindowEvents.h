#pragma once

#include "Event.h"

namespace Behemoth
{
	using namespace Events;
	
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int w, int h) : width(w), height(h) {}
		virtual EventType GetEventType() override { return EventType::WindowResize; }
		virtual uint16_t GetEventFlags() override { return EventFlags::Window; }

	private:
		int width;
		int height;
	};

	class WindowMoveEvent : public Event
	{
	public:
		WindowMoveEvent(int x, int y) : xPos(x), yPos(y) {}
		virtual EventType GetEventType() override { return EventType::WindowMove; }
		virtual uint16_t GetEventFlags() override { return EventFlags::Window; }

	private:
		int xPos;
		int yPos;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}
		virtual EventType GetEventType() override { return EventType::WindowClose; }
		virtual uint16_t GetEventFlags() override { return EventFlags::Window; }
	};
}