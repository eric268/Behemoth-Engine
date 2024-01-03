#pragma once

namespace Behemoth::Collision
{
	enum CollisionMask
	{
		NoCollision = 0x0000,
		Terrain		= 0x0001,
		Player		= 0x0002,
		EnvObject	= 0x0004,
		Character   = 0x0008,

		Everything	= 0xFFFF
	};
}