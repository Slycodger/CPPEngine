/*
* The AObjectDraw header file
*/
#pragma once
#ifndef AOBJECTDRAW
#define AOBJECTDRAW
#include "ObjectClasses.h"
#ifdef AOBJECTDRAWMAIN
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
void RenderCustomObj(unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int EBOCount, Vector3 MeshColor, transform Transform);
void RenderScene(std::string NoTextureShader, std::string TextureShader);

glm::mat4 ObjTransformMat(1);
glm::mat4 ViewMat(1);

#endif

namespace AObjectDraw
{
	void Start();
	void Update();
	void End();
}

void RenderSingleObj(Object*& ObjToDraw, std::string NoTextureShader, std::string TextureShader);

#endif