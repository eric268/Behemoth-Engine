#pragma once

#include "Math/MathCore.h"
#include "NextAPI/App/SimpleSprite.h"
#include "NextAPI/App/app.h"

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
		Primitives(std::string& path, std::string& textureName);
		Primitives(const std::string& path, const std::string& textureName, PrimitiveType type, Math::Vector4 verticies[], Math::Vector3 normals[], Math::Vector2 uv[]);
		~Primitives();

		Primitives(const Primitives& obj);
		Primitives(Primitives&& obj) noexcept;

		Primitives& operator=(Primitives&& obj) noexcept;
 		Primitives& operator=(const Primitives& obj);

		void Draw();

		Math::Vector4 verticies[4];
		Math::Vector3 normals[4];
		Math::Vector2 uv[4];

		PrimitiveType primitiveType;

		double depth;
		Math::Vector3 diffuse;
		Math::Vector3 specular;
		float shininess;

		void SetSpriteUVs(PrimitiveType type, Math::Vector2 uv[]);
		void SetSpriteVerticies(const int numVerticies, const Math::Vector4 vert[]);
		void SetSpriteVerticies(PrimitiveType type, Math::Vector4 vert[], Math::Vector2 uv[]);
		void SetPrimitiveVerticies(PrimitiveType type, Math::Vector4 vert[], Math::Vector3 normal[], Math::Vector2 uv[]);

		inline void SetLighting(Math::Vector3 c)
		{
			color = c;
			sprite->SetColor(c.x, c.y, c.z);
		}
		inline void AddLighting(Math::Vector3 light)
		{
			color += light;
			sprite->SetColor(color.x, color.y, color.z);
		}

	private:

		inline void CopyVertexData(const Math::Vector4* verticies, const Math::Vector3* normals, const Math::Vector2* uv) 
		{
			std::memcpy(this->verticies, verticies, sizeof(Math::Vector4) * 4);
			std::memcpy(this->normals, normals, sizeof(Math::Vector3) * 4);
			std::memcpy(this->uv, uv, sizeof(Math::Vector2) * 4);
		}

		CSimpleSprite* sprite;
		std::string textureName;

		Math::Vector3 color;
	};
}

