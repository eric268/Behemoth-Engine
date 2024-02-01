#pragma once

#include "Math/Vector.h"
#include "Math/MathCore.h"
#include "NextAPI/App/SimpleSprite.h"

#include <vector>
#include <string>

namespace Behemoth
{
	enum PrimitiveType
	{
		TRIANGLE = 3,
		QUAD = 4
	};

	class Primitive
	{
	public:
		Primitive();
		Primitive(std::string& path, std::string& textureName);
		Primitive(
			const std::string& path,
			const std::string& textureName,
			PrimitiveType type,
			BMath::Vector4 vertices[],
			BMath::Vector3 normals[],
			BMath::Vector2 uv[],
			bool affectedByLighting);

		~Primitive();

		Primitive(const Primitive& obj);
		Primitive(Primitive&& obj) noexcept;

 		Primitive& operator=(const Primitive& obj);
		Primitive& operator=(Primitive&& obj) noexcept;

		void Draw();

		void SetSpriteVertices(const int numVertices, const BMath::Vector4 vert[]);
		void SetSpriteVertices(PrimitiveType type, BMath::Vector4 vert[], BMath::Vector2 uv[]);
		void SetPrimitiveVertices(PrimitiveType type, BMath::Vector4 vert[], BMath::Vector3 normal[], BMath::Vector2 uv[]);
		void SetSpriteUVs(PrimitiveType type, BMath::Vector2 uv[]);

		void SetLighting(BMath::Vector3 c);
		void AddLighting(BMath::Vector3 light);

		BMath::Vector4 vertices[4];
		BMath::Vector3 normals[4];
		BMath::Vector2 uv[4];

		PrimitiveType primitiveType;

		double depth;
		BMath::Vector3 diffuse;
		BMath::Vector3 specular;
		float shininess;
		bool affectedByLighting;

	private:
		void CopyVertexData(const BMath::Vector4* vertices, const BMath::Vector3* normals, const BMath::Vector2* uv);

		CSimpleSprite* sprite;
		std::string textureName;

		// Used for lighting
		BMath::Vector3 color;
	};
}

