#include "pch.h"
#include "Collision.h"
#include "Components/Components.h"
#include "ECS/ECSCore.h"

namespace Behemoth
{
	bool CheckAABBCollision(const Math::Vector3& box1Min, const Math::Vector3& box1Max, const Math::Vector3& box2Min, const Math::Vector3& box2Max)
	{
		if (box1Max.x < box2Min.x || box1Min.x > box2Max.x)
			return false;
		if (box1Max.y < box2Min.y || box1Min.y > box2Max.y)
			return false;
		if (box1Max.z < box2Min.z || box1Min.z > box2Max.z)
			return false;

		return true;
	}

	bool CheckSphereSphereCollision(const Math::Vector3 sphere1Pos, const float sphere1Radius, const Math::Vector3 sphere2Pos, const float sphere2Radius)
	{
		float distance = Math::Vector3::SquaredDistance(sphere1Pos, sphere2Pos);
		float rad = sphere1Radius + sphere2Radius;
		return distance <= (rad * rad);
	}

	// Real-Time Collision Detection-Morgan Kaufmann (2005)
	bool CheckOBBCollision(const OBB& box1, const OBB& box2)
	{
		float rBox1, rBox2;

		Math::Matrix3x3 R, AbsR;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				R.data[i][j] = Math::Vector3::Dot(box1.rotVecs[i], box2.rotVecs[j]);
			}
		}

		Math::Vector3 t = box2.point - box1.point;
		t = Math::Vector3(Math::Vector3::Dot(t, box1.rotVecs[0]), Math::Vector3::Dot(t, box1.rotVecs[1]), Math::Vector3::Dot(t, box1.rotVecs[2]));

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
}