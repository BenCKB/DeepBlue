#include "LambertMaterial.h"

void LambertMaterial::addTextures() {
}

void LambertMaterial::setShaderValues() {

	int programID = shader->getProgramID();
	u_m = glGetUniformLocation(programID, "u_m");
	u_v = glGetUniformLocation(programID, "u_v");
	u_p = glGetUniformLocation(programID, "u_p");

}