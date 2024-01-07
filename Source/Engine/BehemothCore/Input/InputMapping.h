#pragma once
#include "InputCodes.h"

#include <map>
#include <XInput.h>

namespace Behemoth
{
	static std::map<int, uint16_t> controllerButtonMap =
	{
		{CC_DPAD_UP, XINPUT_GAMEPAD_DPAD_UP},
		{CC_DPAD_DOWN, XINPUT_GAMEPAD_DPAD_DOWN},
		{CC_DPAD_LEFT, XINPUT_GAMEPAD_DPAD_LEFT},
		{CC_DPAD_RIGHT, XINPUT_GAMEPAD_DPAD_RIGHT},
		{CC_START, XINPUT_GAMEPAD_START},
		{CC_BACK, XINPUT_GAMEPAD_BACK},
		{CC_L3_THUMB, XINPUT_GAMEPAD_LEFT_THUMB},
		{CC_R3_THUMB, XINPUT_GAMEPAD_RIGHT_THUMB},
		{CC_L_SHOULDER, XINPUT_GAMEPAD_LEFT_SHOULDER},
		{CC_R_SHOULDER, XINPUT_GAMEPAD_RIGHT_SHOULDER},
		{CC_A, XINPUT_GAMEPAD_A},
		{CC_B, XINPUT_GAMEPAD_B},
		{CC_X, XINPUT_GAMEPAD_X},
		{CC_Y, XINPUT_GAMEPAD_Y},
	};

	static std::map<int, AnalogInput> controllerAnalogMap =
	{
		{CC_L_ANALOG,  {0,0}},
		{CC_R_ANALOG,  {0,0}},
		{CC_L_TRIGGER, {0,0}},
		{CC_R_TRIGGER, {0,0}},
	};
}