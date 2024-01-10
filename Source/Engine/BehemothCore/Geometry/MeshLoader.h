#pragma once

#include "Mesh.h"
#include "Math/MathCore.h"

//stl
#include <string>

namespace Behemoth
{
	class AABBCollider;

	class MeshLoader
	{
	public:
		MeshLoader() = default;
		bool LoadModel(const std::string& filepath, std::vector<VertexData>& data, MeshData& meshData);

	private:
		void ParseFaceData(std::string& faceData, int& vertexIndex, int& uvIndex, int& normalIndex);
		bool IsNonPlanarQuad(const BMath::Vector3& v1, const BMath::Vector3& v2, const BMath::Vector3& v3, const BMath::Vector3& v4);

		std::vector<BMath::Vector3> vertexPositions;
		std::vector<BMath::Vector3> vertexNormals;
		std::vector<BMath::Vector2> vertexUVs;
	};
}


