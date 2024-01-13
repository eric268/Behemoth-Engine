#pragma once

#include "Entity.h"
#include <vector>

#define DEFAULT_PAGE_SIZE 4096

namespace ECS
{
	template <typename T>
	class Pages
	{
	public:
		using page_ptr = std::unique_ptr<T[]>;

		std::vector<page_ptr> pages;

		inline void AddPage()
		{
			vertexPages.push_back(std::make_unique<T[]>(DEFAULT_PAGE_SIZE));
		}
		inline T* GetPage(std::size_t index)
		{
			std::size_t i = index / DEFAULT_PAGE_SIZE;
			assert(vertexPages.size() > i);
			return vertexPages[i].get();
		}

		void AddComponentToPage(const ECS::entity_id& identifier)
		{
			while (handle / DEFAULT_PAGE_SIZE >= vertexPages.size())
			{
				AddPage();
			}

			std::size_t pageIndex = i / DEFAULT_PAGE_SIZE;
			std::size_t elementIndex = i % DEFAULT_PAGE_SIZE;
		}


		T& opeartor[](const ECS::entity_id& identifier)
		{
			std::size_t pageIndex = identifier / DEFAULT_PAGE_SIZE;
			std::size_t elementIndex = identifier % DEFAULT_PAGE_SIZE;

			assert(pageIndex < pages.size());

			return vertexPages[pageIndex][elementIndex];
		}
	private:

		
	};
}
