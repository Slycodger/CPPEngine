#include "Camera.h"

/*
Create a perspective matrix
Make sure to intialize Mat to a size 16 array
*/
void createPerspectiveMat(float*& Mat, float ScreenRatio, float FOVAngle, float FarClip, float NearClip)
{
	float RangeRecip = 1 / (NearClip - FarClip);
	float FOVRatio = glm::tan(0.5 * FOVAngle * DegToRad);
	float temp[16]
	{
		1 / (FOVRatio * ScreenRatio), 0, 0, 0,
		0, 1 / FOVRatio, 0, 0,
		0, 0, (NearClip + FarClip) * RangeRecip, -1,
		0, 0, NearClip * FarClip * RangeRecip * 2, 0
	};
	for (int i = 0; i < 16; i++)
		Mat[i] = temp[i];
}

/*
Create an orthographic matrix
Make sure to intialize Mat to a size 16 array
*/
void createOrthographicMat(float*& Mat, float ScreenRatio, float RightClip, float LeftClip, float TopClip, float BottomClip, float FarClip, float NearClip)
{
	float RangeRecip = 1 / (NearClip - FarClip);
	float temp[16] =
	{
		2 / ((RightClip - LeftClip) * ScreenRatio), 0, 0, 0,
		0, 2 / (TopClip - BottomClip), 0, 0,
		0, 0, -2 * RangeRecip, 0,
		0, 0, -(FarClip + NearClip) / (FarClip - NearClip), 1
	};
	for (int i = 0; i < 16; i++)
		Mat[i] = temp[i];
}