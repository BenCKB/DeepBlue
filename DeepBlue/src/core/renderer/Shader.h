#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <sstream>

#include "glad\glad.h"

#include "../../util/Log.h"
#include "Uniform.h"


class IUniform;

enum ShaderType {VERTEXSHADER = 0, FRAGMENTSHADER = 1, GEOMETRYSHADER};


class Shader{
public:
	ShaderType type = ShaderType::VERTEXSHADER;
	
	Shader();

	~Shader() { glDeleteProgram(program);}

	int addShader(ShaderType type, const std::string& shaderFile);

	void bind();

	void activateShader();

	template <typename T>
	void setUniform(const char* name, T& data);

	void getUniforms();

	int getProgramID() {
		return program;
	}

private:

	int program;
	std::string shaderSource; 
	std::unordered_map<std::string, std::shared_ptr<IUniform> > uniforms;

	void loadShader(const std::string& shaderFile);

};