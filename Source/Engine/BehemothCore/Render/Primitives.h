#pragma once

#include "Math/MathCore.h"
#include "SimpleSprite.h"
#include "app.h"
// stl
#include <vector>
#include <string>

namespace Behemoth
{
	enum PrimitiveType
	{
		TRIANGLE = 3,
		QUAD = 4,
		NUM_PRIMITIVE_TYPES

	};

	class Primitives
	{
	public:
		Primitives();
		Primitives(const char*);

		std::vector<Math::Vector3> GetVerticies();
		const void PrintVerticies() const;

		void Draw();
		void DrawWireMesh();

		Math::Vector3 verticies[4];
		Math::Vector3 normals[4];
		Math::Vector2 uv[4];

		PrimitiveType primitiveType;

		void SetSpriteVerticies(PrimitiveType type, Math::Vector4 vert[], Math::Vector2 uv[]);
		void SetPrimitiveVerticies(PrimitiveType type, Math::Vector3 vert[], Math::Vector3 normal[], Math::Vector2 uv[]);
		
	private:
		CSimpleSprite* sprite;
		Math::Vector3 color;

		void RandomizeColor();
	};
}

