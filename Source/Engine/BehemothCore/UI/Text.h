#pragma once

#include "Misc/BColors.h"
#include <string>

namespace Behemoth
{
	class Text
	{
	public:
		Text();
		Text(const std::string& t, BColors::BColor = BColors::White);
		void Print(const BMath::Vector2& pos);
		inline void SetText(const std::string& t) { text = t; }

	private:
		std::string text;
		BColors::BColor textColor;
	};
}
