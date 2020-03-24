#pragma once
#include <memory>
#include "Shader.h"
#include "Texture.h"
#include "../../math/Transform.h"
#include "GraphicsContext.h"

class Material {
public:

	//Constructors
	Material(std::shared_ptr<Shader>_shader);

	//Pure virtual functions
	virtual void addTextures() = 0;
	virtual void setShaderValues() = 0;

	//Texture loading and binding
	int loadTexture(const std::string& name);
	void setBaseValues(std::shared_ptr<Transform> modelMatrix);
	void bind();
	void unbind();

	int getProgramID() {
		return shader->getProgramID();
	}

private:
	//Current index, to bind the next texture to
	int currentIndex = 0;

protected:
	//Shader used for material
	std::shared_ptr<Shader> shader = nullptr;

	//List of all textures used
	std::vector< std::shared_ptr<Texture> > textures;
};
