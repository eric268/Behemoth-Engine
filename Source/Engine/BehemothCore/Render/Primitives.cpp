#include "Primitives.h"
#include "Misc/Log.h"

#include <random>

namespace Behemoth
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
		sprite{ new CSimpleSprite(path)}
	{
		verticies[0] = Math::Vector3();
		verticies[1] = Math::Vector3();
		verticies[2] = Math::Vector3();
	}

	void Primitives::Draw()
	{
		if (!sprite)
		{
			LOG_ERROR("Null primitive attempted to be drawn");
		}


		sprite->SetColor(1.0f, 1.0f, 1.0f);
		sprite->Draw();
	}

	void Primitives::DrawWireMesh()
	{
		App::DrawLine(sprite->GetVertexX(0), sprite->GetVertexY(0), sprite->GetVertexX(1), sprite->GetVertexY(1));
		App::DrawLine(sprite->GetVertexX(1), sprite->GetVertexY(1), sprite->GetVertexX(2), sprite->GetVertexY(2));
		App::DrawLine(sprite->GetVertexX(2), sprite->GetVertexY(2), sprite->GetVertexX(0), sprite->GetVertexY(0));
	}

	void Primitives::SetPrimitiveVerticies(PrimitiveType type, Math::Vector3 vert[4], Math::Vector3 normal[4], Math::Vector2 uv[4])
	{
		primitiveType = type;
		for (int i = 0; i < static_cast<int>(type); i++)
		{
			verticies[i] = vert[i];
			normals[i] = normal[i];
			this->uv[i] = uv[i];
		}
	}

	void Primitives::SetSpriteVerticies(PrimitiveType type, Math::Vector4 vert[], Math::Vector2 uv[])
	{
		if (!sprite)
		{
			LOG_ERROR("Null sprite found");
			return;
		}

		for (int i = 0; i < static_cast<int>(type); i++)
		{
			sprite->SetVertex(i, vert[i].x , vert[i].y);
			sprite->SetUV(i, uv[i].x, uv[i].y);
		}

		if (type == TRIANGLE)
		{
			sprite->SetVertex(3, vert[0].x, vert[0].y);
			sprite->SetUV(3, uv[0].x, uv[0].y);
		}
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