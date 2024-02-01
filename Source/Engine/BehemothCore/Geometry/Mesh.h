#pragma once

#include "Math/MathCore.h"
#include "Geometry/Primitives.h"
#include "VertexData.h"

#include <string>
#include <vector>
#include <mutex>

namespace Behemoth
{
	struct MeshData
	{
		MeshData() : modelFileName(), totalPrimitives(0), triangleVertexCount(0), quadVertexCount(0),maxVertexDistance(0.0f) {}
		std::string  modelFileName;
		std::uint32_t totalPrimitives;
		std::uint32_t triangleVertexCount;
		std::uint32_t quadVertexCount;
		float maxVertexDistance;
	};

	struct MaterialData
	{
		MaterialData() :textureFileName(), uvScale(), diffuse(), specular(), shininess(0.0f) {}

		std::string    textureFileName;
		BMath::Vector2 uvScale;
		BMath::Vector3 diffuse;
		BMath::Vector3 specular;
		float shininess;
	};

	class Mesh
	{
	public:
		Mesh();
		Mesh(const std::string& modelPath, const std::string& texturePath = "", const BMath::Vector2 uv = BMath::Vector2(1.0f, 1.0f), bool affectedByLighting = true);
		void GenerateMesh(const MeshData& meshData, const std::vector<VertexData>& vertexData);

		bool affectedByLighting;
		std::vector<Primitive> meshPrimitives;
		MeshData meshData;
		MaterialData materialData;

	private:
		void GeneratePrimitives(const std::vector<VertexData>& vertexData, PrimitiveType type, int totalVertices, int vertexOffset);
	};
}