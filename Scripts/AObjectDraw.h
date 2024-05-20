/*
* The AObjectDraw header file
*/
#pragma once
#ifndef AOBJECTDRAW
#define AOBJECTDRAW
#ifdef AOBJECTDRAWMAIN
#include "ObjectClasses.h"
#include <iostream>
#include <string>
#include "Constants.h"
#include "ModelLoading.h"
#include "ApplicationMain.h"
#include "KeyPresses.h"
#include "IntializeShaders.h"
#include "GlobalVars.h"
#include "MathFunctions.h"
#include "Camera.h"
#include "List.h"
#include "EngineBase.h"

void SetToTransform(Object* Obj);
void CreateModelMat(glm::mat4& Mat, Vector3 Position, Vector3 Rotation, Vector3 Scale);
void CreateCamMat(glm::mat4& Mat, Vector3 Position, Vector3 Rotation);
void RenderCustomObj(Shader& shader, unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int EBOCount, Vector3 MeshColor, transform Transform);
void RenderSingleObj(Shader& shader, Object*& ObjToDraw, std::string NoTextureShader, std::string TextureShader);
void RenderScene(Shader& shader, std::string NoTextureShader, std::string TextureShader);

//Camera setings
float CameraNearClip = 0.01;
float CameraFarClip = 2000;

//Perspective Settings
float CamFOV = 70;

float camFOVRatio = glm::tan(0.5 * CamFOV * DegToRad);
float perpRangeRecip = 1 / (CameraNearClip - CameraFarClip);
float SW = ScreenSpace::Width;
float SH = ScreenSpace::Height;


//Orthographic settings
float OrthSize = 5;
float orthLeft = -OrthSize * (SW / SH);
float orthRight = OrthSize * (SW / SH);
float orthBottom = -OrthSize;
float orthTop = OrthSize;

float orthoRangeRecip = 1 / (CameraFarClip - CameraNearClip);

float* PerspectiveMat = new float[16]
{
	1 / (camFOVRatio * SW / SH), 0, 0, 0,
	0, 1 / camFOVRatio, 0, 0,
	0, 0, (CameraNearClip + CameraFarClip)* perpRangeRecip, -1,
	0, 0, CameraNearClip* CameraFarClip* perpRangeRecip * 2, 0
};

float* OrthographicMat = new float[16]
{
	2 / (orthRight - orthLeft), 0, 0, 0,
	0, 2 / (orthTop - orthBottom), 0, 0,
	0, 0, -2 * orthoRangeRecip, 0,
	0, 0, -(CameraFarClip + CameraNearClip) / (CameraFarClip - CameraNearClip), 1
};

glm::mat4 ObjTransformMat(1);
glm::mat4 ViewMat(1);

#endif

namespace AObjectDraw
{
	void Start(Shader& shader);
	void Update(Shader& shader);
	void End();
}

#endif