#include "pch.h"
#include "Core/Log.h"
#include "Geometry/MeshLoader.h"
#include "ResourceManager.h"
#include "Physics/BoundingGenerator.h"


namespace Behemoth
{
	ResourceManager& ResourceManager::GetInstance()
	{
		static ResourceManager instance;
		return instance;
	}

	const MeshData& ResourceManager::GetMeshData(const std::string& filepath)
	{
		return GetMesh(filepath).first;
	}

	const std::vector<VertexData>& ResourceManager::GetMeshVertices(const std::string& filePath)
	{
		return GetMesh(filePath).second;
	}

	const std::pair<MeshData, std::vector<VertexData>>& ResourceManager::GetMesh(const std::string& filePath)
	{
		std::size_t id = hasher(filePath);

		if (meshMap.find(id) != meshMap.end() || LoadMeshData(filePath, id))
		{
			return meshMap[id];
		}

		if (filePath.size())
		{
			LOGMESSAGE(MessageType::Error, "Failed to copy mesh");
		}

		return meshMap[id];
	}

	bool ResourceManager::LoadMeshData(const std::string& filePath, const std::size_t id)
	{
		if (!filePath.size())
		{
			return false;
		}

		MeshLoader meshLoader{};
		std::vector<VertexData> meshVertices;
		MeshData meshData{};
		meshData.modelFileName = filePath;

		AABBCollider broadCollider{};
		SphereCollider sphereCollider{};

		bool result = meshLoader.LoadModel("Models/" + filePath, meshVertices, meshData);
		broadCollider.extents = BMath::Vector3(meshData.maxVertexDistance);
		float maxDistSq = meshData.maxVertexDistance * meshData.maxVertexDistance;

		if (result)
		{
			meshMap[id] = { std::move(meshData), std::move(meshVertices) };
			meshAABBBounds[id] = broadCollider;

			BoundingGenerator::GenerateSphereBoundingVolume(GetMeshVertices(filePath), sphereCollider);
			meshSphereBounds[id] = sphereCollider;
		}

		return result;
	}

	const std::string& ResourceManager::GetTexturePath(const std::string& textureName)
	{
		if (textureMap.find(textureName) != textureMap.end())
		{
			return textureMap[textureName];
		}

		std::filesystem::path full_path = std::filesystem::current_path() / "Textures" / textureName;
		textureMap[textureName] = full_path.string();
		return textureMap[textureName];
	}

	const std::string& ResourceManager::GetSoundClipPath(const std::string& soundclipName)
	{
		if (soundClipMap.find(soundclipName) != soundClipMap.end())
		{
			return soundClipMap[soundclipName];
		}

		std::filesystem::path full_path = std::filesystem::current_path() / "Audio" / soundclipName;
		soundClipMap[soundclipName] = full_path.string();
		return soundClipMap[soundclipName];
	}

	const std::string& ResourceManager::GetImagePath(const std::string& imagePath)
	{
		if (imageMap.find(imagePath) != imageMap.end())
		{
			return imageMap[imagePath];
		}

		std::filesystem::path full_path = std::filesystem::current_path() / "Images" / imagePath;
		imageMap[imagePath] = full_path.string();
		return imageMap[imagePath];
	}

	AABBCollider ResourceManager::GetMeshAABBBounds(const std::string& filepath)
	{
		std::size_t id = hasher(filepath);
		if (meshAABBBounds.find(id) != meshAABBBounds.end())
		{
			return meshAABBBounds[id]; 
		}
		else
		{
			AABBCollider collider{};
			if (BoundingGenerator::GenerateAABBBoundingVolume(GetMeshVertices(filepath), collider))
			{
				meshAABBBounds[id] = collider;
				return meshAABBBounds[id];
			}
			else
			{
				return {};
			}
		}
		return {};
	}

	SphereCollider ResourceManager::GetMeshSphereBounds(const std::string& filepath)
	{
		std::size_t id = hasher(filepath);
		if (meshSphereBounds.find(id) != meshSphereBounds.end())
		{
			return meshSphereBounds[id];
		}
		else
		{
			SphereCollider collider{};

			if (BoundingGenerator::GenerateSphereBoundingVolume(GetMeshVertices(filepath), collider))
			{
				meshSphereBounds[id] = collider;
				return collider;
			}
			else
			{
				return {};
			}
		}
		return {};
	}
}