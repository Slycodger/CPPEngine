/*
* Application default method of handling and rendering objects
*/

#define AppObjHandler_MAIN
#include "AppObjHandler.h"

using namespace AppObjHandler;

Camera MainCamera = Camera();

static Object* Objects[MAXOBJECTS];
static unsigned int ObjCount = 0;
static bool Cstr = false;

static glm::mat4 ObjTransformMat(1);
static glm::mat4 ViewMat(1);

//Deletes every object in the handlerList
void deleteScene()
{
	for (int Obj = 0; Obj < MAXOBJECTS; Obj++)
	{
		if (Objects[Obj] == nullptr)
			continue;
		for (unsigned int i = 0; i < Objects[Obj]->Scripts.Length; i++)
		{
			((ObjectBehavior*)(Objects[Obj]->Scripts[i]->Script))->deleteHeap();
			delete(Objects[Obj]->Scripts[i]->Script);
			Objects[Obj]->Scripts[i]->Script = nullptr;
		}
		Objects[Obj]->Scripts.deleteList();
		delete[](Objects[Obj]->mesh.Vertices);
		delete[](Objects[Obj]->mesh.Indices);
		delete(Objects[Obj]);
		Objects[Obj] = nullptr;
	}
}

//Calls the StartOpenGL function to make sure it started, then sets all objects in handlerList to null
void appIntializer()
{
	if (Cstr)
		return;
	Cstr = true;
	startOpenGL();
	for (int Obj = 0; Obj < MAXOBJECTS; Obj++)
	{
		Objects[Obj] = nullptr;
	}
}

namespace AppObjHandler
{
	//Called at the start
	void start()
	{
		appIntializer();

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//Allows scripts to access their objects and then starts them
		for (int Obj = 0; Obj < MAXOBJECTS; Obj++)
		{
			if (Objects[Obj] == nullptr)
				continue;
			for (int Scr = 0; Scr < Objects[Obj]->Scripts.Length; Scr++) 
			{
				ObjectBehavior* ScrObj = ((ObjectBehavior*)(Objects[Obj]->Scripts[Scr]->Script));
				if (ScrObj->thisObj == nullptr)
				{
					ScrObj->setObject(Objects[Obj]);
					ScrObj->start();
				}
			}
		}
	}

	//Called every frame
	void update()
	{
		if (Keys::keyPressed(GLFW_KEY_ESCAPE))
		{
			App::Stop = true;
			return;
		}

		updateObjects();

		renderScene("BasicNoTextureShader", "BasicTextureShader");
	}

	//The ending function
	void end()
	{
		deleteScene();
	}
}

//deletes an object that's within handlerList, uses object pointer
void deleteObj(Object *&Obj)
{
	if (Obj->mesh.Index < 0 || Objects[Obj->mesh.Index] == nullptr)
		return;
	for (unsigned int i = 0; i < Obj->Scripts.Length; i++)
	{
		((ObjectBehavior*)(Obj->Scripts[i]->Script))->deleteHeap();
		delete(Obj->Scripts[i]->Script);
		Obj->Scripts[i]->Script = nullptr;
	}
	Obj->Scripts.deleteList();

	int Index = Obj->mesh.Index;
	delete(Objects[Index]);
	Objects[Index] = nullptr;
	ObjCount--;
}

//deletes an object that's within handlerList, uses objects index
void deleteObj(int Obj)
{
	if (Obj < 0 || Objects[Obj] == nullptr)
		return;
	for (unsigned int i = 0; i < Objects[Obj]->Scripts.Length; i++)
	{
		((ObjectBehavior*)(Objects[Obj]->Scripts[i]->Script))->deleteHeap();
		delete(Objects[Obj]->Scripts[i]->Script);
		Objects[Obj]->Scripts[i]->Script = nullptr;
	}
	Objects[Obj]->Scripts.deleteList();

	delete(Objects[Obj]);
	Objects[Obj] = nullptr;
	ObjCount--;
}

//Returns the closet index open in handlerList
int findOpenSlot()
{
	for (int i = 0; i < MAXOBJECTS; i++)
	{
		if (Objects[i] == nullptr)
			return i;
	}
	return -1;
}

//Adds an object to the handlerList
//Automatically called if you create an object via model
void addObjToList(Object* Obj)
{
	if (ObjCount >= MAXOBJECTS)
	{
		free(Obj);
		return;
	}
	int Spot = findOpenSlot();
	Objects[Spot] = Obj;
	Obj->mesh.Index = Spot;

	ObjCount++;
}

//Creates an objects transform matrix to be used within a shader, takes in object pointer
void createObjTransformMatrix(glm::mat4& Mat, Object*& Obj)
{
	Vector3 Position = Obj->Transform.Position;
	Vector3 Rotation = Obj->Transform.Rotation;
	Vector3 Scale = Obj->Transform.Scale;
	Mat = glm::mat4(1);

	Mat = glm::translate(Mat, glm::vec3(Position.x, Position.y, Position.z));
	Mat = glm::rotate(Mat, glm::radians<float>(Rotation.x), glm::vec3(1, 0, 0));
	Mat = glm::rotate(Mat, glm::radians<float>(Rotation.y), glm::vec3(0, 1, 0));
	Mat = glm::rotate(Mat, glm::radians<float>(Rotation.z), glm::vec3(0, 0, 1));
	Mat = glm::scale(Mat, glm::vec3(Scale.x, Scale.y, Scale.z));
}

//Creates an objects transform matrix to be used within a shader, takes in values individually
void createObjTransformMatrix(glm::mat4& Mat, Vector3 Pos, Vector3 Rot, Vector3 Scale)
{
	Mat = glm::mat4(1);
	Mat = glm::translate(Mat, glm::vec3(Pos.x, Pos.y, Pos.z));
	Mat = glm::rotate(Mat, glm::radians<float>(Rot.x), glm::vec3(1, 0, 0));
	Mat = glm::rotate(Mat, glm::radians<float>(Rot.y), glm::vec3(0, 1, 0));
	Mat = glm::rotate(Mat, glm::radians<float>(Rot.z), glm::vec3(0, 0, 1));
	Mat = glm::scale(Mat, glm::vec3(Scale.x, Scale.y, Scale.z));
}

//Creates the view matrix, values passed in should be from camera
void createViewMatrix(glm::mat4& Mat, Vector3 Position, Vector3 Rotation)
{
	Mat = glm::mat4(1);
	Mat = glm::rotate(Mat, glm::radians<float>(Rotation.x), glm::vec3(1, 0, 0));
	Mat = glm::rotate(Mat, glm::radians<float>(Rotation.y), glm::vec3(0, 1, 0));
	Mat = glm::rotate(Mat, glm::radians<float>(Rotation.z), glm::vec3(0, 0, 1));
	Mat = glm::translate(Mat, glm::vec3(Position.x, Position.y, Position.z));
}

//Runs object scripts in handlerList
void runObjectScripts(int Obj)
{
	for (int Scr = 0; Scr < Objects[Obj]->Scripts.Length; Scr++)
	{
		ObjectBehavior* ScrObj = ((ObjectBehavior*)(Objects[Obj]->Scripts[Scr]->Script));
		if (ScrObj->thisObj == nullptr)
		{
			ScrObj->setObject(Objects[Obj]);
			ScrObj->start();
		}
		if (Objects[Obj]->Scripts[Scr]->Active)
			ScrObj->update();
	}
}

//Renders a custom object
void renderCustomObj(unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int EBOCount, Vector3 MeshColor, transform Transform)
{
	shader.setMat4("perp", MainCamera.GetProjectionMatrix());

	Vector3 Pos = MainCamera.Position;
	Vector3 Rot = MainCamera.RotToVec();
	Vector3 Scale;
	createViewMatrix(ViewMat, -Pos, -Rot);
	shader.setMat4("view", glm::value_ptr(ViewMat));

	Pos = Transform.Position;
	Rot = Transform.Rotation;
	Scale = Transform.Scale;
	createObjTransformMatrix(ObjTransformMat, Pos, Rot, Scale);
	shader.setMat4("transform", glm::value_ptr(ObjTransformMat));

	shader.setVec3("MeshColor", MeshColor);

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

//Renders a single object
void renderSingleObject(Object*& ObjToDraw, std::string NoTextureShader, std::string TextureShader, bool GenView)
{
	if(GenView)
		createViewMatrix(ViewMat, -MainCamera.Position, -MainCamera.Rotation());

	int Obj = ObjToDraw->mesh.Index;

	glBindVertexArray(ObjToDraw->mesh.VAO);


	if (Objects[Obj]->mesh.Texture == -1)
	{
		shader.use(Shading::ShaderPrograms[NoTextureShader]);
	}
	else
	{
		shader.use(Shading::ShaderPrograms[TextureShader]);
		shader.setInt("Texture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Objects[Obj]->mesh.Texture);
	}

	if (!Objects[Obj]->mesh.UI)
	{
		shader.setMat4("perp", MainCamera.GetProjectionMatrix());
		shader.setMat4("view", glm::value_ptr(ViewMat));
	}
	else
	{
		shader.setMat4("perp", (float*)IDENTITYMAT);
		shader.setMat4("view", (float*)IDENTITYMAT);
	}

	createObjTransformMatrix(ObjTransformMat, Objects[Obj]);
	shader.setMat4("transform", glm::value_ptr(ObjTransformMat));
	shader.setVec4("MeshColor", Objects[Obj]->mesh.Color);
	glDrawElements(GL_TRIANGLES, Objects[Obj]->mesh.IndiceCount, GL_UNSIGNED_INT, (void*)(0));
	glBindVertexArray(0);
	return;
}

//Renders an object to the stencil buffer
void renderObjectStencil(Object*& Obj)
{
	glBindVertexArray(Obj->mesh.VAO);

	shader.use(Shading::ShaderPrograms["WhiteSpaceShader"]);

	if (!Obj->mesh.UI)
	{
		shader.setMat4("perp", MainCamera.GetProjectionMatrix());
		shader.setMat4("view", glm::value_ptr(ViewMat));
	}
	else
	{
		shader.setMat4("perp", (float*)IDENTITYMAT);
		shader.setMat4("view", (float*)IDENTITYMAT);
	}

	createObjTransformMatrix(ObjTransformMat, Obj);
	shader.setMat4("transform", glm::value_ptr(ObjTransformMat));
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);
	glDrawElements(GL_TRIANGLES, Obj->mesh.IndiceCount, GL_UNSIGNED_INT, (void*)(0));
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);
	glBindVertexArray(0);
	return;
}

//Renders a chain of objects
//Makes the dependencies work
void renderObjectChain(std::string& NoTextureShader, std::string& TextureShader, Object*& Obj)
{
	glBindVertexArray(Obj->mesh.VAO);

	if (Obj->mesh.Texture == -1)
	{
		shader.use(Shading::ShaderPrograms[NoTextureShader]);
	}
	else
	{
		shader.use(Shading::ShaderPrograms[TextureShader]);
		shader.setInt("Texture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Obj->mesh.Texture);
	}

	createObjTransformMatrix(ObjTransformMat, Obj);

	if (!Obj->mesh.UI)
	{
		shader.setMat4("perp", MainCamera.GetProjectionMatrix());
		shader.setMat4("view", glm::value_ptr(ViewMat));
	}
	else
	{
		shader.setMat4("perp", (float*)IDENTITYMAT);
		shader.setMat4("view", (float*)IDENTITYMAT);
	}
	shader.setMat4("transform", glm::value_ptr(ObjTransformMat));


	shader.setVec4("MeshColor", Obj->mesh.Color);
	glDrawElements(GL_TRIANGLES, Obj->mesh.IndiceCount, GL_UNSIGNED_INT, (void*)(0));

	for (int i = 0; i < Obj->Children.Length; i++)
	{
		if((*Obj->Children[i])->Dependency == nullptr)
			renderObjectChain(NoTextureShader, TextureShader, *Obj->Children[i]);
	}
}

//Renders an objects dependencies
void renderObjectDependencies(std::string& NoTextureShader, std::string& TextureShader, Object*& Obj)
{
	glEnable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);
	glStencilFunc(GL_ALWAYS, 1, 0xff);
	glStencilOp(GL_INCR, GL_INCR, GL_INCR);
	renderObjectStencil(Obj);
	glStencilFunc(GL_NOTEQUAL, 0, 0xff);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	for (int i = 0; i < Obj->Dependents.Length; i++)
		renderObjectChain(NoTextureShader, TextureShader, *Obj->Dependents[i]);
	for (int i = 0; i < Obj->Dependents.Length; i++)
	{
		if ((*Obj->Dependents[i])->Dependents.Length <= 0)
			continue;
		renderObjectDependencies(NoTextureShader, TextureShader, *Obj->Dependents[i]);
	}
	glDisable(GL_STENCIL_TEST);
}

/***
* Need to update
* Meant for rendering an object chain from the top-level down
* I think objects are being double drawn under certain circumstances
* 
* Goals : 
* Organize and remove at least 1 function from the process
***/
void renderSceneObject(std::string& NoTextureShader, std::string& TextureShader, Object*& Obj)
{
	if (Obj->Dependency != nullptr)
		return;
	renderObjectChain(NoTextureShader, TextureShader, Obj);

	for (int i = 0; i < Obj->Children.Length; i++)
	{
		renderSceneObject(NoTextureShader, TextureShader, *Obj->Children[i]);
	}
	if (Obj->Dependents.Length > 0)
		renderObjectDependencies(NoTextureShader, TextureShader, Obj);
}

//Starts the rendering process for all objects in handlerNet
void renderScene(std::string NoTextureShader, std::string TextureShader)
{
	glClearColor(0.2, 0.3, 0.3, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	createViewMatrix(ViewMat, -MainCamera.Position, -MainCamera.RotToVec());

	for (int Obj = 0; Obj < MAXOBJECTS; Obj++)
	{
		if (Objects[Obj] == nullptr || Objects[Obj]->mesh.Vertices == nullptr || !Objects[Obj]->BranchHead)
			continue;
		if (!Objects[Obj]->mesh.Active)
			continue;

		renderSceneObject(NoTextureShader, TextureShader, Objects[Obj]);
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//Runs scripts and moves child objects within handlerList
void updateObjects()
{
	for (int Obj = 0; Obj < MAXOBJECTS; Obj++)
	{
		if (Objects[Obj] == nullptr || !Objects[Obj]->mesh.Active)
			continue;

		if (Objects[Obj]->Children.Length > 0)
		{
			if (Objects[Obj]->BranchHead)
				Objects[Obj]->updateChildren();
		}
		else if (Objects[Obj]->Parent == nullptr)
			Objects[Obj]->RelativeTransform = Objects[Obj]->Transform;
		if (Objects[Obj]->Scripts.Length > 0)
			runObjectScripts(Obj);
	}
}
