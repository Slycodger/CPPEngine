#ifndef MODELLOADING
#define MODELLOADING
#include <iostream>
#include "Constants.h"
struct Model
{
	unsigned int ID = 0;
	float* Vertices = nullptr;
	unsigned int* Indices = nullptr;
	unsigned int VertCount = 0;
	unsigned int IndiceCount = 0;
	std::string Name;

	void DeleteModel()
	{
		delete[](Vertices);
		Vertices = nullptr;
		delete[](Indices);
		Indices = nullptr;

		VertCount = 0;
		IndiceCount = 0;
	}

	~Model()
	{
		DeleteModel();
	}
	Model() : Vertices(nullptr), Indices(nullptr), VertCount(0), IndiceCount(0) {}

	float*& GetVertices()
	{
		float* NewVerts = new float[VertCount];
		for (int i = 0; i < VertCount; i++)
		{
			NewVerts[i] = Vertices[i];
		}
		return NewVerts;
	}
	unsigned int*& GetIndices()
	{
		unsigned int* NewIndices = new unsigned int[IndiceCount];
		for (int i = 0; i < IndiceCount; i++)
		{
			NewIndices[i] = Indices[i];
		}
		return NewIndices;
	}
	void ResetModel()
	{
		Model ReturnModelCopy(unsigned int ID);
		*this = ReturnModelCopy(ID);
	}
};

void LoadModel(Model& CurrentModel, std::string ModelName, std::string FileType);

namespace ModelLoading
{
	void DeleteHeap();
}

#ifdef MODELLOADINGMAIN

#include <sstream>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <set>
#include <cmath>
#include "MathFunctions.h"


struct LoadedModel
{
	float* Vertices = nullptr;
	unsigned int* Indices = nullptr;
	unsigned int VertCount = 0;
	unsigned int IndiceCount = 0;
	std::string Name;
};
struct Edge
{
	Vector3 Vert1;
	Vector3 Vert2;
	Vector3 Normal;
	bool operator == (const Edge& Obj)
	{
		if ((Vert1 == Obj.Vert1 && Vert2 == Obj.Vert2) || (Vert2 == Obj.Vert1 && Vert1 == Obj.Vert2))
			return true;
		return false;
	}
	Edge(Vector3 vec1, Vector3 vec2, Vector3 vec3) : Vert1(vec1), Vert2(vec2), Normal(vec3) {}
	Edge() : Vert1(), Vert2(), Normal() {}
};

#endif
#endif