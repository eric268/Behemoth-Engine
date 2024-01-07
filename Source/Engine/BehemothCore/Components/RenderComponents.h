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
		MeshComponent(const std::string& modelName, const std::string& textureName, const Math::Vector2 uvScale = { 1.0f,1.0f }, bool visible = true) :
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
		WireframeComponent(const std::string& modelName, Math::Vector3 scale = Math::Vector3::One(), bool visible = false, Math::Vector3 color = Math::Vector3(1.0f, 1.0f, 1.0f)) : modelFileName(modelName), scale(scale), isVisible(visible), mesh(modelName), wireframeColor(color) {}
		std::string modelFileName;
		Math::Vector3 scale;
		bool isVisible;
		Math::Vector3 wireframeColor;
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
		using Point = Math::Vector3;

		DebugLineComponent(Point start, Point end, float displayTime, float displayCounter = 0.0f,  Math::Vector3 c = Math::Vector3(0.0f, 1.0f, 0.0f)) : 
			startPoint(start), 
			endPoint(end), 
			lifetime(displayTime), 
			displayCounter(displayCounter), 
			color(c) 
		{}

		float lifetime;
		float displayCounter;
		Point startPoint;
		Point endPoint;
		Math::Vector3 color;
	};
}