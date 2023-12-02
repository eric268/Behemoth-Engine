#include "Mesh.h"

namespace BehemothEngine
{
	Mesh::Mesh(const std::string& path) : filePath(path)
	{

	}

	void Mesh::GeneratePrimitives(const std::vector<VertexData>& data)
	{
		for (int i = 0; i < data.size(); i += 3)
		{
			Math::Vector3 v[3];
			Math::Vector3 n[3];
			
			for (int j = 0; j < 3; j++)
			{
				v[j].x = data[i + j].vertex.x * WORLD_SCALE;
				v[j].y = data[i + j].vertex.y * WORLD_SCALE;
				v[j].z = data[i + j].vertex.z * WORLD_SCALE;

				n[j] = data[i + j].normal;
			}
			Primitives p{};
			p.SetPrimitiveVerticies(v, n);
			meshPrimitives.push_back(p);
		}
	}
}
