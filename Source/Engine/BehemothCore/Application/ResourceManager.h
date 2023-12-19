#pragma once

#include "Render/Mesh.h"
#include "Math/MathCore.h"
#include "MeshMetaData.h"
// stl
#include <unordered_map>
#include <string>
#include <functional>
#include <memory>

#define DEFAULT_PAGE_SIZE 4096

namespace Behemoth
{
	class ResourceManager
	{
	public:
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;

		static ResourceManager& GetInstance() 
		{
			static ResourceManager instance;
			return instance;
		}

		const MeshData* GetMesh(const std::string& filePath);

		std::string& GetTexture(std::string& textureName);

	private:
		ResourceManager();
		bool LoadMesh(const std::string& filePath, const std::size_t id);
		std::unordered_map<std::size_t, MeshData> meshContainer;
		std::hash<std::string> hasher;

		std::unordered_map<std::string, std::string> textureMap;
	};

}
