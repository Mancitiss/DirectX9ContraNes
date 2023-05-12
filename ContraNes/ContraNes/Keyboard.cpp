#include "Keyboard.h"
#include <Windows.h>
#include "CONSTANTS.h"

int Keyboard::default_down = VK_DOWN;
int Keyboard::default_up = VK_UP;
int Keyboard::default_left = VK_LEFT;
int Keyboard::default_right = VK_RIGHT;
int Keyboard::default_jump = VK_SPACE;
int Keyboard::default_shoot = VK_F;
int Keyboard::default_select = VK_RETURN;
int Keyboard::default_pause = VK_ESCAPE;

int Keyboard::DOWN = Keyboard::default_down;
int Keyboard::UP = Keyboard::default_up;
int Keyboard::LEFT = Keyboard::default_left;
int Keyboard::RIGHT = Keyboard::default_right;
int Keyboard::JUMP = Keyboard::default_jump;
int Keyboard::SHOOT = Keyboard::default_shoot;
int Keyboard::SELECT = Keyboard::default_select;
int Keyboard::PAUSE = Keyboard::default_pause;

void Keyboard::SetKey(Action action, int key)
{
	switch (action)
	{
	case Action::UP:
		UP = key;
		break;
	case Action::DOWN:
		DOWN = key;
		break;
	case Action::LEFT:
		LEFT = key;
		break;
	case Action::RIGHT:
		RIGHT = key;
		break;
	case Action::JUMP:
		JUMP = key;
		break;
	case Action::SHOOT:
		SHOOT = key;
		break;
	case Action::SELECT:
		SELECT = key;
		break;
	case Action::PAUSE:
		PAUSE = key;
		break;
	}
}

void Keyboard::ResetAll()
{
	UP = default_up;
	DOWN = default_down;
	LEFT = default_left;
	RIGHT = default_right;
	JUMP = default_jump;
	SHOOT = default_shoot;
	SELECT = default_select;
	PAUSE = default_pause;
}
