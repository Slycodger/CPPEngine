#pragma once
#ifndef VECTOR
#define VECTOR
#include <fstream>
#include "Constants.h"
#include "GlStuff.h"

struct Vector3
{
	float x;
	float y;
	float z;

	Vector3(float a, float b, float c)
	{
		x = a;
		y = b;
		z = c;
	}
	Vector3(glm::vec4 glmVec)
	{
		x = glmVec.x;
		y = glmVec.y;
		z = glmVec.z;
	}
	Vector3(float v)
	{
		x = v;
		y = v;
		z = v;
	}
	Vector3()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	glm::vec3 make_glmVec3() 
	{
		return glm::vec3(x, y, z);
	}
	glm::vec4 make_glmVec4()
	{
		return glm::vec4(x, y, z, 1);
	}


	void Scale(Vector3 Vec)
	{
		x *= Vec.x;
		y *= Vec.y;
		z *= Vec.z;
	}

	void operator = (glm::vec4 const& obj)
	{
		x = obj.x;
		y = obj.y;
		z = obj.z;
	}
	Vector3 operator +=(Vector3 const& obj)
	{
		Vector3 ret;
		ret.x = x += obj.x;
		ret.y = y += obj.y;
		ret.z = z += obj.z;
		return ret;
	}
	Vector3 operator -=(Vector3 const& obj)
	{
		Vector3 ret;
		ret.x = x -= obj.x;
		ret.y = y -= obj.y;
		ret.z = z -= obj.z;
		return ret;
	}
	Vector3 operator *(float const& obj)
	{
		Vector3 ret;
		ret.x = x * obj;
		ret.y = y * obj;
		ret.z = z * obj;
		return ret;
	}
	Vector3 operator *=(float const& obj)
	{
		Vector3 ret;
		ret.x = x *= obj;
		ret.y = y *= obj;
		ret.z = z *= obj;
		return ret;
	}
	Vector3 operator *(Vector3 const& obj)
	{
		Vector3 ret;
		ret.x = x * obj.x;
		ret.y = y * obj.y;
		ret.z = z * obj.z;
		return ret;
	}
	Vector3 operator /(float const& obj)
	{
		Vector3 ret;
		ret.x = x / obj;
		ret.y = y / obj;
		ret.z = z / obj;
		return ret;
	}
	Vector3 operator /(Vector3 const& obj)
	{
		Vector3 ret;
		ret.x = x / obj.x;
		ret.y = y / obj.y;
		ret.z = z / obj.z;
		return ret;
	}
	Vector3 operator /=(float const& obj)
	{
		Vector3 ret;
		ret.x = x /= obj;
		ret.y = y /= obj;
		ret.z = z /= obj;
		return ret;
	}
	Vector3 operator -(Vector3 const& obj)
	{
		Vector3 ret;
		ret.x = x - obj.x;
		ret.y = y - obj.y;
		ret.z = z - obj.z;
		return ret;
	}
	Vector3 operator +(Vector3 const& obj)
	{
		Vector3 ret;
		ret.x = x + obj.x;
		ret.y = y + obj.y;
		ret.z = z + obj.z;
		return ret;
	}
	friend std::ostream& operator << (std::ostream& os, Vector3 const& obj);
	friend bool operator == (Vector3 const& obj1, Vector3 const obj2);
	friend Vector3 operator -(Vector3 const& obj);
};

struct Vector2
{
	float x = 0;
	float y = 0;

	Vector2() : x(0), y(0){}
	Vector2(float val) : x(val), y(val){}
	Vector2(float a, float b) : x(a), y(b){}

	enum Vec2{Zero};

	Vector2 operator = (Vec2 Vec)
	{
		Vector2 ret;
		switch (Vec)
		{
		case Zero:
			ret.x = 0;
			ret.y = 0;
			break;
		}
		return ret;
	}
	friend std::ostream& operator << (std::ostream& os, Vector2 const& obj);
};

struct Vector4
{
	float x;
	float y;
	float z;
	float w;
	Vector4(float val) : x(val), y(val), z(val), w(val){}
	Vector4(float r, float g, float b, float a) : x(r), y(g), z(b), w(a) {}
	Vector4(Vector3 Vec) : x(Vec.x), y(Vec.y), z(Vec.z), w(0) {}
	Vector4(glm::vec4 Vec) : x(Vec.x), y(Vec.y), z(Vec.z), w(Vec.w) {}

	bool operator != (Vector4 Obj)
	{
		if (x != Obj.x)
			return false;
		if (y != Obj.y)
			return false;
		if (z != Obj.z)
			return false;
		if (w != Obj.w)
			return false;
		return true;
	}
	Vector4 operator -()
	{
		Vector4 Ret = 0;
		Ret.x = -x;
		Ret.y = -y;
		Ret.z = -z;
		Ret.w = -w;
		return Ret;
	}
	friend std::ostream& operator << (std::ostream& os, Vector4 const& obj);
};

#endif