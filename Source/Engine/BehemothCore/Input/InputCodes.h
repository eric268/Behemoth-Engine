#pragma once

/**
 * @file
 * @brief Defines the Behemoth namespace with input structures and enumerations.
 */

namespace Behemoth
{
	/**
	* @struct AnalogInput
	* @brief Represents an analog input with x and y coordinates.
	*/
	struct AnalogInput
	{
		float x;
		float y;
	};
	/**
	* @enum KeyCode
	* @brief Enumerates keyboard key codes.
	*/
	enum KeyCode : uint16_t
	{
		KC_None = 0, ///< No key pressed
		KC_Backspace = 8, ///< Backspace key
		KC_Tab = 9, ///< Tab key
		KC_Enter					= 13,
		KC_Escape					= 27,
		KC_Space					= 32,
		KC_ExclamationMark			= 33, // !
		KC_DoubleQuote				= 34, // "
		KC_Hashtag					= 35, // #
		KC_DollarSign				= 36, // $
		KC_PercentageSign			= 37, // %
		KC_AND						= 38, // &
		KC_SingleQuote				= 39, // '
		KC_LBracket					= 40, // (
		KC_RBracket					= 41, // )
		KC_Asterisk					= 42, // *
		KC_Equals					= 43, // =
		KC_Comma					= 44, // ,
		KC_Minus					= 45, // -
		KC_Period					= 46, // .
		KC_ForwardSlash				= 47, // /
		KC_LessThan					= 60, // <
		KC_GreaterThan				= 62, // >
		KC_QuestionMark				= 63, // ?
		KC_At						= 64, // @
		KC_0						= 48,
		KC_1						= 49,
		KC_2						= 50,
		KC_3						= 51,
		KC_4						= 52,
		KC_5						= 53,
		KC_6						= 54,
		KC_7						= 55,
		KC_8						= 56,
		KC_9						= 57,
		KC_Colon					= 58, // :
		KC_SemiColon				= 59, // ;
		KC_Plus						= 61, // +
		KC_LeftCBracket				= 91, // {
		KC_Backslash				= 92, /* \ */
		KC_RightCBracket			= 93, // }
		KC_XOR						= 94, // ^
		KC_Caret					= KC_XOR, // ^
		KC_Underscore				= 95, // _
		KC_Tilda					= 96, // ~
		KC_A						= 97,
		KC_B						= 98,
		KC_C						= 99,
		KC_D						= 100,
		KC_E						= 101,
		KC_F						= 102,
		KC_G						= 103,
		KC_H						= 104,
		KC_I						= 105,
		KC_J						= 106,
		KC_K						= 107,
		KC_L						= 108,
		KC_M						= 109,
		KC_N						= 110,
		KC_O						= 111,
		KC_P						= 112,
		KC_Q						= 113,
		KC_R						= 114,
		KC_S						= 115,
		KC_T						= 116,
		KC_U						= 117,
		KC_V						= 118,
		KC_W						= 119,
		KC_X						= 120,
		KC_Y						= 121,
		KC_Z						= 122,
		KC_Delete					= 127,

		// Special Key Inputs
		SpecialKeyOffset		= 256,
		KC_F1					= SpecialKeyOffset + 1,
		KC_F2					= SpecialKeyOffset + 2,
		KC_F3					= SpecialKeyOffset + 3,
		KC_F4					= SpecialKeyOffset + 4,
		KC_F5					= SpecialKeyOffset + 5,
		KC_F6					= SpecialKeyOffset + 6,
		KC_F7					= SpecialKeyOffset + 7,
		KC_F8					= SpecialKeyOffset + 8,
		KC_F9					= SpecialKeyOffset + 9,
		KC_F10					= SpecialKeyOffset + 10,
		KC_F11					= SpecialKeyOffset + 11,
		KC_F12					= SpecialKeyOffset + 12,
		KC_Left					= SpecialKeyOffset + 100,
		KC_Up					= SpecialKeyOffset + 101,
		KC_Right				= SpecialKeyOffset + 102,
		KC_Down					= SpecialKeyOffset + 103,
		KC_Home					= SpecialKeyOffset + 106,
		KC_End					= SpecialKeyOffset + 107,
		KC_Insert				= SpecialKeyOffset + 108,
		KC_L_Shift				= SpecialKeyOffset + 112,
		KC_R_Shift				= SpecialKeyOffset + 113,
		KC_L_Ctrl				= SpecialKeyOffset + 114,
		KC_R_Ctrl				= SpecialKeyOffset + 115,
		KC_L_Alt				= SpecialKeyOffset + 116,
		KC_R_Alt				= SpecialKeyOffset + 117,
		KC_Numpad_0				= SpecialKeyOffset + 120,
		KC_Numpad_1				= SpecialKeyOffset + 121,
		KC_Numpad_2				= SpecialKeyOffset + 122,
		KC_Numpad_3				= SpecialKeyOffset + 123,
		KC_Numpad_4				= SpecialKeyOffset + 124,
		KC_Numpad_5				= SpecialKeyOffset + 125,
		KC_Numpad_6				= SpecialKeyOffset + 126,
		KC_Numpad_7				= SpecialKeyOffset + 127,
		KC_Numpad_8				= SpecialKeyOffset + 128,
		KC_Numpad_9				= SpecialKeyOffset + 129,

		NUM_KC					= SpecialKeyOffset + 130
	};

	/**
	* @enum MouseCode
	* @brief Enumerates mouse button and wheel codes.
	*/
	enum MouseCode : uint8_t
	{
		MC_Left_Click	= 0,
		MC_Scroll_Click  = 1,
		MC_Right_Click	= 2,
		MC_Scroll_Up		= 3,
		MC_Scroll_Down	= 4,

		NUM_MC = 5
	};

	/**
	* @enum ControllerCode
	* @brief Enumerates controller button codes.
	*/
	enum ControllerCode : uint16_t
	{
		CC_DPAD_UP				= 0,
		CC_DPAD_DOWN			= 1,
		CC_DPAD_LEFT			= 2,
		CC_DPAD_RIGHT			= 3,
		CC_START				= 4,
		CC_BACK					= 5,
		CC_L3_THUMB				= 6,
		CC_R3_THUMB				= 7,
		CC_L_SHOULDER			= 8,
		CC_R_SHOULDER			= 9,
		CC_A					= 10,
		CC_B					= 11,
		CC_X					= 12,
		CC_Y					= 13,
		CC_L_ANALOG				= 14,
		CC_R_ANALOG				= 15,
		CC_L_TRIGGER			= 16,
		CC_R_TRIGGER			= 17,
		NUM_CC					= 18,
	};

}