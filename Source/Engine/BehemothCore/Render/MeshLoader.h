#pragma once

#include "Mesh.h"
#include "Math/MathCore.h"

//stl
#include <string>

namespace Behemoth
{
	class MeshLoader
	{
	public:
		MeshLoader() = default;
		bool LoadModel(const std::string& filepath, std::vector<VertexData>& tData, std::vector<VertexData>& qData);

	private:
		void ParseFaceData(std::string& faceData, int& vertexIndex, int& uvIndex, int& normalIndex);
		bool IsNonPlanarQuad(const Math::Vector3& v1, const Math::Vector3& v2, const Math::Vector3& v3, const Math::Vector3& v4);

		std::vector<Math::Vector3> vertexPositions;
		std::vector<Math::Vector3> vertexNormals;
		std::vector<Math::Vector2> vertexUVs;
	};
}


