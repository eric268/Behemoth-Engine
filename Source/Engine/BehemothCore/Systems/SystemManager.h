#pragma once

#include "ECS/System.h"
#include "ECS/Generator.h"
#include "ECS/SparseSet.h"

#include <unordered_set>
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
			if (systemTypeID.count(typeIndex))
				return;

			systemTypeID.insert(typeIndex);
			systemContainer.push_back(std::make_shared<ECS::System<T>>());
		}

		void Run(ECS::Registry& registry)
		{
			for (const auto& system : systemContainer)
			{
				system->Run(registry);
			}
		}

	private:
		SystemManager() = default;

		
		// ECS::SparseSet<std::shared_ptr<ECS::ISystem>> sparseSet;

		std::unordered_set<int> systemTypeID;
		std::vector<std::shared_ptr<ECS::ISystem>> systemContainer;
	};
}

