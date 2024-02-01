#pragma once

namespace Behemoth
{
	struct PhysicsMaterial
	{
		PhysicsMaterial() : restitution(0.0f), dampening(0.0f) {}
		PhysicsMaterial(float restitution, float dampening) : restitution(restitution), dampening(dampening) {}

		float restitution;
		float dampening;

		static PhysicsMaterial None() { return PhysicsMaterial(0.0f, 0.0f); }

		bool operator == (PhysicsMaterial& obj)
		{
			return restitution == obj.restitution && dampening == obj.dampening;
		}

		bool operator != (PhysicsMaterial& obj)
		{
			return !(*this == obj);
		}
	};
}