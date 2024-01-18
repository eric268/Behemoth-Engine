#pragma once

#include "Math/MathCore.h"
#include "Geometry/Line.h"


// stl
#include <vector>
#include <atomic>
#include <mutex>

namespace Behemoth
{
	class Primitive;
	class Vector4;

	class Renderer
	{
	public:
		static Renderer& GetInstance()
		{
			static Renderer instance;
			return instance;
		}
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		std::vector<Line> linesToDraw;
		std::vector<Primitive*> primitivesToDraw;

		void Draw();
		void ReservePrimitives(std::size_t numPrimitives);
		void AddPrimitive(Primitive* primitive, int index);
		void ReserveLines(std::size_t numLines);
		void AddLine(const Line& line);
		void FreeResourceOverflow();
		void ClearResources();

		std::mutex primitiveMutex;
		std::mutex lineMutex;

	private:
		Renderer();

		void SortPrimitivesByDepth();
	};
}