#pragma once


namespace Behemoth
{
	enum KeyCode : uint16_t
	{
		Backspace				= 8,
		Tab						= 9,
		Enter					= 13,
		Escape					= 27,
		Space					= 32,
		ExclamationMark			= 33, // !
		DoubleQuote				= 34, // "
		Hashtag					= 35, // #
		DollarSign				= 36, // $
		PercentageSign			= 37, // %
		AND						= 38, // &
		SingleQuote				= 39, // '
		LBracket				= 40, // (
		RBracket				= 41, // )
		Asterisk				= 42, // *
		Equals					= 43, // =
		Comma					= 44, // ,
		Minus					= 45, // -
		Period					= 46, // .
		ForwardSlash			= 47, // /
		LessThan				= 60, // <
		GreaterThan				= 62, // >
		QuestionMark			= 63, // ?
		B_At					= 64, // @
		B_0						= 48,
		B_1						= 49,
		B_2						= 50,
		B_3						= 51,
		B_4						= 52,
		B_5						= 53,
		B_6						= 54,
		B_7						= 55,
		B_8						= 56,
		B_9						= 57,
		Colon					= 58, // :
		SemiColon				= 59, // ;
		Plus					= 61, // +
		LeftCBracket			= 91, // {
		Backslash				= 92, /* \ */
		RightCBracket			= 93, // }
		XOR						= 94, // ^
		Caret					= XOR, // ^
		Underscore				= 95, // _
		B_Tilda					= 96, // ~
		B_A						= 97,
		B_B						= 98,
		B_C						= 99,
		B_D						= 100,
		B_E						= 101,
		B_F						= 102,
		B_G						= 103,
		B_H						= 104,
		B_I						= 105,
		B_J						= 106,
		B_K						= 107,
		B_L						= 108,
		B_M						= 109,
		B_N						= 110,
		B_O						= 111,
		B_P						= 112,
		B_Q						= 113,
		B_R						= 114,
		B_S						= 115,
		B_T						= 116,
		B_U						= 117,
		B_V						= 118,
		B_W						= 119,
		B_X						= 120,
		B_Y						= 121,
		B_Z						= 122,
		Delete					= 127,

		// Special Key Inputs
		SpecialKeyOffset		= 256,
		B_F1					= SpecialKeyOffset + 1,
		B_F2					= SpecialKeyOffset + 2,
		B_F3					= SpecialKeyOffset + 3,
		B_F4					= SpecialKeyOffset + 4,
		B_F5					= SpecialKeyOffset + 5,
		B_F6					= SpecialKeyOffset + 6,
		B_F7					= SpecialKeyOffset + 7,
		B_F8					= SpecialKeyOffset + 8,
		B_F9					= SpecialKeyOffset + 9,
		B_F10					= SpecialKeyOffset + 10,
		B_F11					= SpecialKeyOffset + 11,
		B_F12					= SpecialKeyOffset + 12,
		B_Left					= SpecialKeyOffset + 100,
		B_Up					= SpecialKeyOffset + 101,
		B_Right					= SpecialKeyOffset + 102,
		B_Down					= SpecialKeyOffset + 103,
		Home					= SpecialKeyOffset + 106,
		End						= SpecialKeyOffset + 107,
		Insert					= SpecialKeyOffset + 108,
		LShift					= SpecialKeyOffset + 112,
		RShift					= SpecialKeyOffset + 113,
		LCtrl					= SpecialKeyOffset + 114,
		RCtrl					= SpecialKeyOffset + 115,
		LAlt					= SpecialKeyOffset + 116,
		RAlt					= SpecialKeyOffset + 117,
		Numpad_0				= SpecialKeyOffset + 120,
		Numpad_1				= SpecialKeyOffset + 121,
		Numpad_2				= SpecialKeyOffset + 122,
		Numpad_3				= SpecialKeyOffset + 123,
		Numpad_4				= SpecialKeyOffset + 124,
		Numpad_5				= SpecialKeyOffset + 125,
		Numpad_6				= SpecialKeyOffset + 126,
		Numpad_7				= SpecialKeyOffset + 127,
		Numpad_8				= SpecialKeyOffset + 128,
		Numpad_9				= SpecialKeyOffset + 129,

		NUM_KEYS				= SpecialKeyOffset + 130
	};

	enum MouseCode : uint8_t
	{
		LeftClick	= 0,
		ScrollClick = 1,
		RightClick	= 2,

		NUM_MOUSE_KEYS = 3
	};

	enum ControllerCode : uint8_t
	{

	};
}