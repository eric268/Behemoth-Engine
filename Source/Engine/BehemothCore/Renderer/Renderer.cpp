#include "pch.h"
#include "Renderer.h"
#include "Geometry/Primitives.h"
#include "Core/Log.h"

#include "NextAPI/App/app.h"

namespace Behemoth
{
	Renderer::Renderer() {}

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
			[&](const Primitive* p1, const Primitive* p2)
			{
				return p1->depth > p2->depth;
			});
	}

	void Renderer::AddPrimitive(Primitive* primitive, int index)
	{
		if (index > primitivesToDraw.size())
		{
			LOGMESSAGE(Error, "Primitive Container overflow");
		}
		primitivesToDraw[index] = primitive;
	}

	void Renderer::AddLine(const Line& line)
	{
		linesToDraw.push_back(line);
	}

	void Renderer::FreePrimitiveResourceOverflow()
	{
		primitivesToDraw.erase(std::remove(primitivesToDraw.begin(), primitivesToDraw.end(), nullptr), primitivesToDraw.end());
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
		linesToDraw.clear();
	}
}