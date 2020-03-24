#pragma once
#include "IComponent.h"
#include "../renderer/Mesh.h"
#include "../renderer/Material.h"

class CMeshRenderer : public IComponent {
public:

	CMeshRenderer(std::shared_ptr<Mesh> mesh) {
		m_mesh = mesh;
	}

	void input() {}

	void update() {

		//Switch programs to set uniforms
		glUseProgram(m_mesh->material->getProgramID());

		//Set uniform values
		m_mesh->material->setBaseValues(m_parent->getTransform());
		m_mesh->material->setShaderValues();

	}

	void render() {
		update();
		m_mesh->draw();
	}

	~CMeshRenderer() {}

private:
	std::shared_ptr<Mesh> m_mesh;
};