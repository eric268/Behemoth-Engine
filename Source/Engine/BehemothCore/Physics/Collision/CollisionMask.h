#pragma once
#include <cstddef>

namespace Behemoth::BMask
{
	enum CollisionLayer : std::uint16_t
	{
		NoCollision			= 0x0000,
		Terrain				= 0x0001,
		Player				= 0x0002,
		EnvObject			= 0x0004,
		Character			= 0x0008,
		BVHCollider			= 0x0010,
		Raycast				= 0x0020,
		Everything			= 0xFFFF & ~BVHCollider
	};

	enum CollisionType : std::uint16_t
	{
		IgnoreAllCollision	= NoCollision,
		BVHComponent		= BVHCollider,
		StaticType			= Terrain | Player | Character | EnvObject | Raycast,
		DynamicType			= StaticType,
		ChildMesh			= Raycast,
		IgnoreRaycast		= StaticType & ~Raycast,
		RaycastOnly			= Raycast,
		AllCollision		= Everything
	};
}