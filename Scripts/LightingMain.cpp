//#define LIGHTINGMAINMAIN
//#include "LightingMain.h"
//
//struct LightingContainer
//{
//	ComputeShader computeShader = ComputeShader();
//	AloneEdge* AloneEdgeIn = nullptr;
//	ConnectedEdge* ConnectedEdgeIn = nullptr;
//	OutputStruct *Output = nullptr;
//
//	int AloneCount = 0;
//	int ConnectedCount = 0;
//	int OutputCount = 0;
//
//	Object **Objects = new Object*[MAXOBJECTS];
//	int ObjectCount = 0;
//
//	unsigned int AloneSSBO = 0, ConnectedSSBO = 0, OutputSSBO = 0;
//
//	unsigned int MVAO = 0, MVBO = 0, MEBO = 0;
//
//	unsigned int IndiceCount = 0;
//
//	LightingContainer()
//	{
//		glGenVertexArrays(1, &MVAO);
//		computeShader.CreateShader("GenerateShadowEdges", computeShader.ID);
//	}
//}LightingVars;
//
//void SetEdgeCounts(unsigned int &AloneCount, unsigned int &ConnectedCount, unsigned int *&Indices)
//{
//	AloneCount = 0;
//	ConnectedCount = 0;
//
//	List<Edge> AloneList = 0;
//	List<Edge> ConnectedList = 0;
//
//	Edge TempEdge = 0;
//
//	for (int i = 1; i <= Indices[0]; i++)
//	{
//		TempEdge.Vert1 = Indices[i];
//		switch ((i - 1) % 3)
//		{
//		case 0:
//			TempEdge.Vert2 = Indices[i + 1];
//			break;
//		case 1:
//			TempEdge.Vert2 = Indices[i + 1];
//			break;
//		case 2:
//			TempEdge.Vert2 = Indices[i - 2];
//			break;
//		}
//		if(AloneList.Contains(TempEdge))
//		{
//			ConnectedCount++;
//			AloneCount--;
//			ConnectedList.AddToList(new Edge(TempEdge));
//			AloneList.RemoveObjFromList(TempEdge);
//		}
//		if(!ConnectedList.Contains(TempEdge))
//		{
//			AloneCount++;
//			AloneList.AddToList(new Edge(TempEdge));
//		}
//	}
//	AloneList.Delete();
//	ConnectedList.Delete();
//}
//
//void SizeToLargest(Object** Objects)
//{
//	for(int Obj = 0; Obj < MAXOBJECTS; Obj++)
//	{
//		if (Objects[Obj] == nullptr || Objects[Obj]->mesh.Active == false)
//			continue;
//
//		LightingVars.AloneCount += *Objects[Obj]->mesh.AloneEdgeCount;
//		LightingVars.ConnectedCount += *Objects[Obj]->mesh.ConnectedEdgeCount;
//
//		Objects[Obj]->mesh.LargerEdgeCount = *Objects[Obj]->mesh.AloneEdgeCount > *Objects[Obj]->mesh.ConnectedEdgeCount ? *Objects[Obj]->mesh.AloneEdgeCount : *Objects[Obj]->mesh.ConnectedEdgeCount;
//	}
//
//	LightingVars.OutputCount = LightingVars.AloneCount > LightingVars.ConnectedCount ? LightingVars.AloneCount : LightingVars.ConnectedCount;
//
//	LightingVars.AloneEdgeIn = new AloneEdge[LightingVars.AloneCount];
//	LightingVars.ConnectedEdgeIn = new ConnectedEdge[LightingVars.ConnectedCount];
//
//	glGenBuffers(1, &LightingVars.AloneSSBO);
//	glGenBuffers(1, &LightingVars.ConnectedSSBO);
//	glGenBuffers(1, &LightingVars.OutputSSBO);
//
//	glGenBuffers(1, &LightingVars.MVBO);
//	glGenBuffers(1, &LightingVars.MEBO);
//}
//
//void CreateEdgeGroup(Object** Objects)
//{
//	unsigned int* Indices = nullptr;
//	float* Verts = nullptr;
//
//	glm::mat4 ObjTransformMat(1);
//
//	AloneEdge AloneTemp;
//	ConnectedEdge ConnectedTemp;
//
//	List<AloneEdge> AloneList = 0;
//	List<ConnectedEdge> ConnectedList = 0;
//
//	int CurrentAloneCount = 0;
//	int CurrentConnectedCount = 0;
//
//	for (int Obj = 0; Obj < MAXOBJECTS; Obj++)
//	{
//		if (Objects[Obj] == nullptr || Objects[Obj]->mesh.Active == false)
//			continue;
//
//		LightingVars.Objects[LightingVars.ObjectCount] = Objects[Obj];
//		LightingVars.ObjectCount++;
//
//		Indices = new unsigned int[Objects[Obj]->mesh.Indices[0] + 1];
//		Verts = new float[Objects[Obj]->mesh.Vertices[0] + 1];
//
//		CreateModelMat(ObjTransformMat, Objects[Obj]->Transform.Position(), Objects[Obj]->Transform.Rotation(), Objects[Obj]->Transform.Scale());
//
//		for (int i = 0; i <= Objects[Obj]->mesh.Indices[0]; i++)
//		{
//			Indices[i] = Objects[Obj]->mesh.Indices[i];
//		}
//		Verts[0] = Objects[Obj]->mesh.Vertices[0];
//		for (int i = 0; i <= Objects[Obj]->mesh.Vertices[0] - 9; i += 9)
//		{
//			glm::vec4 glmVert = ObjTransformMat * glm::vec4(Objects[Obj]->mesh.Vertices[i + 1], Objects[Obj]->mesh.Vertices[i + 2], Objects[Obj]->mesh.Vertices[i + 3], 1);
//
//			Verts[i + 1] = glmVert.x;
//			Verts[i + 2] = glmVert.y;
//			Verts[i + 3] = glmVert.z;
//		}
//
//		for (int i = 1; i <= Objects[Obj]->mesh.Indices[0]; i++)
//		{
//			unsigned int Vert1 = Indices[i];
//			unsigned int Vert2;
//			unsigned int Vert3;
//			switch ((i - 1) % 3)
//			{
//			case 0:
//				Vert2 = Indices[i + 1];
//				Vert3 = Indices[i + 2];
//				break;
//			case 1:
//				Vert2 = Indices[i + 1];
//				Vert3 = Indices[i - 1];
//				break;
//			case 2:
//				Vert2 = Indices[i - 2];
//				Vert3 = Indices[i - 1];
//				break;
//			}
//			Edge edge = 0;
//			edge.Vert1 = Vert1;
//			edge.Vert2 = Vert2;
//
//			Vector3 Vert1Pos = Vector3(Verts[Vert1 * VERTLENGTH + 1], Verts[Vert1 * VERTLENGTH + 2], Verts[Vert1 * VERTLENGTH + 3]);
//			Vector3 Vert2Pos = Vector3(Verts[Vert2 * VERTLENGTH + 1], Verts[Vert2 * VERTLENGTH + 2], Verts[Vert2 * VERTLENGTH + 3]);
//			Vector3 Vert3Pos = Vector3(Verts[Vert3 * VERTLENGTH + 1], Verts[Vert3 * VERTLENGTH + 2], Verts[Vert3 * VERTLENGTH + 3]);
//
//			AloneTemp.edge = edge;
//			AloneTemp.Vert1 = Vert1Pos;
//			AloneTemp.Vert2 = Vert2Pos;
//			AloneTemp.Indice3 = Vert3;
//			ConnectedTemp = ConnectedEdge(AloneTemp, Vert3Pos, Vert3);
//			if(AloneList.Contains(AloneTemp))
//			{
//				AloneTemp = *AloneList[AloneList.GetIndex(AloneTemp)];
//				ConnectedTemp = ConnectedEdge(AloneTemp, Vert3Pos, Vert3);
//				ConnectedList.AddToList(new ConnectedEdge(ConnectedTemp));
//				AloneList.RemoveObjFromList(AloneTemp);
//			}
//			AloneTemp.Vert3 = Vert3Pos;
//			if (!ConnectedList.Contains(ConnectedTemp))
//			{
//				AloneList.AddToList(new AloneEdge(AloneTemp));
//			}
//		}
//
//		for (int i = 0; i < AloneList.Length; i++)
//			LightingVars.AloneEdgeIn[i + CurrentAloneCount] = *AloneList[i];
//
//		for (int i = 0; i < ConnectedList.Length; i++)
//			LightingVars.ConnectedEdgeIn[i + CurrentConnectedCount] = *ConnectedList[i];
//
//		CurrentAloneCount += AloneList.Length;
//		CurrentConnectedCount += ConnectedList.Length;
//
//		AloneList.Delete();
//		ConnectedList.Delete();
//
//		delete[](Verts);
//		delete[](Indices);
//	}
//}
//
//void UpdateEdgeData()
//{
//	int ObjIterator = 0;
//	int CountIterator = 0;
//
//	glm::mat4 ObjTransformMat;
//
//	Vector3 Vert1;
//	Vector3 Vert2;
//	Vector3 Vert3;
//	Vector3 Vert4;
//
//	CreateModelMat(ObjTransformMat, LightingVars.Objects[ObjIterator]->Transform.Position(), LightingVars.Objects[ObjIterator]->Transform.Rotation(), LightingVars.Objects[ObjIterator]->Transform.Scale());
//
//	for(int i = 0; i < LightingVars.AloneCount; i++)
//	{
//		LightingVars.AloneEdgeIn[i].Vert1 = ObjTransformMat * glm::vec4(LightingVars.Objects[ObjIterator]->mesh.Vertices[LightingVars.AloneEdgeIn[i].edge.Vert1 * 9 + 1], LightingVars.Objects[ObjIterator]->mesh.Vertices[LightingVars.AloneEdgeIn[i].edge.Vert1 * 9 + 2], LightingVars.Objects[ObjIterator]->mesh.Vertices[LightingVars.AloneEdgeIn[i].edge.Vert1 * 9 + 3], 1);
//		LightingVars.AloneEdgeIn[i].Vert2 = ObjTransformMat * glm::vec4(LightingVars.Objects[ObjIterator]->mesh.Vertices[LightingVars.AloneEdgeIn[i].edge.Vert2 * 9 + 1], LightingVars.Objects[ObjIterator]->mesh.Vertices[LightingVars.AloneEdgeIn[i].edge.Vert2 * 9 + 2], LightingVars.Objects[ObjIterator]->mesh.Vertices[LightingVars.AloneEdgeIn[i].edge.Vert2 * 9 + 3], 1);
//		LightingVars.AloneEdgeIn[i].Vert3 = ObjTransformMat * glm::vec4(LightingVars.Objects[ObjIterator]->mesh.Vertices[LightingVars.AloneEdgeIn[i].Indice3 * 9 + 1], LightingVars.Objects[ObjIterator]->mesh.Vertices[LightingVars.AloneEdgeIn[i].Indice3 * 9 + 2], LightingVars.Objects[ObjIterator]->mesh.Vertices[LightingVars.AloneEdgeIn[i].Indice3 * 9 + 3], 1);
//
//		if (i >= LightingVars.AloneCount - 1)
//			continue;
//
//		if (CountIterator < *LightingVars.Objects[ObjIterator]->mesh.AloneEdgeCount - 1)
//			CountIterator++;
//		else
//		{
//			CountIterator = 0;
//			ObjIterator++;
//			CreateModelMat(ObjTransformMat, LightingVars.Objects[ObjIterator]->Transform.Position(), LightingVars.Objects[ObjIterator]->Transform.Rotation(), LightingVars.Objects[ObjIterator]->Transform.Scale());
//		}
//	}
//	CountIterator = 0;
//	for (int i = 0; i < LightingVars.ConnectedCount; i++)
//	{
//		LightingVars.ConnectedEdgeIn[i].Vert1 = ObjTransformMat * glm::vec4(LightingVars.Objects[ObjIterator]->mesh.Vertices[LightingVars.ConnectedEdgeIn[i].edge.Vert1 * 9 + 1], LightingVars.Objects[ObjIterator]->mesh.Vertices[LightingVars.ConnectedEdgeIn[i].edge.Vert1 * 9 + 2], LightingVars.Objects[ObjIterator]->mesh.Vertices[LightingVars.ConnectedEdgeIn[i].edge.Vert1 * 9 + 3], 1);
//		LightingVars.ConnectedEdgeIn[i].Vert2 = ObjTransformMat * glm::vec4(LightingVars.Objects[ObjIterator]->mesh.Vertices[LightingVars.ConnectedEdgeIn[i].edge.Vert2 * 9 + 1], LightingVars.Objects[ObjIterator]->mesh.Vertices[LightingVars.ConnectedEdgeIn[i].edge.Vert2 * 9 + 2], LightingVars.Objects[ObjIterator]->mesh.Vertices[LightingVars.ConnectedEdgeIn[i].edge.Vert2 * 9 + 3], 1);
//		LightingVars.ConnectedEdgeIn[i].Vert3 = ObjTransformMat * glm::vec4(LightingVars.Objects[ObjIterator]->mesh.Vertices[LightingVars.ConnectedEdgeIn[i].Outpoints.Vert1 * 9 + 1], LightingVars.Objects[ObjIterator]->mesh.Vertices[LightingVars.ConnectedEdgeIn[i].Outpoints.Vert1 * 9 + 2], LightingVars.Objects[ObjIterator]->mesh.Vertices[LightingVars.ConnectedEdgeIn[i].Outpoints.Vert1 * 9 + 3], 1);
//		LightingVars.ConnectedEdgeIn[i].Vert4 = ObjTransformMat * glm::vec4(LightingVars.Objects[ObjIterator]->mesh.Vertices[LightingVars.ConnectedEdgeIn[i].Outpoints.Vert2 * 9 + 1], LightingVars.Objects[ObjIterator]->mesh.Vertices[LightingVars.ConnectedEdgeIn[i].Outpoints.Vert2 * 9 + 2], LightingVars.Objects[ObjIterator]->mesh.Vertices[LightingVars.ConnectedEdgeIn[i].Outpoints.Vert2 * 9 + 3], 1);
//
//		if (i >= LightingVars.ConnectedCount - 1)
//			continue;
//
//		if (CountIterator < *LightingVars.Objects[ObjIterator]->mesh.ConnectedEdgeCount - 1)
//			CountIterator++;
//		else
//		{
//			CountIterator = 0;
//			ObjIterator++;
//			CreateModelMat(ObjTransformMat, LightingVars.Objects[ObjIterator]->Transform.Position(), LightingVars.Objects[ObjIterator]->Transform.Rotation(), LightingVars.Objects[ObjIterator]->Transform.Scale());
//		}
//	}
//}
//
//int OutputPassedCount()
//{
//	int Ret = 0;
//
//	for(int i = 0; i < LightingVars.OutputCount; i++)
//	{
//		if (LightingVars.Output[i].Alone.Vert1 != -1 && LightingVars.Output[i].Alone.Vert2 != -1)
//		{
//			Ret++;
//			continue;
//		}
//		if (LightingVars.Output[i].Connected.Vert1 != -1 && LightingVars.Output[i].Connected.Vert2 != -1)
//			Ret++;
//	}
//
//	return Ret;
//}
//
//void CreateShadowMesh(Vector3 LightPos)
//{
//	if (LightingVars.Output == nullptr)
//		return;
//
//	glm::mat4 ObjectTransformMat(1);
//
//	CreateModelMat(ObjectTransformMat, LightingVars.Objects[0]->Transform.Position(), LightingVars.Objects[0]->Transform.Rotation(), LightingVars.Objects[0]->Transform.Scale());
//
//	int EdgeIterator = 0;
//	int ObjIterator = 0;
//
//	int VertIterator = 0, IndiceIterator = 0, IndiceCounter = 0;;
//
//	int OutputPassed = OutputPassedCount();
//
//	float* VertList = new float[OutputPassed * VERTLENGTH * 4];
//	unsigned int* IndiceList = new unsigned int[OutputPassed * VERTLENGTH * 6];
//	for (int i = 0; i < LightingVars.OutputCount; i++) 
//	{
//		if(EdgeIterator < *LightingVars.Objects[ObjIterator]->mesh.AloneEdgeCount && LightingVars.Output[i].Alone.Vert1 != -1 && LightingVars.Output[i].Alone.Vert2 != -1)
//		{
//			Vector3 Vert1 = ObjectTransformMat * glm::vec4(LightingVars.Objects[ObjIterator]->mesh.Vertices[VERTLENGTH * LightingVars.Output[i].Alone.Vert1 + 1], LightingVars.Objects[ObjIterator]->mesh.Vertices[VERTLENGTH * LightingVars.Output[i].Alone.Vert1 + 2], LightingVars.Objects[ObjIterator]->mesh.Vertices[VERTLENGTH * LightingVars.Output[i].Alone.Vert1 + 3], 1);
//			Vector3 Vert2 = ObjectTransformMat * glm::vec4(LightingVars.Objects[ObjIterator]->mesh.Vertices[VERTLENGTH * LightingVars.Output[i].Alone.Vert2 + 1], LightingVars.Objects[ObjIterator]->mesh.Vertices[VERTLENGTH * LightingVars.Output[i].Alone.Vert2 + 2], LightingVars.Objects[ObjIterator]->mesh.Vertices[VERTLENGTH * LightingVars.Output[i].Alone.Vert2 + 3], 1);
//
//			VertList[VertIterator] = Vert1.x;
//			VertList[VertIterator + 1] = Vert1.y;
//			VertList[VertIterator + 2] = Vert1.z;
//			VertList[VertIterator + 3] = 1;
//			VertList[VertIterator + 4] = 1;
//			VertList[VertIterator + 5] = 1;
//			VertList[VertIterator + 6] = 1;
//			VertList[VertIterator + 7] = 1;
//			VertList[VertIterator + 8] = 1;
//
//			VertList[VertIterator + 9] = Vert2.x;
//			VertList[VertIterator + 10] = Vert2.y;
//			VertList[VertIterator + 11] = Vert2.z;
//			VertList[VertIterator + 12] = 1;
//			VertList[VertIterator + 13] = 1;
//			VertList[VertIterator + 14] = 1;
//			VertList[VertIterator + 15] = 1;
//			VertList[VertIterator + 16] = 1;
//			VertList[VertIterator + 17] = 1;
//
//			Vector3 Dir1 = Normalize(Vert1 - LightPos) * LIGHTEXTENDDISTANCE;
//			Vector3 Dir2 = Normalize(Vert2 - LightPos) * LIGHTEXTENDDISTANCE;
//
//			VertList[VertIterator + 18] = Vert1.x + Dir1.x;
//			VertList[VertIterator + 19] = Vert1.y + Dir1.y;
//			VertList[VertIterator + 20] = Vert1.z + Dir1.z;
//			VertList[VertIterator + 21] = 1;
//			VertList[VertIterator + 22] = 1;
//			VertList[VertIterator + 23] = 1;
//			VertList[VertIterator + 24] = 1;
//			VertList[VertIterator + 25] = 1;
//			VertList[VertIterator + 26] = 1;
//
//			VertList[VertIterator + 27] = Vert2.x + Dir2.x;
//			VertList[VertIterator + 28] = Vert2.y + Dir2.y;
//			VertList[VertIterator + 29] = Vert2.z + Dir2.z;
//			VertList[VertIterator + 30] = 1;
//			VertList[VertIterator + 31] = 1;
//			VertList[VertIterator + 32] = 1;
//			VertList[VertIterator + 33] = 1;
//			VertList[VertIterator + 34] = 1;
//			VertList[VertIterator + 35] = 1;
//
//
//			IndiceList[IndiceIterator] = IndiceCounter;
//			IndiceList[IndiceIterator + 1] = IndiceCounter + 1;
//			IndiceList[IndiceIterator + 2] = IndiceCounter + 2;
//
//			IndiceList[IndiceIterator + 3] = IndiceCounter + 1;
//			IndiceList[IndiceIterator + 4] = IndiceCounter + 3;
//			IndiceList[IndiceIterator + 5] = IndiceCounter + 2;
//
//			IndiceCounter += 4;
//			IndiceIterator += 6;
//			VertIterator += 36;
//			continue;
//		}
//		if(EdgeIterator < *LightingVars.Objects[ObjIterator]->mesh.ConnectedEdgeCount && LightingVars.Output[i].Connected.Vert1 != -1 && LightingVars.Output[i].Connected.Vert2 != -1)
//		{
//
//			Vector3 Vert1 = ObjectTransformMat * glm::vec4(LightingVars.Objects[ObjIterator]->mesh.Vertices[VERTLENGTH * LightingVars.Output[i].Connected.Vert1 + 1], LightingVars.Objects[ObjIterator]->mesh.Vertices[VERTLENGTH * LightingVars.Output[i].Connected.Vert1 + 2], LightingVars.Objects[ObjIterator]->mesh.Vertices[VERTLENGTH * LightingVars.Output[i].Connected.Vert1 + 3], 1);
//			Vector3 Vert2 = ObjectTransformMat * glm::vec4(LightingVars.Objects[ObjIterator]->mesh.Vertices[VERTLENGTH * LightingVars.Output[i].Connected.Vert2 + 1], LightingVars.Objects[ObjIterator]->mesh.Vertices[VERTLENGTH * LightingVars.Output[i].Connected.Vert2 + 2], LightingVars.Objects[ObjIterator]->mesh.Vertices[VERTLENGTH * LightingVars.Output[i].Connected.Vert2 + 3], 1);
//
//			VertList[VertIterator] = Vert1.x;
//			VertList[VertIterator + 1] = Vert1.y;
//			VertList[VertIterator + 2] = Vert1.z;
//			VertList[VertIterator + 3] = 1;
//			VertList[VertIterator + 4] = 1;
//			VertList[VertIterator + 5] = 1;
//			VertList[VertIterator + 6] = 1;
//			VertList[VertIterator + 7] = 1;
//			VertList[VertIterator + 8] = 1;
//
//			VertList[VertIterator + 9] = Vert2.x;
//			VertList[VertIterator + 10] = Vert2.y;
//			VertList[VertIterator + 11] = Vert2.z;
//			VertList[VertIterator + 12] = 1;
//			VertList[VertIterator + 13] = 1;
//			VertList[VertIterator + 14] = 1;
//			VertList[VertIterator + 15] = 1;
//			VertList[VertIterator + 16] = 1;
//			VertList[VertIterator + 17] = 1;
//
//			Vector3 Dir1 = Normalize(Vert1 - LightPos) * LIGHTEXTENDDISTANCE;
//			Vector3 Dir2 = Normalize(Vert2 - LightPos) * LIGHTEXTENDDISTANCE;
//
//			VertList[VertIterator + 18] = Vert1.x + Dir1.x;
//			VertList[VertIterator + 19] = Vert1.y + Dir1.y;
//			VertList[VertIterator + 20] = Vert1.z + Dir1.z;
//			VertList[VertIterator + 21] = 1;
//			VertList[VertIterator + 22] = 1;
//			VertList[VertIterator + 23] = 1;
//			VertList[VertIterator + 24] = 1;
//			VertList[VertIterator + 25] = 1;
//			VertList[VertIterator + 26] = 1;
//
//			VertList[VertIterator + 27] = Vert2.x + Dir2.x;
//			VertList[VertIterator + 28] = Vert2.y + Dir2.y;
//			VertList[VertIterator + 29] = Vert2.z + Dir2.z;
//			VertList[VertIterator + 30] = 1;
//			VertList[VertIterator + 31] = 1;
//			VertList[VertIterator + 32] = 1;
//			VertList[VertIterator + 33] = 1;
//			VertList[VertIterator + 34] = 1;
//			VertList[VertIterator + 35] = 1;
//
//
//			IndiceList[IndiceIterator] = IndiceCounter;
//			IndiceList[IndiceIterator + 1] = IndiceCounter + 1;
//			IndiceList[IndiceIterator + 2] = IndiceCounter + 2;
//
//			IndiceList[IndiceIterator + 3] = IndiceCounter + 1;
//			IndiceList[IndiceIterator + 4] = IndiceCounter + 3;
//			IndiceList[IndiceIterator + 5] = IndiceCounter + 2;
//
//			IndiceCounter += 4;
//			IndiceIterator += 6;
//			VertIterator += 36;
//		}
//
//		if (i >= LightingVars.OutputCount - 1)
//			continue;
//
//		if (EdgeIterator < LightingVars.Objects[ObjIterator]->mesh.LargerEdgeCount - 1)
//		{
//			EdgeIterator++;
//		}
//		else
//		{
//			EdgeIterator = 0;
//			ObjIterator++;
//			CreateModelMat(ObjectTransformMat, LightingVars.Objects[ObjIterator]->Transform.Position(), LightingVars.Objects[ObjIterator]->Transform.Rotation(), LightingVars.Objects[ObjIterator]->Transform.Scale());
//		}
//	}
//
//	glBindBuffer(GL_ARRAY_BUFFER, LightingVars.MVBO);
//	glBufferData(GL_ARRAY_BUFFER, VertIterator * sizeof(float), VertList, GL_STREAM_DRAW);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, LightingVars.MEBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndiceIterator * sizeof(unsigned int), IndiceList, GL_STREAM_DRAW);
//	LightingVars.IndiceCount = IndiceIterator;
//	delete[](VertList);
//}
//
//void GenerateShadowData(Vector3 LightPosition)
//{
//	LightingVars.computeShader.Use(LightingVars.computeShader.ID);
//
//	//Uniforms
//	LightingVars.computeShader.SetVec3("LightPos", LightPosition);
//	LightingVars.computeShader.SetInt("AloneCount", LightingVars.AloneCount);
//	LightingVars.computeShader.SetInt("ConnectedCount", LightingVars.ConnectedCount);
//
//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, LightingVars.AloneSSBO);
//	glBufferData(GL_SHADER_STORAGE_BUFFER, LightingVars.AloneCount * 64, LightingVars.AloneEdgeIn, GL_STATIC_DRAW);
//	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, LightingVars.AloneSSBO);
//
//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, LightingVars.ConnectedSSBO);
//	glBufferData(GL_SHADER_STORAGE_BUFFER, LightingVars.ConnectedCount * 80, LightingVars.ConnectedEdgeIn, GL_STATIC_DRAW);
//	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, LightingVars.ConnectedSSBO);
//
//	//Output
//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, LightingVars.OutputSSBO);
//	glBufferData(GL_SHADER_STORAGE_BUFFER, LightingVars.OutputCount * 16, nullptr, GL_STATIC_DRAW);
//	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, LightingVars.OutputSSBO);
//
//	//Evalulate
//	glDispatchCompute(LightingVars.OutputCount, 1, 1);
//	glMemoryBarrier(GL_ALL_BARRIER_BITS);
//
//	//Get data
//	LightingVars.Output = (OutputStruct*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
//
//	//Create mesh
//	CreateShadowMesh(LightPosition);
//}
//
//void RenderLight(Shader& shader, Vector3 LightPos)
//{
//	shader.Use(Shading::ShaderPrograms["EdgeShader"]);
//
//	RenderCustomObj(shader, LightingVars.MVAO, LightingVars.MVBO, LightingVars.MEBO, LightingVars.IndiceCount, Vector3(1), transform());
//}
//
//void ClearShadowEdges()
//{
//	delete[](LightingVars.AloneEdgeIn);
//	delete[](LightingVars.ConnectedEdgeIn);
//	LightingVars.AloneEdgeIn = nullptr;
//	LightingVars.ConnectedEdgeIn = nullptr;
//	LightingVars.OutputCount = 0;
//	LightingVars.AloneCount = 0;
//	LightingVars.ConnectedCount = 0;
//	LightingVars.ObjectCount = 0;
//
//	glDeleteBuffers(1, &LightingVars.MVBO);
//	glDeleteBuffers(1, &LightingVars.MEBO);
//	glDeleteBuffers(1, &LightingVars.AloneSSBO);
//	glDeleteBuffers(1, &LightingVars.ConnectedSSBO);
//	glDeleteBuffers(1, &LightingVars.OutputSSBO);
//	LightingVars.MVBO = 0;
//	LightingVars.MEBO = 0;
//}