#pragma once

enum class Action
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	JUMP,
	SHOOT,
	SELECT,
	PAUSE
};

class Keyboard
{
public:
	static int UP;
	static int DOWN;
	static int LEFT;
	static int RIGHT;
	static int JUMP;
	static int SHOOT;
	static int SELECT;
	static int PAUSE;

	static void SetKey(Action action, int key);
	static void ResetAll();

private:
	static int default_up;
	static int default_down;
	static int default_left;
	static int default_right;
	static int default_jump;
	static int default_shoot;
	static int default_select;
	static int default_pause;
};