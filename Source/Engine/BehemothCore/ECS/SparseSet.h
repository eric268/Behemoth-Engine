#pragma once


#include "Component.h"
#include "Entity.h"
#include "Misc/Log.h"

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

	template<typename T>
	class SparseSet
	{
	public:
		SparseSet(const std::uint16_t maxSize = DEFAULT_SPARSE_SIZE) : maxSize(maxSize), sparse(maxSize, NULL_IDENTIFIER), index{}
		{
			Generator::Value<T>();
		}

		void RemoveComponent(const Entity& entity)
		{
			entity_identifier identifier = entity.GetIdentifier();

			if (identifier >= maxSize || sparse[identifier] == NULL_IDENTIFIER)
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

			sparse[identifier] = NULL_IDENTIFIER;
			index--;
		}

		void AddComponent(const Entity& entity, const T& component)
		{
			std::size_t identifier = entity.GetIdentifier();

			// If this entity already has this component then remove it and add new one assuming that the newest component is the desired one
			if (sparse[identifier] == entity.GetIdentifier())
			{
				LOG_MESSAGE(MessageType::Warning, entity.GetName() + " already has " + typeid(component).name() + " old component removed, new one added");
				RemoveComponent(entity);

				// Could also just set the component but worried about not following proper delete process for components. May lead to dangling pointers/references/memory leaks
// 				components[sparse[identifier]] = component;
// 				return;
			}

			if (identifier >= maxSize || sparse[identifier] != NULL_IDENTIFIER)
			{
				return;
			}

			sparse[identifier] = index++;
			dense.push_back(entity);
			components.push_back(component);
		}

		T* GetComponent(const Entity& entity)
		{
			entity_identifier identifier = entity.GetIdentifier();

			if (identifier >= maxSize || sparse[identifier] == NULL_IDENTIFIER)
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
			entity_identifier identifier = entity.GetIdentifier();
			if (identifier < maxSize && sparse[identifier] != NULL_IDENTIFIER)
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
			const entity_identifier leftIdentifier = lhs.GetIdentifier();
			const entity_identifier rightIdentifier = rhs.GetIdentifier();

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

		std::vector<entity_identifier> sparse;
		std::vector<T> components;

		int maxSize;
		size_t index;
	};
}
