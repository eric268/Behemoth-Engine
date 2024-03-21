#pragma once

#include <string>

#define NULL_ENTITY 0xFFFFFFFF
#define NULL_IDENTIFIER 0xFFFF
#define NULL_VERSION 0xFFFF

namespace ECS
{
	using entity_id = std::uint32_t;
	using entity_identifier = std::uint16_t;
	using entity_version = std::uint16_t;

	class Entity
	{
	public:

		entity_id GetID() const
		{
			return ID;
		}

		entity_identifier GetIdentifier() const
		{
			return ID & NULL_IDENTIFIER;
		}

		inline void IncrementVersion()
		{
			entity_version version = (ID >> 16) + 1;
			ID = (version << 16) | (ID & 0xFFFF);
		}

		inline void SetVersionToNull()
		{
			ID = (NULL_VERSION << 16) | (ID & NULL_IDENTIFIER);
		}

		entity_version GetVersion() const
		{
			return ID >> 16;
		}

		const std::string GetName() const 
		{
			return name;
		}

		void SetName(const std::string& n)
		{
			name = n;
		}

		void SetIDToNull()
		{
			ID = NULL_ENTITY;
		}

		static void SetVersion(entity_id& ID, entity_id version)
		{
			ID = (version << 16) | (ID & NULL_IDENTIFIER);
		}

		const bool operator == (const Entity& e) const
		{
			return GetIdentifier() == e.GetIdentifier();
		}
		const bool operator != (const Entity & e) const
		{
			return !(*this == e);
		}

		static inline entity_identifier GetIdentifier(entity_id ID)
		{
			return ID & NULL_IDENTIFIER;
		}

		static inline entity_version GetVersion(entity_id ID)
		{
			return ID >> 16;
		}

		inline void SetIdentifier(const entity_identifier newIdentifier)
		{
			ID = (ID & 0xFFFF0000) | newIdentifier;
		}

		bool IsValid()
		{
			return ID != NULL_IDENTIFIER;
		}
		
	private:
		friend class Registry;

		// Only want the registry class to be able to create entities
		Entity(std::string name) : name(name), ID(0) {}

		inline void SetID(const entity_id newID)
		{
			ID = newID;
		}

		entity_id ID;
		std::string name;
	};

	struct EntityHandle
	{
		EntityHandle() : ID(NULL_ENTITY) {}
		EntityHandle(entity_id id) : ID(id) {}
		EntityHandle(Entity e) : ID(e.GetID()) {}

		bool operator==(EntityHandle& otherHandle)
		{
			return ID == otherHandle.ID;
		}

		bool operator !=(EntityHandle& otherHandle)
		{
			return ID != otherHandle.ID;
		}

		entity_id ID;
	};

}