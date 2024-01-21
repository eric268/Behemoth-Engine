#pragma once

#include <string>
#include <unordered_map>

class GolfUIHelper
{
public:
	static std::string GetHoleResultText(int numStrokes, int par);

private:
	static std::unordered_map<int, std::string> holeResultsText;
};

