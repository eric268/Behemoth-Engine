#include "pch.h"
#include "Primitives.h"
#include "Core/Log.h"
#include "Core/ResourceManager.h"

namespace Behemoth
{
	Primitive::Primitive() : 
		sprite{nullptr},
		depth(0.0),
		textureName(""),
		diffuse(BMath::Vector3(0.8f, 0.8f, 0.8f)),
		specular(BMath::Vector3(1.0f, 1.0f, 1.0f)),
		shininess(32.0f),
		affectedByLighting(true),
		primitiveType(TRIANGLE)
	{
		vertices[0] = BMath::Vector4();
		vertices[1] = BMath::Vector4();
		vertices[2] = BMath::Vector4();
		uv[0] = BMath::Vector2();
		uv[1] = BMath::Vector2();
		uv[2] = BMath::Vector2();
		uv[3] = BMath::Vector2();
	}

	Primitive::Primitive(std::string& path, std::string& textureName) :
		depth(0.0),
		textureName(textureName),
		sprite{ new CSimpleSprite(path.c_str())},
		diffuse(BMath::Vector3(0.8f, 0.8f, 0.8f)),
		specular(BMath::Vector3(1.0f, 1.0f, 1.0f)),
		shininess(32.0f),
		affectedByLighting(true)
	{
		vertices[0] = BMath::Vector4();
		vertices[1] = BMath::Vector4();
		vertices[2] = BMath::Vector4();
		SetLighting(BMath::Vector3::Zero());
	}

	Primitive::Primitive(
		const std::string& path,
		const std::string& textureName, 
		PrimitiveType type,
		BMath::Vector4 vertices[],
		BMath::Vector3 normals[],
		BMath::Vector2 uv[],
		bool affectedByLighting) 
		:
		textureName(textureName),
		sprite{ new CSimpleSprite(path.c_str()) },
		primitiveType(type),
		depth(0.0),
		diffuse(BMath::Vector3(0.8f, 0.8f, 0.8f)),
		specular(BMath::Vector3(1.0f, 1.0f, 1.0f)),
		shininess(32.0f),
		affectedByLighting(affectedByLighting)
	{
		SetLighting(BMath::Vector3::Zero());
		CopyVertexData(vertices, normals, uv);
		SetSpriteUVs(type, uv);
	}

	Primitive::~Primitive()
	{
		delete sprite;
	}

	Primitive::Primitive(const Primitive& obj) : 
		depth(obj.depth),
		primitiveType(obj.primitiveType),
		diffuse(obj.diffuse),
		specular(obj.specular),
		shininess(obj.shininess)
	{
		affectedByLighting = obj.affectedByLighting;
		CopyVertexData(obj.vertices, obj.normals, obj.uv);
		const std::string& str = Behemoth::ResourceManager::GetInstance().GetTexturePath(textureName);
		sprite = new CSimpleSprite(str.c_str());
	}

	Primitive::Primitive(Primitive&& obj) noexcept
	{
		CopyVertexData(obj.vertices, obj.normals, obj.uv);

		sprite = obj.sprite;
		depth = obj.depth;
		primitiveType = obj.primitiveType;
		diffuse = obj.diffuse;
		specular = obj.specular;
		shininess = obj.shininess;
		textureName = obj.textureName;
		affectedByLighting = obj.affectedByLighting;

		SetSpriteUVs(primitiveType, uv);

		obj.sprite = nullptr;
	}

	Primitive& Primitive::operator=(const Primitive& obj)
	{
		if (this != &obj) 
		{ 
			delete sprite;

			CopyVertexData(obj.vertices, obj.normals, obj.uv);

			textureName = obj.textureName;
			const std::string& str = Behemoth::ResourceManager::GetInstance().GetTexturePath(textureName);
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

	Primitive& Primitive::operator=(Primitive&& obj)  noexcept
	{
		if (this != &obj) {

			delete sprite;
			sprite = obj.sprite;

			CopyVertexData(obj.vertices, obj.normals, obj.uv);
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

	void Primitive::Draw()
	{
		if (!sprite)
		{
			LOGMESSAGE(MessageType::Error, "Null primitive attempted to be drawn");
			return;
		}
		sprite->Draw();
	}

	void Primitive::SetSpriteVertices(const int numVertices, const BMath::Vector4 vert[])
	{
		if (!sprite)
		{
			LOGMESSAGE(MessageType::Error, "Null sprite found");
			return;
		}

		for (int i = 0; i < numVertices; i++)
		{
			sprite->SetVertex(i, vert[i].x, vert[i].y);
		}

		if (numVertices == TRIANGLE)
		{
			sprite->SetVertex(3, vert[0].x, vert[0].y);
		}
	}

	void Primitive::SetSpriteVertices(PrimitiveType type, BMath::Vector4 vert[], BMath::Vector2 uv[])
	{
		if (!sprite)
		{
			LOGMESSAGE(MessageType::Error, "Null sprite found");
			return;
		}

		for (int i = 0; i < static_cast<int>(type); i++)
		{
			sprite->SetVertex(i, vert[i].x, vert[i].y);
			sprite->SetUV(i, uv[i].x, uv[i].y);
		}

		if (type == TRIANGLE)
		{
			sprite->SetVertex(3, vert[0].x, vert[0].y);
		}
	}

	void Primitive::SetPrimitiveVertices(PrimitiveType type, BMath::Vector4 vert[], BMath::Vector3 normal[], BMath::Vector2 uv[])
	{
		primitiveType = type;
		for (int i = 0; i < static_cast<int>(type); i++)
		{
			vertices[i] = vert[i];
			normals[i] = normal[i];
		}
	}

	void Primitive::SetSpriteUVs(PrimitiveType type, BMath::Vector2 uv[])
	{
		if (!sprite)
		{
			LOGMESSAGE(MessageType::Error, "Null sprite found");
			return;
		}

		for (int i = 0; i < static_cast<int>(type); i++)
		{
			sprite->SetUV(i, uv[i].x, uv[i].y);
		}
	}

	void Primitive::SetLighting(BMath::Vector3 c)
	{
		if (!sprite)
		{
			LOGMESSAGE(MessageType::Error, "Null sprite found");
			return;
		}

		color = c;
		sprite->SetColor(c.x, c.y, c.z);
	}

	void Primitive::AddLighting(BMath::Vector3 light)
	{
		if (!sprite)
		{
			LOGMESSAGE(MessageType::Error, "Null sprite found");
			return;
		}

		color += light;
		sprite->SetColor(color.x, color.y, color.z);
	}

	void Primitive::CopyVertexData(const BMath::Vector4* vertices, const BMath::Vector3* normals, const BMath::Vector2* uv)
	{
		std::memcpy(this->vertices, vertices, sizeof(BMath::Vector4) * 4);
		std::memcpy(this->normals, normals, sizeof(BMath::Vector3) * 4);
		std::memcpy(this->uv, uv, sizeof(BMath::Vector2) * 4);
	}
}