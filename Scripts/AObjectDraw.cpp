/*
* The applications main method of drawing objects
*/

#define AOBJECTDRAWMAIN
#include "AObjectDraw.h"

using namespace AObjectDraw;

Camera MainCamera = Camera();

static Object* Objects[MAXOBJECTS];
static unsigned int ObjCount = 0;
static bool Cstr = false;

//Deletes every object being controlled here
void DeleteAll()
{
	for (int Obj = 0; Obj < MAXOBJECTS; Obj++)
	{
		if (Objects[Obj] == nullptr)
			continue;
		for (int i = 0; i < Objects[Obj]->Scripts.Length; i++)
		{
			((ObjectBehavior*)Objects[Obj]->Scripts[i]->Script)->DestroyHeap();
			delete(Objects[Obj]->Scripts[i]->Script);
			Objects[Obj]->Scripts[i]->Script = nullptr;
		}
		Objects[Obj]->Scripts.Delete();
		delete(Objects[Obj]);
		Objects[Obj] = nullptr;
	}
}

//Begins a few things to start
void Constructor()
{
	if (Cstr)
		return;
	Cstr = true;
	StartOpenGL();
	for (int Obj = 0; Obj < MAXOBJECTS; Obj++)
	{
		Objects[Obj] = nullptr;
	}
}

namespace AObjectDraw
{
	//Called at the start
	void Start(Shader& shader)
	{
		Constructor();

		glEnable(GL_DEPTH_CLAMP);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//Set the object for all scripts
		for (int Obj = 0; Obj < MAXOBJECTS; Obj++)
		{
			if (Objects[Obj] == nullptr)
				continue;
			for (int Scr = 0; Scr < Objects[Obj]->Scripts.Length; Scr++) 
			{
				ObjectBehavior* ScrObj = ((ObjectBehavior*)(Objects[Obj]->Scripts[Scr]->Script));
				if (ScrObj->thisObj == nullptr)
					ScrObj->SetObject(Objects[Obj]);
			}
		}
	}

	//Called every frame
	void Update(Shader& shader)
	{
		glClearColor(0.2, 0.3, 0.3, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		RenderScene(shader, "BasicNoTextureShader", "BasicTextureShader");
	}

	//The ending function
	void End()
	{
		DeleteAll();

		delete[](PerspectiveMat);
		delete[](OrthographicMat);
	}
}

//Removes an object from list
void DeleteObj(Object *&Obj)
{
	if (Obj->mesh.Index < 0 || Objects[Obj->mesh.Index] == nullptr)
		return;
	Objects[Obj->mesh.Index]->Scripts.Delete();

	//This function is called from an objects destructor so use 'free' to avoid loops from forming
	free(Objects[Obj->mesh.Index]);
	Objects[Obj->mesh.Index] = nullptr;
	ObjCount--;
}

//Returns the closet index open in object list
int FindOpenObj()
{
	for (int i = 0; i < MAXOBJECTS; i++)
	{
		if (Objects[i] == nullptr)
			return i;
	}
	return -1;
}

/*
***OUTDATED NAME; NEEDS UPDATING***

Add an object to total list so it can be managed by script calling and rendering
*/
void SetToTransform(Object* Obj)
{
	if (ObjCount >= MAXOBJECTS)
	{
		free(Obj);
		return;
	}
	Constructor();
	int Spot = FindOpenObj();
	Objects[Spot] = Obj;
	Obj->mesh.Index = Spot;

	ObjCount++;
}

//Create the model matrix, 'transform'
void CreateModelMat(glm::mat4& Mat, Vector3 Position, Vector3 Rotation, Vector3 Scale)
{
	Mat = glm::mat4(1);
	Mat = glm::translate(Mat, glm::vec3(Position.x, Position.y, Position.z));
	Mat = glm::rotate(Mat, glm::radians<float>(Rotation.x), glm::vec3(1, 0, 0));
	Mat = glm::rotate(Mat, glm::radians<float>(Rotation.y), glm::vec3(0, 1, 0));
	Mat = glm::rotate(Mat, glm::radians<float>(Rotation.z), glm::vec3(0, 0, 1));
	Mat = glm::scale(Mat, glm::vec3(Scale.x, Scale.y, Scale.z));
}

//Create the camera matrix, 'view'
void CreateCamMat(glm::mat4& Mat, Vector3 Position, Vector3 Rotation)
{
	Mat = glm::mat4(1);
	Mat = glm::rotate(Mat, glm::radians<float>(Rotation.x), glm::vec3(1, 0, 0));
	Mat = glm::rotate(Mat, glm::radians<float>(Rotation.y), glm::vec3(0, 1, 0));
	Mat = glm::rotate(Mat, glm::radians<float>(Rotation.z), glm::vec3(0, 0, 1));
	Mat = glm::translate(Mat, glm::vec3(Position.x, Position.y, Position.z));
}

//Runs all object scripts
void RunScripts(int Obj)
{
	for (int Scr = 0; Scr < Objects[Obj]->Scripts.Length; Scr++)
	{
		ObjectBehavior* ScrObj = ((ObjectBehavior*)(Objects[Obj]->Scripts[Scr]->Script));
		if (ScrObj->thisObj == nullptr)
		{
			ScrObj->SetObject(Objects[Obj]);
			ScrObj->Start();
		}
		if (Objects[Obj]->Scripts[Scr]->Active)
		{
			ScrObj->Update();
		}
	}
}

//Render a custom object, mostly meant for customs like shadow volumes.
void RenderCustomObj(Shader& shader, unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int EBOCount, Vector3 MeshColor, transform Transform)
{
	shader.SetMat4("perp", MainCamera.GetProjectionMatrix());

	Vector3 Pos = MainCamera.Position;
	Vector3 Rot = MainCamera.RotToVec();
	Vector3 Scale;
	CreateCamMat(ViewMat, -Pos, -Rot);
	shader.SetMat4("view", glm::value_ptr(ViewMat));

	Pos = Transform.Position;
	Rot = Transform.Rotation;
	Scale = Transform.Scale;
	CreateModelMat(ObjTransformMat, Pos, Rot, Scale);
	shader.SetMat4("transform", glm::value_ptr(ObjTransformMat));

	shader.SetVec3("MeshColor", MeshColor);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTLENGTH * (sizeof(float)), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, VERTLENGTH * (sizeof(float)), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, VERTLENGTH * (sizeof(float)), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glDrawElements(GL_TRIANGLES, EBOCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

//Render a single object, can be used to have objects appear through walls
void RenderSingleObj(Shader& shader, Object*& ObjToDraw, std::string NoTextureShader, std::string TextureShader)
{
	Vector3 Pos = MainCamera.Position;
	Vector3 Rot = MainCamera.RotToVec();
	Vector3 Scale;
	CreateCamMat(ViewMat, -Pos, -Rot);

	int Obj = ObjToDraw->mesh.Index;

	glBindVertexArray(ObjToDraw->mesh.VAO);


	if (Objects[Obj]->mesh.Texture == -1)
	{
		shader.Use(Shading::ShaderPrograms[NoTextureShader]);
	}
	else
	{
		shader.Use(Shading::ShaderPrograms[TextureShader]);
		shader.SetInt("Texture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Objects[Obj]->mesh.Texture);
	}

	if (!Objects[Obj]->mesh.UI)
	{
		shader.SetMat4("perp", MainCamera.GetProjectionMatrix());
		shader.SetMat4("view", glm::value_ptr(ViewMat));
	}
	else
	{
		shader.SetMat4("perp", (float*)IDENTITYMAT);
		shader.SetMat4("view", (float*)IDENTITYMAT);
	}

	Pos = Objects[Obj]->Transform.Position();
	Rot = Objects[Obj]->Transform.Rotation();
	Scale = Objects[Obj]->Transform.Scale();
	CreateModelMat(ObjTransformMat, Pos, Rot, Scale);
	shader.SetMat4("transform", glm::value_ptr(ObjTransformMat));
	shader.SetVec3("MeshColor", Objects[Obj]->mesh.Color);
	glDrawElements(GL_TRIANGLES, Objects[Obj]->mesh.IndiceCount, GL_UNSIGNED_INT, (void*)(0));
	glBindVertexArray(0);
	return;
}

//Render the basic scene
void RenderScene(Shader& shader, std::string NoTextureShader, std::string TextureShader)
{
	Vector3 Pos = MainCamera.Position;
	Vector3 Rot = MainCamera.RotToVec();
	Vector3 Scale;
	CreateCamMat(ViewMat, -Pos, -Rot);

	for (int Obj = 0; Obj < MAXOBJECTS; Obj++)
	{
		if (Objects[Obj] != nullptr)
		{
			if (Objects[Obj]->Scripts.Length > 0)
				RunScripts(Obj);
			if (Objects[Obj]->Children.Length > 0 && Objects[Obj]->BranchHead)
				Objects[Obj]->UpdateChildren();
		}
	}

	int Count = 0;
	for (int Obj = 0; Obj < MAXOBJECTS; Obj++)
	{
		if (Objects[Obj] == nullptr)
			continue;
		if (!Objects[Obj]->mesh.Active)
			return;

		glBindVertexArray(Objects[Obj]->mesh.VAO);

		if (Objects[Obj]->mesh.Texture == -1)
		{
			shader.Use(Shading::ShaderPrograms[NoTextureShader]);
		}
		else
		{
			shader.Use(Shading::ShaderPrograms[TextureShader]);
			shader.SetInt("Texture", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Objects[Obj]->mesh.Texture);
		}

		if (!Objects[Obj]->mesh.UI)
		{
			shader.SetMat4("perp", MainCamera.GetProjectionMatrix());
			shader.SetMat4("view", glm::value_ptr(ViewMat));
		}
		else
		{
			shader.SetMat4("perp", (float*)IDENTITYMAT);
			shader.SetMat4("view", (float*)IDENTITYMAT);
		}

		Pos = Objects[Obj]->Transform.Position();
		Rot = Objects[Obj]->Transform.Rotation();
		Scale = Objects[Obj]->Transform.Scale();
		CreateModelMat(ObjTransformMat, Pos, Rot, Scale);
		shader.SetMat4("transform", glm::value_ptr(ObjTransformMat));
		shader.SetVec3("MeshColor", Objects[Obj]->mesh.Color);
		glDrawElements(GL_TRIANGLES, Objects[Obj]->mesh.IndiceCount, GL_UNSIGNED_INT, (void*)(0));
	}
}