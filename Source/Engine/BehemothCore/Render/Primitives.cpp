#include "Primitives.h"
#include "Misc/Log.h"

namespace BehemothEngine
{
	Primitives::Primitives() : 
		sprite{ new CSimpleSprite("") },
		verticies(std::vector<Math::Vector3>(3)) 
	{}

	Primitives::Primitives(const char* path) : 
		sprite{ new CSimpleSprite(path) },
		verticies(std::vector<Math::Vector3>(3)) 
	{}

	void Primitives::SetSpriteVerticies()
	{
		if (!sprite)
		{
			LOG_ERROR("Null sprite found");
			return;
		}

		for (int i = 0; i < verticies.size(); i++)
		{
			sprite->SetVertex(i, verticies[i].x, verticies[i].y);
		}
		// Set the 4th vertex to the first vertex position since CSimpleSprite takes 4 verticies
		sprite->SetVertex(3, verticies[0].x, verticies[0].y);

		sprite->SetColor(1, 1, 1);
	}

	std::vector<Math::Vector3> Primitives::GetVerticies()
	{
		std::vector<Math::Vector3> vert(3);
		for (int i = 0; i < vert.size(); i++)
		{
			vert[i].x = sprite->GetVertexX(i);
			vert[i].y = sprite->GetVertexY(i);
		}
		return vert;
	}

	const void Primitives::PrintVerticies() const
	{
		for (int i = 0; i < verticies.size(); i++)
		{
			std::cout << "Vertex: " << std::to_string(i) 
					  << " (X: " << std::to_string(verticies[i].x)
					  << " Y: " << std::to_string(verticies[i].y)
					  << " Z: " << std::to_string(verticies[i].z) 
					  << '\n';
		}
	}
}