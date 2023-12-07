#include "MeshLoader.h"
#include "Misc/Log.h"

#include <fstream>
#include <sstream>
#include <algorithm>


namespace Behemoth
{
	bool MeshLoader::LoadModel(const std::string& modelPath, std::vector<VertexData>& tData, std::vector<VertexData>& qData)
	{
		std::ifstream file(modelPath, std::ios::in);
		if (!file.is_open())
		{
			LOG_ERROR(modelPath + " not found");
			return false;
		}

		tData.clear();
		qData.clear();
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

				for (int i = 0; i < vertexIndices.size(); ++i)
				{
					VertexData vData{};
					vData.vertex = vertexPositions[vertexIndices[i] - 1];
					vData.normal = vertexNormals[normalIndices[i] - 1];
					vData.uv = vertexUVs[uvIndices[i] - 1];
					vertexIndices.size() == 3 ? tData.push_back(vData) : qData.push_back(vData);
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
