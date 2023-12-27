#pragma once

#include "Event.h"
#include "NextAPI/App/app.h"

#include <queue>
#include <functional>
#include <unordered_map>

namespace Behemoth
{
	class EventManager
	{
	public:
		EventManager(EventManager& obj) = delete;
		EventManager& operator= (const EventManager& obj) = delete;

		static EventManager& GetInstance();

		void BindEventCallbacks();

		// TODO:
		// container for keyboard input tracking
		// container for mouse input tracking
		// container for controller 
		// Need to track previous frame input states as well as current frames input states

		// Some way to track multiple controller inputs
		template<typename T>
		using EventFunc = std::function<void(T&)>;

		std::function<void(Event&)> OnEventDelegate;

	private:
		EventManager() = default;
	};
}
