#pragma once

#include <functional>

namespace Behemoth
{
	namespace Events
	{
		enum class EventType
		{
			None = 0,
			WindowResize, 
			WindowClose,
			WindowMove,
			KeyPressed,
			KeyReleased,
			KeyHeld,
			MouseDown,
			MouseUp,
			MouseMove,
			MouseScrollWheel,
			MouseDrag,
			ControllerPressed,
			ControllerMove,
			ControllerTriggerPressed
		};

		enum EventFlags : uint16_t
		{
			None			= 0x0000,
			Window			= 0x0001,
			Input			= 0x0002,
			Keyboard		= 0x0004,
			Controller		= 0x0008,
			Mouse			= 0x0010,
			MouseMove		= 0x0020,
			MouseClick		= 0x0040,
			MouseDrag		= 0x0080
		};
	}


	struct Event
	{
		virtual ~Event() = default;
		virtual Events::EventType GetEventType() = 0;
		virtual uint16_t GetEventFlags() = 0;
	};

	// Need to find a better way then comparing the type names
	class EventHandler
	{
	public:
		EventHandler(Event& e) : e(e) {}

		template <typename T, typename Func>
		bool ProcessEvent(Func func)
		{
			if (typeid(e).name() == typeid(T).name())
			{
				func(static_cast<T&>(e));
				return true;
			}
			return false;
		}
	private:
		Event& e;
	};
}

