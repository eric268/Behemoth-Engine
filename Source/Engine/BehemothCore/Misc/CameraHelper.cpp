#include "CameraHelper.h"

using namespace Math;

Math::Matrix4x4 CameraHelper::LookAt(const Math::Vector3& eye, const Math::Vector3& target, const Math::Vector3& up)
{
	Vector3 zAxis = Vector3::Normalize(eye - target);
	Vector3 xAxis = Vector3::Normalize(Vector3::Cross(up, zAxis));
	Vector3 yAxis = Vector3::Cross(zAxis, xAxis);

	Matrix4x4 viewMatrix;

	viewMatrix[0][0] = xAxis.x;
	viewMatrix[1][0] = xAxis.y;
	viewMatrix[2][0] = xAxis.z;
	viewMatrix[3][0] = -Vector3::Dot(xAxis, eye);

	viewMatrix[0][1] = yAxis.x;
	viewMatrix[1][1] = yAxis.y;
	viewMatrix[2][1] = yAxis.z;
	viewMatrix[3][1] = -Vector3::Dot(yAxis, eye);

	viewMatrix[0][2] = zAxis.x;
	viewMatrix[1][2] = zAxis.y;
	viewMatrix[2][2] = zAxis.z;
	viewMatrix[3][2] = -Vector3::Dot(zAxis, eye);

	viewMatrix[0][3] = 0;
	viewMatrix[1][3] = 0;
	viewMatrix[2][3] = 0;
	viewMatrix[3][3] = 1;

	return viewMatrix;
}