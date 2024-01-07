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
		QUAD = 4
	};

	class Primitives
	{
	public:
		Primitives();
		Primitives(std::string& path, std::string& textureName);
		Primitives(const std::string& path, const std::string& textureName, PrimitiveType type, BMath::Vector4 verticies[], BMath::Vector3 normals[], BMath::Vector2 uv[]);
		~Primitives();

		Primitives(const Primitives& obj);
		Primitives(Primitives&& obj) noexcept;

		Primitives& operator=(Primitives&& obj) noexcept;
 		Primitives& operator=(const Primitives& obj);

		void Draw();

		BMath::Vector4 verticies[4];
		BMath::Vector3 normals[4];
		BMath::Vector2 uv[4];

		PrimitiveType primitiveType;

		double depth;
		BMath::Vector3 diffuse;
		BMath::Vector3 specular;
		float shininess;

		void SetSpriteUVs(PrimitiveType type, BMath::Vector2 uv[]);
		void SetSpriteVerticies(const int numVerticies, const BMath::Vector4 vert[]);
		void SetSpriteVerticies(PrimitiveType type, BMath::Vector4 vert[], BMath::Vector2 uv[]);
		void SetPrimitiveVerticies(PrimitiveType type, BMath::Vector4 vert[], BMath::Vector3 normal[], BMath::Vector2 uv[]);

		inline void SetLighting(BMath::Vector3 c)
		{
			color = c;
			sprite->SetColor(c.x, c.y, c.z);
		}
		inline void AddLighting(BMath::Vector3 light)
		{
			color += light;
			sprite->SetColor(color.x, color.y, color.z);
		}

	private:

		inline void CopyVertexData(const BMath::Vector4* verticies, const BMath::Vector3* normals, const BMath::Vector2* uv) 
		{
			std::memcpy(this->verticies, verticies, sizeof(BMath::Vector4) * 4);
			std::memcpy(this->normals, normals, sizeof(BMath::Vector3) * 4);
			std::memcpy(this->uv, uv, sizeof(BMath::Vector2) * 4);
		}

		CSimpleSprite* sprite;
		std::string textureName;

		BMath::Vector3 color;
	};
}

