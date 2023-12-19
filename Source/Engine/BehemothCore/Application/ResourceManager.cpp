#include "Misc/Log.h"
#include "Render/MeshLoader.h"
#include "ResourceManager.h"

//stl
#include <filesystem>

namespace Behemoth
{
	ResourceManager::ResourceManager()
	{

	}

	const MeshData* ResourceManager::GetMesh(const std::string& filePath)
	{
		std::size_t id = hasher(filePath);

		if (meshContainer.find(id) != meshContainer.end() || LoadMesh(filePath, id))
		{
			return &meshContainer[id];
		}

		LOG_ERROR("Failed to copy mesh");
		return nullptr;
	}

	bool ResourceManager::LoadMesh(const std::string& filePath, const std::size_t id)
	{
		MeshLoader meshLoader{};
		meshContainer[id] = MeshData();

		bool result = meshLoader.LoadModel("Models/" + filePath, meshContainer[id].triangleData, meshContainer[id].quadData);
		if (!result)
		{
			meshContainer.erase(id);
		}

		return result;
	}

	std::string& ResourceManager::GetTexture(std::string& textureName)
	{
		if (textureMap.find(textureName) != textureMap.end())
			return textureMap[textureName];

		std::filesystem::path full_path = std::filesystem::current_path() / "Textures" / textureName;
		textureMap[textureName] = full_path.string();
		return textureMap[textureName];
	}
}