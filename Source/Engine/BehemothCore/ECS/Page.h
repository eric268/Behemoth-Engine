#pragma once

#include "Entity.h"
#include <vector>
#include <memory>
#include <cassert>

#define DEFAULT_PAGE_SIZE 4096

namespace ECS
{
	template <typename T>
	class Pages
	{
	public:
		using page_ptr = std::unique_ptr<T[]>;

		std::size_t pageSize;

		Pages(std::size_t size = DEFAULT_PAGE_SIZE) : pageSize(size) {}

		std::vector<page_ptr> pages;

		inline void AddPage()
		{
			pages.push_back(std::make_unique<T[]>(pageSize));
		}
		inline T* GetPage(std::size_t index)
		{
			std::size_t i = index / pageSize;
			assert(pages.size() > i);
			return pages[i].get();
		}

		T& Add(const ECS::entity_identifier& identifier, T component)
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
// 
// 		T& opeartor[](const ECS::entity_id& identifier)
// 		{
// 			std::size_t pageIndex = identifier / pageSize;
// 			std::size_t elementIndex = identifier % pageSize;
// 
// 			assert(pageIndex < pages.size());
// 			assert(elementIndex < pages.size());
// 
// 			return pages[pageIndex][elementIndex];
// 		}

		T& Get(const ECS::entity_id& identifier)
		{
			std::size_t pageIndex = identifier / pageSize;
			std::size_t elementIndex = identifier % pageSize;

			assert(pageIndex < pages.size());
			assert(elementIndex < pageSize);

			return pages[pageIndex][elementIndex];
		}

	private:

		
	};
}
