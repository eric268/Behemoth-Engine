#include "Primitives.h"
#include "Misc/Log.h"

#include <random>
#include <cstring>

namespace Behemoth
{
	Primitives::Primitives() : 
		sprite{nullptr},
		depth(0.0),
		texturePath(nullptr),
		diffuse(Math::Vector3(0.8f, 0.8f, 0.8f)),
		specular(Math::Vector3(1.0f, 1.0f, 1.0f)),
		shininess(32.0f)
	{
		verticies[0] = Math::Vector3();
		verticies[1] = Math::Vector3();
		verticies[2] = Math::Vector3();
	}

	Primitives::Primitives(const char* path) : 
		sprite{ new CSimpleSprite(path)},
		depth(0.0),
		texturePath(path),
		diffuse(Math::Vector3(0.8f, 0.8f, 0.8f)),
		specular(Math::Vector3(1.0f, 1.0f, 1.0f)),
		shininess(32.0f)
	{
		verticies[0] = Math::Vector3();
		verticies[1] = Math::Vector3();
		verticies[2] = Math::Vector3();
		SetLighting(Math::Vector3::Zero());
	}

	Primitives::Primitives(const char* path, PrimitiveType type, Math::Vector3 verticies[], Math::Vector3 normals[], Math::Vector2 uv[]) :
		sprite{ new CSimpleSprite(path) },
		texturePath(path),
		primitiveType(type),
		depth(0.0),
		diffuse(Math::Vector3(0.8f, 0.8f, 0.8f)),
		specular(Math::Vector3(1.0f, 1.0f, 1.0f)),
		shininess(32.0f)
	{
		SetLighting(Math::Vector3::Zero());
		CopyVertexData(verticies, normals, uv);
		SetSpriteUVs(type, uv);
	}

	Primitives::~Primitives()
	{
		delete sprite;
	}

	Primitives::Primitives(const Primitives& obj) : 
		depth(obj.depth),
		primitiveType(obj.primitiveType),
		diffuse(obj.diffuse),
		specular(obj.specular),
		shininess(obj.shininess)
	{
		CopyVertexData(obj.verticies, obj.normals, obj.uv);

		texturePath = (obj.texturePath) ? obj.texturePath : "";
		sprite = new CSimpleSprite(texturePath);
	}

	Primitives::Primitives(Primitives&& obj) noexcept
	{
		CopyVertexData(obj.verticies, obj.normals, obj.uv);

		sprite = obj.sprite;
		obj.sprite = nullptr;
		depth = obj.depth;
		primitiveType = obj.primitiveType;
		diffuse = obj.diffuse;
		specular = obj.specular;
		shininess = obj.shininess;

		SetSpriteUVs(primitiveType, uv);
	}

	Primitives& Primitives::operator=(const Primitives& obj)
	{
		if (this != &obj) 
		{ 
			delete sprite;

			CopyVertexData(obj.verticies, obj.normals, obj.uv);

			texturePath = (obj.texturePath) ? obj.texturePath : "";
			sprite = new CSimpleSprite(texturePath);

			depth = obj.depth;
			primitiveType = obj.primitiveType;
			diffuse = obj.diffuse;
			specular = obj.specular;
			shininess = obj.shininess;

			// Set UVs for sprite
			// SetSpriteUVs(primitiveType, uv);
		}
		return *this;
	}

	Primitives& Primitives::operator=(Primitives&& obj)  noexcept
	{
		if (this != &obj) {

			delete sprite;
			sprite = obj.sprite;
			obj.sprite = nullptr;

			CopyVertexData(obj.verticies, obj.normals, obj.uv);
			depth = obj.depth;
			primitiveType = obj.primitiveType;
			texturePath = obj.texturePath;
			diffuse = obj.diffuse;
			specular = obj.specular;
			shininess = obj.shininess;
			// SetSpriteUVs(primitiveType, uv);
		}

		return *this;
	}

	void Primitives::Draw()
	{
		if (!sprite)
		{
			LOG_ERROR("Null primitive attempted to be drawn");
		}
		sprite->Draw();
	}

	void Primitives::SetSpriteVerticies(const int numVerticies, const Math::Vector4 vert[])
	{
		if (!sprite)
		{
			LOG_ERROR("Null sprite found");
			return;
		}

		for (int i = 0; i < numVerticies; i++)
		{
			sprite->SetVertex(i, vert[i].x, vert[i].y);
		}

		if (numVerticies == TRIANGLE)
		{
			sprite->SetVertex(3, vert[0].x, vert[0].y);
		}
	}

	void Primitives::SetSpriteUVs(PrimitiveType type, Math::Vector2 uv[])
	{
		for (int i = 0; i < static_cast<int>(type); i++)
		{
			sprite->SetUV(i, uv[i].x, uv[i].y);
		}

		if (type == TRIANGLE)
		{
			sprite->SetUV(3, uv[0].x, uv[0].y);
		}
	}

	void Primitives::SetPrimitiveVerticies(PrimitiveType type, Math::Vector3 vert[], Math::Vector3 normal[], Math::Vector2 uv[])
	{
		primitiveType = type;
		for (int i = 0; i < static_cast<int>(type); i++)
		{
			verticies[i] = vert[i];
			normals[i] = normal[i];
			this->uv[i] = uv[i];
		}
		SetSpriteUVs(type, uv);
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
}