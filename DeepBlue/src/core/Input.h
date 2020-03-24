#pragma once

class Input {
public:
	static Input* getInstance();
		
	void init() {
		down_keys = new bool[512];

		mouseButtons = new bool[2];

		for (int i = 0; i < 512; i++) {
			down_keys[i] = 0;
		}
	}

	bool isKeyPressed(int key);

	bool isKeyDown(int key);

	bool isKeyReleased(int key);

	/*void update();*/

	void setKey(int key, int scancode, int action, int mods);

	void setForce(float f);
	float getForce();

	~Input();

private:
	static Input* INPUT;
	float force = 1.0;
	bool* down_keys = nullptr;
	bool* mouseButtons = nullptr;
	Input() { }
};