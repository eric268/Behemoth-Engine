#pragma once

#include "SparseSet.h"
#include "Entity.h"

#include <vector>

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
			LOGMESSAGE(MessageType::Error, "No child component pool type found of IComponentPool type");
			throw std::runtime_error("No child component pool type found of IComponentPool type");
		}

		virtual void RemoveComponent(const Entity& e) {}

		virtual entity_identifier GetNext()
		{
			return NULL_IDENTIFIER;
		}

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

		virtual std::vector<Entity>& GetEntities() override
		{
			return SparseSet<T>::dense;
		}

		virtual entity_identifier GetNext() override
		{
			return SparseSet<T>::next;
		}
	};
}