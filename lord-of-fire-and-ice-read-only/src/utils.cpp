/**
 * utils.cpp
 * 
 * Author   :   Balz Caflisch, Nico Colic, Jan Meier, Henry Raymond, Fatih Erol
 * Date     :   07.05.2011
 *
 * All rights reserved.
 */

// Class declaration include
#include "utils.hpp"
#include "gl_includes.hpp"

// Includes
#include <cmath>


GRAPHICSLAB_NAMESPACE_BEGIN

const float Utils::PI = 3.14159265;

float Utils::toRadian( float degree )
{
    return ( degree*PI/180.0);
}

float Utils::toDegree( float radian )
{
    return ( radian*180.0/PI );
}


float Utils::length( const vec3f &v )
{
    return std::sqrt( v.x()*v.x() + v.y()*v.y() + v.z()*v.z() );
}

void Utils::normalize( vec3f &v )
{
    float l = length( v );
    v = vec3f( v.x()/l, v.y()/l, v.z()/l );
}


float Utils::dot( const vec3f &v1, const vec3f &v2 )
{
    return ( v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z() );
}


vec3f Utils::cross( const vec3f &v1, const vec3f &v2 )
{
    vec3f result( v1.y() * v2.z() - v1.z() * v2.y(), v1.z() * v2.x() - v1.x() * v2.z(), v1.x() * v2.y() - v1.y() * v2.x() );
    return result;
}

vec3f Utils::normal( const Point &p1, const Point &p2, const Point &p3 )
{
    vec3f v1( p2.x - p1.x, p2.y - p1.y, p2.z - p1.z );
    vec3f v2( p3.x - p1.x, p3.y - p1.y, p3.z - p1.z );
    
    vec3f result = cross( v1, v2 );
    normalize( result );
    
    return result;
}

vec3f Utils::normal( const vec3f &p1, const vec3f &p2, const vec3f &p3 )
{
    return normal( Point( p1.x(), p1.y(), p1.z() ), Point( p2.x(), p2.y(), p2.z() ), Point( p3.x(), p3.y(), p3.z() ) );
}

float Utils::getAzimuth(float fromX, float fromY, float toX, float toY) {
	float diffX = toX - fromX;
	float diffY = toY - fromY;
	if(diffX == 0.0 && diffY == 0.0) {
		return 0.0;
	}
	float angle = toDegree(acos(-diffY / sqrt(diffX * diffX + diffY * diffY)));
	if(diffX < 0.0) {
		angle *= -1;
	}
	return angle;
}

float Utils::angleDifference(float alpha, float beta) {
	float difference = alpha - beta;
	while(difference > 180.0) {
		difference -= 360.0;
	}
	while(difference <= -180.0) {
		difference += 360.0;
	}
	return difference;
}

int Utils::abs(int value) {
	if(value < 0) {
		value *= -1;
	}
	return value;
}


float Utils::abs(float value) {
	if(value < 0.0) {
		value *= -1.0;
	}
	return value;
}


float Utils::distance(float x1, float y1, float x2, float y2) {
	return sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
}


vec3f Utils::worldToScreen(float worldX, float worldY, float worldZ) {
	double screenX, screenY, screenZ;

	GLdouble modelview[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

	GLdouble projection[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	gluProject(worldX, worldY, worldZ, modelview, projection, viewport, &screenX, &screenY, &screenZ);

	vec3f result(screenX, screenY, screenZ);
	return result;
}


void Utils::drawTexturedCylinder(float radius1, float radius2, float height, int slices, float textureX, float textureY, float textureW, float textureH, bool covers) {
	float vertexTextureX1, vertexTextureX2, vertexTextureY;
	float longitude0, longitude1, sin0, cos0, sin1, cos1;
	for(int i = 1; i <= slices; i++) {
		longitude0 = 2 * 3.14159 * ((float)(i - 1) / (float)slices);
		sin0  = sin(longitude0);
		cos0 =  cos(longitude0);

		longitude1 = 2 * 3.14159 * ((float)i / (float)slices);
		sin1 = sin(longitude1);
		cos1 = cos(longitude1);

		vertexTextureX1 = textureX + (((float)i - 1.0) / (float)slices) * textureW;
		vertexTextureX2 = textureX + ((float)i / (float)slices) * textureW;

		vertexTextureY = textureY + textureH;

		if(covers) {
			glBegin(GL_TRIANGLES);

			glTexCoord2f(vertexTextureX2, vertexTextureY);
			glNormal3f(0.0, -1.0, 0.0);
			glVertex3f(0.0, 0.0, 0.0);

			glTexCoord2f(vertexTextureX1, vertexTextureY);
			glNormal3f(0.0, -1.0, 0.0);
			glVertex3f(cos0 * radius1, 0.0, sin0 * radius1);

			glTexCoord2f(vertexTextureX2, vertexTextureY);
			glNormal3f(0.0, -1.0, 0.0);
			glVertex3f(cos1 * radius1, 0.0, sin1 * radius1);

			glEnd();
		}

		glBegin(GL_QUADS);

		glTexCoord2f(vertexTextureX1, vertexTextureY);
		glNormal3f(cos0, 0.0, sin0);
		glVertex3f(cos0 * radius1, 0.0, sin0 * radius1);

		glTexCoord2f(vertexTextureX2, vertexTextureY);
		glNormal3f(cos1, 0.0, sin1);
		glVertex3f(cos1 * radius1, 0.0, sin1 * radius1);

		vertexTextureY = textureY;

		glTexCoord2f(vertexTextureX2, vertexTextureY);
		glNormal3f(cos1, 0.0, sin1);
		glVertex3f(cos1 * radius2, height, sin1 * radius2);

		glTexCoord2f(vertexTextureX1, vertexTextureY);
		glNormal3f(cos0, 0.0, sin0);
		glVertex3f(cos0 * radius2, height, sin0 * radius2);

		glEnd();

		if(covers) {
			glBegin(GL_TRIANGLES);

			glTexCoord2f(vertexTextureX1, vertexTextureY);
			glNormal3f(0.0, 1.0, 0.0);
			glVertex3f(cos0 * radius2, height, sin0 * radius2);

			glTexCoord2f(vertexTextureX2, vertexTextureY);
			glNormal3f(0.0, 1.0, 0.0);
			glVertex3f(cos1 * radius2, height, sin1 * radius2);

			glTexCoord2f(vertexTextureX1, vertexTextureY);
			glNormal3f(0.0, 1.0, 0.0);
			glVertex3f(0.0, height, 0.0);

			glEnd();
		}
	}
}


void Utils::drawTexturedHalfCylinder(float radius1, float radius2, float height, int slices, float textureX, float textureY, float textureW, float textureH, bool covers) {
	float vertexTextureX1, vertexTextureX2, vertexTextureY;
	float longitude0, longitude1, sin0, cos0, sin1, cos1;
	for(int i = 1; i <= slices; i++) {
		longitude0 = 3.14159 * ((float)(i - 1) / (float)slices);
		sin0  = sin(longitude0);
		cos0 =  cos(longitude0);

		longitude1 = 3.14159 * ((float)i / (float)slices);
		sin1 = sin(longitude1);
		cos1 = cos(longitude1);

		vertexTextureX1 = textureX + (((float)i - 1.0) / (float)slices) * textureW * 0.5;
		vertexTextureX2 = textureX + ((float)i / (float)slices) * textureW * 0.5;

		vertexTextureY = textureY + textureH;

		if(covers) {
			glBegin(GL_TRIANGLES);

			glTexCoord2f(vertexTextureX2, vertexTextureY);
			glNormal3f(0.0, -1.0, 0.0);
			glVertex3f(0.0, 0.0, 0.0);

			glTexCoord2f(vertexTextureX1, vertexTextureY);
			glNormal3f(0.0, -1.0, 0.0);
			glVertex3f(cos0 * radius1, 0.0, sin0 * radius1);

			glTexCoord2f(vertexTextureX2, vertexTextureY);
			glNormal3f(0.0, -1.0, 0.0);
			glVertex3f(cos1 * radius1, 0.0, sin1 * radius1);

			glEnd();
		}

		glBegin(GL_QUADS);

		glTexCoord2f(vertexTextureX1, vertexTextureY);
		glNormal3f(cos0, 0.0, sin0);
		glVertex3f(cos0 * radius1, 0.0, sin0 * radius1);

		glTexCoord2f(vertexTextureX2, vertexTextureY);
		glNormal3f(cos1, 0.0, sin1);
		glVertex3f(cos1 * radius1, 0.0, sin1 * radius1);

		vertexTextureY = textureY;

		glTexCoord2f(vertexTextureX2, vertexTextureY);
		glNormal3f(cos1, 0.0, sin1);
		glVertex3f(cos1 * radius2, height, sin1 * radius2);

		glTexCoord2f(vertexTextureX1, vertexTextureY);
		glNormal3f(cos0, 0.0, sin0);
		glVertex3f(cos0 * radius2, height, sin0 * radius2);

		glEnd();

		if(covers) {
			glBegin(GL_TRIANGLES);

			glTexCoord2f(vertexTextureX1, vertexTextureY);
			glNormal3f(0.0, 1.0, 0.0);
			glVertex3f(cos0 * radius2, height, sin0 * radius2);

			glTexCoord2f(vertexTextureX2, vertexTextureY);
			glNormal3f(0.0, 1.0, 0.0);
			glVertex3f(cos1 * radius2, height, sin1 * radius2);

			glTexCoord2f(vertexTextureX1, vertexTextureY);
			glNormal3f(0.0, 1.0, 0.0);
			glVertex3f(0.0, height, 0.0);

			glEnd();
		}
	}

	glBegin(GL_QUADS);

	glTexCoord2f(textureX + textureW, textureY + textureH);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(-radius1, 0.0, 0.0);

	glTexCoord2f(textureX + (textureW * 0.5), textureY + textureH);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(radius1, 0.0, 0.0);

	glTexCoord2f(textureX + (textureW * 0.5), textureY);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(radius2, height, 0.0);

	glTexCoord2f(textureX + textureW, textureY);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(-radius2, height, 0.0);

	glEnd();

}


void Utils::drawTexturedSphere(float radius, int slices, int stacks, float textureX, float textureY, float textureW, float textureH) {
	float vertexTextureX, vertexTextureY;
	float longitude0, longitude1, sin0, cos0, sin1, cos1;
	for(int i = 1; i <= slices; i++) {
		longitude0 = 2 * 3.14159 * ((float)(i - 1) / (float)slices);
		sin0  = sin(longitude0);
		cos0 =  cos(longitude0);

		longitude1 = 2 * 3.14159 * ((float)i / (float)slices);
		sin1 = sin(longitude1);
		cos1 = cos(longitude1);

		glBegin(GL_QUAD_STRIP);
		for(int j = 0; j <= stacks; j++) {
			float latitude = 3.14159 * ((float)j / (float)stacks);
			float x = sin(latitude);
			float y = cos(latitude);
			vertexTextureX = textureX + (((float)i - 1.0) / (float)slices) * textureW;
			vertexTextureY = textureY + ((float)j / (float)stacks) * textureH;
			glTexCoord2f(vertexTextureX, vertexTextureY);
			glNormal3f(x * cos0, y, x * sin0);
			glVertex3f(x * cos0 * radius, y * radius, x * sin0 * radius);

			vertexTextureX = textureX + ((float)i / (float)slices) * textureW;
			vertexTextureY = textureY + ((float)j / (float)stacks) * textureH;
			glTexCoord2f(vertexTextureX, vertexTextureY);
			glNormal3f(x * cos1, y, x * sin1);
			glVertex3f(x * cos1 * radius, y * radius, x * sin1 * radius);
		}
		glEnd();
	}
}

uchar Utils::getByteOfFloat(int byte, float number) {
	if(byte == 0) {
		return (uchar)((int)(number / 256) % 256);
	}
	if(byte == 1) {
		return (uchar)((int)(number) % 256);
	}
	if(byte == 2) {
		return (uchar)((int)(number * 256.0) % 256);
	}
}

float Utils::getFloatFromBytes(uchar byte0, uchar byte1, uchar byte2) {
	return ((float)byte0 * 256.0) + (float)byte1 + ((float)byte2 / 256.0);
}


GRAPHICSLAB_NAMESPACE_END


