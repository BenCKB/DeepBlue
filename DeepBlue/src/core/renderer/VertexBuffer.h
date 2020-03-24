#pragma once
#include "../../math/Vector.h"
#include "../../math/Matrix.h"
#include "../../util/Log.h"
#include "glad\glad.h"

class VertexBuffer {
public:
	VertexBuffer(std::vector<Vec3f>* verts, std::vector<Vec3f>* norms, std::vector<Vec2f>* uvs) {
		
		int sizeV = (verts->size() * 3 * sizeof(float));
		int sizeN = (norms->size() * 3 * sizeof(float));
		int sizeT = (uvs->size() * 2 * sizeof(float));
		int size = sizeV + sizeN + sizeT;

		LOG_WARN("Verts {}, norms {}, tex {}", sizeV, sizeN, sizeT);

		glGenBuffers(1, &bufferID);
		
		bind();
		
		glBufferData(GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeV, verts->data());
		glBufferSubData(GL_ARRAY_BUFFER, sizeV, sizeN, norms->data());
		glBufferSubData(GL_ARRAY_BUFFER, sizeV + sizeN, sizeT, uvs->data());
	
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	VertexBuffer(std::vector<Vec3f>* verts, int size) {
		glGenBuffers(1, &bufferID);
		bind();
		glBufferData(GL_ARRAY_BUFFER, size, verts->data(), GL_STATIC_DRAW);
	}

	void bind() {
		glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	}

	void unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	~VertexBuffer() {
		glDeleteBuffers(1, &bufferID);
	}

private:
	unsigned int bufferID;
};