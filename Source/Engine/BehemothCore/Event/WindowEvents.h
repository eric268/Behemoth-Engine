#pragma once
#include "Event.h"

namespace Behemoth
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int w, int h) : width(w), height(h) {}

		static EventType GetStaticEventType() { return EventType::WindowResize; }
		virtual EventType GetEventType() override { return EventType::WindowResize; }
		virtual uint16_t GetEventFlags() override { return EventFlags::Window; }
		virtual const char* GetEventName() { return "Window Resize Event"; }

	private:
		int width;
		int height;
	};

	class WindowMoveEvent : public Event
	{
	public:
		WindowMoveEvent(int x, int y) : xPos(x), yPos(y) {}

		static EventType GetStaticEventType() { return EventType::WindowMove; }
		virtual EventType GetEventType() override { return EventType::WindowMove; }
		virtual uint16_t GetEventFlags() override { return EventFlags::Window; }
		virtual const char* GetEventName() { return "Window Move Event"; }

	private:
		int xPos;
		int yPos;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		static EventType GetStaticEventType() { return EventType::WindowClose; }
		virtual EventType GetEventType() override { return EventType::WindowClose; }
		virtual uint16_t GetEventFlags() override { return EventFlags::Window; }
		virtual const char* GetEventName() { return "Window Close Event"; }
	};
}