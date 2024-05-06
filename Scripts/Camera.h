#pragma once
#ifndef CAMERA
#define CAMERA
#include "ObjectClasses.h"
#include "GlStuff.h"
class Camera
{
	float Pitch = 0;
	float Yaw = 0;
	float Roll = 0;
public:
	Vector3 Position = Vector3();
	void Translate(float x, float y, float z)
	{
		Position += Vector3(x, y, z);
	}
	void SetRotation(Vector3 Rotation)
	{
		Pitch = Rotation.x;
		Yaw = Rotation.y;
		Roll = Rotation.z;
		Pitch = glm::mod(Pitch, 360.f);
		Yaw = glm::mod(Yaw, 360.f);
		Roll = glm::mod(Roll, 360.f);
	}
	void SetRotation(float x, float y, float z)
	{
		Pitch = x;
		Yaw = y;
		Roll = z;
		Pitch = glm::mod(Pitch, 360.f);
		Yaw = glm::mod(Yaw, 360.f);
		Roll = glm::mod(Roll, 360.f);
	}
	void Rotate(Vector3 Rotation)
	{
		Pitch += Rotation.x;
		Yaw += Rotation.y;
		Roll += Rotation.z;
		Pitch = glm::mod(Pitch, 360.f);
		Yaw = glm::mod(Yaw, 360.f);
		Roll = glm::mod(Roll, 360.f);
	}	
	void Rotate(float x, float y, float z)
	{
		Pitch += x;
		Yaw += y;
		Roll += z;

		Pitch = glm::mod(Pitch, 360.f);
		Yaw = glm::mod(Yaw, 360.f);
		Roll = glm::mod(Roll, 360.f);
	}
	Vector3 Rotation()
	{
		return Vector3(Pitch, Yaw, Roll);
	}
	Vector3 DirectionFacing()
	{
		Vector3 ret;
		ret.x = glm::cos(glm::radians<float>(Yaw + 90)) * glm::cos(glm::radians<float>(Pitch));
		ret.y = glm::sin(glm::radians<float>(Pitch));
		ret.z = glm::sin(glm::radians<float>(Yaw - 90)) * glm::cos(glm::radians<float>(Pitch));
		return ret;
	}
	Vector3 DirectionUp()
	{
		Vector3 ret;
		ret.x = glm::cos(glm::radians<float>(Yaw - 90)) * glm::cos(glm::radians<float>(Pitch - 90));
		ret.y = glm::sin(glm::radians<float>(Pitch + 90));
		ret.z = glm::sin(glm::radians<float>(Yaw + 90)) * glm::cos(glm::radians<float>(Pitch - 90));
		return ret;
	}
	Vector3 DirectionRight()
	{
		return CrossProduct(DirectionFacing(), DirectionUp());
	}
	Vector3 RotToVec()
	{
		return Vector3(Pitch, Yaw, Roll);
	}
};

extern Camera MainCamera;

#endif