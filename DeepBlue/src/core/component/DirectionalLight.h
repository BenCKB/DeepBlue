#pragma once
#include "Light.h"

class DirectionalLight : public Light {
public:

	DirectionalLight(Vec3f colour, float intensity, Vec3f dir) {
		m_colour = colour;
		m_intensity = intensity;
		m_direction = dir;
	}

	void input()
	{
		//Do nothing
	}

	void update()
	{
		//Do nothing
	}

	void render()
	{
		//Do nothing
	}
private:
	Vec3f m_direction;
};