#include "pch.h"
#include "Primitives.h"
#include "Misc/Log.h"
#include "Application/ResourceManager.h"

#include <random>
#include <cstring>
#include <iostream>

namespace Behemoth
{
	Primitives::Primitives() : 
		sprite{nullptr},
		depth(0.0),
		textureName(""),
		diffuse(BMath::Vector3(0.8f, 0.8f, 0.8f)),
		specular(BMath::Vector3(1.0f, 1.0f, 1.0f)),
		shininess(32.0f)
	{
		verticies[0] = BMath::Vector4();
		verticies[1] = BMath::Vector4();
		verticies[2] = BMath::Vector4();
	}

	Primitives::Primitives(std::string& path, std::string& textureName) :
		depth(0.0),
		textureName(textureName),
		sprite{ new CSimpleSprite(path.c_str())},
		diffuse(BMath::Vector3(0.8f, 0.8f, 0.8f)),
		specular(BMath::Vector3(1.0f, 1.0f, 1.0f)),
		shininess(32.0f)
	{
		verticies[0] = BMath::Vector4();
		verticies[1] = BMath::Vector4();
		verticies[2] = BMath::Vector4();
		SetLighting(BMath::Vector3::Zero());
	}

	Primitives::Primitives(const std::string& path, const std::string& textureName, PrimitiveType type, BMath::Vector4 verticies[], BMath::Vector3 normals[], BMath::Vector2 uv[]) :
		textureName(textureName),
		sprite{ new CSimpleSprite(path.c_str()) },
		primitiveType(type),
		depth(0.0),
		diffuse(BMath::Vector3(0.8f, 0.8f, 0.8f)),
		specular(BMath::Vector3(1.0f, 1.0f, 1.0f)),
		shininess(32.0f)
	{
		SetLighting(BMath::Vector3::Zero());
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
		std::string& str = Behemoth::ResourceManager::GetInstance().GetTexturePath(textureName);
		sprite = new CSimpleSprite(str.c_str());
	}

	Primitives::Primitives(Primitives&& obj) noexcept
	{
		CopyVertexData(obj.verticies, obj.normals, obj.uv);

		sprite = obj.sprite;
		depth = obj.depth;
		primitiveType = obj.primitiveType;
		diffuse = obj.diffuse;
		specular = obj.specular;
		shininess = obj.shininess;
		textureName = obj.textureName;

		SetSpriteUVs(primitiveType, uv);

		obj.sprite = nullptr;
	}

	Primitives& Primitives::operator=(const Primitives& obj)
	{
		if (this != &obj) 
		{ 
			delete sprite;

			CopyVertexData(obj.verticies, obj.normals, obj.uv);

			textureName = obj.textureName;
			std::string& str = Behemoth::ResourceManager::GetInstance().GetTexturePath(textureName);
			sprite = new CSimpleSprite(str.c_str());

			depth = obj.depth;
			primitiveType = obj.primitiveType;
			diffuse = obj.diffuse;
			specular = obj.specular;
			shininess = obj.shininess;

			// Set UVs for sprite
			SetSpriteUVs(primitiveType, uv);
		}
		return *this;
	}

	Primitives& Primitives::operator=(Primitives&& obj)  noexcept
	{
		if (this != &obj) {

			delete sprite;
			sprite = obj.sprite;


			CopyVertexData(obj.verticies, obj.normals, obj.uv);
			depth = obj.depth;
			primitiveType = obj.primitiveType;
			textureName = std::move(obj.textureName);
			diffuse = obj.diffuse;
			specular = obj.specular;
			shininess = obj.shininess;
			SetSpriteUVs(primitiveType, uv);
			obj.sprite = nullptr;
		}

		return *this;
	}

	void Primitives::Draw()
	{
		if (!sprite)
		{
			LOG_MESSAGE(MessageType::Error, "Null primitive attempted to be drawn");
		}
		sprite->Draw();
		
	}

	void Primitives::SetSpriteVerticies(const int numVerticies, const BMath::Vector4 vert[])
	{
		if (!sprite)
		{
			LOG_MESSAGE(MessageType::Error, "Null sprite found");
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

	void Primitives::SetSpriteUVs(PrimitiveType type, BMath::Vector2 uv[])
	{
		for (int i = 0; i < static_cast<int>(type); i++)
		{
			sprite->SetUV(i, uv[i].x, uv[i].y);
		}

		if (type == TRIANGLE)
		{
			// sprite->SetUV(3, uv[0].x, uv[0].y);
		}
	}

	void Primitives::SetPrimitiveVerticies(PrimitiveType type, BMath::Vector4 vert[], BMath::Vector3 normal[], BMath::Vector2 uv[])
	{
		primitiveType = type;
		for (int i = 0; i < static_cast<int>(type); i++)
		{
			verticies[i] = vert[i];
			normals[i] = normal[i];
			// this->uv[i] = uv[i];
		}
		// SetSpriteUVs(type, uv);
	}

	void Primitives::SetSpriteVerticies(PrimitiveType type, BMath::Vector4 vert[], BMath::Vector2 uv[])
	{
		if (!sprite)
		{
			LOG_MESSAGE(MessageType::Error, "Null sprite found");
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
 			// sprite->SetUV(3, uv[0].x, uv[0].y);
 		}
	}
}