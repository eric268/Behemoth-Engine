#include "Renderer.h"
#include "Primitives.h"

// stl
#include <algorithm>
#include <iostream>

namespace Behemoth
{
	Renderer::Renderer() : primitivesIndex(0) {}

	void Renderer::ReservePrimitives(std::size_t numPrimitives)
	{
		primitivesToDraw.resize(primitivesToDraw.size() + numPrimitives);
	}

	void Renderer::SortPrimitivesByDepth()
	{
		std::sort(primitivesToDraw.begin(), primitivesToDraw.end(),
			[&](const Primitives* p1, const Primitives* p2)
			{
				return p1->depth > p2->depth;
			});
	}

	void Renderer::AddPrimitive(Primitives* primitive)
	{
		primitivesToDraw[primitivesIndex++] = primitive;
	}

	void Renderer::RemovePrimitiveOverflow()
	{
		// primitivesToDraw.erase(primitivesToDraw.begin() + primitivesIndex, primitivesToDraw.end());
		primitivesToDraw.erase(std::remove(primitivesToDraw.begin(), primitivesToDraw.end(), nullptr), primitivesToDraw.end());
	}

	void Renderer::Draw()
	{
		SortPrimitivesByDepth();

		for (const auto& p : primitivesToDraw)
		{
			p->Draw();
		}

		primitivesToDraw.clear();
		primitivesIndex = 0;
	}

}