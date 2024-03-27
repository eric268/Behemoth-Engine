#pragma once

#include "Component.h"
#include "Entity.h"
#include "Core/Log.h"
#include "Page.h"

#include <vector>
#include <cassert>
#include <type_traits>
#include <concepts>
#include <algorithm>
#include <string>

#define DEFAULT_SPARSE_SIZE 10'000

namespace ECS
{
	template<typename T>
	concept IsComponent = std::is_convertible_v<T, Component>;

	template<typename T>
	class SparseSet
	{
	public:
		SparseSet(const std::uint16_t maxSize = DEFAULT_SPARSE_SIZE) : maxSize(maxSize), sparse(maxSize, NULL_ENTITY), index()
		{
			Generator::Value<T>();
		}

		void RemoveComponent(Entity entity)
		{
			entity_identifier identifier = entity.GetIdentifier();
			if (!Contains(entity))
			{
				return;
			}

			entity_identifier denseIndex = Entity::GetIdentifier(sparse[identifier]);

			dense[denseIndex].SetName("Destroyed");

			// Use dense identifier to signal the next recycled entity to be reused
			if (available > 0)
			{
				dense[denseIndex].SetIdentifier(next);
			}

			// Next is used to track the position of the next recycled entity to be used
			next = denseIndex;

			// Null version is used to indicate that an ID has been recycled and is therefore not valid
			// This is important because sparse identifiers are used to track position of entity in dense array so we can't use that.
			// Could use the dense identifier for this but then we would have to use the entity identifier, through the sparse to get dense version
			// this way we can skip one of those steps
			// dense[sparse[identifier]].SetVersionToNull();
			Entity::SetVersion(sparse[identifier], NULL_VERSION);

			available++;
		}

		T* AddComponent(const Entity& entity, const T& component)
		{
			entity_identifier identifier = entity.GetIdentifier();

			if (Contains(entity))
			{
				RemoveComponent(entity);
			}

			// Use one of the recycled entities instead of creating a new one
			if (available > 0 && next != NULL_IDENTIFIER)
			{
				entity_identifier nextIdentifier = next;
				assert(nextIdentifier < dense.size());
				sparse[entity.GetIdentifier()] = nextIdentifier;
				Entity::SetVersion(sparse[entity.GetIdentifier()], entity.GetVersion());

				next = dense[nextIdentifier].GetIdentifier();
				dense[nextIdentifier] = entity;
				available--;
			}
			else
			{
				sparse[identifier] = index++;
				dense.push_back(entity);
			}

			components.Add(identifier, component);
			return &components[identifier];
		}

		T* GetComponent(const Entity& entity)
		{
			entity_identifier identifier = entity.GetIdentifier();

			if (!Contains(entity))
			{
				return nullptr;
			}

			if (entity.GetVersion() != Entity::GetVersion(sparse[identifier]))
			{
				// RemoveComponent(entity);
				return nullptr;
			}

			return &components[identifier];
		}

		bool Contains(const Entity& entity)
		{
			entity_identifier identifier = entity.GetIdentifier();
//  			entity_version version = Entity::GetVersion(sparse[identifier]);
//   			return (identifier < sparse.size() && version < NULL_VERSION && entity.GetVersion() == version);

			return (identifier < sparse.size()) && (((~NULL_VERSION & entity.GetID()) ^ sparse[identifier]) < NULL_VERSION);
		}

		size_t size() const
		{
			return dense.size();
		}

		std::vector<Entity> dense;
		entity_id next {};

	private:
		friend class Registry;

		std::vector<entity_id> sparse;
		Pages<T> components;

		int maxSize;
		size_t index;
		std::size_t available{};
	};
}
