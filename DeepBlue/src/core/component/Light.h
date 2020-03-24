#pragma once
#include "../SceneNode.h"

class Light : public IComponent {
public:

	void input() {}

	void update() {}

	void render() {}

	Transform getTransform() const {
		return m_transform;
	}

protected:
	Vec3f m_colour;
	float m_intensity;
	Transform m_transform;
};