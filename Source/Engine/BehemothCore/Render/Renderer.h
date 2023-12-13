#pragma once

#include "Math/MathCore.h"


// stl
#include <vector>

namespace Behemoth
{
	class Primitives;
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

		std::vector<Math::Vector4> linesToDraw;
		std::vector<Primitives*> primitivesToDraw;
		std::size_t primitivesIndex;
		std::size_t lineIndex;

		void Draw();
		void ReservePrimitives(std::size_t numPrimitives);
		void AddPrimitive(Primitives* primitive);
		void ReserveLines(std::size_t numLines);
		void AddLine(const Math::Vector4& line);
		void FreeResourceOverflow();
		void ClearResources();

	private:
		Renderer();

		void SortPrimitivesByDepth();
	};
}