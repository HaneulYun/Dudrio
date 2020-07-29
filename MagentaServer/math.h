#pragma once
#include <math.h>

double degreeToRadian(double degree);
double radianToDegree(double radian);

class Vector2D {
public:
	float x, z;

	Vector2D() {}
	Vector2D(float vx, float vz) : x(vx), z(vz) {}
	Vector2D Normalize()
	{
		Vector2D res;
		double vect_size = sqrt(pow(x, 2) + pow(z, 2));
		res.x = x / vect_size;
		res.z = z / vect_size;
		return res;
	}

	Vector2D Rotate(float angle)
	{
		Vector2D res;
		res.x = (x * cos(degreeToRadian(angle))) - (z * sin(degreeToRadian(angle)));
		res.z = (x * sin(degreeToRadian(angle))) + (z * cos(degreeToRadian(angle)));
		x = res.x;
		z = res.z;
		return res;
	}

	Vector2D operator+(const Vector2D lhs)
	{
		Vector2D res;
		res.x = x + lhs.x;
		res.z = z + lhs.z;
		return res;
	}

	Vector2D operator*(const float lhs)
	{
		Vector2D res;
		res.x = x * lhs;
		res.z = z * lhs;
		return res;
	}
};


class Vector3D {
public:
	float x, y, z;

	Vector3D() {}
	Vector3D(float vx, float vy, float vz) : x(vx), y(vy), z(vz) {}
	Vector3D Normalize()
	{
		Vector3D res;
		double vect_size = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
		res.x = x / vect_size;
		res.y = y / vect_size;
		res.z = z / vect_size;
		return res;
	}

	Vector3D operator+(const Vector3D lhs)
	{
		Vector3D res;
		res.x = x + lhs.x;
		res.y = y + lhs.y;
		res.z = z + lhs.z;
		return res;
	}

	Vector3D operator*(const float lhs)
	{
		Vector3D res;
		res.x = x * lhs;
		res.y = y * lhs;
		res.z = z * lhs;
		return res;
	}
};

Vector2D addVector(Vector2D a, Vector2D b);
float dotproduct(Vector2D a, Vector2D b);

Vector2D getDistanceVector(float ax, float az, float bx, float bz);
Vector2D getHeightVector(float minZ, float maxZ, float angle);
Vector2D getWidthVector(float minX, float maxX, float angle);

float dotproduct(Vector3D a, Vector3D b);
Vector3D crossproduct(Vector3D a, Vector3D b);
float getDegreeAngle(Vector3D a, Vector3D b);