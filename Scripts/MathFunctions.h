#pragma once
#ifndef MATHFUNCTIONS
#define MATHFUNCTIONS

#include "ObjectClasses.h"
#include <math.h>

Vector3 CrossProduct(Vector3 Vec1, Vector3 Vec2);
float DotProduct(Vector3 Vec1, Vector3 Vec2);
float Magnitude(Vector3 Vec);
Vector3 Normalize(Vector3 Vec);
float AngleBetween(Vector3 Vec1, Vector3 Vec2);
Vector3 GetNormal(Vector3 Vert1, Vector3 Vert2, Vector3 Vert3);
Vector3 GetOrigin(Vector3 Vert1, Vector3 Vert2, Vector3 Vert3);
extern const float PI;
extern const float DegToRad;
#endif