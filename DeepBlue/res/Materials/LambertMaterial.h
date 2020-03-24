#pragma once
#include "..\..\src\core\renderer\Material.h"

class LambertMaterial : public Material {
public:
	LambertMaterial(std::shared_ptr<Shader>_shader) : Material(_shader) {}

	void addTextures();
	void setShaderValues();

private:
	int u_m = 0;
	int u_v = 0;
	int u_p = 0;
};