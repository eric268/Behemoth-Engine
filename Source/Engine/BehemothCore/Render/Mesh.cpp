#include "Mesh.h"

#include "filesystem"

namespace BehemothEngine
{
	Mesh::Mesh(const std::string& modelPath, const std::string& texturePath) : modelFileName(modelPath), textureFileName(texturePath)
	{

	}

	void Mesh::GeneratePrimitives(const std::vector<VertexData>& data)
	{
		std::filesystem::path full_path = std::filesystem::current_path() / "Textures" / textureFileName;
		std::string path = full_path.string();

		for (int i = 0; i < data.size(); i += 3)
		{
			Math::Vector3 v[3];
			Math::Vector3 n[3];
			Math::Vector2 uv[3];
			
			for (int j = 0; j < 3; j++)
			{
				v[j].x = data[i + j].vertex.x * WORLD_SCALE;
				v[j].y = data[i + j].vertex.y * WORLD_SCALE;
				v[j].z = data[i + j].vertex.z * WORLD_SCALE;

				n[j] = data[i + j].normal;

				uv[j] = data[i + j].uv;
			}

			Primitives p(path.c_str());
			p.SetPrimitiveVerticies(v, n, uv);
			meshPrimitives.push_back(p);
		}
	}
}
