#pragma once

#include "ECS/Component.h"
#include "Input/InputCodes.h"
#include "Renderer/Mesh.h"

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
}