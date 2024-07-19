#pragma once

#include "ObjectClasses.h"
namespace AppObjHandler
{
	void start();
	void update();
	void end();
}

#ifdef AppObjHandler_MAIN
#include <iostream>
#include <string>
#include "Constants.h"
#include "ModelLoading.h"
#include "ApplicationBase.h"
#include "KeyPresses.h"
#include "IntializeShaders.h"
#include "GlobalVars.h"
#include "MathFunctions.h"
#include "Camera.h"
#include "List.h"
#include "ScriptBase.h"
#include "Button.h"

void appIntializer();
void addObjToList(Object* Obj);
void createObjTransformMatrix(glm::mat4& Mat, Object*& Obj);
void createObjTransformMatrix(glm::mat4& Mat, Vector3 Pos, Vector3 Rot, Vector3 Scale);
void createViewMatrix(glm::mat4& Mat, Vector3 Position, Vector3 Rotation);
void deleteObj(Object*& Obj);
void deleteObj(int Obj);
void deleteScene();
int findOpenSlot();
void renderCustomObj(unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int EBOCount, Vector3 MeshColor, transform Transform);
void renderObjectChain(std::string& NoTextureShader, std::string& TextureShader, Object*& Obj);
void renderObjectDependencies(std::string& NoTextureShader, std::string& TextureShader, Object*& Obj);
void renderObjectStencil(Object*& Obj);
void renderSceneObject(std::string& NoTextureShader, std::string& TextureShader, Object*& Obj);
void renderScene(std::string NoTextureShader, std::string TextureShader);
void renderSingleObject(Object*& ObjToDraw, std::string NoTextureShader, std::string TextureShader, bool GenView);
void runObjectScripts(int Obj);
void updateObjects();

#endif //ifdef AppObjHandler_MAIN