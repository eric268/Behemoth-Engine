#pragma once

#include "Math/MathCore.h"
#include "SimpleSprite.h"
// stl
#include <vector>
#include <string>

namespace BehemothEngine
{
	class Primitives
	{
	public:
		Primitives();
		Primitives(const char*);

		std::vector<Math::Vector3> GetVerticies();
		const void PrintVerticies() const;

		inline void Draw()
		{
			sprite->SetPosition(100, 400);
			SetSpriteVerticies();
			sprite->Draw();
		}

		inline void SetVerticies(const std::vector<Math::Vector3>& v)
		{
			verticies = v;
			SetSpriteVerticies();
		}

		std::vector<Math::Vector3> verticies;

	private:
		void SetSpriteVerticies();
		CSimpleSprite* sprite;
	};
}

