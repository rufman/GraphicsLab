/*
 * Mirror.cpp
 *
 *  Created on: 22.05.2012
 *      Author: gregory
 */


// Class declaration include
#include "Mirror.hpp"

// commons includes
#include "../common/GLIncludes.hpp"
#include "../common/Utils.hpp"
#include "../common/Material.hpp"

#include <cmath>

namespace game_space {

Mirror::Mirror(Scene &scene) :
		Drawable(scene) {
	_width =  glutGet(GLUT_WINDOW_WIDTH);
	_length = glutGet(GLUT_WINDOW_HEIGHT);

	/*
	_material.setAmbient(Color(0.4, 0.4, 0.4));
	_material.setDiffuse(Color(0.6, 0.6, 0.6));
	_material.setSpecular(Color(0.2, 0.2, 0.2));
	_material.setEmission(Color(0.2, 0.2, 0.2));
	_material.setShininess(64);

	int numVertices = _widthResolution * _lengthResolution;
	_vertices = new Point[numVertices];
	_vertexNormals = new Vector3D[numVertices];

	int numTriangles = 2 * (_widthResolution - 1) * (_lengthResolution - 1);
	_triangles = new Triangle[numTriangles];
	_triangleNormals = new Vector3D[numTriangles];

	// Calculate vertices
	for (int widthPoint = 0; widthPoint < _widthResolution; widthPoint++) {
		for (int lengthPoint = 0; lengthPoint < _lengthResolution;
				lengthPoint++) {
			Point &vertex = _vertices[_widthResolution * lengthPoint
					+ widthPoint];
			vertex.x = -_width / 2.0
					+ widthPoint * (_width / (_widthResolution - 1));
			vertex.z = _length / 2.0
					- lengthPoint * (_length / (_lengthResolution - 1));
		}
	}

	// Build triangles
	for (int widthPoint = 0; widthPoint < (_widthResolution - 1);
			widthPoint++) {
		for (int lengthPoint = 0; lengthPoint < (_lengthResolution - 1);
				lengthPoint++) {
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
	*/



	reset();
}

Mirror::~Mirror() {

}

void Mirror::reset() {

}


void Mirror::update(float seconds) {

}

void Mirror::draw() const {
	//uint xSlice = _texture.getWidth() / _widthResolution;
	//uint zSlice = _texture.getHeight() / _lengthResolution;

	glShadeModel(
			_renderingParameters.shadeMode == RenderingParameters::FLAT ?
					GL_FLAT : GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK,
			_renderingParameters.drawMode == RenderingParameters::WIREFRAME ?
					GL_LINE : GL_FILL);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();


	_texture.setActive(true);

	/*glBegin(GL_TRIANGLES);

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
		glTexCoord2f(
				((vertex1.x + _width / 2.0) / (_width / (_widthResolution - 1)))
						* xSlice,
				((-vertex1.z + _length / 2.0)
						/ (_length / (_lengthResolution - 1))) * zSlice);
		glVertex3f(vertex1.x, vertex1.y, vertex1.z);

		if (_renderingParameters.shadeMode == RenderingParameters::SMOOTH) {
			const Vector3D &normal = _vertexNormals[_triangles[t].vertex2];
			glNormal3f(normal.x, normal.y, normal.z);
		}
		glTexCoord2f(
				((vertex2.x + _width / 2.0) / (_width / (_widthResolution - 1)))
						* xSlice,
				((-vertex2.z + _length / 2.0)
						/ (_length / (_lengthResolution - 1))) * zSlice);
		glVertex3f(vertex2.x, vertex2.y, vertex2.z);

		if (_renderingParameters.shadeMode == RenderingParameters::SMOOTH) {
			const Vector3D &normal = _vertexNormals[_triangles[t].vertex3];
			glNormal3f(normal.x, normal.y, normal.z);
		}
		glTexCoord2f(
				((vertex3.x + _width / 2.0) / (_width / (_widthResolution - 1)))
						* xSlice,
				((-vertex3.z + _length / 2.0)
						/ (_length / (_lengthResolution - 1))) * zSlice);
		glVertex3f(vertex3.x, vertex3.y, vertex3.z);
	}

	*/
	glBegin( GL_QUADS );

	glTexCoord2f( 0,0 );
	glVertex2f( _width/2.0 - _width/16.0, _length/16.0 + _length/8.0 );

	glTexCoord2f( _texture.getWidth(), 0 );
	glVertex2f( _width/2.0 + _width/16.0, _length/16.0 + _length/8.0 );

	glTexCoord2f( _texture.getWidth(), _texture.getHeight() );
	glVertex2f( _width/2.0 + _width/16.0, _length/16.0 );

	glTexCoord2f( 0, _texture.getHeight() );
	glVertex2f( _width/2.0 - _width/16.0, _length/16.0 );

	glEnd();

	GLboolean lightingEnabled;
	glGetBooleanv(GL_LIGHTING, &lightingEnabled);
	glDisable(GL_LIGHTING);

	/*if (_renderingParameters.normalMode == RenderingParameters::VERTEX) {
		glColor3f(Color::WHITE.r, Color::WHITE.g, Color::WHITE.b);
		glBegin(GL_LINES);
		for (int widthPoint = 0; widthPoint < _widthResolution; widthPoint++) {
			for (int lengthPoint = 0; lengthPoint < _lengthResolution;
					lengthPoint++) {
				int v = _widthResolution * lengthPoint + widthPoint;
				const Point &vertex = _vertices[v];
				const Vector3D &normal = _vertexNormals[v];
				glVertex3f(vertex.x, vertex.y, vertex.z);
				glVertex3f(vertex.x + normal.x, vertex.y + normal.y,
						vertex.z + normal.z);
			}
		}
		glEnd();
	} else if (_renderingParameters.normalMode
			== RenderingParameters::TRIANGLE) {
		glColor3f(Color::TAN.r, Color::TAN.g, Color::TAN.b);
		glBegin(GL_LINES);
		for (int t = 0; t < numTriangles; t++) {
			const Point &vertex1 = _vertices[_triangles[t].vertex1];
			const Point &vertex2 = _vertices[_triangles[t].vertex2];
			const Point &vertex3 = _vertices[_triangles[t].vertex3];
			Point baryCenter((vertex1.x + vertex2.x + vertex3.x) / 3.0f,
					(vertex1.y + vertex2.y + vertex3.y) / 3.0f,
					(vertex1.z + vertex2.z + vertex3.z) / 3.0f);
			const Vector3D &normal = _triangleNormals[t];

			glVertex3f(baryCenter.x, baryCenter.y, baryCenter.z);
			glVertex3f(baryCenter.x + normal.x, baryCenter.y + normal.y,
					baryCenter.z + normal.z);
		}
		glEnd();
	}

	*/

	if (lightingEnabled)
		glEnable(GL_LIGHTING);

	_texture.setActive( false );
	glPopMatrix();
}


void Mirror::applyCamera( Point tankPosition ) {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0,
			1.0 / 1.0,
			0.1, 2000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(tankPosition.x, tankPosition.y, tankPosition.z,
			tankPosition.x, tankPosition.y, tankPosition.z - 1,
			0,1,0);
}

void Mirror::capture() {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	_texture.capture(0, 0, width, height);
}

}

