#include "Material.h"

Material::Material(std::shared_ptr<Shader>_shader) {
	shader = _shader;
}

int Material::loadTexture(const std::string& file){
	std::shared_ptr<Texture> tex((new Texture(file, currentIndex)));
	textures.push_back(tex);
	++currentIndex;
	return currentIndex - 1;
}

void Material::setBaseValues(std::shared_ptr<Transform> modelMatrix) {
	int m = glGetUniformLocation(shader->getProgramID(), "u_m");
	int v = glGetUniformLocation(shader->getProgramID(), "u_v");
	int p = glGetUniformLocation(shader->getProgramID(), "u_p");

	glUniformMatrix4fv(m, 1, false, &(modelMatrix->getModelMatrix()).transpose().m[0][0]);
	glUniformMatrix4fv(v, 1, false, &(GraphicsContext::getInstance().getCamera()->getViewMatrix()).transpose().m[0][0]);
	glUniformMatrix4fv(p, 1, false, &(GraphicsContext::getInstance().getCamera()->getProjectionMatrix().transpose().m[0][0]));

	/*for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			std::cout << GraphicsContext::getInstance().getCamera()->getViewMatrix().m[i][j] << " ";
		}
		std::cout << std::endl;
	}*/

	for (unsigned int i = 0; i < GraphicsContext::getInstance().getPointLights().size(); ++i) {

		std::string uName = "u_pointLights[" + std::to_string(i) + "]";
		int lColour = glGetUniformLocation(shader->getProgramID(), (uName + ".LightColour").c_str());
		int lIntensity = glGetUniformLocation(shader->getProgramID(), (uName + ".LightIntensity").c_str());
		int lPosition = glGetUniformLocation(shader->getProgramID(), (uName + ".LightPos").c_str());
		int lConstant = glGetUniformLocation(shader->getProgramID(), (uName + ".LightConstant").c_str());
		int lLinear = glGetUniformLocation(shader->getProgramID(), (uName + ".LightLinear").c_str());
		int lExp = glGetUniformLocation(shader->getProgramID(), (uName + ".LightExp").c_str());

		std::shared_ptr<PointLight> temp = GraphicsContext::getInstance().getPointLights()[i];
		glUniform3f(lColour, temp->getColour().x, temp->getColour().y, temp->getColour().z);
		glUniform1f(lIntensity, temp->getIntensity());
		Vec4f v_pos = GraphicsContext::getInstance().getCamera()->getViewMatrix() * Vec4f(temp->getPos(), 1.0);
		glUniform3f(lPosition, v_pos.x, v_pos.y, v_pos.z);
		glUniform1f(lConstant, temp->getConstant());
		glUniform1f(lLinear, temp->getLinear());
		glUniform1f(lExp, temp->getExp());
	}

	for (unsigned int i = 0; i < GraphicsContext::getInstance().getDirectionalLights().size(); ++i) {

	}
}

void Material::bind() {
	shader->activateShader();
	for (unsigned int i = 0; i < textures.size(); i++) {
		textures[i]->bind(i);
	}
}

void Material::unbind() {

	for (unsigned int i = 0; i < textures.size(); i++) {
		textures[i]->unbind(i);
	}
}