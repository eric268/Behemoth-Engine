#pragma once

#include "Entity.h"
#include "Components.h"
#include "SparseSet.h"
#include "ComponentPool.h"

// stl
#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <any>
#include <tuple>
#include <functional>
#include <variant>
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

		void DestroyEntity(Entity entity)
		{
			// 		for (int i = 0; i < components.size(); i++)
			// 		{
			// 			std::any_cast<SparseSet<Components>>(components[i]).RemoveComponent(entity);
			// 		}


			entity.IncrementVersion();
			auto identifier = entity.GetIdentifier();
			if (available > 0)
				entity.SetIdentifier(next);

			entities[identifier] = entity;
			next = identifier;
			available++;
		}

		Entity CreateEntity(std::string name)
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

			return e;
		}

		template<IsComponent T, typename ... Args>
		void AddComponent(Entity entity, Args&& ... parameters)
		{
			T component(std::forward<Args>(parameters)...);
			size_t index = Generator::Value<T>();
			auto set = GetComponent<T>();
			set->AddComponent(entity, component);
		}

		template<IsComponent T>
		T* GetComponent(Entity entity)
		{
			size_t index = Generator::Value<T>();
			auto set = GetComponent<T>();
			return set->GetComponent(entity);
		}

		template<IsComponent... T>
		std::vector<std::tuple<T*...>> Group()
		{
			auto componentSets = std::make_tuple((GetComponent<T>()) ...);
			SortSets(componentSets);
			std::vector<std::tuple<T* ...>> group = GetSets<T...>(componentSets);

			return group;
		}

		template<IsComponent... T>
		std::vector<std::tuple<T*...>> Get()
		{
			auto componentSets = std::make_tuple((std::ref(GetSparseSet<T>())) ...);
			std::vector<std::tuple<T* ...>> group = GetSets<T...>(componentSets);

			return group;
		}

		template<typename T>
		std::shared_ptr<ComponentPool<T>> GetComponent()
		{
			type_index typeIndex = Generator::Value<T>();
			assert(typeIndex <= componentArray.size());

			if (typeIndex == componentArray.size())
			{
				auto newArray = std::make_shared<ComponentPool<T>>();
				newArray->typeID = Generator::Value<T>();
				componentArray.push_back(newArray);
			}

			return std::static_pointer_cast<ComponentPool<T>>(componentArray[typeIndex]);
		}

	private:
		using type_index = int;
		std::vector<Entity> entities;
		std::size_t available{};
		std::uint32_t next{};
		int IDIndex = 0;

		std::vector<std::shared_ptr<IComponentPool>> componentArray;

		std::uint32_t RecycleEntity()
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
					return (set->HasEntity(entity.GetIdentifier()) && ...);
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
		std::vector<std::tuple<T*...>> GetSets(U tuple)
		{
			std::vector<std::tuple<T*...>> group;

			// Need to find a way to get the smallest component set

			auto smallestPool = GetSmallestPool<T...>(tuple);

			// Iterate over each entity in the smallest set
			for (const auto& entity : smallestPool->GetEntities())
			{
				// Check if the entity is in all other sets
				bool isInAllSets = ContainsEntity(tuple, entity);

				// If the entity is in all sets, get the component pointers and add to the group
				if (isInAllSets)
				{
					group.emplace_back(std::apply([&](auto&... sets)
						{
							return std::make_tuple(sets->GetComponent(entity)...);
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
				if (set.count(componentArray[i]->typeID) == 0)
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