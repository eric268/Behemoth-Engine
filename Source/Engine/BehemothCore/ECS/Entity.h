#pragma once

// stl
#include <string>

namespace ECS
{
	using EntityID = std::uint32_t;
	using EntityIdentifier = std::uint16_t;
	using EntityVersion = std::uint16_t;

	struct EntityHandle
	{
		EntityID ID;
	};

	class Entity
	{
	public:

		EntityID GetID() const
		{
			return ID;
		}

		EntityIdentifier GetIdentifier() const
		{
			return ID & 0x0000FFFF;
		}

		inline void IncrementVersion()
		{
			EntityVersion version = (ID >> 16) + 1;
			ID = (version << 16) | (ID & 0x0000FFFF);
		}

		EntityVersion GetVersion() const
		{
			return ID >> 16;
		}

		const std::string GetName() const {
			return name;
		}

		const bool operator == (const Entity& e) const
		{
			return GetIdentifier() == e.GetIdentifier();
		}
		const bool operator != (const Entity & e) const
		{
			return !(*this == e);
		}
		
	private:
		friend class Registry;

		Entity(std::string name) : name(name), ID(0) {}

		static inline EntityIdentifier GetIdentifier(EntityID ID)
		{
			return ID & 0x0000FFFF;
		}

		static inline EntityVersion GetVersion(EntityID ID)
		{
			return ID >> 16;
		}

		// Only want to be able to use this in Registry class to point to next recycled entity ID
		inline void SetIdentifier(const EntityIdentifier newIdentifier)
		{
			ID = (ID & 0xFFFF0000) | newIdentifier;
		}

		inline void SetID(const std::uint32_t newID)
		{
			ID = newID;
		}

		bool IsValid()
		{
			return ID != 0xFFFFFFFF;
		}

		EntityID ID;
		std::string name;
	};

}