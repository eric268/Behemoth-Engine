#pragma once

#include "Math/MathCore.h"
#include "SimpleSprite.h"
#include "app.h"
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

		void Draw();
		void DrawWireMesh();

		Math::Vector3 verticies[3];
		Math::Vector3 normals[3];

		void SetSpriteVerticies(Math::Vector4 vert[3]);
		void SetPrimitiveVerticies(Math::Vector3 vert[3], Math::Vector3 normal[3]);

	private:
		CSimpleSprite* sprite;
		Math::Vector3 color;

		void RandomizeColor();
	};
}

