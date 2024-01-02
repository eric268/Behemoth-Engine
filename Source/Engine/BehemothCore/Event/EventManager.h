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

		template<typename T>
		using EventFunc = std::function<void(T&)>;

		std::function<void(Event&)> OnEventDelegate;

	private:
		EventManager() = default;
	};
}
