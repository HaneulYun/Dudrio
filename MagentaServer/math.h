#pragma once
#include "main.h"

struct Vector2D {
	float x, z;

	Vector2D() {}
	Vector2D(float vx, float vz) : x(vx), z(vz){}
	Vector2D Normalize() 
	{
		Vector2D res;
		double vect_size = sqrt(pow(x, 2) + pow(z, 2));
		res.x = x / vect_size;
		res.z = z / vect_size;
		return res;
	}
};

Vector2D addVector(Vector2D a, Vector2D b);
float dotproduct(Vector2D a, Vector2D b);
double degreeToRadian(double degree);
Vector2D getDistanceVector(float ax, float az, float bx, float bz);
Vector2D getHeightVector(float minZ, float maxZ, float angle);
Vector2D getWidthVector(float minX, float maxX, float angle);
