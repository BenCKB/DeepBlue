#include "Uniform.h"

void setUniform(Uniform<float>& val){
	glUniform1f(val.index, val.data);
}

void setUniform(Uniform<Vec3f>& val) {
	glUniform3fv(val.index, 1, &val.data.x);
}

void setUniform(Uniform<Mat4>& val) {
	glUniformMatrix4fv(val.index, 1, false, &val.data.m[0][0]);
}

template<typename T>
void Uniform<T>::set(T& val) {
	data = val;
	setUniform(*this);
}

