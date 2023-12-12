#pragma once

// stl
#include <vector>

namespace Behemoth
{
	class Primitives;

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

		std::vector<Primitives*> primitivesToDraw;
		std::size_t primitivesIndex;

		void Draw();
		void ReservePrimitives(std::size_t numPrimitives);
		void AddPrimitive(Primitives* primitive);

	private:
		Renderer();

		void SortPrimitivesByDepth();
	};
}