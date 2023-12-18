#pragma once

// stl
#include <string>

namespace ECS
{
	class Entity
	{
	public:

		inline std::uint32_t GetID() const
		{
			return ID;
		}

		inline std::uint16_t GetIdentifier() const
		{
			return ID & 0x0000FFFF;
		}

		inline void IncrementVersion()
		{
			std::uint16_t version = (ID >> 16) + 1;
			ID = (version << 16) | (ID & 0x0000FFFF);
		}

		std::uint16_t GetVersion() const
		{
			return ID >> 16;
		}

		const std::string GetName() const {
			return name;
		}
		
	private:
		friend class Registry;

		Entity(std::string name) : name(name), ID(0) {}

		// Only want to be able to use this in Registry class to point to next recycled entity ID
		inline void SetIdentifier(const std::uint16_t newIdentifier)
		{
			ID = (ID & 0xFFFF0000) | newIdentifier;
		}

		inline void SetID(const std::uint32_t newID)
		{
			ID = newID;
		}

		std::uint32_t ID;
		std::string name;
	};

}