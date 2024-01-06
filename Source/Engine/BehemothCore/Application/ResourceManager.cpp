#include "pch.h"
#include "Misc/Log.h"
#include "Renderer/MeshLoader.h"
#include "ResourceManager.h"


namespace Behemoth
{
	ResourceManager::ResourceManager()
	{

	}

	const MeshData& ResourceManager::GetMeshData(const std::string& filepath)
	{
		return GetMesh(filepath).first;
	}

	const std::vector<VertexData>& ResourceManager::GetMeshVerticies(const std::string& filePath) 
	{
		return GetMesh(filePath).second;
	}

	const std::pair<MeshData, std::vector<VertexData>>& ResourceManager::GetMesh(const std::string& filePath)
	{
		std::size_t id = hasher(filePath);

		if (meshMap.find(id) != meshMap.end() || LoadMesh(filePath, id))
		{
			return meshMap[id];
		}

		LOG_MESSAGE(MessageType::Error, "Failed to copy mesh");
		return meshMap[id];
	}

	bool ResourceManager::LoadMesh(const std::string& filePath, const std::size_t id)
	{
		MeshLoader meshLoader{};

		MeshData meshData{};
		std::vector<VertexData> meshVerticies;

		bool result = meshLoader.LoadModel("Models/" + filePath, meshVerticies, meshData);
		meshData.modelFileName = filePath;

		if (result)
		{
			meshMap[id] = {std::move(meshData), std::move(meshVerticies)};
		}

		return result;
	}

	std::string& ResourceManager::GetTexturePath(const std::string& textureName)
	{
		if (textureMap.find(textureName) != textureMap.end())
			return textureMap[textureName];

		std::filesystem::path full_path = std::filesystem::current_path() / "Textures" / textureName;
		textureMap[textureName] = full_path.string();
		return textureMap[textureName];
	}
}