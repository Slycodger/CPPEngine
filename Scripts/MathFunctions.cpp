#include "MathFunctions.h"
#include <math.h>

//Returns the cross product of two vectors
Vector3 crossProduct(Vector3 Vec1, Vector3 Vec2)
{
	Vector3 Output;

	Output.x = (Vec1.y * Vec2.z) - (Vec2.y * Vec1.z);
	Output.y = -((Vec1.x * Vec2.z) - (Vec2.x * Vec1.z));
	Output.z = (Vec1.x * Vec2.y) - (Vec2.x * Vec1.y);

	return Output;
}

//Returns dot product of two vectors
float dotProduct(Vector3 Vec1, Vector3 Vec2)
{
	return (Vec1.x * Vec2.x) + (Vec1.y * Vec2.y) + (Vec1.z * Vec2.z);
}

//Returns the magnitude of a vector
float magnitude(Vector3 Vec)
{
	return std::sqrt(std::pow(Vec.x, 2) + std::pow(Vec.y, 2) + std::pow(Vec.z, 2));
}

//Normalizes a vector
Vector3 normalize(Vector3 Vec)
{
	Vec /= magnitude(Vec);
	return Vec;
}

//Only works when vectors are aligned on the x-y plane
//Gets the angle between two vectors
float angleBetween(Vector3 Vec1, Vector3 Vec2)
{
	float inti = std::atan((Vec1.y - Vec2.y) / (Vec1.x - Vec2.x)) * RadToDeg;
	if ((Vec1.x - Vec2.x < 0 && Vec1.y - Vec2.y < 0) || Vec1.x - Vec2.x < 0)
		return 180 + inti;
	return inti;
}

//Gets the normal from the vectors
Vector3 getNormal(Vector3 Vert1, Vector3 Vert2, Vector3 Vert3)
{
	Vector3 Ret;

	Ret = crossProduct(Vert2 - Vert1, Vert3 - Vert1);

	return Ret;
}

//Gets the origin point between the vectors
Vector3 getOrigin(Vector3 Vert1, Vector3 Vert2, Vector3 Vert3)
{
	Vector3 Ret;

	Ret = (Vert1 + Vert2 + Vert3) / 3;

	return Ret;
}