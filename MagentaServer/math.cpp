#include "pch.h"
#include "math.h"

Vector2D addVector(Vector2D a, Vector2D b)
{
	Vector2D res;
	res.x = a.x + b.x;
	res.z = a.z + b.z;
	return res;
}

float dotproduct(Vector2D a, Vector2D b)
{
	return fabs((a.x * b.x) + (a.z * b.z));
}

double degreeToRadian(double degree)
{
	return degree / 180.0 * 3.141592;
}

double radianToDegree(double radian)
{
	return radian * 180.0 / 3.141592;
}

Vector2D getDistanceVector(float ax, float az, float bx, float bz)
{
	Vector2D res;
	res.x = ax - bx;
	res.z = az - bz;
	return res;
}

Vector2D getHeightVector(float minZ, float maxZ, float angle)
{
	Vector2D res;
	res.x = (maxZ - minZ) * cos(degreeToRadian(angle - 90)) / 2;
	res.z = (maxZ - minZ) * sin(degreeToRadian(angle - 90)) / 2;
	return res;
}

Vector2D getWidthVector(float minX, float maxX, float angle)
{
	Vector2D res;
	res.x = (maxX - minX) * cos(degreeToRadian(angle)) / 2;
	res.z = (maxX - minX) * sin(degreeToRadian(angle)) / 2;
	return res;
}

float dotproduct(Vector3D a, Vector3D b)
{
	return fabs((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

Vector3D crossproduct(Vector3D a, Vector3D b)
{
	Vector3D res;
	res.x = (a.y * b.z) - (a.z * b.y);
	res.y = (a.z * b.x) - (a.x * b.z);
	res.z = (a.x * b.y) - (a.y * b.x);
	return res;
}

float getDegreeAngle(Vector3D a, Vector3D b)
{
	Vector3D v1 = a.Normalize();
	Vector3D v2 = b.Normalize();
	float angle = acos(dotproduct(v1, v2));
	return radianToDegree(fabs(angle));
}