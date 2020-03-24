#pragma once
#include <string>
#include "../../math/Matrix.h"
#include "../../math/Vector.h"
#include "glad/glad.h"


class IUniform {
public:
	//Index retrieved from glGetUnifromLocation
	int index = -1;

	//Uniform name as present in shader
	std::string uniformName;
};

template<typename T>
class Uniform : public IUniform {
public:
	void set(T &value);

	//Data stored
	T data;
};

/* All Uniform types */
void setUniform(Uniform<float>& val);
void setUniform(Uniform<int>& val);
void setUniform(Uniform<Vec3f>& val);
void setUniform(Uniform<Mat4>& val);
