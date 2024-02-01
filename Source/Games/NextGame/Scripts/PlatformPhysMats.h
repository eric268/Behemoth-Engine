#pragma once

#include "Physics/Collision/PhysicsMaterial.h"

#include <unordered_map>

namespace PhysMat
{
	enum PlatformPhysMats
	{
		None,
		Sand,
		Grass,
		Rock
	};

	static std::unordered_map<PlatformPhysMats, Behemoth::PhysicsMaterial> platformPhysicsMats =
	{
		{ None, Behemoth::PhysicsMaterial(0.0f,0.0f) },
		{ Sand, Behemoth::PhysicsMaterial(0.01f, 10.0f) },
		{ Grass, Behemoth::PhysicsMaterial(0.25f, 3.0f) },
		{ Rock, Behemoth::PhysicsMaterial(0.7f, 10.0f) }
	};
}

