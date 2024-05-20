#include "MathFunctions.h"
#include <math.h>

Vector3 CrossProduct(Vector3 Vec1, Vector3 Vec2)
{
	Vector3 Output;

	Output.x = (Vec1.y * Vec2.z) - (Vec2.y * Vec1.z);
	Output.y = -((Vec1.x * Vec2.z) - (Vec2.x * Vec1.z));
	Output.z = (Vec1.x * Vec2.y) - (Vec2.x * Vec1.y);

	return Output;
}
float DotProduct(Vector3 Vec1, Vector3 Vec2)
{
	return (Vec1.x * Vec2.x) + (Vec1.y * Vec2.y) + (Vec1.z * Vec2.z);
}
float Magnitude(Vector3 Vec)
{
	return std::sqrt(std::pow(Vec.x, 2) + std::pow(Vec.y, 2) + std::pow(Vec.z, 2));
}
Vector3 Normalize(Vector3 Vec)
{
	Vec /= Magnitude(Vec);
	return Vec;
}
float AngleBetween(Vector3 Vec1, Vector3 Vec2)
{
	float inti = std::atan((Vec1.y - Vec2.y) / (Vec1.x - Vec2.x)) * (180 / PI);
	if ((Vec1.x - Vec2.x < 0 && Vec1.y - Vec2.y < 0) || Vec1.x - Vec2.x < 0)
		return 180 + inti;
	return inti;
}

Vector3 GetNormal(Vector3 Vert1, Vector3 Vert2, Vector3 Vert3)
{
	Vector3 Ret;

	Ret = CrossProduct(Vert2 - Vert1, Vert3 - Vert1);

	return Ret;
}

Vector3 GetOrigin(Vector3 Vert1, Vector3 Vert2, Vector3 Vert3)
{
	Vector3 Ret;

	Ret = (Vert1 + Vert2 + Vert3) / 3;

	return Ret;
}