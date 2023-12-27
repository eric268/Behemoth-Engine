#pragma once

#include "ECS/System.h"
#include "ECS/Generator.h"
#include "ECS/SparseSet.h"

#include <unordered_map>
#include <vector>
#include <memory>

namespace Behemoth
{
	class SystemManager
	{
	public:
		SystemManager(SystemManager&) = delete;
		SystemManager& operator=(const SystemManager&) = delete;

		static SystemManager& GetInstance()
		{
			static SystemManager instance;
			return instance;
		}

		template <ECS::HasRunMethod T>
		void AddSystem()
		{
			int typeIndex = ECS::Generator::Value<T>();
			if (systemTypeID.find(typeIndex) != systemTypeID.end())
				return;

			systemContainer.push_back(std::make_shared<ECS::System<T>>());
			systemTypeID[typeIndex] = systemContainer.size() - 1;
		}

		void Run(const float deltaTime, ECS::Registry& registry)
		{
			for (const auto& system : systemContainer)
			{
				system->Run(deltaTime, registry);
			}
		}

	private:
		SystemManager() = default;

		
		// ECS::SparseSet<std::shared_ptr<ECS::ISystem>> sparseSet;

		std::unordered_map<int, int> systemTypeID;
		std::vector<std::shared_ptr<ECS::ISystem>> systemContainer;
	};
}

