#pragma once

#include "Math/MathCore.h"
#include "Render/Primitives.h"

//stl
#include <string>
#include <vector>

namespace BehemothEngine
{
	struct VertexData
	{
		Math::Vector3 vertex;
		Math::Vector3 normal;
		Math::Vector2 uv;
	};

	struct MeshData
	{
		std::vector<VertexData> meshData;
	};

	class Mesh
	{
	public:
		Mesh() = default;
		Mesh(const std::string& path);
		std::string filePath;
		std::vector<Primitives> meshPrimitives;
		void GeneratePrimitives(const std::vector<VertexData>& data);
	};
}