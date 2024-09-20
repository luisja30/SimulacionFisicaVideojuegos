#include "Vector3D.h"
#include <cmath>

Vector3D::Vector3D() : x(0), y(0), z(0) {
}

Vector3D::Vector3D(float x, float y, float z) : x(x), y(y), z(z) {
}

float Vector3D::Modulo() {
	return sqrt(x * x + y * y + z * z);
}

void Vector3D::Normalize() {
	x = x / Modulo(); 
	y = y / Modulo(); 
	z = z / Modulo();
}

float Vector3D::Escalar(Vector3D& const other) {
	return x * other.x + y * other.y + z * other.z;
}

Vector3D Vector3D::operator=(const Vector3D& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
}

Vector3D Vector3D::operator+(const Vector3D& other) {
	return Vector3D(x + other.x, y + other.y, z + other.z);
}

Vector3D Vector3D::operator-(const Vector3D& other) {
	return Vector3D(x - other.x, y - other.y, z - other.z);
}



