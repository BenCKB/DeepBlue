#include "Shader.h"

Shader::Shader() {
	program = glCreateProgram();

	if (program == 0) {
		LOG_WARN("Error creating program");
	}

}

int Shader::addShader(ShaderType type, const std::string& shaderFile) {
	int id = 0;

	if (type == VERTEXSHADER) {
		id = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (type == FRAGMENTSHADER) {
		id = glCreateShader(GL_FRAGMENT_SHADER);
	}

	if (id == 0) {
		LOG_WARN("Error creating shader");
		return -1;
	}

	loadShader(shaderFile);

	const char* str = shaderSource.c_str();

	//Set shader source code and compile
	glShaderSource(id, 1, &str, nullptr);
	glCompileShader(id);

	//Handle Errors
	int res;
	glGetShaderiv(id, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE) {

		GLint infoL = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoL);
		if (infoL > 1)
		{
			char* infoLog = (char*)malloc(sizeof(char) * infoL);

			glGetShaderInfoLog(id, infoL, NULL, infoLog);
			LOG_WARN("Error compiling shader: {}", infoLog);

			free(infoLog);
		}
	}

	glAttachShader(program, id);

	return id;
}

void Shader::bind() {
	glLinkProgram(program);
	glValidateProgram(program);
	glUseProgram(program);
}

void Shader::activateShader() {
	glUseProgram(program);
}

void Shader::getUniforms(){

	int nUni;
	//Get number of uniforms found in shader
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &nUni);

	//Go through all uniforms and add them to the list
	for (int i = 0; i < nUni; i++) {
		
		//Declare variables for glGetActiveUniform call
		const int buffsize = 512;
		int u_length = 0;
		int u_size = 0;
		GLenum u_type;
		char u_name[buffsize];

		//Get uniform information
		glGetActiveUniform(program, i, buffsize, &u_length, &u_size, &u_type, u_name);
		
		
		IUniform* uniform = nullptr;

		//Set uniform according to its type
		switch (u_type) {
			case GL_FLOAT:
				uniform = new Uniform<float>();
				break;
			case GL_INT:
				uniform = new Uniform<int>();
				break;
			case GL_FLOAT_VEC3:
				uniform = new Uniform<Vec3f>();
				break;
			case GL_FLOAT_MAT4:
				uniform = new Uniform<Mat4>();
				break;
			case GL_SAMPLER_2D:
				uniform = new Uniform<int>();
				break;
			default:
				LOG_WARN("Unrecognized uniform type found in shader: {}", u_type);
				return;
				break;
		}

		if (uniform != nullptr) {
			//Set Uniform data
			uniform->index = i;
			uniform->uniformName = u_name;

			//Add to uniform list
			uniforms.insert({ u_name, std::shared_ptr<IUniform>(uniform)});

			LOG_INFO("Uniform {}, Length {}, Size {}, Type {}, Name {}", i, u_length, u_size, u_type, u_name);

		}
		else {
			return;
		}

		
	}
}

template<typename T>
void Shader::setUniform(const char* name, T& data) {
	auto uniIterator = uniforms.find(name);

	if (uniIterator != uniforms.end()) {
		Uniform<T> uni = uniIterator->second;
		uni.set(data);
	}
}

void Shader::loadShader(const std::string& shaderFile) {
	std::ifstream file(shaderFile);
	std::string line;

	std::stringstream ss;

	while (std::getline(file, line)) {
		ss << line << '\n';
	}

	file.close();

	shaderSource = ss.str();
}