#include "pch.h"
#include "Collision.h"
#include "Math/MathCore.h"
#include "Colliders.h"
#include "ECS/ECSCore.h"

namespace Behemoth::Collision
{
	bool CheckAABBCollision(const AABBCollider& box1, const AABBCollider& box2)
	{
		Math::Vector3 minPos1 = box1.pos - box1.halfwidthExtents;
		Math::Vector3 maxPos1 = box1.pos + box1.halfwidthExtents;

		Math::Vector3 minPos2 = box2.pos - box2.halfwidthExtents;
		Math::Vector3 maxPos2 = box2.pos + box2.halfwidthExtents;


		if (maxPos1.x < minPos2.x || minPos1.x > maxPos2.x)
			return false;
		if (maxPos1.y < minPos2.y || minPos1.y > maxPos2.y)
			return false;
		if (maxPos1.z < minPos2.z || minPos1.z > maxPos2.z)
			return false;

		return true;
	}

	bool CheckSphereSphereCollision(const SphereCollider& sphere1, const SphereCollider& sphere2)
	{
		float distance = Math::Vector3::SquaredDistance(sphere1.pos, sphere2.pos);
		float rad = sphere1.radius + sphere2.radius;
		return distance <= (rad * rad);
	}

	// Real-Time Collision Detection-Morgan Kaufmann (2005)
	bool CheckOBBCollision(const OBBCollider& box1, const OBBCollider& box2)
	{
		float rBox1, rBox2;

		Math::Matrix3x3 R, AbsR;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				R.data[i][j] = Math::Vector3::Dot(box1.orientation[i], box2.orientation[j]);
			}
		}

		Math::Vector3 t = box2.pos - box1.pos;
		t = Math::Vector3(Math::Vector3::Dot(t, box1.orientation[0]), Math::Vector3::Dot(t, box1.orientation[1]), Math::Vector3::Dot(t, box1.orientation[2]));

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				AbsR.data[i][j] = std::abs(R.data[i][j]) + EPSILON;
			}
		}

		for (int i = 0; i < 3; i++)
		{
			rBox1 = box1.halfwidthExtents[i];
			rBox2 = box2.halfwidthExtents[0] * AbsR.data[i][0] + box2.halfwidthExtents[1] * AbsR.data[i][1] + box2.halfwidthExtents[2] * AbsR.data[i][2];
			if (std::abs(t[i]) > (rBox1 + rBox2))
			{
				return false;
			}
		}

		for (int i = 0; i < 3; i++)
		{
			rBox1 = box1.halfwidthExtents[0] * AbsR.data[0][i] + box1.halfwidthExtents[1] * AbsR.data[1][i] + box1.halfwidthExtents[2] * AbsR.data[2][i];
			rBox2 = box2.halfwidthExtents[i];
			if (std::abs(t[0] * R.data[0][i] + t[1] * R.data[1][i] + t[2] * R.data[2][i]) > (rBox1 + rBox2))
			{
				return false;
			}
		}

		rBox1 = box1.halfwidthExtents[1] * AbsR.data[2][0] + box1.halfwidthExtents[2] * AbsR.data[1][0];
		rBox2 = box2.halfwidthExtents[1] * AbsR.data[0][2] + box2.halfwidthExtents[2] * AbsR.data[0][1];
		if (std::abs(t[2] + R.data[1][0] - t[1] * R.data[2][0] > rBox1 + rBox2))
		{
			return false;
		}

		rBox1 = box1.halfwidthExtents[1] * AbsR.data[2][1] + box1.halfwidthExtents[2] * AbsR.data[1][1];
		rBox2 = box2.halfwidthExtents[0] * AbsR.data[0][2] + box2.halfwidthExtents[2] * AbsR.data[0][0];
		if (std::abs(t[2] + R.data[1][1] - t[1] * R.data[2][1] > rBox1 + rBox2))
		{
			return false;
		}

		rBox1 = box1.halfwidthExtents[1] * AbsR.data[2][2] + box1.halfwidthExtents[2] * AbsR.data[1][2];
		rBox2 = box2.halfwidthExtents[0] * AbsR.data[0][1] + box2.halfwidthExtents[1] * AbsR.data[0][0];
		if (std::abs(t[2] + R.data[1][2] - t[1] * R.data[2][2] > rBox1 + rBox2))
		{
			return false;
		}

		rBox1 = box1.halfwidthExtents[0] * AbsR.data[2][0] + box1.halfwidthExtents[2] * AbsR.data[0][0];
		rBox2 = box2.halfwidthExtents[1] * AbsR.data[1][2] + box2.halfwidthExtents[2] * AbsR.data[1][1];
		if (std::abs(t[0] + R.data[2][0] - t[2] * R.data[0][0] > rBox1 + rBox2))
		{
			return false;
		}

		rBox1 = box1.halfwidthExtents[0] * AbsR.data[2][1] + box1.halfwidthExtents[2] * AbsR.data[0][1];
		rBox2 = box2.halfwidthExtents[0] * AbsR.data[1][2] + box2.halfwidthExtents[2] * AbsR.data[1][0];
		if (std::abs(t[0] + R.data[2][1] - t[2] * R.data[0][1] > rBox1 + rBox2))
		{
			return false;
		}

		rBox1 = box1.halfwidthExtents[0] * AbsR.data[2][2] + box1.halfwidthExtents[2] * AbsR.data[0][2];
		rBox2 = box2.halfwidthExtents[0] * AbsR.data[1][1] + box2.halfwidthExtents[1] * AbsR.data[1][0];
		if (std::abs(t[0] + R.data[2][2] - t[2] * R.data[0][2] > rBox1 + rBox2))
		{
			return false;
		}

		rBox1 = box1.halfwidthExtents[0] * AbsR.data[1][0] + box1.halfwidthExtents[1] * AbsR.data[0][0];
		rBox2 = box2.halfwidthExtents[1] * AbsR.data[2][2] + box2.halfwidthExtents[2] * AbsR.data[2][1];
		if (std::abs(t[1] + R.data[0][0] - t[0] * R.data[1][0] > rBox1 + rBox2))
		{
			return false;
		}

		rBox1 = box1.halfwidthExtents[0] * AbsR.data[1][1] + box1.halfwidthExtents[1] * AbsR.data[0][1];
		rBox2 = box2.halfwidthExtents[0] * AbsR.data[2][2] + box2.halfwidthExtents[2] * AbsR.data[2][0];
		if (std::abs(t[1] + R.data[0][1] - t[0] * R.data[1][1] > rBox1 + rBox2))
		{
			return false;
		}

		rBox1 = box1.halfwidthExtents[0] * AbsR.data[1][2] + box1.halfwidthExtents[1] * AbsR.data[0][2];
		rBox2 = box2.halfwidthExtents[0] * AbsR.data[2][1] + box2.halfwidthExtents[1] * AbsR.data[2][0];
		if (std::abs(t[1] + R.data[0][2] - t[0] * R.data[1][2] > rBox1 + rBox2))
		{
			return false;
		}

		return true;
	}

	bool CheckSphereAABBCollision(const SphereCollider& sphere, const AABBCollider& box)
	{
		float squaredDist = 0;
		float squaredRad = sphere.radius * sphere.radius;

		for (int axis = 0; axis < 3; axis++)
		{
			float diff = 0;
			float minVal = box.pos[axis] - box.halfwidthExtents[axis];
			float maxVal = box.pos[axis] + box.halfwidthExtents[axis];

			float spherePos = sphere.pos[axis];

			if (spherePos < minVal)
			{
				diff = minVal - spherePos;
			}
			else if (spherePos > maxVal)
			{
				diff = spherePos - maxVal;
			}

			squaredDist += diff * diff;
		}

		return squaredDist <= squaredRad;
	}
}