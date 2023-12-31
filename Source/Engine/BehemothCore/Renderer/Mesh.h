#pragma once

#include "Math/MathCore.h"
#include "Renderer/Primitives.h"

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
		std::string modelFileName;
		std::uint32_t totalPrimitives;
		std::uint32_t triangleVertexCount;
		std::uint32_t quadVertexCount;
	};

	struct MaterialData
	{
		std::string textureFileName;
		Math::Vector2 uvScale;
		Math::Vector3 diffuse;
		Math::Vector3 specular;
		float shininess;
	};

	class Mesh
	{
	public:
		Mesh(const std::string& modelPath, const std::string& texturePath = "", const Math::Vector2 uv = {1.0f, 1.0f});
		void GenerateMesh(const MeshData& meshData, const std::vector<VertexData>& vertexData);

		MaterialData materialData;
		MeshData meshData;
		std::vector<Primitives> meshPrimitives;
		
	private:
		void GeneratePrimitives(const std::vector<VertexData>& vertexData, PrimitiveType type, int totalVerticies, int vertexOffset);
	};
}