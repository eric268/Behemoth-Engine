#include "pch.h"
#include "MeshLoader.h"
#include "Misc/Log.h"

#include <fstream>
#include <sstream>
#include <algorithm>


namespace Behemoth
{
	bool MeshLoader::LoadModel(const std::string& modelPath, std::vector<VertexData>& data, MeshData& meshData)
	{
		std::ifstream file(modelPath, std::ios::in);
		if (!file.is_open())
		{
			LOG_ERROR(modelPath + " not found");
			return false;
		}
		std::vector<VertexData> triangle;
		std::vector<VertexData> quad;
		std::vector<std::pair<int, VertexData>> verticies;

		std::string line;

		while (std::getline(file, line))
		{
			std::istringstream iss(line);
			std::string prefix;
			iss >> prefix;

			if (prefix == "v")
			{
				Math::Vector3 vertex;
				iss >> vertex.x >> vertex.y >> vertex.z;
				vertexPositions.push_back(vertex);
			}
			else if (prefix == "vt")
			{
				Math::Vector2 uv;
				iss >> uv.x >> uv.y;
				vertexUVs.push_back(uv);
			}
			else if (prefix == "vn")
			{
				Math::Vector3 normal;
				iss >> normal.x >> normal.y >> normal.z;
				vertexNormals.push_back(normal);
			}
			
			else if (prefix == "f")
			{
				std::vector<int> vertexIndices, uvIndices, normalIndices;
				std::string vertexData;
				while (iss >> vertexData) 
				{
					int vertexIndex = 0;
					int uvIndex = 0;
					int normalIndex = 0;
					ParseFaceData(vertexData, vertexIndex, uvIndex, normalIndex);
					vertexIndices.push_back(vertexIndex);
					uvIndices.push_back(uvIndex);
					normalIndices.push_back(normalIndex);
				}

				int numVerticies = vertexIndices.size();
				bool isNonPlanarQuad = false;

				// Transform non-planar quads into triangles, leave planar quads alone
				if (numVerticies == 4 && 
					IsNonPlanarQuad(
						vertexPositions[vertexIndices[0] - 1],
						vertexPositions[vertexIndices[1] - 1],
						vertexPositions[vertexIndices[2] - 1], 
						vertexPositions[vertexIndices[3] - 1]))
				{
					numVerticies = 6;
					isNonPlanarQuad = true;
				}

				int quadIndex[6] = { 0,1,2,2,3,0 };
				for (int i = 0; i < numVerticies; i++)
				{
					int index = isNonPlanarQuad ? quadIndex[i] : i;
					VertexData vData{};
					vData.vertex = vertexPositions[vertexIndices[index] - 1];
					vData.normal = vertexNormals[normalIndices[index] - 1];
					vData.uv = vertexUVs[uvIndices[index] - 1];

					(numVerticies == 4) ? quad.push_back(vData) : triangle.push_back(vData);
				}
				
			}
		}
		meshData.triangleVertexCount = triangle.size();
		meshData.quadVertexCount = quad.size();
		meshData.totalPrimitives = triangle.size()/3 + quad.size()/4;

		data.clear();
		data.reserve(meshData.totalPrimitives);

		for (int i = 0; i < triangle.size(); i++)
		{
			data.push_back(std::move(triangle[i]));
		}
		for (int i = 0; i < quad.size(); i++)
		{
			data.push_back(std::move(quad[i]));
		}

		return true;
	}

	void MeshLoader::ParseFaceData(std::string& faceData, int& vertexIndex, int& uvIndex, int& normalIndex)
	{
		std::replace(faceData.begin(), faceData.end(), '/', ' ');
		std::istringstream iss(faceData);
		iss >> vertexIndex >> uvIndex >> normalIndex;
	}

	bool MeshLoader::IsNonPlanarQuad(const Math::Vector3& v1, const Math::Vector3& v2, const Math::Vector3& v3, const Math::Vector3& v4)
	{
		Math::Vector3 vec1 = v2 - v1;
		Math::Vector3 vec2 = v3 - v1;
		Math::Vector3 vec3 = v4 - v1;

		Math::Vector3 normal = Math::Vector3::Cross(vec1, vec2);
		float dotProduct = Math::Vector3::Dot(normal, vec3);
		return std::fabs(dotProduct) > 1e-5;
	}
}