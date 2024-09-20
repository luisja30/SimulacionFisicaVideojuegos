#pragma once
class Vector3D {
protected:
	float x, y, z;
public:
	Vector3D();
	Vector3D(float x, float y, float z);

	float Modulo();
	void Normalize();
	float Escalar(Vector3D& const other);

	Vector3D operator=(const Vector3D& other);
	Vector3D operator+(const Vector3D& other);
	Vector3D operator-(const Vector3D& other);
	Vector3D operator*(const Vector3D& other);

};

