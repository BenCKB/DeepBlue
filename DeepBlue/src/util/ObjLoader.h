#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <memory>

#include "../math/Vector.h"
#include "Timer.h"

std::vector<std::string> getToken(std::string str, std::string delim);

struct MESH {
	std::vector<Vec3f> VERTS;
	std::vector<unsigned int> IND;
	std::vector<Vec3f> NORMS;
	std::vector<Vec2f> TEXVERTS;
	std::string NAME = "";
	std::string MAT;
};

struct MATERIAL {
	std::string NAME = "";
	std::string DIFFUSE = "";
	std::string AMBIENT = "";
	std::string SPECULAR = "";
	std::string DISSOLVE = "";
	std::string NORMAL = "";
};

class ObjLoader {
public:
	ObjLoader(std::string objPath, std::string matPath) {
		loadGeometryData(objPath);
		loadMaterialData(matPath);
	}

	void loadMaterialData(std::string matPath) {
		Timer* time = new Timer();
		time->start();

		std::ifstream file(matPath);
		std::string line;

		std::shared_ptr<MATERIAL> m = std::make_shared<MATERIAL>();

		while (std::getline(file, line)) {

			auto tokens = getToken(line, " \t"); //Spaces and tabs

			if (tokens.empty())
				continue;

			if (tokens[0].compare("newmtl") == 0) {
				if (m->NAME.compare("") == 0) {
					m->NAME = tokens[1];
				}
				else {
					materials.push_back(m);
					m = std::make_shared<MATERIAL>();
					m->NAME = tokens[1];
				}
			}

			if (tokens[0].compare("map_Kd") == 0) {
				m->DIFFUSE = tokens[1];
			}
			if (tokens[0].compare("map_Ka") == 0) {
				m->AMBIENT = tokens[1];
			}
			if (tokens[0].compare("map_Ns") == 0) {
				m->SPECULAR = tokens[1];
			}
			if (tokens[0].compare("map_d") == 0) {
				m->DISSOLVE = tokens[1];
			}
			if (tokens[0].compare("map_bump") == 0) {
				m->NORMAL = tokens[1];
			}

		}

		materials.push_back(m);

		time->stop();

		std::cout << "Materials: " << std::endl;
		for (auto mat : materials) {
			std::cout << mat->NAME << ": " << mat->DIFFUSE << std::endl;
		}

		std::cout << "Loading materials took " << time->getDelta() / 1000 << " ms (" << time->getDelta() / 1000000 << " s)" << std::endl;

	}

	void loadGeometryData(std::string objPath) {
		Timer* time = new Timer();
		time->start();

		std::string currentMAT = "";

		std::shared_ptr<MESH> m = std::make_shared<MESH>();

		std::ifstream file(objPath);
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
				vertsCollection.push_back(vec);
			}
			if (tokens[0].compare("vt") == 0) {
				Vec2f vecT;
				vecT.x = std::stof(tokens[1]);
				vecT.y = std::stof(tokens[2]);
				texVertsCollection.push_back(vecT);
			}
			if (tokens[0].compare("vn") == 0) {
				Vec3f vecN;
				vecN.x = std::stof(tokens[1]);
				vecN.y = std::stof(tokens[2]);
				vecN.z = std::stof(tokens[3]);
				normalsCollection.push_back(vecN);
			}
			if (tokens[0].compare("g") == 0) {
				if (m->NAME.compare("") == 0) {
					m->NAME = tokens[1];
				}
				else {
					m->IND = indicesCurrent;
					m->VERTS = vertsCurrent;
					m->NORMS = normalsCurrent;
					m->TEXVERTS = texVertsCurrent;
					m->MAT = currentMAT;
					meshes.push_back(m);

					vertsCurrent.clear();
					normalsCurrent.clear();
					texVertsCurrent.clear();
					indicesCurrent.clear();

					m = std::make_shared<MESH>();
					m->NAME = tokens[1];
					nVar = 0;
				}
			}
			if (tokens[0].compare("usemtl") == 0) {
				currentMAT = tokens[1];
			}
			if (tokens[0].compare("f") == 0) {

				//Determine type of face syntax
				size_t occu = std::count(tokens[1].begin(), tokens[1].end(), '/');

				//TODO: Save TextureCoords and Normals too 

				//Case 1: VertexPosition
				if (occu == 0) {
					for (size_t i = 1; i < tokens.size(); i++) {
						indicesCurrent.push_back(std::stoul(tokens[i]) - 1); //Verts start at 1 and not 0
					}
				}
				//Case  2: VertexPosition/VertexTexture 
				else if (occu == 1) {
					for (size_t i = 1; i < tokens.size(); i++) {
						auto faceComp = getToken(tokens[i], "/");
						indicesCurrent.push_back(std::stoul(faceComp[0]) - 1);
					}
				}
				//Case  3: VertexPosition/VertexTexture/VertexNormal
				// or VertexPosition//VertexNormal
				else if (occu == 2) {

					for (size_t i = 1; i < tokens.size(); i++) {
						auto faceComp = getToken(tokens[i], "/");

						indicesCurrent.push_back(nVar);
						++nVar;

						vertsCurrent.push_back(vertsCollection[std::stoul(faceComp[0]) - 1]);
						texVertsCurrent.push_back(texVertsCollection[std::stoul(faceComp[1]) - 1]);
						normalsCurrent.push_back(normalsCollection[std::stoul(faceComp[2]) - 1]);

					}
				}
			}
		}

		m->IND = indicesCurrent;
		m->VERTS = vertsCurrent;
		m->NORMS = normalsCurrent;
		m->TEXVERTS = texVertsCurrent;
		m->MAT = currentMAT;
		meshes.push_back(m);

		file.close();

		vertsCurrent.clear();
		normalsCurrent.clear();
		texVertsCurrent.clear();

		time->stop();

		std::cout << "Loaded " << vertsCollection.size() << " verts, " << texVertsCollection.size() << " texture verts, " << normalsCollection.size() << " normals" << std::endl;

		std::cout << "Groups: " << std::endl;
		for (auto s : meshes) {
			std::cout << s->NAME << ": " << s->MAT << " Material, "
				<< s->IND.size() << " indices, "
				<< s->VERTS.size() << " verts, "
				<< s->NORMS.size() << " norms, "
				<< s->TEXVERTS.size() << " texverts."
				<< std::endl;
		}

		std::cout << "Loading took " << time->getDelta() / 1000 << " ms (" << time->getDelta() / 1000000 << " s)" << std::endl;

	}

	std::vector<std::shared_ptr<MESH> > meshes;
	std::vector<std::shared_ptr<MATERIAL> > materials;

private:

	std::vector<Vec3f> vertsCollection;
	std::vector<Vec3f> normalsCollection;
	std::vector<Vec2f> texVertsCollection;

	std::vector<Vec3f> vertsCurrent;
	std::vector<Vec3f> normalsCurrent;
	std::vector<Vec2f> texVertsCurrent;
	std::vector<unsigned int> indicesCurrent;

	std::string name;


};