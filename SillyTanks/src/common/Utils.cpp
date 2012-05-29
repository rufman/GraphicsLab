/**
 * utils.cpp
 * Utilities used in this game, most of them are used for 3D graphics operations and measuring distances.
 */

// Class declaration include
#include "Utils.hpp"

// Includes
#include "GLIncludes.hpp"

#include <cmath>

namespace game_space {

const float Utils::PI = 3.14159265f;

float Utils::toRadian(float degree) {
	return (degree * PI / 180.0f);
}

float Utils::toDegree(float radian) {
	return (radian * 180.0f / PI);
}

float Utils::distance(const Point &p1, const Point &p2) {
	float xDiff = p2.x - p1.x;
	float yDiff = p2.y - p1.y;
	float zDiff = p2.z - p1.z;

	return std::sqrt(xDiff * xDiff + yDiff * yDiff + zDiff * zDiff);
}

float Utils::length(const Vector3D &v) {
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

void Utils::normalize(Vector3D &v) {
	float l = length(v);
	v /= l;
}

float Utils::dot(const Vector3D &v1, const Vector3D &v2) {
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

Vector3D Utils::cross(const Vector3D &v1, const Vector3D &v2) {
	Vector3D result(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
	return result;
}

Vector3D Utils::normal(const Point &p1, const Point &p2, const Point &p3) {
	Vector3D v1(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
	Vector3D v2(p3.x - p1.x, p3.y - p1.y, p3.z - p1.z);

	Vector3D result = cross(v1, v2);
	normalize(result);

	return result;
}

Vector3D Utils::rotate(float angle, const Vector3D &v, const Vector3D &axis) {
	if (angle == 0)
		return Vector3D(v);

	float cosTheta = std::cos(toRadian(angle));
	float sinTheta = std::sin(toRadian(angle));

	Vector3D v_ = v;
	normalize(v_);
	Vector3D axis_ = axis;
	normalize(axis_);

	Vector3D result(((1.0 - cosTheta) * axis_.x * axis_.x + cosTheta) * v_.x + ((1.0 - cosTheta) * axis_.x * axis_.y + sinTheta * axis_.z) * v_.y + ((1.0 - cosTheta) * axis_.x * axis_.z - sinTheta * axis_.y) * v_.z,
			((1.0 - cosTheta) * axis_.x * axis_.y - sinTheta * axis_.z) * v_.x + ((1.0 - cosTheta) * axis_.y * axis_.y + cosTheta) * v_.y + ((1.0 - cosTheta) * axis_.y * axis_.z + sinTheta * axis_.x) * v_.z,
			((1.0 - cosTheta) * axis_.x * axis_.z + sinTheta * axis_.y) * v_.x + ((1.0 - cosTheta) * axis_.y * axis_.z - sinTheta * axis_.x) * v_.y + ((1.0 - cosTheta) * axis_.z * axis_.z + cosTheta) * v_.z);
	normalize(result);

	result *= length(v);

	return result;
}

void Utils::applyGLRotation(const Vector3D &v1, const Vector3D &v2) {
	Vector3D vec1 = v1, vec2 = v2;
	normalize(vec1);
	normalize(vec2);
	float angle = std::acos(Utils::dot(vec1, vec2));
	if (angle == 0)
		return;

	Vector3D rotateAxis = Utils::cross(vec1, vec2);
	Utils::normalize(rotateAxis);
	glRotatef(Utils::toDegree(angle), rotateAxis.x, rotateAxis.y, rotateAxis.z);
}

float Utils::getElevation(Point startPosition, Point targetPosition, float velocity, bool mortarAim, float weight) {
	//remove the third component and make it the same height
	Point targetPosFlat = targetPosition;
	targetPosFlat.y = startPosition.y;

	//the distance given a flat situation
	float flatDistance = Utils::distance(startPosition, targetPosFlat);

	float heightDifference = targetPosition.y - startPosition.y;

	//g is how much the gravitational acceleration is influencing the object ( g * weight of the object
	float g = GRAVITATIONAL_ACCELERATION * weight;

	float r1 = pow(velocity, 4) - g * (g * flatDistance * flatDistance + 2 * heightDifference * velocity * velocity);

	//if r1 is negative, the object is out of reach
	if (r1 < 0) {
		return -1;
	} else {
		r1 = sqrt(r1);
	}

	float a1;
	if(mortarAim)
	{
		a1 = ((velocity * velocity) + r1) / (g * flatDistance);
	}
	else
	{
		a1 = ((velocity * velocity) - r1) / (g * flatDistance);
	}


	return std::abs(Utils::toDegree(atan(a1)));

}
}
