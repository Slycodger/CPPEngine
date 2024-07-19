#define ModelLoading_MAIN
#include "ModelLoading.h"


namespace StringEnum
{
	enum StringResolve { Invalid = -1, MYOBJ };
};


std::set<std::string> FileTypes = { "MYOBJ" };
std::map<std::string, int> ObjsLoaded;

LoadedModel ModelsLoaded[MAXMODELCOUNT];
static unsigned int ModelLoadedCount = 0;


//returns the type of file the string is
StringEnum::StringResolve evaluateString(std::string Compare)
{
	if (Compare == "MYOBJ")
		return StringEnum::MYOBJ;

	return StringEnum::Invalid;
}

//Creates a new float pointer to hold a models vertices
float*& returnVerticeCopy(unsigned int ID)
{
	float* NewVerts = new float[ModelsLoaded[ID].VertCount];
	for (int i = 0; i < ModelsLoaded[ID].VertCount; i++)
	{
		NewVerts[i] = ModelsLoaded[ID].Vertices[i];
	}
	return NewVerts;
}

//Creates a new unsigned int pointer to hold a models indices
unsigned int*& returnIndiceCopy(unsigned int ID)
{
	unsigned int* NewIndices = new unsigned int[ModelsLoaded[ID].IndiceCount];
	for (int i = 0; i < ModelsLoaded[ID].IndiceCount; i++)
	{
		NewIndices[i] = ModelsLoaded[ID].Indices[i];
	}
	return NewIndices;
}

//Creates a model copy for an object
//Meant for if one script wants to change the mesh of an object, it won't mess with all the other models
void loadModelCopy(Model &Model, unsigned int ID)
{
	Model.deleteModel();
	Model.Vertices = returnVerticeCopy(ID);
	Model.Indices = returnIndiceCopy(ID);
	Model.VertCount = ModelsLoaded[ID].VertCount;
	Model.IndiceCount = ModelsLoaded[ID].IndiceCount;
	Model.Name = ModelsLoaded[ID].Name;
}

//Gets an open index in the loaded model list
int getFreeModelSlot()
{
	for (unsigned int i = 0; i < MAXMODELCOUNT; i++)
	{
		if (ModelsLoaded[i].VertCount == 0)
			return i;
	}
	return -1;
}

//Parses a double
//I uses this instead of std::stod for reasons
double parseDouble(std::string String)
{
	bool Neg = false, Expo = false, Deci = false;
	double RetVal = 0, UseVal = 0, MulDivVal = 1;
	for (char const Char : String)
	{
		if (Char == '-')
		{
			Neg = true;
			continue;
		}
		if (Char == 'e' || Char == 'E')
		{
			Expo = true;
			RetVal += UseVal * MulDivVal;
			if (Neg)
				RetVal *= -1;
			Neg = false;
			Deci = false;
			UseVal = 0;
			continue;
		}
		if (Char == '.' && !Expo)
		{
			RetVal += UseVal;
			UseVal = 0;
			Deci = true;
			continue;
		}
		UseVal *= 10;
		UseVal += (Char - 48);
		if (Deci)
			MulDivVal /= 10;
	}
	if (Neg)
		UseVal *= -1;
	if (Expo)
	{
		RetVal *= std::pow(10, UseVal);
		return RetVal;
	}
	RetVal += UseVal * MulDivVal;
	return RetVal;
}

//Parses an unsigned int
unsigned int parseUnsignedInterger(std::string String)
{
	unsigned int Ret = 0;
	for (const char Char : String)
	{
		Ret *= 10;
		Ret += Char - 48;
	}
	return Ret;
}

//Converts a meshes vertices from element based to array based for drawing
void elementToArrayDrawing(float*& OldVertices, unsigned int*& Indices, unsigned int IndiceCount, unsigned int VertLength)
{
	float* Vertices = new float[IndiceCount * VertLength];

	for (unsigned int Index = 0; Index < IndiceCount; Index++)
	{
		for (int i = 0; i < VertLength; i++)
		{
			Vertices[Index * VertLength + i] = OldVertices[Indices[Index] * VertLength + i];
		}
	}
	delete[](OldVertices);
	OldVertices = Vertices;
}

//Adds normals to an array types vertices
//Only works with array typed
void addNormalsToArray(float*& Vertices, unsigned int VertCount, unsigned int VertLength, unsigned int Padding)
{
	unsigned int TrueLength = VertLength + Padding;

	Vector3 Vert1;
	Vector3 Vert2;
	Vector3 Vert3;

	for (int i = 0; i < VertCount; i += TrueLength * 3)
	{
		Vert1.x = Vertices[i];
		Vert1.y = Vertices[i + 1];
		Vert1.z = Vertices[i + 2];

		Vert2.x = Vertices[i + TrueLength];
		Vert2.y = Vertices[i + TrueLength + 1];
		Vert2.z = Vertices[i + TrueLength + 2];

		Vert3.x = Vertices[i + (TrueLength * 2)];
		Vert3.y = Vertices[i + (TrueLength * 2) + 1];
		Vert3.z = Vertices[i + (TrueLength * 2) + 2];

		Vector3 Normal = getNormal(Vert1, Vert2, Vert3);

		if (Padding > 0)
		{
			Vertices[i + VertLength] = Normal.x;
			Vertices[i + TrueLength + VertLength] = Normal.x;
			Vertices[i + (TrueLength * 2) + VertLength] = Normal.x;
		}
		if (Padding > 1)
		{
			Vertices[i + VertLength + 1] = Normal.y;
			Vertices[i + TrueLength + VertLength + 1] = Normal.y;
			Vertices[i + (TrueLength * 2) + VertLength + 1] = Normal.y;
		}
		if (Padding > 2)
		{
			Vertices[i + VertLength + 2] = Normal.z;
			Vertices[i + TrueLength + VertLength + 2] = Normal.z;
			Vertices[i + (TrueLength * 2) + VertLength + 2] = Normal.z;
		}
	}
}


//Loads a MYOBJ file
int loadMYOBJFile(LoadedModel& CurrentModel, std::string ModelName, std::string FileType)
{
	unsigned int VertPadding = 0;

	//Load file
	std::string FileString;
	std::ifstream FileStream;
	unsigned int CharCount = 0;
	FileStream.exceptions(std::ios::failbit);
	try
	{
		FileStream.open("./Objects/" + ModelName + "." + FileType);
		std::stringstream Stream;
		Stream << FileStream.rdbuf();
		FileString = Stream.str();
		FileStream.seekg(0, std::ios::end);
		CharCount = FileStream.tellg();
		FileStream.close();
	}
	catch (const std::ios::failure& fail)
	{
		std::cout << "Failed to open " + ModelName + " file.";
	}

	//Get counts
	unsigned int VertCount = 0;
	unsigned int IndiceCount = 0;
	unsigned int StartingPoint = 0;
	std::string NumString;

	for (unsigned int Char = 0; Char < CharCount; Char++)
	{
		if (FileString[Char] == 'n')
			break;
		if ((FileString[Char] == ' ' && FileString[Char - 1] != ' ') || (FileString[Char] == '\r' && FileString[Char + 1] == '\n' && FileString[Char - 1] != '\n') || (FileString[Char] == '\n' && FileString[Char - 1] != '\n'))
		{
			if (VertCount == 0)
				VertCount = parseUnsignedInterger(NumString) * VERTLENGTH;
			else
			{
				IndiceCount = parseUnsignedInterger(NumString);
				StartingPoint = Char + 1;
				break;
			}
			NumString.clear();
		}
		if (FileString[Char] != ' ' && FileString[Char] != '\n' && FileString[Char] != '\r')
			NumString += FileString[Char];
	}

	VertCount += (VertCount / VERTLENGTH) * VertPadding;

	NumString.clear();

	//Get data
	float* Vertices = new float[VertCount];
	unsigned int* Indices = new unsigned int[IndiceCount];

	unsigned int VerticesLoaded = 0;
	unsigned int IndicesLoaded = 0;
	unsigned int CountInVert = 0;

	for (unsigned int Char = StartingPoint; Char < CharCount; Char++)
	{
		if (FileString[Char] == 'n')
			break;
		if ((FileString[Char] == ' ' && FileString[Char - 1] != ' ') || (FileString[Char] == '\r' && FileString[Char + 1] == '\n' && FileString[Char - 1] != '\n') || (FileString[Char] == '\n' && FileString[Char - 1] != '\n'))
		{
			if (VerticesLoaded >= VertCount && IndicesLoaded < IndiceCount)
			{
				Indices[IndicesLoaded] = parseUnsignedInterger(NumString);
				IndicesLoaded++;
				NumString.clear();
				continue;
			}
			Vertices[VerticesLoaded] = parseDouble(NumString);
			VerticesLoaded++;
			CountInVert++;
			if (CountInVert >= 9)
			{
				for (unsigned int i = 0; i < VertPadding; i++)
				{
					Vertices[VerticesLoaded + i] = 0;
				}
				VerticesLoaded += VertPadding;
				CountInVert = 0;
			}
			NumString.clear();
			continue;
		}
		if (FileString[Char] != ' ' && FileString[Char] != '\n' && FileString[Char] != '\r')
			NumString += FileString[Char];
	}
	Indices[IndicesLoaded] = parseUnsignedInterger(NumString);

	CurrentModel.Vertices = Vertices;
	CurrentModel.Indices = Indices;
	CurrentModel.VertCount = VertCount;
	CurrentModel.IndiceCount = IndiceCount;
	CurrentModel.Name = ModelName;

	return 0;
}

//Loads an object from file into data
int loadModelIntoBuffer(Model& CurrentModel, std::string ModelName, std::string FileType)
{
	if (CurrentModel.VertCount > 0)
		CurrentModel.deleteModel();
	if (ObjsLoaded.contains(ModelName))
		return 0;

	if (ModelLoadedCount >= MAXMODELCOUNT)
		return 0;
	if (FileTypes.find(FileType) == FileTypes.end())
		return 0;
	if (!std::filesystem::is_regular_file("./Objects/" + ModelName + "." + FileType))
		return 0;

	unsigned int FreeModelSlot = getFreeModelSlot();

	switch (evaluateString(FileType))
	{
	case StringEnum::MYOBJ:
		loadMYOBJFile(ModelsLoaded[FreeModelSlot], ModelName, FileType);
		break;
	case StringEnum::Invalid:
		break;
	default:
		break;
	}
	loadModelCopy(CurrentModel, FreeModelSlot);
	ObjsLoaded.insert({ ModelName, FreeModelSlot });
	ModelLoadedCount++;
	return 0;
}

//Unloads a loaded model from the buffer
//models created through 'loadModelCopy' must be deleted manually
void unloadModelFromBuffer(unsigned int ID)
{
	delete[](ModelsLoaded[ID].Vertices);
	ModelsLoaded[ID].Vertices = nullptr;
	delete[](ModelsLoaded[ID].Indices);
	ModelsLoaded[ID].Indices = nullptr;
	ModelsLoaded[ID].VertCount = 0;
	ModelsLoaded[ID].IndiceCount = 0;
	ModelLoadedCount--;
	ObjsLoaded.erase(ModelsLoaded[ID].Name);
}

//Loads a model
//Will load into loaded models if it doesn't exist
//If it does exist then will copy from the loaded model
void loadModel(Model& CurrentModel, std::string ModelName, std::string FileType)
{
	if (CurrentModel.VertCount > 0)
		CurrentModel.deleteModel();
	if (!ObjsLoaded.contains(ModelName))
		loadModelIntoBuffer(CurrentModel, ModelName, FileType);
	else
		loadModelCopy(CurrentModel, ObjsLoaded[ModelName]);
}

namespace ModelLoading
{
	void deleteHeap()
	{
		for (int i = 0; i < MAXMODELCOUNT; i++)
		{
			delete[](ModelsLoaded[i].Vertices);
			ModelsLoaded[i].Vertices = nullptr;
			delete[](ModelsLoaded[i].Indices);
			ModelsLoaded[i].Indices = nullptr;

			ModelsLoaded[i].VertCount = 0;
			ModelsLoaded[i].IndiceCount = 0;
		}
	}
}