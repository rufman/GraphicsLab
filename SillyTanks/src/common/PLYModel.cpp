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
}

PLYModel::~PLYModel() {
	glDeleteLists(_displayLists, _numDisplayLists);
}

void PLYModel::load(const std::string &file) {
	_plyData.reset();

	processFile(file);

	buildDisplayLists();
	_angle = 0;
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
			_plyData.textureCoords.push_back(Point(u, v, 0));
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
		_plyData.triangles.push_back(PLYData::Triangle(vertex1, vertex2, vertex3));
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

	glEndList();

	// Smooth shading
	glNewList(_displayLists + 1, GL_COMPILE);

	glShadeModel(GL_SMOOTH);
	_material.setActive();

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

}
