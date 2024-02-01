#pragma once

#include "Entity.h"

#include <vector>
#include <memory>
#include <cassert>

#define DEFAULT_PAGE_SIZE 512

namespace ECS
{
	// This class is the container for ECS components. This container paginates the data into arrays.
	// This ensure pointer stability since only a vector of unique pointers, which point to the component arrays,
	// may need to be moved if additional pages are added
	template <typename T>
	class Pages
	{
	public:
		using page_ptr = std::unique_ptr<T[]>;

		Pages(std::size_t size = DEFAULT_PAGE_SIZE) : pageSize(size) {}

		T& Add(const std::uint32_t identifier, T component)
		{
			while (identifier / pageSize >= pages.size())
			{
				AddPage();
			}

			std::size_t pageIndex = identifier / pageSize;
			std::size_t elementIndex = identifier % pageSize;

			assert(pageIndex < pages.size());
			assert(elementIndex < pageSize);

			pages[pageIndex][elementIndex] = std::move(component);
			return pages[pageIndex][elementIndex];
		}

		T& operator[](const std::uint32_t identifier)
		{
			std::size_t pageIndex = identifier / pageSize;
			std::size_t elementIndex = identifier % pageSize;

			assert(pageIndex < pages.size());
			assert(elementIndex < pageSize);

			return pages[pageIndex][elementIndex];
		}

	private:
		inline void AddPage()
		{
			pages.push_back(std::make_unique<T[]>(pageSize));
		}

		std::vector<page_ptr> pages;
		std::size_t pageSize;
	};
}