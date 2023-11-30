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
			std::vector<Math::Vector3> v(3);
			
			for (int j = 0; j < 3; j++)
			{
				// Using a temp scale of 50 pixels to be able to see shapes

				v[j].x = data[i + j].vertex.x * 50.0f;
				v[j].y = data[i + j].vertex.y * 50.0f;
				v[j].z = data[i + j].vertex.z * 50.0f;
			}
			Primitives p{};
			p.SetVerticies(v);
			meshPrimitives.push_back(p);
		}
	}
}
