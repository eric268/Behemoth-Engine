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

		void RemoveComponent(Entity entity)
		{
			entity_identifier identifier = entity.GetIdentifier();

			if (!Contains(entity))
			{
				return;
			}
			Entity e = entity;

			dense[sparse[identifier]].name = "Deleted";

			if (available > 0)
			{
				dense[sparse[identifier]].SetIdentifier(next);
			}

			next = sparse[identifier];
			Entity::SetVersion(sparse[identifier], NULL_VERSION);
			available++;
		}

		T* AddComponent(const Entity& entity, const T& component)
		{
			entity_identifier identifier = entity.GetIdentifier();


			if (Contains(entity))
			{
				LOGMESSAGE(MessageType::Warning, entity.GetName() + " already has " + typeid(component).name() + " old component removed, new one added");
				RemoveComponent(entity);

			}

// 			if (identifier >= maxSize || Entity::GetVersion(sparse[identifier]) != NULL_VERSION)
// 			{
// 				LOGMESSAGE(MessageType::Error, std::string("Failed to add ") + typeid(component).name() + "to entity " + entity.GetName());
// 				return nullptr;
// 			}

			if (available > 0 && next != NULL_IDENTIFIER)
			{
				entity_identifier identifier = next;
				sparse[entity.GetIdentifier()] = identifier;
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

			if (!Contains(entity))
			{
				return nullptr;
			}
// 
// 			if (entity.GetVersion() != dense[Entity::GetIdentifier(sparse[identifier])].GetVersion())
// 			{
// 				RemoveComponent(entity);
// 				return nullptr;
// 			}

			if (Contains(entity))
			{
				return &components[identifier];
			}

			return nullptr;
		}

		bool Contains(const Entity& entity)
		{
			entity_identifier identifier = entity.GetIdentifier();
			std::uint32_t id = Entity::GetIdentifier(sparse[identifier]);
			std::uint32_t version = Entity::GetVersion(sparse[identifier]);
			long long val = id + version;
 			return (identifier < sparse.size() && val < NULL_VERSION);
		}
// 
// 		bool HasEntity(const Entity& entity)
// 		{
// 			entity_identifier identifier = entity.GetIdentifier();
// 
// 			auto id = Entity::GetIdentifier(sparse[identifier]);
// 			return identifier < sparse.size() && id < dense.size() && dense[id].ID == entity.ID;
// 			//bool result = (identifier < sparse.size() && ((~0xFFFF & entity.GetIdentifier()) ^ sparse[identifier]) < NULL_IDENTIFIER);
// 			// return result;
// 
// 			if (identifier < maxSize || Entity::GetVersion(sparse[identifier]) == NULL_VERSION)
// 			{
// 				if (entity.GetVersion() == dense[Entity::GetIdentifier(sparse[identifier])].GetVersion())
// 				{
// 					return true;
// 				}
// 				else
// 				{
// 					RemoveComponent(entity);
// 				}
// 			}
// 
// 			return false;
// 		}

		size_t size() const
		{
			return dense.size();
		}

		std::vector<Entity> dense;

	private:
		friend class Registry;

		std::vector<entity_id> sparse;
		Pages<T> components;

		int maxSize;
		size_t index;

		entity_id next {};
		std::size_t available{};
	};
}
