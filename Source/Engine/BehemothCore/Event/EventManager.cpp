#include "pch.h"
#include "EventManager.h"
#include "Input/InputCodes.h"
#include "KeyboardEvents.h"
#include "MouseEvents.h"
#include "WindowEvents.h"

namespace Behemoth
{
	EventManager& EventManager::GetInstance()
	{
		static EventManager instance;
		return instance;
	}

	void EventManager::BindEventCallbacks()
	{
		// Window Callbacks

		// Resize
		glutReshapeFunc([](int width, int height)
		{
			WindowResizeEvent e(width, height);
			EventManager::GetInstance().OnEventDelegate(e);
		});

		// Move
		glutPositionFunc([](int x, int y)
		{
			WindowMoveEvent e(x, y);
			EventManager::GetInstance().OnEventDelegate(e);
		});

		// Close
		glutCloseFunc([]()
		{
			WindowCloseEvent e;
			EventManager::GetInstance().OnEventDelegate(e);
		});

		// Keyboard pressed
		glutKeyboardFunc([](unsigned char key, int x, int y)
		{
			// Temporary solution to remap capital letters to lower case letters
			if (key >= 65 && key <= 90)
			{
				key += 32;
			}
				
			// Do a check for key down events to see if it is held or this is the first press
			KeyCode code = static_cast<KeyCode>(key);
			KeyDownEvent e(code);
			EventManager::GetInstance().OnEventDelegate(e);
		});

		// keyboard up
		glutKeyboardUpFunc([](unsigned char key, int x, int y)
		{
			// Temporary solution to remap capital letters to lower case letters
			if (key >= 65 && key <= 90)
			{
				key += 32;
			}

			KeyCode code = static_cast<KeyCode>(key);
			KeyReleasedEvent e(code);
			EventManager::GetInstance().OnEventDelegate(e);
		});

		// Special Down Keyboard input - uses integer codes for input mapping
		glutSpecialFunc([](int key, int x, int y)
		{
			// Do a check for key down events to see if it is held or this is the first press
			KeyCode code = static_cast<KeyCode>(key + KeyCode::SpecialKeyOffset);
			KeyDownEvent e(code);
			EventManager::GetInstance().OnEventDelegate(e);
		});

		// Special keyboard up input
		glutSpecialUpFunc([](int key, int x, int y)
		{
			KeyCode code = static_cast<KeyCode>(key + KeyCode::SpecialKeyOffset);
			KeyReleasedEvent e(code);
			EventManager::GetInstance().OnEventDelegate(e);
		});

		// Mouse Callbacks

		// Mouse buttons
		glutMouseFunc([] (int button, int state, int x, int y)
		{
			MouseCode code = static_cast<MouseCode>(button);
			if (state)
			{
				MouseUpEvent e(code, x, y);
				EventManager::GetInstance().OnEventDelegate(e);
			}
			else
			{
				MouseDownEvent e(code, x, y);
				EventManager::GetInstance().OnEventDelegate(e);
			}
		});

		// Mouse Move
		glutPassiveMotionFunc([](int x, int y)
		{
			MouseMoveEvent e(x, y);
			EventManager::GetInstance().OnEventDelegate(e);
		});

		// Mouse Drag
		glutMotionFunc([](int x, int y)
		{
			MouseDragEvent e(x, y);
			EventManager::GetInstance().OnEventDelegate(e);
		});
	}
}