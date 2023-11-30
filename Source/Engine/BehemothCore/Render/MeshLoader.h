#pragma once

#include "Mesh.h"
#include "Math/MathCore.h"
//stl
#include <string>

namespace BehemothEngine
{
	class MeshLoader
	{
	public:
		MeshLoader() = default;
		bool LoadModel(const std::string& filepath, std::vector<VertexData>& meshContainer);

	private:
		void ParseFaceData(std::string& faceData, int& vertexIndex, int& uvIndex, int& normalIndex);

		std::vector<Math::Vector3> vertexPositions;
		std::vector<Math::Vector3> vertexNormals;
		std::vector<Math::Vector2> vertexUVs;
	};
}


