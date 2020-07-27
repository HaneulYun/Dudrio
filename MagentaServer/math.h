#pragma once
#include <math.h>

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
};

Vector2D addVector(Vector2D a, Vector2D b);
float dotproduct(Vector2D a, Vector2D b);

Vector2D getDistanceVector(float ax, float az, float bx, float bz);
Vector2D getHeightVector(float minZ, float maxZ, float angle);
Vector2D getWidthVector(float minX, float maxX, float angle);

float dotproduct(Vector3D a, Vector3D b);
Vector3D crossproduct(Vector3D a, Vector3D b);
float getDegreeAngle(Vector3D a, Vector3D b);

double degreeToRadian(double degree);
double radianToDegree(double radian);