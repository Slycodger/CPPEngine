//#pragma once
//#ifndef LIGHTINGMAIN
//#define LIGHTINGMAIN
//
//#include "IntializeShaders.h"
//#include "ObjectClasses.h"
//#include "Constants.h"
//#include "GlobalVars.h"
//
//
//struct OutputStruct
//{
//	Edge Alone;
//	Edge Connected;
//};
//
//
//#ifdef LIGHTINGMAINMAIN
//
//#include "MathFunctions.h"
//void CreateModelMat(glm::mat4& Mat, Vector3 Position, Vector3 Rotation, Vector3 Scale);
//void RenderCustomObj(Shader& shader, unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int EBOCount, Vector3 MeshColor, transform Transform);
//#endif
//
//
//void SetEdgeCounts(unsigned int& AloneCount, unsigned int &ConnectedCount, unsigned int*& Indices);
//void SizeToLargest(Object** Objects);
//void CreateEdgeGroup(Object** Objects);
//void UpdateEdgeData();
//void LoadShadowEdge(Object*& Obj);
//void ClearShadowEdges();
//void RenderLight(Shader& shader, Vector3 LightPos);
//void GenerateShadowData(Vector3 LightPosition);
//
//#endif