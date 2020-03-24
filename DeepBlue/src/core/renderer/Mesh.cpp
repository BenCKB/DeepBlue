#include "Mesh.h"

Mesh::Mesh(std::string path) {

	//Load geometry data from file
	loadGeometryData(path);

	//Collect data for video memory
	vb = new VertexBuffer(&verts, &normals, &texVerts);
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

Mesh::Mesh(std::vector<Vec3f>* v, std::vector<Vec3f>* n, std::vector<Vec2f>* t, std::vector<unsigned int>* in) {
	vb = new VertexBuffer(v, n, t);
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, in->size() * sizeof(unsigned int), in->data(), GL_STATIC_DRAW);
	verts = *v;
	normals = *n;
	texVerts = *t;
	indices = *in;
}

Mesh::~Mesh() {
	vb->unbind();
	delete vb;
	vb = nullptr;
	if(material != nullptr)
		material->unbind();
	glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, &ibo);
}

void Mesh::setMaterial(std::shared_ptr<Material> mat) {
	material = mat;
}

void Mesh::move(float x, float y, float z) {
	modelTransform.t = modelTransform.t + Vec3f(x, y, z);
}

void Mesh::rotate(float angle) {
	modelTransform.r = modelTransform.r + Vec3f(0.0f, angle, 0.0f);
}

void Mesh::scale(float s) {
	//modelTransform.s = modelTransform.s + Vec3f(s, s, s);
	modelTransform.s = Vec3f(s, s, s);
}

Mat4 Mesh::getModelMatrix() {
	return modelTransform.getModelMatrix();
}

void Mesh::draw() {
	
	//Set shader program
	material->bind(); //!!!!!!!!!!!!!
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	vb->bind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(verts.size() * 3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(verts.size() * 3 * sizeof(float) + normals.size() * 3 * sizeof(float)));
	
	/* === Timing Debugging === */

	//GLuint timeQuery;
	//GLuint64 time;

	//glGenQueries(1, &timeQuery);

	//glBeginQuery(GL_TIME_ELAPSED, timeQuery);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);//(void*)0

	//glEndQuery(GL_TIME_ELAPSED);
	//int done = 0;
	//while (!done) {
	//	glGetQueryObjectiv(timeQuery,
	//		GL_QUERY_RESULT_AVAILABLE,
	//		&done);
	//}

	//glGetQueryObjectui64v(timeQuery, GL_QUERY_RESULT, &time);
	//GraphicsContext::getInstance().addTime(time / 1000000.0); //from ns to ms
	//LOG_INFO("It took {} ms", time / 1000000.0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	vb->unbind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	material->unbind();

}

void Mesh::loadGeometryData(std::string path) {
	Timer* time = new Timer();
	time->start();

	std::ifstream file(path);
	std::string line;
	int nVar = 0; //Index counter
	while (std::getline(file, line)) {

		auto tokens = getToken(line, " \t");

		if (tokens.empty())
			continue;

		if (tokens[0].compare("v") == 0) {
			Vec3f vec;
			vec.x = std::stof(tokens[1]);
			vec.y = std::stof(tokens[2]);
			vec.z = std::stof(tokens[3]);
			verts.push_back(vec);
		}
		if (tokens[0].compare("vt") == 0) {
			Vec2f vecT;
			vecT.x = std::stof(tokens[1]);
			vecT.y = std::stof(tokens[2]);
			texVerts.push_back(vecT);
		}
		if (tokens[0].compare("vn") == 0) {
			Vec3f vecN;
			vecN.x = std::stof(tokens[1]);
			vecN.y = std::stof(tokens[2]);
			vecN.z = std::stof(tokens[3]);
			normals.push_back(vecN);
		}
		if (tokens[0].compare("f") == 0) {

			//Determine type of face syntax
			size_t occu = std::count(tokens[1].begin(), tokens[1].end(), '/');

			//Case 1: VertexPosition
			if (occu == 0) {
				for (size_t i = 1; i < tokens.size(); i++) {
					indices.push_back(std::stoul(tokens[i]) - 1); //Verts start at 1 and not 0
				}
			}
			//Case  2: VertexPosition/VertexTexture 
			else if (occu == 1) {
				for (size_t i = 1; i < tokens.size(); i++) {
					auto faceComp = getToken(tokens[i], "/");
					indices.push_back(std::stoul(faceComp[0]) - 1);
				}
			}
			//Case  3: VertexPosition/VertexTexture/VertexNormal
			// or VertexPosition//VertexNormal
			else if (occu == 2) {

				for (size_t i = 1; i < tokens.size(); i++) {
					auto faceComp = getToken(tokens[i], "/");

					indices.push_back(nVar);
					++nVar;

					verts_.push_back(verts[std::stoul(faceComp[0]) - 1]);
					texVerts_.push_back(texVerts[std::stoul(faceComp[1]) - 1]);
					normals_.push_back(normals[std::stoul(faceComp[2]) - 1]);

				}
			}
		}
	}

	file.close();

	verts.clear();
	normals.clear();
	texVerts.clear();

	verts = verts_;
	normals = normals_;
	texVerts = texVerts_;

	time->stop();
	LOG_INFO("Loading object took {} ms ({} s)", time->getDelta()/1000, time->getDelta() / 1000000);
	LOG_INFO("Object Loader: Found {} verts, {} texture verts, {} normals and {} faces.", verts.size(), texVerts.size(), normals.size(), indices.size() / 3);
	delete time;
	time = nullptr;
}