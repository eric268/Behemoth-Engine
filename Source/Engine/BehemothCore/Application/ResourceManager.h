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
		using page_vertex = std::unique_ptr<Math::Vector3[]>;
		using page_normal = std::unique_ptr<Math::Vector3[]>;
		using page_uv = std::unique_ptr<Math::Vector2[]>;

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
