#pragma once
#include "Light.h"

class PointLight : public Light {
public:
	PointLight(Vec3f colour, float intensity, Vec3f pos, float constant, float linear, float expo) {
		m_colour = colour;
		m_intensity = intensity;
		m_position = pos;
		m_constant = constant;
		m_linear = linear;
		m_exponential = expo;
		m_transform.t = m_position;
	}

	Vec3f getPos() {
		return m_position;
	}

	Vec3f getColour()
	{
		return m_colour;
	}

	float getIntensity()
	{
		return m_intensity;
	}

	float getConstant() const
	{
		return m_constant;
	}

	float getLinear() const
	{
		return m_linear;
	}

	float getExp() const
	{
		return m_exponential;
	}

private:
	Vec3f m_position;
	float m_constant;
	float m_linear;
	float m_exponential;
};