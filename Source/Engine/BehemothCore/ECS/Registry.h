#pragma once

#include "Entity.h"
#include "Component.h"
#include "SparseSet.h"
#include "ComponentPool.h"

// stl
#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <tuple>
#include <functional>
#include <concepts>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>

#define MAX_NUM_ENTITIES 10'000

namespace ECS
{
	template<typename T>
	struct is_tuple : std::false_type {};

	template<typename ... Ts>
	struct is_tuple<std::tuple<Ts...>> : std::true_type {};

	template <typename T>
	concept IsTuple = is_tuple<T>::value;

	class Registry
	{
	public:
		Registry() = default;
		Registry(Registry&& obj) = delete;
		Registry(const Registry& other) = delete;
		~Registry()
		{
			std::cout << "Registry destructor called\n";
		}

		std::string GetName(EntityHandle entityHandle)
		{
			Entity entity = GetEntityFromHandle(entityHandle);
			return (entity.IsValid()) ? entity.GetName() : " Null Entity";
		}

		bool DestroyEntity(EntityHandle entityHandle)
		{
			Entity entity = GetEntityFromHandle(entityHandle);
			if (entity.IsValid())
			{
				DestroyEntity(entity);
				return true;
			}

			return false;
		}

		bool IsValidHandle(EntityHandle entityHandle)
		{
			Entity entity = GetEntityFromHandle(entityHandle);
			return entity.IsValid();
		}

		EntityHandle CreateEntity(std::string name = "Entity")
		{
			Entity e(name);

			if (available)
			{
				e.SetID(RecycleEntity());
				entities[e.GetIdentifier()].name = name;
			}
			else
			{
				e.SetIdentifier(IDIndex++);
				entities.push_back(e);
			}

			EntityHandle entityHandle{};
			entityHandle.ID = e.ID;

			return entityHandle;
		}

		Entity CreateNullEntity()
		{
			Entity e("Null Entity");
			e.SetID(0xFFFFFFFF);
			return e;
		}

		template<IsComponent T, typename ... Args>
		T* AddComponent(EntityHandle handle, Args&& ... parameters)
		{
			Entity entity = GetEntityFromHandle(handle);
			if (entity.IsValid())
			{
				return AddComponent<T>(entity, std::forward<Args>(parameters)...);
			}
			else
			{
				std::string message = std::string("Failed to add ") + typeid(T).name() + " to entity: " + entity.name;
				LOGMESSAGE(MessageType::Error, message);
				return nullptr;
			}
		}

		template<IsComponent T>
		T* GetComponent(EntityHandle handle)
		{
			Entity entity = GetEntityFromHandle(handle);
			if (entity.IsValid())
			{
				return GetComponent<T>(entity);
			}
			return nullptr;
		}

		template<IsComponent T, typename ... Args>
		T* AddComponent(Entity entity, Args&& ... parameters)
		{
			T component(std::forward<Args>(parameters)...);
			size_t index = Generator::Value<T>();
			auto set = GetComponent<T>();
			return set->AddComponent(entity, component);
		}

		template<IsComponent T>
		void RemoveComponent(EntityHandle handle)
		{
			Entity entity = GetEntityFromHandle(handle);
			if (entity.IsValid())
			{
				auto set = GetComponent<T>();
				set->RemoveComponent(entity);
			}
		}

		template<IsComponent T>
		T* GetComponent(Entity entity)
		{
			auto set = GetComponent<T>();
			return set->GetComponent(entity);
		}

		template <IsComponent ...T>
		std::tuple<T*...> GetMultipleComponents(EntityHandle handle)
		{
			Entity entity = GetEntityFromHandle(handle);
			if (entity.IsValid())
			{
				return GetMultipleComponents<T...>(entity);
			}
			return {};
		}

		template<IsComponent ...T>
		std::tuple<T*...> GetMultipleComponents(Entity entity)
		{
			return std::make_tuple((GetComponent<T>(entity)) ...);
		}

		template<IsComponent... T>
		std::vector<std::tuple<Entity, T*...>> Group()
		{
			auto componentSets = std::make_tuple((GetComponent<T>()) ...);
			SortSets(componentSets);
			std::vector<std::tuple<Entity, T*...>> group = GetSets<T...>(componentSets);

			return group;
		}

		template<IsComponent... T>
		std::vector<std::tuple<Entity, T*...>> Get()
		{
			auto componentSets = std::make_tuple((GetComponent<T>()) ...);
			std::vector<std::tuple<Entity, T*...>> group = GetSets<T...>(componentSets);

			return group;
		}

		template<typename T>
		std::shared_ptr<ComponentPool<T>> GetComponent()
		{
			type_index typeIndex = Generator::Value<T>();

			if (componentID.find(typeIndex) == componentID.end())
			{
				auto newArray = std::make_shared<ComponentPool<T>>();
				newArray->typeID = Generator::Value<T>();
				componentArray.push_back(newArray);

				componentID[typeIndex] = componentArray.size() - 1;
			}

			return std::static_pointer_cast<ComponentPool<T>>(componentArray[componentID[typeIndex]]);
		}

		template <typename T> 
		void RemoveAllComopnents()
		{
			auto componentsToRemove = GetComponent<T>();

			for (int i = componentsToRemove->dense.size() - 1; i >= 0; i--)
			{
				componentsToRemove->RemoveComponent(componentsToRemove->dense[i]);
			}
		}


	private:
		using type_index = int;
		std::vector<Entity> entities;
		std::size_t available{};
		entity_id next{};
		int IDIndex = 0;

		std::unordered_map<int,int> componentID;
		std::vector<std::shared_ptr<IComponentPool>> componentArray;

		Entity GetEntityFromHandle(EntityHandle handle)
		{
			entity_identifier identifier = Entity::GetIdentifier(handle.ID);

			if (identifier >= entities.size() || entities[identifier].GetID() != handle.ID)
			{
				return CreateNullEntity();
			}

			return entities[identifier];
		}

		void DestroyEntity(Entity entity)
		{
			for (int i = 0; i < componentArray.size(); i++)
			{
				componentArray[i]->RemoveComponent(entity);
			}

			entity.IncrementVersion();
			auto identifier = entity.GetIdentifier();
			if (available > 0)
			{
				entity.SetIdentifier(next);
			}

			entities[identifier] = entity;
			next = identifier;
			available++;
		}

		entity_id RecycleEntity()
		{
			assert(available > 0);

			std::uint16_t identifier = next;
			next = entities[next].GetIdentifier();
			entities[identifier].SetIdentifier(identifier);

			available--;

			return entities[identifier].GetID();
		}

		template<IsTuple T>
		bool ContainsEntity(T tuple, const Entity& entity)
		{
			return std::apply([&](auto&... set)
				{
					return (set->Contains(entity) && ...);
				}, tuple);
		}

		template<IsTuple T>
		void SortSets(T& tuple)
		{
			// First sort the arrays so that 
			auto compare = [&](const Entity& e1, const Entity& e2)
			{
				bool lhs = ContainsEntity(tuple, e1);
				bool rhs = ContainsEntity(tuple, e2);
				if (lhs == rhs)
				{
					return e1.GetIdentifier() < e2.GetIdentifier();
				}
				else
				{
					return lhs > rhs;
				}
			};

			std::apply([&](auto&... sets)
				{
					(..., sets->Sort(compare));
				}, tuple);
		}

		template<IsComponent ... T, IsTuple U>
		std::vector<std::tuple<Entity, T*...>> GetSets(U tuple)
		{
			std::vector<std::tuple<Entity, T*...>> group;

			auto smallestPool = GetSmallestPool<T...>(tuple);

			// Iterate over smallest set
			for (const auto& entity : smallestPool->GetEntities())
			{
				// Check if the entity is in all other sets
				bool isInAllSets = ContainsEntity(tuple, entity);

				// If the entity is in all sets, get the component pointers and add to the group
				if (isInAllSets)
				{
					group.emplace_back(std::apply([&](auto&... sets)
						{
							return std::make_tuple(entity, sets->GetComponent(entity)...);
						}, tuple));
				}
			}
			return group;
		}

		template<IsComponent ... T, IsTuple U>
		auto GetSmallestPool(U tuple)
		{
			std::shared_ptr<IComponentPool> componentPoolPtr = nullptr;

			std::unordered_set<int> set;
			if constexpr ((sizeof ... (T)) > 0)
			{
				(set.insert(Generator::Value<T>()), ...);
			}

			for (int i = 0; i < componentArray.size(); i++)
			{
				if (!set.count(componentArray[i]->typeID))
					continue;
					
				if (!componentPoolPtr || componentArray[i]->size() < componentPoolPtr->size())
				{
					componentPoolPtr = componentArray[i];
				}
			}

			return componentPoolPtr;
		}
	};
}