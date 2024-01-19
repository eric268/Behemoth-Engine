#pragma once
#include "Vector.h"

namespace BMath	
{
	struct BRotation
	{
		BRotation() : pitch(0.0f), yaw(0.0f), roll(0.0f) {}
		BRotation(float p, float y, float r) : pitch(p), yaw(y), roll(r) {}
		explicit BRotation(Vector3 vec) : pitch(vec.x), yaw(vec.y), roll(vec.z) {}

		inline Vector3 ToVec() { return BMath::Vector3(pitch, yaw, roll); }

		float pitch;
		float yaw;
		float roll;
	};
}