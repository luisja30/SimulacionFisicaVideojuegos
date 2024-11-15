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

//Realizar el producto escalar de este vector por otro
float Vector3D::ProductoEscalar(Vector3D& const other) {
	return x * other.x + y * other.y + z * other.z;
}

Vector3D& Vector3D::operator=(const Vector3D& other) {
	x = other.x; y = other.y; z = other.z;
	return *this;
}

Vector3D Vector3D::operator+(const Vector3D& other) const {
	return Vector3D(x + other.x, y + other.y, z + other.z);
}

Vector3D Vector3D::operator-(const Vector3D& other) const {
	return Vector3D(x - other.x, y - other.y, z - other.z);
}

Vector3D Vector3D::operator*(const float& e) const {
	return Vector3D(x * e, y * e, z * e);
}

float Vector3D::getX() const {
	return x;
}

float Vector3D::getY() const{
	return y;
}

float Vector3D::getZ() const{
	return z;
}
