#include "Mesh.h"

#include <filesystem>
#include <iostream>
namespace Behemoth
{
	Mesh::Mesh(const std::string& modelPath, const std::string& texturePath) : 
		modelFileName(modelPath), 
		textureFileName(texturePath), 
		totalPrimitives(0)
	{}

	void Mesh::GenerateMesh(const std::vector<VertexData>& triangleData, const std::vector<VertexData>& quadData)
	{
		meshPrimitives.clear();
		totalPrimitives = triangleData.size() / 3 + quadData.size() / 4;
		meshPrimitives.resize(totalPrimitives);
		GeneratePrimitives(triangleData, PrimitiveType::TRIANGLE, 0);
		GeneratePrimitives(quadData, PrimitiveType::QUAD, triangleData.size() / 3);
	}

	void Mesh::GeneratePrimitives(const std::vector<VertexData>& data, PrimitiveType type, std::size_t offset)
	{
		std::filesystem::path full_path = std::filesystem::current_path() / "Textures" / textureFileName;
		std::string path = full_path.string();
		int numVerticies = static_cast<int>(type);

		for (int i = 0; i < data.size(); i += numVerticies)
		{
			Math::Vector3 v[4];
			Math::Vector3 n[4];
			Math::Vector2 uv[4];

			for (int j = 0; j < numVerticies; j++)
			{
				v[j].x = data[i + j].vertex.x;
				v[j].y = data[i + j].vertex.y;
				v[j].z = data[i + j].vertex.z;

				n[j] = data[i + j].normal;

				uv[j] = data[i + j].uv;
			}

			meshPrimitives[i/numVerticies + offset] = std::move(Primitives(path.c_str(), type, v, n ,uv));
/*			meshPrimitives[i / numVerticies + offset].SetPrimitiveVerticies(type, v, n, uv);*/
		}
	}
}
