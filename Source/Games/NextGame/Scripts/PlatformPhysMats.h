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
		{ PlatformPhysMats::None, Behemoth::PhysicsMaterial(0.0f,0.0f) },
		{ PlatformPhysMats::Sand, Behemoth::PhysicsMaterial(0.05f,2.99f) },
		{ PlatformPhysMats::Grass, Behemoth::PhysicsMaterial(0.25f, 0.9f) },
		{ PlatformPhysMats::Rock, Behemoth::PhysicsMaterial(0.7f, 0.95f) }
	};
}

