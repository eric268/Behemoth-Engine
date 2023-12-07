#include "CameraHelper.h"
#include "Math/Vector.h"
#include "ECS/Components.h"
#include "Log.h"
using namespace Math;

Math::Matrix4x4 CameraHelper::LookAt(const Math::Vector3& eye, const Math::Vector3& target, const Math::Vector3& up)
{
	const Vector3 zAxis = Vector3::Normalize(eye - target);
	const Vector3 xAxis = Vector3::Normalize(Vector3::Cross(up, zAxis));
	const Vector3 yAxis = Vector3::Cross(zAxis, xAxis);

	const float tx = -Vector3::Dot(xAxis, eye);
	const float ty = -Vector3::Dot(yAxis, eye);
	const float tz = -Vector3::Dot(zAxis, eye);

	Matrix4x4 viewMatrix =
	{
		{ xAxis.x, xAxis.y, xAxis.z, 0},
		{ yAxis.x, yAxis.y, yAxis.z, 0},
		{ zAxis.x, zAxis.y, zAxis.z, 0},
		{ tx,	   ty,      tz,    1.0f}
	};

	return viewMatrix;
}