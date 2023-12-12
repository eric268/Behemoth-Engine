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
		Primitives(const char* path, PrimitiveType type, Math::Vector3 verticies[], Math::Vector3 normals[], Math::Vector2 uv[]);
		~Primitives();

		Primitives(const Primitives& obj);
		Primitives(Primitives&& obj) noexcept;

		Primitives& operator=(Primitives&& obj) noexcept;
 		Primitives& operator=(const Primitives& obj);


		std::vector<Math::Vector3> GetVerticies();
		const void PrintVerticies() const;

		void Draw();
		void DrawWireMesh(const PrimitiveType type);

		Math::Vector3 verticies[4];
		Math::Vector3 normals[4];
		Math::Vector2 uv[4];

		PrimitiveType primitiveType;

		double depth;

		void SetSpriteUVs(PrimitiveType type, Math::Vector2 uv[]);
		void SetSpriteVerticies(PrimitiveType type, Math::Vector4 vert[]);
		void SetSpriteVerticies(PrimitiveType type, Math::Vector4 vert[], Math::Vector2 uv[]);
		void SetPrimitiveVerticies(PrimitiveType type, Math::Vector3 vert[], Math::Vector3 normal[], Math::Vector2 uv[]);
		
	private:

		inline void CopyVertexData(const Math::Vector3* verticies, const Math::Vector3* normals, const Math::Vector2* uv) 
		{
			std::memcpy(this->verticies, verticies, sizeof(Math::Vector3) * 4);
			std::memcpy(this->normals, normals, sizeof(Math::Vector3) * 4);
			std::memcpy(this->uv, uv, sizeof(Math::Vector2) * 4);
		}

		CSimpleSprite* sprite;
		Math::Vector3 color;
		const char* texturePath;
	};
}

