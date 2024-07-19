#pragma once

#include "Constants.h"
struct Model
{
	unsigned int ID = 0;
	float* Vertices = nullptr;
	unsigned int* Indices = nullptr;
	unsigned int VertCount = 0;
	unsigned int IndiceCount = 0;
	std::string Name;

	void deleteModel()
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
		deleteModel();
	}
	Model() : Vertices(nullptr), Indices(nullptr), VertCount(0), IndiceCount(0) {}

	float*& getVertices()
	{
		float* NewVerts = new float[VertCount];
		for (unsigned int i = 0; i < VertCount; i++)
		{
			NewVerts[i] = Vertices[i];
		}
		return NewVerts;
	}
	unsigned int*& getIndices()
	{
		unsigned int* NewIndices = new unsigned int[IndiceCount];
		for (unsigned int i = 0; i < IndiceCount; i++)
		{
			NewIndices[i] = Indices[i];
		}
		return NewIndices;
	}
	void resetModel()
	{
		deleteModel();
		void LoadModelCopy(Model &Model, unsigned int ID);
		LoadModelCopy(*this, ID);
	}
};

void loadModel(Model& CurrentModel, std::string ModelName, std::string FileType);

namespace ModelLoading
{
	void deleteHeap();
}

#ifdef ModelLoading_MAIN

#include <sstream>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <set>
#include "MathFunctions.h"
#include <map>

struct LoadedModel
{
	float* Vertices = nullptr;
	unsigned int* Indices = nullptr;
	unsigned int VertCount = 0;
	unsigned int IndiceCount = 0;
	std::string Name;
};

#endif