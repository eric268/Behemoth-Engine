#pragma once

#include "ECS/Component.h"
#include "Input/InputCodes.h"
#include "Geometry/Mesh.h"

#include <functional>
#include <unordered_map>

namespace Behemoth
{
	struct MeshComponent : public ECS::Component
	{
		MeshComponent(const std::string& modelName, const std::string& textureName, const BMath::Vector2 uvScale = { 1.0f,1.0f }, bool visible = true) :
			modelFileName(modelName),
			textureFileName(textureName),
			mesh(modelName, textureName, uvScale),
			isVisible(visible)
		{}

		bool isVisible;

		Mesh mesh;
		std::string modelFileName;
		std::string textureFileName;
	};

	struct WireframeComponent : public ECS::Component
	{
		WireframeComponent(const std::string& modelName, BMath::Vector3 scale = BMath::Vector3::One(), bool visible = false, BMath::Vector3 color = BMath::Vector3(1.0f, 1.0f, 1.0f)) : modelFileName(modelName), scale(scale), isVisible(visible), mesh(modelName), wireframeColor(color) {}
		std::string modelFileName;
		BMath::Vector3 scale;
		bool isVisible;
		BMath::Vector3 wireframeColor;
		Mesh mesh;
	};

	struct BoundingVolumeComponent : public ECS::Component
	{
		BoundingVolumeComponent() : mesh("sphere.obj"), volumeRadius(1.0f), isVisible(false) {}
		BoundingVolumeComponent(float radius, bool visible) : mesh("sphere.obj"), volumeRadius(radius), isVisible(visible) {}
		float volumeRadius;
		bool isVisible;
		Mesh mesh;
	};

	struct DebugLineComponent : public ECS::Component
	{
		using Point = BMath::Vector3;

		DebugLineComponent() :
			startPoint(BMath::Vector3()),
			endPoint(BMath::Vector3()),
			lifetime(2.0f),
			displayCounter(0.0f),
			color(BMath::Vector3(0.0f, 1.0f, 0.0f))
		{}

		DebugLineComponent(Point start, Point end, float displayTime, BMath::Vector3 c = BMath::Vector3(0.0f, 1.0f, 0.0f)) :
			startPoint(start), 
			endPoint(end), 
			lifetime(displayTime), 
			displayCounter(0.0f), 
			color(c) 
		{}

		float lifetime;
		float displayCounter;
		Point startPoint;
		Point endPoint;
		BMath::Vector3 color;
	};
}