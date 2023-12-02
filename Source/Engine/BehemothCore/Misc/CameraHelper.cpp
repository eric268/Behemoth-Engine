#include "CameraHelper.h"
#include "Math/Vector.h"
#include "ECS/Components.h"
#include "Log.h"
using namespace Math;

Math::Matrix4x4 CameraHelper::LookAt(const Math::Vector3& eye, const Math::Vector3& target, const Math::Vector3& up)
{
	const Vector3 zAxis = Vector3::Normalize(target - eye);
	const Vector3 xAxis = Vector3::Normalize(Vector3::Cross(zAxis, up));
	const Vector3 yAxis = Vector3::Cross(xAxis, zAxis);

	Matrix4x4 viewMatrix =
	{
		{ xAxis.x, xAxis.y, xAxis.z, 0 },
		{ yAxis.x, yAxis.y, yAxis.z, 0 },
		{ zAxis.x, zAxis.y, zAxis.z, 0 },
		{ -Vector3::Dot(xAxis, eye * WORLD_SCALE), -Vector3::Dot(yAxis, eye * WORLD_SCALE), -Vector3::Dot(zAxis, eye * WORLD_SCALE), 1}
	};

	return viewMatrix;
}

Math::Vector3 CameraHelper::GetPosition(const ECS::CameraComponent* component)
{
	if (!component)
	{
		LOG_ERROR("Null camera component found");
		return Vector3{};
	}
	return Vector3(component->viewMatrix[3][0], component->viewMatrix[3][1], component->viewMatrix[3][2]);
}