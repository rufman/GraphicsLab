/**
 * plyModel.cpp
 * This class loads a plyModel to be used as a model in the game.
 */

// Class declaration include
#include "PLYModel.hpp"

//common includes
#include "Exception.hpp"
#include "Utils.hpp"

#include <fstream>
#include <sstream>

namespace game_space {

PLYModel::PLYModel(Scene &scene) :
		Drawable(scene), _numDisplayLists(2), _displayLists(glGenLists(_numDisplayLists)) {
	_material.setAmbient(Color(0.3, 0.3, 0.3));
	_material.setDiffuse(Color(0.5, 0.5, 0.5));
	_material.setSpecular(Color(0.7, 0.7, 0.7));
	_material.setEmission(Color(0.1, 0.1, 0.1));
	_material.setShininess(16);
	_texture=NULL;
}

PLYModel::~PLYModel() {
	glDeleteLists(_displayLists, _numDisplayLists);
}

void PLYModel::load(const std::string &modelFile) {
	_plyData.reset();

	processFile(modelFile);

	buildDisplayLists();
	_angle = 0;
}

void PLYModel::load(const std::string &modelFile, const std::string &textureFile) {
	_texture = new TGATexture(textureFile.c_str());
	load(modelFile);
}

void PLYModel::processFile(const std::string &file) {
	std::ifstream f(file.c_str());
	if (!f.is_open()) {
		std::stringstream error;
		error << "Failed to open file " << file;
		throw Exception(error.str());
	}

	// Read file type
	std::string fileType;
	f >> fileType;
	if (fileType != "ply") {
		std::stringstream error;
		error << "File type " << fileType << " not recognized";
		throw Exception(error.str());
	}

	// Read format: binary/ascii, litte/big endian
	std::string format, mode, version;
	f >> format >> mode >> version;
	if ((format != "format") || ((mode != "ascii") && (mode != "binary_big_endian") && (mode != "binary_little_endian"))) {
		std::stringstream error;
		error << "Format specification invalid: \"" << format << " " << mode << "\"";
		throw Exception(error.str());
	}

	char buffer[256];
	std::string element, elementType;
	uint numVertices, numFaces;
	_plyData.textured = false;
	while (true) {
		f >> element;
		if (element == "comment") {
			f.getline(buffer, 256);
		} else if (element == "element") {
			f >> elementType;
			if (elementType == "vertex") {
				f >> numVertices;
			} else if (elementType == "face") {
				f >> numFaces;
			}
		} else if (element == "property") {
			char type[5];
			char name;
			f >> type;
			f >> name;
			if (name == 's') {
				_plyData.textured = true;
			}
			f.getline(buffer, 256);
		} else if (element == "end_header") {
			break;
		}
	}

	// Read vertices
	for (uint vertexID = 0; vertexID < numVertices; vertexID++) {
		float x, y, z, u, v;
		f >> x >> y >> z;
		if (_plyData.textured) {
			f >> u >> v;
			_plyData.textureCoords.push_back(Point(_texture->getWidth()*u, _texture->getHeight()*v, 0));
		}
		_plyData.vertices.push_back(Point(x, y, z));

		if (x < _plyData.min.x)
			_plyData.min.x = x;
		if (y < _plyData.min.y)
			_plyData.min.y = y;
		if (z < _plyData.min.z)
			_plyData.min.z = z;

		if (x > _plyData.max.x)
			_plyData.max.x = x;
		if (y > _plyData.max.y)
			_plyData.max.y = y;
		if (z > _plyData.max.z)
			_plyData.max.z = z;
	}

	// Scale vertices to be in [0, 1] and y >= 0
	float scaleX = 1.0 / (_plyData.max.x - _plyData.min.x), scaleY = 1.0 / (_plyData.max.y - _plyData.min.y), scaleZ = 1.0 / (_plyData.max.z - _plyData.min.z);
	float scale = (scaleX < scaleY) ? ((scaleZ < scaleX) ? scaleZ : scaleX) : ((scaleZ < scaleY) ? scaleZ : scaleY);
	for (uint vertexID = 0; vertexID < numVertices; vertexID++) {
		Point &vertex = _plyData.vertices[vertexID];
		vertex = Point(vertex.x * scale, (vertex.y - _plyData.min.y) * scale, vertex.z * scale);
	}

	// Read faces
	for (uint faceID = 0; faceID < numFaces; faceID++) {
		uint num, vertex1, vertex2, vertex3;
		f >> num >> vertex1 >> vertex2 >> vertex3;
		_plyData.triangles.push_back(
				PLYData::Triangle(vertex1, vertex2, vertex3));
		_plyData.triangleNeighbors.push_back(PLYData::Neighbors());
		_plyData.triangleVisible.push_back(0);
	}

	// Calculate face normals
	for (uint faceID = 0; faceID < numFaces; faceID++) {
		const PLYData::Triangle &triangle = _plyData.triangles[faceID];
		const Point &vertex1 = _plyData.vertices[triangle.vertex1];
		const Point &vertex2 = _plyData.vertices[triangle.vertex2];
		const Point &vertex3 = _plyData.vertices[triangle.vertex3];

		Vector3D triangleNormal = Utils::normal(vertex1, vertex2, vertex3);
		_plyData.triangleNormals.push_back(triangleNormal);
	}

	// Calculate averaged vertex normals
	for (uint vertexID = 0; vertexID < numVertices; vertexID++) {
		_plyData.vertexNormals.push_back(Vector3D(0.0, 0.0, 0.0));
	}
	for (uint faceID = 0; faceID < numFaces; faceID++) {
		const PLYData::Triangle &triangle = _plyData.triangles[faceID];
		const Vector3D &triangleNormal = _plyData.triangleNormals[faceID];
		_plyData.vertexNormals[triangle.vertex1] += triangleNormal;
		_plyData.vertexNormals[triangle.vertex2] += triangleNormal;
		_plyData.vertexNormals[triangle.vertex3] += triangleNormal;
	}
	for (uint vertexID = 0; vertexID < numVertices; vertexID++) {
		Utils::normalize(_plyData.vertexNormals[vertexID]);
	}

}

void PLYModel::buildDisplayLists() {
	// Flat shading
	glNewList(_displayLists, GL_COMPILE);

	glShadeModel(GL_FLAT);
	_material.setActive();
	if(_plyData.textured&&_texture){
		_texture->setActive(true);
	}
	glBegin(GL_TRIANGLES);

	for (uint triangleID = 0; triangleID < _plyData.triangles.size(); triangleID++) {
		const PLYData::Triangle &triangle = _plyData.triangles[triangleID];

		const Vector3D &triangleNormal = _plyData.triangleNormals[triangleID];

		glNormal3fv(triangleNormal());

		const Point &vertex1 = _plyData.vertices[triangle.vertex1];
		const Point &vertex2 = _plyData.vertices[triangle.vertex2];
		const Point &vertex3 = _plyData.vertices[triangle.vertex3];
		if (_plyData.textured) {
			glTexCoord2f(_plyData.textureCoords[triangle.vertex1].x,_plyData.textureCoords[triangle.vertex1].y);
			glVertex3fv(vertex1());
			glTexCoord2f(_plyData.textureCoords[triangle.vertex2].x,_plyData.textureCoords[triangle.vertex2].y);
			glVertex3fv(vertex2());
			glTexCoord2f(_plyData.textureCoords[triangle.vertex3].x,_plyData.textureCoords[triangle.vertex3].y);
			glVertex3fv(vertex3());
		} else {
			glVertex3fv(vertex1());
			glVertex3fv(vertex2());
			glVertex3fv(vertex3());
		}
	}

	glEnd();
	if(_plyData.textured&&_texture){
		_texture->setActive(false);
	}
	glEndList();

	// Smooth shading
	glNewList(_displayLists + 1, GL_COMPILE);

	glShadeModel(GL_SMOOTH);
	_material.setActive();
	if(_plyData.textured&&_texture){
		_texture->setActive(true);
	}
	glBegin(GL_TRIANGLES);

	for (uint triangleID = 0; triangleID < _plyData.triangles.size(); triangleID++) {
		const PLYData::Triangle &triangle = _plyData.triangles[triangleID];

		const Point &vertex1 = _plyData.vertices[triangle.vertex1];
		const Point &vertex2 = _plyData.vertices[triangle.vertex2];
		const Point &vertex3 = _plyData.vertices[triangle.vertex3];

		const Vector3D &vertexNormal1 = _plyData.vertexNormals[triangle.vertex1];
		const Vector3D &vertexNormal2 = _plyData.vertexNormals[triangle.vertex2];
		const Vector3D &vertexNormal3 = _plyData.vertexNormals[triangle.vertex3];

		if (_plyData.textured) {
					glTexCoord2f(_plyData.textureCoords[triangle.vertex1].x,_plyData.textureCoords[triangle.vertex1].y);
					glNormal3fv(vertexNormal1());
					glVertex3fv(vertex1());
					glTexCoord2f(_plyData.textureCoords[triangle.vertex2].x,_plyData.textureCoords[triangle.vertex2].y);
					glNormal3fv(vertexNormal2());
					glVertex3fv(vertex2());
					glTexCoord2f(_plyData.textureCoords[triangle.vertex3].x,_plyData.textureCoords[triangle.vertex3].y);
					glNormal3fv(vertexNormal3());
					glVertex3fv(vertex3());
				} else {
					glNormal3fv(vertexNormal1());
					glVertex3fv(vertex1());
					glNormal3fv(vertexNormal2());
					glVertex3fv(vertex2());
					glNormal3fv(vertexNormal3());
					glVertex3fv(vertex3());
				}
	}

	glEnd();
	if(_plyData.textured&&_texture){
		_texture->setActive(false);
	}
	glEndList();
}

void PLYModel::draw() const {
	glCallList((_renderingParameters.shadeMode == RenderingParameters::FLAT) ? (_displayLists) : (_displayLists + 1));
}

void PLYModel::setPosition(Point &position) {
	_position = position;
}

Point& PLYModel::getPosition() {
	return _position;
}

std::vector<Point>* PLYModel::getVertices(){
	return &_plyData.vertices;
}

void PLYModel::setNeighbors() {
	//set the neighbors
	unsigned int p1i, p2i, p1j, p2j;
	unsigned int P1i, P2i, P1j, P2j;
	unsigned int i, j, ki, kj;

	for (i = 0; i < _plyData.triangles.size() - 1; i++) {
		for (j = i + 1; j < _plyData.triangles.size(); j++) {
			for (ki = 0; ki < 3; ki++) {
				if (!_plyData.triangleNeighbors.at(i).neigh[ki]) {
					for (kj = 0; kj < 3; kj++) {
						p1i = ki;
						p1j = kj;
						p2i = (ki + 1) % 3;
						p2j = (kj + 1) % 3;

						PLYData::Triangle triangle = _plyData.triangles[i];
						PLYData::Triangle triangle2 = _plyData.triangles[j];
						switch (p1i) {
						case 0:
							p1i = triangle.vertex1;
							break;
						case 1:
							p1i = triangle.vertex2;
							break;
						case 2:
							p1i = triangle.vertex3;
							break;
						}
						switch (p2i) {
						case 0:
							p2i = triangle.vertex1;
							break;
						case 1:
							p2i = triangle.vertex2;
							break;
						case 2:
							p2i = triangle.vertex3;
							break;
						}
						switch (p1j) {
						case 0:
							p1j = triangle2.vertex1;
							break;
						case 1:
							p1j = triangle2.vertex2;
							break;
						case 2:
							p1j = triangle2.vertex3;
							break;
						}
						switch (p2j) {
						case 0:
							p2j = triangle2.vertex1;
							break;
						case 1:
							p2j = triangle2.vertex2;
							break;
						case 2:
							p2j = triangle2.vertex3;
							break;
						}

						P1i = ((p1i + p2i) - abs(p1i - p2i)) / 2;
						P2i = ((p1i + p2i) + abs(p1i - p2i)) / 2;
						P1j = ((p1j + p2j) - abs(p1j - p2j)) / 2;
						P2j = ((p1j + p2j) + abs(p1j - p2j)) / 2;

						if ((P1i == P1j) && (P2i == P2j)) { //they are neighbours
							_plyData.triangleNeighbors[i].neigh[ki] = j + 1;
							_plyData.triangleNeighbors[j].neigh[kj] = i + 1;
						}
					}
				}
			}
		}
	}
}

void PLYModel::castShadow(const Point *lightPosition){
	//set the triangle that are visible from the light
	for (uint i = 0; i < _plyData.triangles.size(); i++) {
		Vector3D triangleNormal = _plyData.triangleNormals[i];
		float side = triangleNormal.x * lightPosition->x
				+ triangleNormal.y * lightPosition->y
				+ triangleNormal.z * lightPosition->z;
		if (side > 0) {
			_plyData.triangleVisible[i] = 1;
		} else {
			_plyData.triangleVisible[i] = 0;
		}
	}

	glPushAttrib(
			GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT
					| GL_POLYGON_BIT | GL_STENCIL_BUFFER_BIT);
	glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_STENCIL_TEST);
	glColorMask(0, 0, 0, 0);
	glStencilFunc(GL_ALWAYS, 1, 0xffffffff);

	// first pass, stencil operation decreases stencil value
	glFrontFace(GL_CCW);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	Point p1;
	Point p2;
	Vector3D v1;
	Vector3D v2;
	int jj;
	for (uint i = 0; i < _plyData.triangles.size(); i++) {
		if (_plyData.triangleVisible[i]) {
			for (uint j = 0; j < 3; j++) {
				//_plyData.triangleNeighbors[i].getArray()[j];
				//_plyData.triangleNeighbors[i]._arr[j]
				int k = _plyData.triangleNeighbors.at(i).neigh[j];
				if ((!k) || (!_plyData.triangleVisible[k - 1])) {
					// here we have an edge, we must draw a polygon
					PLYData::Triangle triangle = _plyData.triangles[i];
					switch (j) {
					case 0:
						p1 = _plyData.vertices[triangle.vertex1];
						break;
					case 1:
						p1 = _plyData.vertices[triangle.vertex2];
						break;
					case 2:
						p1 = _plyData.vertices[triangle.vertex3];
						break;
					}
					jj = (j + 1) % 3;
					switch (jj) {
					case 0:
						p2 = _plyData.vertices[triangle.vertex1];
						break;
					case 1:
						p2 = _plyData.vertices[triangle.vertex2];
						break;
					case 2:
						p2 = _plyData.vertices[triangle.vertex3];
						break;
					}

					//calculate the length of the vector
					v1.x = (p1.x - lightPosition->x) * 100;
					v1.y = (p1.y - lightPosition->y) * 100;
					v1.z = (p1.z - lightPosition->z) * 100;

					v2.x = (p2.x - lightPosition->x) * 100;
					v2.y = (p2.y - lightPosition->y) * 100;
					v2.z = (p2.z - lightPosition->z) * 100;

					//draw the polygon
					glBegin(GL_TRIANGLE_STRIP);
					glVertex3f(p1.x, p1.y, p1.z);
					glVertex3f(p1.x + v1.x, p1.y + v1.y, p1.z + v1.z);

					glVertex3f(p2.x, p2.y, p2.z);
					glVertex3f(p2.x + v2.x, p2.y + v2.y, p2.z + v2.z);
					glEnd();
				}
			}
		}
	}

	// second pass, stencil operation increases stencil value
	glFrontFace(GL_CW);
	glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
	for (uint i = 0; i < _plyData.triangles.size(); i++) {
		if (_plyData.triangleVisible[i]) {
			for (uint j = 0; j < 3; j++) {
				//_plyData.triangleNeighbors[i].getArray()[j];
				//_plyData.triangleNeighbors[i]._arr[j]
				int k = _plyData.triangleNeighbors[i].neigh[j];
				if ((!k) || (!_plyData.triangleVisible[k - 1])) {
					// here we have an edge, we must draw a polygon
					PLYData::Triangle triangle = _plyData.triangles[i];
					switch (j) {
					case 0:
						p1 = _plyData.vertices[triangle.vertex1];
						break;
					case 1:
						p1 = _plyData.vertices[triangle.vertex2];
						break;
					case 2:
						p1 = _plyData.vertices[triangle.vertex3];
						break;
					}
					jj = (j + 1) % 3;
					switch (jj) {
					case 0:
						p2 = _plyData.vertices[triangle.vertex1];
						break;
					case 1:
						p2 = _plyData.vertices[triangle.vertex2];
						break;
					case 2:
						p2 = _plyData.vertices[triangle.vertex3];
						break;
					}

					//calculate the length of the vector
					v1.x = (p1.x - lightPosition->x) * 100;
					v1.y = (p1.y - lightPosition->y) * 100;
					v1.z = (p1.z - lightPosition->z) * 100;

					v2.x = (p2.x - lightPosition->x) * 100;
					v2.y = (p2.y - lightPosition->y) * 100;
					v2.z = (p2.z - lightPosition->z) * 100;

					//draw the polygon
					glBegin(GL_TRIANGLE_STRIP);
					glVertex3f(p1.x, p1.y, p1.z);
					glVertex3f(p1.x + v1.x, p1.y + v1.y, p1.z + v1.z);

					glVertex3f(p2.x, p2.y, p2.z);
					glVertex3f(p2.x + v2.x, p2.y + v2.y, p2.z + v2.z);
					glEnd();
				}
			}
		}
	}
	glFrontFace(GL_CCW);
	glColorMask(1, 1, 1, 1);

	//draw a shadowing rectangle covering the entire screen
	glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glStencilFunc(GL_NOTEQUAL, 0, 0xffffffff);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(-0.2f, 0.1f, -0.10f);
	glVertex3f(-0.2f, -0.1f, -0.10f);
	glVertex3f(0.2f, 0.1f, -0.10f);
	glVertex3f(0.2f, -0.1f, -0.10f);
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);

	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glEnable(GL_LIGHTING);
	glDisable(GL_STENCIL_TEST);
}

}
