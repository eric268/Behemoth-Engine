#pragma once


#include "Component.h"
#include "Entity.h"
#include "Misc/Log.h"
#include "Page.h"

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
		SparseSet(const std::uint16_t maxSize = DEFAULT_SPARSE_SIZE) : maxSize(maxSize), sparse(maxSize, NULL_ENTITY), index {}
		{
			Generator::Value<T>();
		}

		void RemoveComponent(const Entity& entity)
		{
			entity_identifier identifier = entity.GetIdentifier();

			if (identifier >= maxSize || sparse[identifier] == NULL_ENTITY)
				return;

			Entity e = dense[sparse[identifier]];

			dense[sparse[identifier]].SetIDToNull();
			
			e.SetVersionToNull();

			if (available > 0)
			{
				e.SetIdentifier(next);
			}

			next = identifier;
			Entity::SetVersion(sparse[identifier], NULL_VERSION);
			available++;
		}

		T* AddComponent(const Entity& entity, const T& component)
		{
			entity_identifier identifier = entity.GetIdentifier();


			// If this entity already has this component then remove it and add new one assuming that the newest component is the desired one
			if (identifier < maxSize && Entity::GetIdentifier(sparse[identifier]) != NULL_IDENTIFIER && dense[sparse[identifier]].GetIdentifier() == entity.GetIdentifier())
			{
				LOGMESSAGE(MessageType::Warning, entity.GetName() + " already has " + typeid(component).name() + " old component removed, new one added");
				RemoveComponent(entity);

				// Could also just set the component but worried about not following proper delete process for components. May lead to dangling pointers/references/memory leaks
// 				components[sparse[identifier]] = component;
// 				return;
			}

			if (identifier >= maxSize || Entity::GetVersion(sparse[identifier]) != NULL_VERSION)
			{
				LOGMESSAGE(MessageType::Error, std::string("Failed to add ") + typeid(component).name() + "to entity " + entity.GetName());
				return nullptr;
			}

			if (available > 0 && next != NULL_IDENTIFIER)
			{
				entity_identifier identifier = next;
				auto v = entity.GetVersion();
				Entity::SetVersion(sparse[identifier], entity.GetVersion());
				next = dense[identifier].GetIdentifier();
				dense[identifier] = entity;
				available--;
			}
			else
			{
				sparse[identifier] = index++;
				dense.push_back(entity);
			}
// 			components.Add(sparse[identifier], component);
// 			return &components[sparse[identifier]];
			components.Add(identifier, component);
			return &components[identifier];
		}

		T* GetComponent(const Entity& entity)
		{
			entity_identifier identifier = entity.GetIdentifier();

			if (identifier >= maxSize || sparse[identifier] == NULL_ENTITY)
			{
				return nullptr;
			}

			if (entity.GetVersion() != dense[sparse[identifier]].GetVersion())
			{
				RemoveComponent(entity);
				return nullptr;
			}

			if (HasEntity(entity))
			{
				return &components[identifier];
			}

			return nullptr;
		}

		bool HasEntity(const Entity& entity)
		{
			entity_identifier identifier = entity.GetIdentifier();
			if (identifier < maxSize && Entity::GetVersion(sparse[identifier]) != NULL_VERSION)
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

		std::vector<entity_id> sparse;
		Pages<T> components;

		int maxSize;
		size_t index;

		entity_id next {};
		std::size_t available{};
	};
}
