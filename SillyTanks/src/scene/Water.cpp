/**
 * Water.cpp
This class creates the water that is starting at a certain level.
 */

// Class declaration include
#include "Water.hpp"

// commons includes
#include "../common/GLIncludes.hpp"
#include "../common/Utils.hpp"

#include <cmath>

namespace game_space {

Water::Water(Scene &scene, float waterHeight, float width, float length) :
		Drawable(scene), _displacement(0.0f), _width(width), _length(length), _widthResolution(30), _lengthResolution(30) {
	_material.setAmbient(Color(0.4, 0.4, 0.4));
	_material.setDiffuse(Color(0.6, 0.6, 0.6));
	_material.setSpecular(Color(0.2, 0.2, 0.2));
	_material.setEmission(Color(0.2, 0.2, 0.2));
	_material.setShininess(64);

	waterHeightOffset = waterHeight;

	int numVertices = _widthResolution * _lengthResolution;
	_vertices = new Point[numVertices];
	_vertexNormals = new Vector3D[numVertices];

	int numTriangles = 2 * (_widthResolution - 1) * (_lengthResolution - 1);
	_triangles = new Triangle[numTriangles];
	_triangleNormals = new Vector3D[numTriangles];

	// Calculate vertices
	for (int widthPoint = 0; widthPoint < _widthResolution; widthPoint++) {
		for (int lengthPoint = 0; lengthPoint < _lengthResolution; lengthPoint++) {
			Point &vertex = _vertices[_widthResolution * lengthPoint + widthPoint];
			vertex.x = -_width / 2.0 + widthPoint * (_width / (_widthResolution - 1));
			vertex.z = _length / 2.0 - lengthPoint * (_length / (_lengthResolution - 1));
		}
	}

	// Build triangles
	for (int widthPoint = 0; widthPoint < (_widthResolution - 1); widthPoint++) {
		for (int lengthPoint = 0; lengthPoint < (_lengthResolution - 1); lengthPoint++) {
			int t1 = 2 * lengthPoint * (_widthResolution - 1) + widthPoint * 2;
			int t2 = t1 + 1;
			Triangle &triangle1 = _triangles[t1];
			Triangle &triangle2 = _triangles[t2];

			int v1 = (lengthPoint) * _widthResolution + (widthPoint);
			int v2 = v1 + 1;
			int v3 = v2 + _widthResolution;
			int v4 = v3 - 1;
			triangle1.vertex1 = v1;
			triangle1.vertex2 = v2;
			triangle1.vertex3 = v3;
			triangle2.vertex1 = v1;
			triangle2.vertex2 = v3;
			triangle2.vertex3 = v4;
		}
	}

	reset();
}

Water::~Water() {
	delete[] _vertices;
	delete[] _vertexNormals;
	delete[] _triangles;
	delete[] _triangleNormals;
}

void Water::reset() {
	_displacement = 0.0f;
	calculateVertexHeights();
	calculateNormals();
}

void Water::calculateVertexHeights() {
	// Calculate vertices
	for (int widthPoint = 0; widthPoint < _widthResolution; widthPoint++) {
		for (int lengthPoint = 0; lengthPoint < _lengthResolution; lengthPoint++) {
			Point &vertex = _vertices[_widthResolution * lengthPoint + widthPoint];
			vertex.y = 3*std::sin((float) widthPoint + _displacement) - std::cos((float) lengthPoint + _displacement) - waterHeightOffset;
		}
	}
}

void Water::calculateNormals() {
	// Calculate triangle normals
	for (int widthPoint = 0; widthPoint < (_widthResolution - 1); widthPoint++) {
		for (int lengthPoint = 0; lengthPoint < (_lengthResolution - 1); lengthPoint++) {
			int t1 = 2 * lengthPoint * (_widthResolution - 1) + widthPoint * 2;
			int t2 = t1 + 1;
			Triangle &triangle1 = _triangles[t1];
			Triangle &triangle2 = _triangles[t2];

			_triangleNormals[t1] = Utils::normal(_vertices[triangle1.vertex1], _vertices[triangle1.vertex2], _vertices[triangle1.vertex3]);
			_triangleNormals[t2] = Utils::normal(_vertices[triangle2.vertex1], _vertices[triangle2.vertex2], _vertices[triangle2.vertex3]);
		}
	}

	// Calculate vertex normals
	for (int widthPoint = 0; widthPoint < _widthResolution; widthPoint++) {
		for (int lengthPoint = 0; lengthPoint < _lengthResolution; lengthPoint++) {
			int v = _widthResolution * lengthPoint + widthPoint;
			Vector3D &normal = _vertexNormals[v];
			normal = Vector3D(0.0, 0.0, 0.0);

			int t1 = (widthPoint > 0 && lengthPoint > 0) ? (2 * (lengthPoint - 1) * (_widthResolution - 1) + 2 * (widthPoint - 1)) : -1, t2 = (widthPoint > 0 && lengthPoint > 0) ? (t1 + 1) : -1, t3 = (widthPoint < (_widthResolution - 1) && lengthPoint > 0) ? (2 * (lengthPoint - 1) * (_widthResolution - 1) + 2 * (widthPoint) + 1) : -1, t4 =
					(widthPoint > 0 && lengthPoint < (_lengthResolution - 1)) ? (2 * (lengthPoint) * (_widthResolution - 1) + 2 * (widthPoint - 1)) : -1, t5 = (widthPoint < (_widthResolution - 1) && lengthPoint < (_lengthResolution - 1)) ? (2 * (lengthPoint) * (_widthResolution - 1) + 2 * (widthPoint)) : -1, t6 =
					(widthPoint < (_widthResolution - 1) && lengthPoint < (_lengthResolution - 1)) ? (t5 + 1) : -1;
			if (t1 >= 0)
				normal += _triangleNormals[t1];
			if (t2 >= 0)
				normal += _triangleNormals[t2];
			if (t3 >= 0)
				normal += _triangleNormals[t3];
			if (t4 >= 0)
				normal += _triangleNormals[t4];
			if (t5 >= 0)
				normal += _triangleNormals[t5];
			if (t6 >= 0)
				normal += _triangleNormals[t6];

			Utils::normalize(normal);
		}
	}
}

float Water::getHeight(const Point &point) const {
	int numTriangles = 2 * (_widthResolution - 1) * (_lengthResolution - 1);
	int triangle_id;
	float point_y;
	for (int i = 0; i < numTriangles; i++) {
		Vector3D A = Vector3D(_vertices[_triangles[i].vertex1].x, _vertices[_triangles[i].vertex1].z, 0);
		Vector3D B = Vector3D(_vertices[_triangles[i].vertex2].x, _vertices[_triangles[i].vertex2].z, 0);
		Vector3D C = Vector3D(_vertices[_triangles[i].vertex3].x, _vertices[_triangles[i].vertex3].z, 0);
		Vector3D P = Vector3D(point.x, point.z, 0);

		Vector3D v0 = Vector3D(B.x - A.x, B.y - A.y, B.z - A.z);
		Vector3D v1 = Vector3D(C.x - A.x, C.y - A.y, C.z - A.z);
		Vector3D v2 = Vector3D(P.x - A.x, P.y - A.y, P.z - A.z);

		float dot00 = Utils::dot(v0, v0);
		float dot01 = Utils::dot(v0, v1);
		float dot02 = Utils::dot(v0, v2);
		float dot11 = Utils::dot(v1, v1);
		float dot12 = Utils::dot(v1, v2);

		// Compute barycentric coordinates
		float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
		float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

		// Check if point is in triangle
		if ((u >= 0) && (v >= 0) && (u + v < 1)) {
			triangle_id = i;
			break;
		}
	}

	point_y = (_triangleNormals[triangle_id].x * (point.x - _vertices[_triangles[triangle_id].vertex1].x) + _triangleNormals[triangle_id].z * (point.z - _vertices[_triangles[triangle_id].vertex1].z)) / -_triangleNormals[triangle_id].y + _vertices[_triangles[triangle_id].vertex1].y;

	return point_y;
}

float Water::getHeightOffset() const {
	return waterHeightOffset;
}

Vector3D Water::getNormal(const Point &point) const {
	int numTriangles = 2 * (_widthResolution - 1) * (_lengthResolution - 1);
	int triangle_id;
	for (int i = 0; i < numTriangles; i++) {
		Vector3D A = Vector3D(_vertices[_triangles[i].vertex1].x, _vertices[_triangles[i].vertex1].z, 0);
		Vector3D B = Vector3D(_vertices[_triangles[i].vertex2].x, _vertices[_triangles[i].vertex2].z, 0);
		Vector3D C = Vector3D(_vertices[_triangles[i].vertex3].x, _vertices[_triangles[i].vertex3].z, 0);
		Vector3D P = Vector3D(point.x, point.z, 0);

		Vector3D v0 = Vector3D(B.x - A.x, B.y - A.y, B.z - A.z);
		Vector3D v1 = Vector3D(C.x - A.x, C.y - A.y, C.z - A.z);
		Vector3D v2 = Vector3D(P.x - A.x, P.y - A.y, P.z - A.z);

		float dot00 = Utils::dot(v0, v0);
		float dot01 = Utils::dot(v0, v1);
		float dot02 = Utils::dot(v0, v2);
		float dot11 = Utils::dot(v1, v1);
		float dot12 = Utils::dot(v1, v2);

		// Compute barycentric coordinates
		float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
		float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

		// Check if point is in triangle
		if ((u >= 0) && (v >= 0) && (u + v < 1)) {
			triangle_id = i;
			break;
		}
	}

	return Vector3D(_triangleNormals[triangle_id].x, _triangleNormals[triangle_id].y, _triangleNormals[triangle_id].z);
}

void Water::update(float seconds) {
	_displacement += seconds;
	if (_displacement > (2 * Utils::PI))
		_displacement -= 2 * Utils::PI;

	calculateVertexHeights();
	calculateNormals();
}

void Water::draw() const {
	uint xSlice = _texture.getWidth() / _widthResolution;
	uint zSlice = _texture.getHeight() / _lengthResolution;

	glShadeModel(_renderingParameters.shadeMode == RenderingParameters::FLAT ? GL_FLAT : GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, _renderingParameters.drawMode == RenderingParameters::WIREFRAME ? GL_LINE : GL_FILL);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glColor4f(1.0f,1.0f,1.0f,0.5f);         // Full Brightness, 50% Alpha ( NEW )
	glBlendFunc (GL_SRC_COLOR, GL_DST_ALPHA);     // Blending Function For Translucency Based On Source Alpha

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	_material.setActive();
	_texture.setActive(true);

	glBegin(GL_TRIANGLES);

	int numTriangles = 2 * (_widthResolution - 1) * (_lengthResolution - 1);
	for (int t = 0; t < numTriangles; t++) {
		if (_renderingParameters.shadeMode == RenderingParameters::FLAT) {
			const Vector3D &normal = _triangleNormals[t];
			glNormal3f(normal.x, normal.y, normal.z);
		}

		const Point &vertex1 = _vertices[_triangles[t].vertex1];
		const Point &vertex2 = _vertices[_triangles[t].vertex2];
		const Point &vertex3 = _vertices[_triangles[t].vertex3];

		if (_renderingParameters.shadeMode == RenderingParameters::SMOOTH) {
			const Vector3D &normal = _vertexNormals[_triangles[t].vertex1];
			glNormal3f(normal.x, normal.y, normal.z);
		}
		glTexCoord2f(_texture.getWidth()-((vertex1.x + _width / 2.0) / (_width / (_widthResolution - 1))) * xSlice,((-vertex1.z + _length / 2.0) / (_length / (_lengthResolution - 1))) * zSlice);
		glVertex3f(vertex1.x, vertex1.y, vertex1.z);

		if (_renderingParameters.shadeMode == RenderingParameters::SMOOTH) {
			const Vector3D &normal = _vertexNormals[_triangles[t].vertex2];
			glNormal3f(normal.x, normal.y, normal.z);
		}
		glTexCoord2f(_texture.getWidth()-((vertex2.x + _width / 2.0) / (_width / (_widthResolution - 1))) * xSlice,((-vertex2.z + _length / 2.0) / (_length / (_lengthResolution - 1))) * zSlice);
		glVertex3f(vertex2.x, vertex2.y, vertex2.z);

		if (_renderingParameters.shadeMode == RenderingParameters::SMOOTH) {
			const Vector3D &normal = _vertexNormals[_triangles[t].vertex3];
			glNormal3f(normal.x, normal.y, normal.z);
		}
		glTexCoord2f(_texture.getWidth()-((vertex3.x + _width / 2.0) / (_width / (_widthResolution - 1))) * xSlice,((-vertex3.z + _length / 2.0) / (_length / (_lengthResolution - 1))) * zSlice);
		glVertex3f(vertex3.x, vertex3.y, vertex3.z);
	}

	glEnd();

	GLboolean lightingEnabled;
	glGetBooleanv(GL_LIGHTING, &lightingEnabled);
	glDisable(GL_LIGHTING);

	if (_renderingParameters.normalMode == RenderingParameters::VERTEX) {
		glColor3f(Color::WHITE.r, Color::WHITE.g, Color::WHITE.b);
		glBegin(GL_LINES);
		for (int widthPoint = 0; widthPoint < _widthResolution; widthPoint++) {
			for (int lengthPoint = 0; lengthPoint < _lengthResolution; lengthPoint++) {
				int v = _widthResolution * lengthPoint + widthPoint;
				const Point &vertex = _vertices[v];
				const Vector3D &normal = _vertexNormals[v];
				glVertex3f(vertex.x, vertex.y, vertex.z);
				glVertex3f(vertex.x + normal.x, vertex.y + normal.y, vertex.z + normal.z);
			}
		}
		glEnd();
	} else if (_renderingParameters.normalMode == RenderingParameters::TRIANGLE) {
		glColor3f(Color::TAN.r, Color::TAN.g, Color::TAN.b);
		glBegin(GL_LINES);
		for (int t = 0; t < numTriangles; t++) {
			const Point &vertex1 = _vertices[_triangles[t].vertex1];
			const Point &vertex2 = _vertices[_triangles[t].vertex2];
			const Point &vertex3 = _vertices[_triangles[t].vertex3];
			Point baryCenter((vertex1.x + vertex2.x + vertex3.x) / 3.0f, (vertex1.y + vertex2.y + vertex3.y) / 3.0f, (vertex1.z + vertex2.z + vertex3.z) / 3.0f);
			const Vector3D &normal = _triangleNormals[t];

			glVertex3f(baryCenter.x, baryCenter.y, baryCenter.z);
			glVertex3f(baryCenter.x + normal.x, baryCenter.y + normal.y, baryCenter.z + normal.z);
		}
		glEnd();
	}

	if (lightingEnabled)
		glEnable(GL_LIGHTING);

	_texture.setActive(false);
	glPopMatrix();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

/**
 * Mirroring functions
 */
void Water::applyCamera() const {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, 1.0 / 1.0, 0.1, 2000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 5, 0, 0, 10, 0, 0, 0, 1);
}

void Water::capture() {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	_texture.capture(0, 0, width, height);
}


}
