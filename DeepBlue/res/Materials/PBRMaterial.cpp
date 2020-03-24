#include "PBRMaterial.h"

void PBRMaterial::addTextures() {
	loadTexture(".\\res\\Textures\\rock_A.jpg");
	loadTexture(".\\res\\Textures\\rock_R.jpg");
	loadTexture(".\\res\\Textures\\_DEFAULT.png");
}

void PBRMaterial::addTextures(std::string A, std::string R, std::string M, std::string N) {
	loadTexture(A);
	loadTexture(R);
	loadTexture(M);
	loadTexture(N);
}

void PBRMaterial::setShaderValues() {

	int programID = shader->getProgramID();
	u_albedo = glGetUniformLocation(programID, "u_AlbedoTexture");
	u_roughness = glGetUniformLocation(programID, "u_RoughnessTexture");
	u_metalness = glGetUniformLocation(programID, "u_MetalnessTexture");
	u_normal = glGetUniformLocation(programID, "u_NormalMap");
	u_m = glGetUniformLocation(programID, "u_m");
	u_v = glGetUniformLocation(programID, "u_v");
	u_p = glGetUniformLocation(programID, "u_p");

	u_pointLights = glGetUniformLocation(programID, "u_pointLights");
	u_directionalLights = glGetUniformLocation(programID, "u_directionalLights");

	glUniform1i(u_albedo, 0);
	glUniform1i(u_roughness, 1);
	glUniform1i(u_metalness, 2);
	glUniform1i(u_normal, 3);

}