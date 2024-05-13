/*
* The applications main method of drawing objects
*/

#define AOBJECTDRAWMAIN
#include "AObjectDraw.h"

using namespace AObjectDraw;

Camera MainCamera;

Object* Objects[MAXOBJECTS];
unsigned int ObjCount = 0;
unsigned int ObjVAO = 0;
bool Cstr = false;

Vector3 LightPosition = Vector3(0, 0, 0);

unsigned int ScreenFBO, ScreenTexture, ScreenDepth;


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

		glGenVertexArrays(1, &ObjVAO);

		//ScreenQuad->mesh.Active = false;
		//ScreenQuad->mesh.UI = true;
		//ScreenQuad->Transform.Translate(Vector3(0, 0, -1));

		glGenFramebuffers(1, &ScreenFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, ScreenFBO);
		
		glGenTextures(1, &ScreenTexture);
		glBindTexture(GL_TEXTURE_2D, ScreenTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, ScreenSpace::Width, ScreenSpace::Height, 0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ScreenTexture, 0);

		glGenTextures(1, &ScreenDepth);
		glBindTexture(GL_TEXTURE_2D, ScreenDepth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, ScreenSpace::Width, ScreenSpace::Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, ScreenDepth, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//ScreenQuad->mesh.Texture = ScreenTexture;

		//TempCube->Transform.Translate(-10, 0, 0);
		MainCamera.Position = LightPosition;
	}

	float LightSpeed = 5;

	void Update(Shader& shader)
	{
		if (Keys::KeyHeld(GLFW_KEY_UP))
			LightPosition.z += LightSpeed * Time::deltaTime;
		if (Keys::KeyHeld(GLFW_KEY_DOWN))
			LightPosition.z -= LightSpeed * Time::deltaTime;
		if (Keys::KeyHeld(GLFW_KEY_RIGHT))
			LightPosition.x += LightSpeed * Time::deltaTime;
		if (Keys::KeyHeld(GLFW_KEY_LEFT))
			LightPosition.x -= LightSpeed * Time::deltaTime;
		if (Keys::KeyHeld(GLFW_KEY_PAGE_UP))
			LightPosition.y += LightSpeed * Time::deltaTime;
		if (Keys::KeyHeld(GLFW_KEY_PAGE_DOWN))
			LightPosition.y -= LightSpeed * Time::deltaTime;
		if (Keys::KeyPressed(GLFW_KEY_LEFT_CONTROL))
			MainCamera.Position = LightPosition;

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

		glClearColor(0.2, 0.3, 0.3, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		RenderScene(shader, "BasicNoTextureShader", "BasicTextureShader");


		//glEnable(GL_STENCIL_TEST);
		//glDepthMask(GL_FALSE);
		//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		//glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR, GL_KEEP);
		//glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR, GL_KEEP);

		//UpdateEdgeData();
		//GenerateShadowData(LightPosition);
		//RenderStretchScene(shader);
		//RenderLight(shader, LightPosition);

		//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		//glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		//glStencilFunc(GL_EQUAL, 0xFF, 0xFF);
		//glEnable(GL_BLEND);
		//glBlendEquation(GL_FUNC_ADD);
		//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		//glDepthMask(GL_TRUE);
		//glClear(GL_DEPTH_BUFFER_BIT);

		//RenderShadows(shader);
	}

	void End()
	{
		DeleteAll();

		delete[](PerspectiveMat);
		delete[](OrthographicMat);
		//ClearShadowEdges();
	}
}

void DeleteObj(Object *&Obj)
{
	if (Obj->mesh.Index < 0 || Objects[Obj->mesh.Index] == nullptr)
		return;
	Objects[Obj->mesh.Index]->Scripts.Delete();
	free(Objects[Obj->mesh.Index]);
	Objects[Obj->mesh.Index] = nullptr;
	ObjCount--;
}

int FindOpenObj()
{
	for (int i = 0; i < MAXOBJECTS; i++)
	{
		if (Objects[i] == nullptr)
			return i;
	}
	return -1;
}

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

void CreateModelMat(glm::mat4& Mat, Vector3 Position, Vector3 Rotation, Vector3 Scale)
{
	Mat = glm::mat4(1);
	Mat = glm::translate(Mat, glm::vec3(Position.x, Position.y, Position.z));
	Mat = glm::rotate(Mat, glm::radians<float>(Rotation.x), glm::vec3(1, 0, 0));
	Mat = glm::rotate(Mat, glm::radians<float>(Rotation.y), glm::vec3(0, 1, 0));
	Mat = glm::rotate(Mat, glm::radians<float>(Rotation.z), glm::vec3(0, 0, 1));
	Mat = glm::scale(Mat, glm::vec3(Scale.x, Scale.y, Scale.z));
}

void CreateCamMat(glm::mat4& Mat, Vector3 Position, Vector3 Rotation)
{
	Mat = glm::mat4(1);
	Mat = glm::rotate(Mat, glm::radians<float>(Rotation.x), glm::vec3(1, 0, 0));
	Mat = glm::rotate(Mat, glm::radians<float>(Rotation.y), glm::vec3(0, 1, 0));
	Mat = glm::rotate(Mat, glm::radians<float>(Rotation.z), glm::vec3(0, 0, 1));
	Mat = glm::translate(Mat, glm::vec3(Position.x, Position.y, Position.z));
}

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

float* CreatePerspectiveMat(float FOVAngle, float Ratio, float FarClip, float NearClip)
{
	float RangeRecip = 1 / (NearClip - FarClip);
	float FOVRatio = glm::tan(0.5 * FOVAngle * DegToRad);
	float *Ret = new float[16]
	{
		1 / (FOVRatio * Ratio), 0, 0, 0,
		0, 1 / FOVRatio, 0, 0,
		0, 0, (NearClip + FarClip) * RangeRecip, -1,
		0, 0, NearClip * FarClip * RangeRecip * 2, 0
	};
	return Ret;
}

void CreateOrthographicMat()
{
	delete[](OrthographicMat);
	OrthographicMat = new float[16]
	{
		2 / (orthRight - orthLeft), 0, 0, 0,
		0, 2 / (orthTop - orthBottom), 0, 0,
		0, 0, -2 * orthoRangeRecip, 0,
		0, 0, -(CameraFarClip + CameraNearClip) / (CameraFarClip - CameraNearClip), 1
	};
}

void RenderCustomObj(Shader &shader, unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int EBOCount, Vector3 MeshColor, transform Transform)
{
	shader.SetMat4("perp", PerspectiveMat);

	Vector3 Pos = MainCamera.Position;
	Vector3 Rot = MainCamera.RotToVec();
	Vector3 Scale;
	CreateCamMat(ViewMat, -Pos, -Rot);
	shader.SetMat4("view", glm::value_ptr(ViewMat));

	Pos = Transform.Position;
	Rot = Transform.Rotation;
	Scale = Transform.Scale;
	CreateModelMat(ObjTransformMat, Pos / ScreenSpace::gridHeight, Rot, Scale / ScreenSpace::gridHeight);
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
		shader.SetMat4("perp", PerspectiveMat);
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
	CreateModelMat(ObjTransformMat, Pos / (ScreenSpace::gridHeight), Rot, Scale / ScreenSpace::gridHeight);
	shader.SetMat4("transform", glm::value_ptr(ObjTransformMat));
	shader.SetVec3("MeshColor", Objects[Obj]->mesh.Color);
	glDrawElements(GL_TRIANGLES, Objects[Obj]->mesh.IndiceCount, GL_UNSIGNED_INT, (void*)(0));
	glBindVertexArray(0);
	return;
}

/*
void RenderShadowScene(Shader &shader, LightPoint *&Light)
{
	glClear(GL_DEPTH_BUFFER_BIT);

	shader.Use(Shading::ShaderPrograms["SShadowingDepthTextureShader"]);
	shader.SetMat4("perp", Light->CurrentPerpMat);

	Vector3 Pos = Light->Position;
	Vector3 Rot = Light->Rotation;
	Vector3 Scale;

	CreateCamMat(ViewMat, -Pos, -Rot);
	shader.SetMat4("view", glm::value_ptr(ViewMat));

	glBindVertexArray(StaticVAO.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, StaticVAO.VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, StaticVAO.EBO);
	for (int GameObj = 0; GameObj < STATICGOBJCOUNT; GameObj++)
	{
		int Start = StaticVAO.EBOStartPoints[GameObj];
		int End = StaticVAO.EBOStartPoints[GameObj + 1];
		int Count = 0;
		for (int Obj = 0; Obj < MAXOBJECTS; Obj++)
		{
			if (Count >= GameObjects.Counts[GameObj])
				break;
			if (GameObjects.Objects[Obj] == nullptr)
				continue;
			if (GameObjects.Objects[Obj]->mesh.GObj != GameObj)
				continue;
			if (!GameObjects.Objects[Obj]->mesh.Active)
			{
				Count++;
				continue;
			}

			if (GameObjects.Objects[Obj]->mesh.UI)
			{
				Count++;
				continue;
			}

			Pos = GameObjects.Objects[Obj]->Transform.Position();
			Rot = GameObjects.Objects[Obj]->Transform.Rotation();
			Scale = GameObjects.Objects[Obj]->Transform.Scale();

			CreateModelMat(ObjTransformMat, Pos / ScreenSpace::gridHeight, Rot, Scale / ScreenSpace::gridHeight);
			shader.SetMat4("transform", glm::value_ptr(ObjTransformMat));
			glDrawElements(GL_TRIANGLES, End - Start, GL_UNSIGNED_INT, (void*)(Start * sizeof(GLuint)));
			Count++;
		}
	}

	glBindVertexArray(DynamicVAO.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, DynamicVAO.VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, DynamicVAO.EBO);
	for (int GameObj = STATICGOBJCOUNT; GameObj < GAMEOBJECTCOUNT; GameObj++)
	{
		int Start = DynamicVAO.EBOStartPoints[GameObj - STATICGOBJCOUNT];
		int End = DynamicVAO.EBOStartPoints[GameObj - STATICGOBJCOUNT + 1];
		int Count = 0;
		for (int Obj = 0; Obj < MAXOBJECTS; Obj++)
		{
			if (Count >= GameObjects.Counts[GameObj])
				break;
			if (GameObjects.Objects[Obj] == nullptr)
				continue;
			if (GameObjects.Objects[Obj]->mesh.GObj != GameObj)
				continue;

			if (GameObjects.Objects[Obj]->mesh.UI)
			{
				Count++;
				continue;
			}

			int VBOSize = (DynamicVAO.VBOStartPoints[GameObj - STATICGOBJCOUNT + 1] - DynamicVAO.VBOStartPoints[GameObj - STATICGOBJCOUNT]) * sizeof(float);
			glBufferSubData(GL_ARRAY_BUFFER, DynamicVAO.VBOStartPoints[GameObj - STATICGOBJCOUNT], VBOSize, GameObjects.Objects[Obj]->mesh.Vertices);

			int EBOSize = (DynamicVAO.EBOStartPoints[GameObj - STATICGOBJCOUNT + 1] - DynamicVAO.EBOStartPoints[GameObj - STATICGOBJCOUNT]) * sizeof(int);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, DynamicVAO.EBOStartPoints[GameObj - STATICGOBJCOUNT], EBOSize, GameObjects.Objects[Obj]->mesh.Indices);

			Pos = GameObjects.Objects[Obj]->Transform.Position();
			Rot = GameObjects.Objects[Obj]->Transform.Rotation();
			Scale = GameObjects.Objects[Obj]->Transform.Scale();
			CreateModelMat(ObjTransformMat, Pos / (ScreenSpace::gridHeight), Rot, Scale / ScreenSpace::gridHeight);
			shader.SetMat4("transform", glm::value_ptr(ObjTransformMat));
			glDrawElements(GL_TRIANGLES, End - Start, GL_UNSIGNED_INT, (void*)(Start * sizeof(GLuint)));
			Count++;
		}
	}
	glBindVertexArray(0);
}
*/

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
			shader.SetMat4("perp", PerspectiveMat);
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
		CreateModelMat(ObjTransformMat, Pos / (ScreenSpace::gridHeight), Rot, Scale / ScreenSpace::gridHeight);
		shader.SetMat4("transform", glm::value_ptr(ObjTransformMat));
		shader.SetVec3("MeshColor", Objects[Obj]->mesh.Color);
		glDrawElements(GL_TRIANGLES, Objects[Obj]->mesh.IndiceCount, GL_UNSIGNED_INT, (void*)(0));
	}
}