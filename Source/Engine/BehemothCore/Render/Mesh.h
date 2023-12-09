#pragma once

#include "Math/MathCore.h"
#include "Render/Primitives.h"

//stl
#include <string>
#include <vector>

namespace Behemoth
{
	struct VertexData
	{
		Math::Vector3 vertex;
		Math::Vector3 normal;
		Math::Vector2 uv;
	};

	struct MeshData
	{
		std::vector<VertexData> triangleData;
		std::vector<VertexData> quadData;
	};

	class Mesh
	{
	public:
		Mesh(const std::string& modelPath, const std::string& texturePath = "");
		std::string modelFileName;
		std::string textureFileName;
		std::vector<Primitives> meshPrimitives;
		void GenerateMesh(const std::vector<VertexData>& triangleData, const std::vector<VertexData>& quadData);

	private:
		void GeneratePrimitives(const std::vector<VertexData>& data, PrimitiveType type, std::size_t offset);
	};
}