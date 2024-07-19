#pragma once

#include <math.h>
#include "Vector.h"

Vector3 crossProduct(Vector3 Vec1, Vector3 Vec2);
float dotProduct(Vector3 Vec1, Vector3 Vec2);
float magnitude(Vector3 Vec);
Vector3 normalize(Vector3 Vec);
float angleBetween(Vector3 Vec1, Vector3 Vec2);
Vector3 getNormal(Vector3 Vert1, Vector3 Vert2, Vector3 Vert3);
Vector3 getOrigin(Vector3 Vert1, Vector3 Vert2, Vector3 Vert3);
constexpr double PI = 3.141592653589793238462643383279502884197;
constexpr double DegToRad = PI / 180;
constexpr double RadToDeg = 180 / PI;