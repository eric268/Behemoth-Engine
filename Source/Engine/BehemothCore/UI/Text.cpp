#include "pch.h"
#include "Text.h"
#include "NextAPI/App/app.h"
#include "Math/Vector.h"

namespace Behemoth
{
	Text::Text() : text(""), textColor(Behemoth::White) {}

	Text::Text(const std::string& t, Behemoth::BColor c) : text(t), textColor(c) {}

	void Text::Print(const BMath::Vector2& pos)
	{
		BMath::Vector3 color = Behemoth::GetColor(textColor);

		App::Print(pos.x,pos.y ,text.c_str(), color.x, color.y, color.z, GLUT_BITMAP_TIMES_ROMAN_24);
	}
}