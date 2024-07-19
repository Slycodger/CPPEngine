#pragma once

#include "Vector.h"
#include "GlStuff.h"
#include "GlobalVars.h"
#include "MathFunctions.h"

void createPerspectiveMat(float*& Mat, float ScreenRatio, float FOVAngle, float FarClip, float NearClip);
void createOrthographicMat(float*& Mat, float ScreenRatio, float RightClip, float LeftClip, float TopClip, float BottomClip, float FarClip, float NearClip);

class Camera
{
	float Pitch = 0;
	float Yaw = 0;
	float Roll = 0;

	float* PerpMat = nullptr;
	float* OrthoMat = nullptr;

	float PerpNearClip = 0.01;
	float PerpFarClip = 2000;

	float OrthoNearClip = 0.01;
	float OrthoFarClip = 2000;

	float FOV = 70;

	float TopClip = 5;
	float BottomClip = -5;
	float LeftClip = -5;
	float RightClip = 5;

	float ScreenRatio = ScreenSpace::Width / ScreenSpace::Height;
public:
	bool Projection = true;

	Camera() : PerpMat(new float[16]), OrthoMat(new float[16])
	{
		createPerspectiveMat(PerpMat, ScreenRatio, FOV, PerpFarClip, PerpNearClip);
		createOrthographicMat(OrthoMat, ScreenRatio, RightClip, LeftClip, TopClip, BottomClip, OrthoFarClip, OrthoNearClip);
	}
	~Camera()
	{
		delete[](PerpMat);
		delete[](OrthoMat);
	}

	void ChangePerpMat(float DScreenRatio, float DFOV, float DFarClip, float DNearClip)
	{
		ScreenRatio += DScreenRatio;

		FOV += DFOV;

		PerpNearClip += DNearClip;
		PerpFarClip += DFarClip;

		createPerspectiveMat(PerpMat, ScreenRatio, FOV, PerpFarClip, PerpNearClip);
	}


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
		return crossProduct(DirectionFacing(), DirectionUp());
	}
	Vector3 RotToVec()
	{
		return Vector3(Pitch, Yaw, Roll);
	}

	float*& const GetProjectionMatrix()
	{
		if (Projection)
			return PerpMat;
		else
			return OrthoMat;
	}

	float*& const PerspectiveMat()
	{
		return PerpMat;
	}
	float*& const OrthographicMat()
	{
		return OrthoMat;
	}
};

extern Camera MainCamera;