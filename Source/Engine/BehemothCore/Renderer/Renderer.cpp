#include "pch.h"
#include "Renderer.h"
#include "Primitives.h"

#include "NextAPI/App/app.h"

// stl
#include <algorithm>
#include <iostream>

namespace Behemoth
{
	Renderer::Renderer() : primitivesIndex(0), lineIndex(0) {}

	void Renderer::ReservePrimitives(std::size_t numPrimitives)
	{
		primitivesToDraw.resize(primitivesToDraw.size() + numPrimitives);
	}

	void Renderer::ReserveLines(std::size_t numLines)
	{
		linesToDraw.resize(linesToDraw.size() + numLines);
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

	void Renderer::FreeResourceOverflow()
	{
		primitivesToDraw.erase(primitivesToDraw.begin() + primitivesIndex, primitivesToDraw.end());
		linesToDraw.erase(linesToDraw.begin() + lineIndex, linesToDraw.end());
	}


	void Renderer::AddLine(const Line& line)
	{
		linesToDraw[lineIndex++] = line;
	}

	void Renderer::Draw()
	{
		SortPrimitivesByDepth();

		for (const auto& p : primitivesToDraw)
		{
			
			p->Draw();
		}

		for (const auto& line : linesToDraw)
		{
			App::DrawLine(line.lineData.x, line.lineData.y, line.lineData.z, line.lineData.w, line.lineColor.x, line.lineColor.y, line.lineColor.z);
		}

		ClearResources();
	}
	void Renderer::ClearResources()
	{
		primitivesToDraw.clear();
		primitivesIndex = 0;

		linesToDraw.clear();
		lineIndex = 0;
	}
}