#pragma once


// stl
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
			return ID & 0x0000FFFF;
		}

		inline void IncrementVersion()
		{
			entity_version version = (ID >> 16) + 1;
			ID = (version << 16) | (ID & 0x0000FFFF);
		}

		entity_version GetVersion() const
		{
			return ID >> 16;
		}

		const std::string GetName() const 
		{
			return name;
		}

		void SetIDToNull()
		{
			ID = 0xFFFFFFFF;
		}

		void SetVersionToNull()
		{
			ID = (0xFFFF << 16) | (ID & 0x0000FFFF);
		}

		static void SetVersion(entity_id& ID, entity_id version)
		{
			ID = (version << 16) | (ID & 0x0000FFFF);
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
			return ID & 0x0000FFFF;
		}

		static inline entity_version GetVersion(entity_id ID)
		{
			return ID >> 16;
		}
		
	public:
		friend class Registry;

		Entity(std::string name) : name(name), ID(0) {}

		// Only want to be able to use this in Registry class to point to next recycled entity ID
		inline void SetIdentifier(const entity_identifier newIdentifier)
		{
			ID = (ID & 0xFFFF0000) | newIdentifier;
		}

		inline void SetID(const entity_id newID)
		{
			ID = newID;
		}

		bool IsValid()
		{
			return ID != 0xFFFFFFFF;
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