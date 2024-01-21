#include "pch.h"
#include "GolfUIHelper.h"

std::string GolfUIHelper::GetHoleResultText(int numStrokes, int par)
{
	int result = par - numStrokes;

	if (numStrokes == 1)
	{
		return std::string("Hole in One! Well Done!");
	}
	else if (numStrokes < 4)
	{
		return holeResultsText[result];
	}
	else
	{
		return std::string("+" + std::to_string(result));
	}
}

std::unordered_map<int, std::string> GolfUIHelper::holeResultsText =
{
	{-3, "Albatross"},
	{-2, "Eagle"},
	{-1, "Birdie"},
	{0, "Par"},
	{1, "Bogey"},
	{2, "Double Bogey"},
	{3, "Tripple Bogey"}
};