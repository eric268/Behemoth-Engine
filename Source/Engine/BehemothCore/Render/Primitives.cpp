#include "Primitives.h"
#include "Misc/Log.h"

#include <random>

namespace BehemothEngine
{
	Primitives::Primitives() : 
		sprite{ new CSimpleSprite("") }
	{
		verticies[0] = Math::Vector3();
		verticies[1] = Math::Vector3();
		verticies[2] = Math::Vector3();

		RandomizeColor();
	}

	Primitives::Primitives(const char* path) : 
		sprite{ new CSimpleSprite(path) }
	{
		verticies[0] = Math::Vector3();
		verticies[1] = Math::Vector3();
		verticies[2] = Math::Vector3();

		RandomizeColor();
	}

	void Primitives::Draw()
	{
		if (!sprite)
		{
			LOG_ERROR("Null primitive attempted to be drawn");
		}
		sprite->SetPosition(400, 400);
		sprite->Draw();
	}

	void Primitives::SetPrimitiveVerticies(Math::Vector3 vert[3])
	{
		for (int i = 0; i < 3; i++)
		{
			verticies[i] = vert[i];
		}
	}

	void Primitives::SetSpriteVerticies(Math::Vector4 vert[3])
	{
		if (!sprite)
		{
			LOG_ERROR("Null sprite found");
			return;
		}

		for (int i = 0; i < 3; i++)
		{
			sprite->SetVertex(i, vert[i].x, vert[i].y);
		}
		// Set the 4th vertex to the first vertex position since CSimpleSprite takes 4 verticies
		sprite->SetVertex(3, vert[0].x, vert[0].y);

		sprite->SetColor(color.x , color.y, color.z);
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
		for (int i = 0; i < 3; i++)
		{
			std::cout << "Vertex: " << std::to_string(i) 
					  << " (X: " << std::to_string(verticies[i].x)
					  << " Y: " << std::to_string(verticies[i].y)
					  << " Z: " << std::to_string(verticies[i].z) 
					  << '\n';
		}
	}

	void Primitives::RandomizeColor()
	{
		std::random_device rd;

		// Initialize a random number generator
		std::mt19937 gen(rd());

		// Create a distribution in the range [0, 1]
		std::uniform_real_distribution<> distrib(0.0, 1.0);

		// Generate a random number
		double randomNumber1 = distrib(gen);
		double randomNumber2 = distrib(gen);
		double randomNumber3 = distrib(gen);

		color.x = randomNumber1;
		color.y = randomNumber2;
		color.z = randomNumber3;
	}
}