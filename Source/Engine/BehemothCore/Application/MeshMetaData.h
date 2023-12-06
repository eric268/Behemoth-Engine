#pragma once

#include <cstddef>

struct MeshMetaData
{
	MeshMetaData(std::size_t start, std::size_t end) : startIndex(start), endIndex(end) {}
	std::size_t startIndex;
	std::size_t endIndex;
};