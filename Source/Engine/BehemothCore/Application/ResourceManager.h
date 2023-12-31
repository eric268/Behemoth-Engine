#pragma once

#include "Renderer/Mesh.h"
#include "Math/MathCore.h"

// stl
#include <unordered_map>
#include <string>
#include <functional>
#include <memory>
#include <vector>

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

		const std::vector<VertexData>& GetMeshVerticies(const std::string& filePath);
		const MeshData& GetMeshData(const std::string& filePath);

		std::string& GetTexturePath(const std::string& textureName);

	private:
		ResourceManager();
		bool LoadMesh(const std::string& filePath, const std::size_t id);

		const std::pair <MeshData, std::vector<VertexData>>& GetMesh(const std::string& filepath);

		std::hash<std::string> hasher;
		std::unordered_map<std::string, std::string> textureMap;
		std::unordered_map<std::size_t, std::pair<MeshData, std::vector<VertexData>>> meshMap;
	};

}
