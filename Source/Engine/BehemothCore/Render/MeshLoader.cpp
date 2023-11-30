#include "MeshLoader.h"
#include "Misc/Log.h"

#include <fstream>
#include <sstream>
#include <algorithm>


namespace BehemothEngine
{
	bool MeshLoader::LoadModel(const std::string& filepath, std::vector<VertexData>& meshContainer)
	{
		std::ifstream file(filepath, std::ios::in);
		if (!file.is_open())
		{
			LOG_ERROR(filepath + " not found");
			return false;
		}

		meshContainer.clear();
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
					int vIndex = 0;
					int uvIndex = 0;
					int nIndex = 0;
					ParseFaceData(vertexData, vIndex, uvIndex, nIndex);
					vertexIndices.push_back(vIndex);
					uvIndices.push_back(uvIndex);
					normalIndices.push_back(nIndex);
				}

				// Handle triangular faces
				if (vertexIndices.size() == 3) 
				{
					for (int i = 0; i < 3; ++i) 
					{
						VertexData vData{};
						vData.vertex = vertexPositions[vertexIndices[i] - 1];
						vData.normal = vertexNormals[normalIndices[i] - 1];
						vData.uv = vertexUVs[uvIndices[i] - 1];
						meshContainer.push_back(vData);
					}
				}
				// Handle quadrilateral faces by triangulating
				else if (vertexIndices.size() == 4) 
				{
					int quadIndices[6] = { 0, 1, 2, 2, 3, 0 }; // Indices to form two triangles from a quad
					for (int i = 0; i < 6; ++i) 
					{
						int idx = quadIndices[i];
						VertexData vData{};
						vData.vertex = vertexPositions[vertexIndices[idx] - 1];
						vData.normal = vertexNormals[normalIndices[idx] - 1];
						vData.uv = vertexUVs[uvIndices[idx] - 1];
						meshContainer.push_back(vData);
					}
				}
				else
				{
					LOG_ERROR("Unable to handle number of verticies per face");
					return false;
				}
			}
		}

		return true;
	}

	void MeshLoader::ParseFaceData(std::string& faceData, int& vertexIndex, int& uvIndex, int& normalIndex)
	{
		std::replace(faceData.begin(), faceData.end(), '/', ' ');
		std::istringstream iss(faceData);
		iss >> vertexIndex >> uvIndex >> normalIndex;
	}
}
