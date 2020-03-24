#pragma once
#include "..\..\src\core\renderer\Material.h"

class PBRMaterial : public Material {
public:
	PBRMaterial(std::shared_ptr<Shader>_shader) : Material(_shader){}

	void addTextures();
	void addTextures(std::string A, std::string R, std::string M, std::string N);
	void setShaderValues();

private:
	int u_albedo = 0;
	int u_roughness = 0;
	int u_metalness = 0;
	int u_normal = 0;
	int u_m = 0;
	int u_v = 0;
	int u_p = 0;
	
	int u_pointLights = 0;
	int u_directionalLights = 0;
};