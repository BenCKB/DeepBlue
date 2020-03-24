#include "Input.h"
#include <iostream>

Input* Input::INPUT = 0;

Input* Input::getInstance()
{
	if (!INPUT)
		INPUT = new Input();
	return INPUT;
}

bool Input::isKeyPressed(int key)
{
	return false;
}

bool Input::isKeyDown(int key)
{
	return down_keys[key];
}

bool Input::isKeyReleased(int key)
{
	return !down_keys[key];
}

void Input::setKey(int key, int scancode, int action, int mods)
{
	down_keys[key] = action; //Pressed = 1, Released = 0, Released = 2
}

void Input::setForce(float f) {
	//std::cout << "Force: before " << force << " after " << f << std::endl;
	force = f;
}

float Input::getForce(){
	return force;
}

Input::~Input()
{
	delete INPUT;
	delete down_keys;
	delete mouseButtons;

	INPUT = nullptr;
	down_keys = nullptr;
	mouseButtons = nullptr;
}

//void Input::update() {
//	/*for (int i = 0; i < 512; i++) {
//		down_keys[i] = 0;
//	}*/
//}
