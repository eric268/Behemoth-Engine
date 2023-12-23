#include "pch.h"
#include "EventManager.h"
#include "InputMapping.h"
#include "KeyboardEvents.h"

namespace Behemoth
{
	EventManager& EventManager::GetInstance()
	{
		static EventManager instance;
		return instance;
	}

	void EventManager::BindEventCallbacks()
	{
		// keyboard down - Uses ASCII for input mapping
		glutKeyboardFunc([](unsigned char key, int x, int y)
		{
			KeyCode code = static_cast<KeyCode>(key);
			KeyDownEvent e(code);
			EventManager::GetInstance().sceneEventFunc(e);
		});

		// keyboard up
		glutKeyboardUpFunc([](unsigned char key, int x, int y)
		{
			KeyCode code = static_cast<KeyCode>(key);
			KeyReleasedEvent e(code);
			EventManager::GetInstance().sceneEventFunc(e);
		});

		// Special Down Keyboard input - uses integer codes for input mapping
		glutSpecialFunc([](int key, int x, int y)
		{

		});

		// Special keyboard up input
		glutSpecialUpFunc([](int key, int x, int y)
		{

		});

		// Mouse pressed
		glutMouseFunc([] (int button, int state, int x, int y)
		{
			
		});

		// Mouse Move
		glutPassiveMotionFunc([](int x, int y)
		{

		});
			// Mouse Drag
		glutMotionFunc([](int x, int y)
		{

		});
	}
}