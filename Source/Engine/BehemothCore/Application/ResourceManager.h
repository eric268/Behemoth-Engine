#pragma once

#include "Render/Mesh.h"
// stl
#include <unordered_map>
#include <string>
#include <functional>

namespace BehemothEngine
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

	private:

		ResourceManager();
		bool LoadMesh(const std::string& filePath, const std::size_t id);
		std::unordered_map<std::size_t, MeshData> meshContainer;
		std::hash<std::string> hasher;
	};

}
