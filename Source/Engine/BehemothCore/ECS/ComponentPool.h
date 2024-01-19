#pragma once

#include <vector>
#include "SparseSet.h"
#include "Entity.h"

namespace ECS
{
	class IComponentPool
	{
	public:
		IComponentPool() : typeID(0) {}
		virtual ~IComponentPool() = default;

		virtual std::size_t size()
		{
			return std::size_t{};
		}

		// This function should never be called, just a virtual function for ComponentPool
		// cannot be a pure virtual class/function since I need to create IComponentPools from componentPool pointers
		// Need to find a better way to handle type erasure and storing/accessing templated Component Pool types
		virtual std::vector<Entity>& GetEntities()
		{
			std::vector<Entity> data;
			return data;
		}

		virtual void RemoveComponent(const Entity& e) {}

		int typeID;
	};

	template<typename T>
	class ComponentPool : public IComponentPool, public SparseSet<T>
	{
	public:
		ComponentPool() = default;

		std::size_t size() override
		{
			return SparseSet<T>::size();
		}

		virtual void RemoveComponent(const Entity& e) override
		{
			SparseSet<T>::RemoveComponent(e);
		}

		virtual std::vector<Entity>& GetEntities()
		{
			return SparseSet<T>::dense;
		}
	};
}