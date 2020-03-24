#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Material.h"
#include "../../math/Transform.h"
#include "../../math/Vector.h"
#include "../../math/Matrix.h"
#include "VertexBuffer.h"
#include "../../util/Timer.h"

inline std::vector<std::string> getToken(std::string str, std::string delim) {
	std::vector<std::string> tokens;
	if (str.empty())
		return tokens;

	//Token beginning and end
	size_t begin = str.find_first_not_of(delim);
	size_t end = str.find_first_of(delim, begin); //Search until end of token reached, begin search at first character of token

	//Only return if there is no invalid position
	while (begin != std::string::npos && end != std::string::npos) {

		//Add token to token list
		tokens.push_back(str.substr(begin, end - begin));

		//Set new Substring starting at the end of token
		str = str.substr(end);
		begin = str.find_first_not_of(delim);
		end = str.find_first_of(delim, begin);

		//If we reach the end of string, add final token to list
		if (begin != std::string::npos && end == std::string::npos) {
			tokens.push_back(str.substr(begin));
		}
	}

	return tokens;
}

class Mesh {
public:

	Mesh(std::string path);

	Mesh(std::vector<Vec3f>* v, std::vector<Vec3f>* n, std::vector<Vec2f>* t, std::vector<unsigned int>* in);

	~Mesh();

	void setMaterial(std::shared_ptr<Material> mat);

	void move(float x, float y, float z);

	void rotate(float angle);

	void scale(float s);

	Mat4 getModelMatrix();

	void draw();

	std::shared_ptr<Material> material;
	std::vector<Vec3f> verts;
	std::vector<unsigned int> indices;
	std::vector<Vec3f> normals;
	std::vector<Vec2f> texVerts;

	Transform modelTransform;

	std::vector<Vec3f> verts_;
	std::vector<Vec3f> normals_;
	std::vector<Vec2f> texVerts_;
	
private:

	VertexBuffer* vb = nullptr;
	GLuint ibo;
	
	void loadGeometryData(std::string path);
};

