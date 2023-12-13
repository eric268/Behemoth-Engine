#pragma once


#include "Component.h"
#include "Entity.h"

// stl
#include <vector>
#include <cassert>
#include <type_traits>
#include <concepts>
#include <algorithm>

#define DEFAULT_SPARSE_SIZE 10'000

namespace ECS
{
	template<typename T>
	concept IsComponent = std::is_convertible_v<T, Component>;

#define NULL_ENTITY 0xFFFF

	template<typename Component>
	class SparseSet
	{
	public:
		SparseSet(const std::uint16_t maxSize = DEFAULT_SPARSE_SIZE) : maxSize(maxSize), sparse(maxSize, NULL_ENTITY), index{}
		{
			Generator::Value<Component>();
		}

		void RemoveComponent(const Entity& entity)
		{
			std::size_t identifier = entity.GetIdentifier();

			if (identifier >= maxSize || sparse[identifier] == NULL_ENTITY)
				return;

			std::uint16_t denseIndex = sparse[identifier];
			int lastPos = dense.size() - 1;

			// No need to swap if component to be removed is already the last component in the dense array
			if (denseIndex != lastPos)
			{
				std::swap(dense[denseIndex], dense[lastPos]);
				std::swap(components[denseIndex], components[lastPos]);

				Entity swappedEntity = dense[denseIndex];
				sparse[swappedEntity.GetIdentifier()] = denseIndex;
			}

			dense.pop_back();
			components.pop_back();

			sparse[identifier] = NULL_ENTITY;
			index--;
		}

		void AddComponent(const Entity& entity, const Component& component)
		{
			std::size_t identifier = entity.GetIdentifier();
			std::size_t id = NULL_ENTITY;

			if (identifier >= maxSize || sparse[identifier] != NULL_ENTITY)
				return;

			sparse[identifier] = index++;
			dense.push_back(entity);
			components.push_back(component);
		}

		Component* GetComponent(const Entity& entity)
		{
			std::size_t identifier = entity.GetIdentifier();

			if (identifier >= maxSize || sparse[identifier] == NULL_ENTITY)
				return nullptr;

			if (entity.GetVersion() != dense[sparse[identifier]].GetVersion())
			{
				RemoveComponent(entity);
				return nullptr;
			}

			if (HasEntity(entity))
			{
				return &components[sparse[identifier]];
			}

			return nullptr;
		}

		bool HasEntity(const Entity& entity)
		{
			std::uint16_t identifier = entity.GetIdentifier();
			if (identifier < maxSize && sparse[identifier] != NULL_ENTITY)
			{
				if (entity.GetVersion() == dense[sparse[identifier]].GetVersion())
				{
					return true;
				}
				else
				{

					RemoveComponent(entity);
				}
			}

			return false;
		}

		size_t size() const
		{
			return dense.size();
		}

		std::vector<Entity> dense;
	private:

		friend class Registry;

		void Adjust(Entity lhs, Entity rhs)
		{
			const std::size_t leftIdentifier = lhs.GetIdentifier();
			const std::size_t rightIdentifier = rhs.GetIdentifier();

			std::swap(components[sparse[leftIdentifier]], components[sparse[rightIdentifier]]);
		}

		template<typename Compare>
		void Sort(Compare compare)
		{
			std::sort(dense.begin(), dense.end(), std::move(compare));

			for (size_t pos = 0; pos < dense.size(); pos++)
			{
				size_t current = pos;
				size_t next = sparse[dense[current].GetIdentifier()];

				while (current != next)
				{
					Adjust(dense[current], dense[next]);
					sparse[dense[current].GetIdentifier()] = current;
					current = next;
					next = sparse[dense[current].GetIdentifier()];
				}
			}
		}

		std::vector<std::uint16_t> sparse;
		std::vector<Component> components;

		int maxSize;
		size_t index;
	};
}
