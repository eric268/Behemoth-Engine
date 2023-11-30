#include "Misc/Log.h"
#include "Render/MeshLoader.h"
#include "ResourceManager.h"


namespace BehemothEngine
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

		bool result = meshLoader.LoadModel("../../Assets/" + filePath, meshContainer[id].meshData);
		if (!result)
		{
			meshContainer.erase(id);
		}

		return result;
	}
}