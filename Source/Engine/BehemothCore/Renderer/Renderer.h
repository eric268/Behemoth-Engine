#pragma once

#include "Geometry/Line.h"

namespace Behemoth
{
	class Primitive;
	class Vector4;
	struct Line;

	class Renderer
	{
	public:
		static Renderer& GetInstance();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		std::vector<Line> linesToDraw;
		std::vector<Primitive*> primitivesToDraw;

		void Draw();
		void ReservePrimitives(std::size_t numPrimitives);
		void AddPrimitive(Primitive* primitive, int index);
		void ReserveLines(std::size_t numLines);
		void AddLine(const Line& line);
		void FreePrimitiveResourceOverflow();
		void ClearResources();
		inline std::uint32_t GetCurrentPrimitiveCount() { return primitivesToDraw.size(); }

	private:
		Renderer() = default;
		void SortPrimitivesByDepth();
	};
}