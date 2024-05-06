/*
* The applications main method of drawing objects
*/

#define AOBJECTDRAWMAIN
#include "AObjectDraw.h"

using namespace AObjectDraw;

Camera MainCamera;

static Object* Objects[MAXOBJECTS];
static unsigned int ObjCount = 0;
static bool Cstr = false;

static List<LightObj> Lights = 3;

static unsigned int ScreenFBO, ScreenDepthStencil, ScreenTexture, ShadowTexture, BackgroundTexture;

static Object* TextureObj = nullptr;
static Model SquareModel;

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
		delete[](Objects[Obj]->mesh.Vertices);
		delete[](Objects[Obj]->mesh.Indices);
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
			
		LoadModel(SquareModel, "Square", "MYOBJ");


		glBindVertexArray(0);

		glGenFramebuffers(1, &ScreenFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, ScreenFBO);
		glGenTextures(1, &ScreenDepthStencil);
		glGenTextures(1, &ScreenTexture);
		glGenTextures(1, &ShadowTexture);
		glGenTextures(1, &BackgroundTexture);

		glBindTexture(GL_TEXTURE_2D, ScreenDepthStencil);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, ScreenSpace::Width, ScreenSpace::Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, ScreenTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, ScreenSpace::Width, ScreenSpace::Height, 0, GL_RGBA, GL_UNSIGNED_INT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, ShadowTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, ScreenSpace::Width, ScreenSpace::Height, 0, GL_RGBA, GL_UNSIGNED_INT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, BackgroundTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, ScreenSpace::Width, ScreenSpace::Height, 0, GL_RGBA, GL_UNSIGNED_INT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, BackgroundTexture, 0);
		glClearColor(0.8, 0.2, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		TextureObj = new Object(SquareModel);
		TextureObj->mesh.UI = true;
		TextureObj->mesh.Active = false;

		Lights.AddToList(LightObj());
		Lights[Lights.Length - 1]->Position = Vector3(5, 0, 0);
		Lights[Lights.Length - 1]->Color = Vector4(1, 0, 0, 1);
		Lights[Lights.Length - 1]->GenTexture();
		Lights.AddToList(LightObj());
		Lights[Lights.Length - 1]->Position = Vector3(-5, 0, 0);
		Lights[Lights.Length - 1]->Color = Vector4(0, 1, 0, 1);
		Lights[Lights.Length - 1]->GenTexture();
		Lights.AddToList(LightObj());
		Lights[Lights.Length - 1]->Position = Vector3(0, 0, -5);
		Lights[Lights.Length - 1]->Color = Vector4(0, 0, 1, 1);
		Lights[Lights.Length - 1]->GenTexture();
	}

	float LightSpeed = 5;

	void Update(Shader& shader)
	{
		if (Keys::KeyHeld(GLFW_KEY_LEFT_CONTROL))
			Lights[0]->Position = MainCamera.Position;
		if (Keys::KeyHeld(GLFW_KEY_1))
			MainCamera.Position = Lights[0]->Position;
		if (Keys::KeyHeld(GLFW_KEY_2))
			MainCamera.Position = Lights[1]->Position;
		if (Keys::KeyHeld(GLFW_KEY_3))
			MainCamera.Position = Lights[2]->Position;

		RenderAllWithShadows(shader, Vector4(0.75, 0.75, 0.75, 1));
	}

	void End()
	{
		DeleteAll();

		delete[](PerspectiveMat);
		delete[](OrthographicMat);	

		Lights.Delete();
		SquareModel.DeleteModel();
	}
}

void RenderAllWithShadows(Shader& shader, Vector4 ShadowColor)
{
	//Bind framebuffer and textures
	glBindFramebuffer(GL_FRAMEBUFFER, ScreenFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ScreenTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, ScreenDepthStencil, 0);

	//Enable/disable anything that could affect drawing
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);

	//Set clear modes and clear main texture
	glClearColor(0, 0, 0, 0);
	glClearStencil(0x00);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);

	//Render and fill depth buffer
	RenderScene(shader, "BasicNoTextureShader", "BasicTextureShader");

	//Set depth modes
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);

	//Render what things look like with shadows
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ShadowTexture, 0);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	RenderShadows(shader, ShadowColor);

	//Draw all light textures
	glEnable(GL_STENCIL_TEST);
	for (int i = 0; i < Lights.Length; i++)
	{
		//Set light teture active
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Lights[i]->Texture, 0);
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		//Set depth and color stuff
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		//Enable and set stencil test mode
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		glClear(GL_STENCIL_BUFFER_BIT);

		//Render light volume to fill stencil buffer
		RenderShadowVolumes(shader, Lights[i]->Position);

		//Allow drawing to screen and adjust stencil testing
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc(GL_EQUAL, 0, 0xFF);

		//Render what will be lit up by light
		RenderShadows(shader, Lights[i]->Color);
	}

	//So that things can draw fine
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);

	//Combine and draw lights to a single texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Lights[0]->Texture, 0);
	for (int i = 1; i < Lights.Length; i++)
	{
		//Add light colors
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
		TextureObj->mesh.Texture = Lights[i]->Texture;
		RenderSingleObj(shader, TextureObj, "BasicNoTextureShader", "BasicTextureShader");
	}

	//Prepare to draw screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_BLEND);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	//Render the base screen
	TextureObj->mesh.Texture = ScreenTexture;
	RenderSingleObj(shader, TextureObj, "BasicNoTextureShader", "BasicTextureShader");

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFuncSeparate(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
	TextureObj->mesh.Texture = Lights[0]->Texture;
	RenderSingleObj(shader, TextureObj, "BasicNoTextureShader", "BasicTextureShader");

	//Remove shadow colors
	glBlendEquationSeparate(GL_FUNC_REVERSE_SUBTRACT, GL_FUNC_ADD);
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ONE);
	TextureObj->mesh.Texture = ShadowTexture;
	RenderSingleObj(shader, TextureObj, "BasicNoTextureShader", "BasicTextureShader");

	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ONE);
	TextureObj->mesh.Texture = BackgroundTexture;
	RenderSingleObj(shader, TextureObj, "BasicNoTextureShader", "BasicTextureShader");
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

void CreateModelMat(glm::mat4& PosMat, glm::mat4& RotMat, glm::mat4& ScaleMat, transform &Transform)
{
	PosMat = glm::mat4(1);
	RotMat = glm::mat4(1);
	ScaleMat = glm::mat4(1);
	PosMat = glm::translate(PosMat, glm::vec3(Transform.Position.x, Transform.Position.y, Transform.Position.z));
	RotMat = glm::rotate(RotMat, glm::radians<float>(Transform.Rotation.x), glm::vec3(1, 0, 0));
	RotMat = glm::rotate(RotMat, glm::radians<float>(Transform.Rotation.y), glm::vec3(0, 1, 0));
	RotMat = glm::rotate(RotMat, glm::radians<float>(Transform.Rotation.z), glm::vec3(0, 0, 1));
	ScaleMat = glm::scale(ScaleMat, glm::vec3(Transform.Scale.x, Transform.Scale.y, Transform.Scale.z));
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

void RenderCustomObj(Shader &shader, unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int EBOCount, Vector3 MeshColor, transform& Transform)
{
	shader.SetMat4("perp", PerspectiveMat);

	glm::mat4 ObjPos;
	glm::mat4 ObjRot;
	glm::mat4 ObjScale;

	Vector3 Pos = MainCamera.Position;
	Vector3 Rot = MainCamera.RotToVec();
	Vector3 Scale;
	CreateCamMat(ViewMat, -Pos, -Rot);
	shader.SetMat4("view", glm::value_ptr(ViewMat));

	Pos = Transform.Position;
	Rot = Transform.Rotation;
	Scale = Transform.Scale;
	CreateModelMat(ObjPos, ObjRot, ObjScale, Transform);
	ObjPos = ObjPos * ObjRot * ObjScale;
	shader.SetMat4("transform", glm::value_ptr(ObjPos));

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
	glm::mat4 ObjPos;
	glm::mat4 ObjRot;
	glm::mat4 ObjScale;

	Vector3 Pos = MainCamera.Position;
	Vector3 Rot = MainCamera.RotToVec();
	Vector3 Scale;
	CreateCamMat(ViewMat, -Pos, -Rot);

	int Obj = ObjToDraw->mesh.Index;

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
	CreateModelMat(ObjPos, ObjRot, ObjScale, Objects[Obj]->Transform.T);
	ObjPos = ObjPos * ObjRot * ObjScale;
	shader.SetMat4("transform", glm::value_ptr(ObjPos));
	shader.SetVec3("MeshColor", Objects[Obj]->mesh.Color);
	glDrawArrays(GL_TRIANGLES, 0, Objects[Obj]->mesh.IndiceCount);
	return;
}

void RenderShadowVolumes(Shader &shader, Vector3 LightPos)
{
	shader.Use(Shading::ShaderPrograms["StretchedShader"]);

	glm::mat4 ObjPos;
	glm::mat4 ObjRot;
	glm::mat4 ObjScale;

	Vector3 Pos = MainCamera.Position;
	Vector3 Rot = MainCamera.RotToVec();
	Vector3 Scale;
	CreateCamMat(ViewMat, -Pos, -Rot);
	shader.SetMat4("perp", PerspectiveMat);
	shader.SetMat4("view", glm::value_ptr(ViewMat));
	shader.SetVec3("LightPos", LightPos);
	shader.SetVec3("CameraPos", MainCamera.Position);
	shader.SetFloat("LED", LIGHTEXTENDDISTANCE);
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
		if (Count > ObjCount)
			break;
		Count++;

		glBindVertexArray(Objects[Obj]->mesh.VAO);

		Pos = Objects[Obj]->Transform.Position();
		Rot = Objects[Obj]->Transform.Rotation();
		Scale = Objects[Obj]->Transform.Scale();
		CreateModelMat(ObjPos, ObjRot, ObjScale, Objects[Obj]->Transform.T);
		shader.SetMat4("PosMat", glm::value_ptr(ObjPos));
		shader.SetMat4("RotMat", glm::value_ptr(ObjRot));
		shader.SetMat4("ScaleMat", glm::value_ptr(ObjScale));
		glDrawArrays(GL_TRIANGLES, 0, Objects[Obj]->mesh.IndiceCount);
	}
}

void RenderShadows(Shader& shader, Vector4 LightColor)
{
	shader.Use(Shading::ShaderPrograms["SSDarkenShader"]);

	glm::mat4 ObjPos;
	glm::mat4 ObjRot;
	glm::mat4 ObjScale;

	Vector3 Pos = MainCamera.Position;
	Vector3 Rot = MainCamera.RotToVec();
	Vector3 Scale;
	CreateCamMat(ViewMat, -Pos, -Rot);
	shader.SetMat4("perp", PerspectiveMat);
	shader.SetMat4("view", glm::value_ptr(ViewMat));
	shader.SetVec4("LightColor", LightColor);

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
		if (Count > ObjCount)
			break;
		Count++;

		glBindVertexArray(Objects[Obj]->mesh.VAO);

		Pos = Objects[Obj]->Transform.Position();
		Rot = Objects[Obj]->Transform.Rotation();
		Scale = Objects[Obj]->Transform.Scale();
		CreateModelMat(ObjPos, ObjRot, ObjScale, Objects[Obj]->Transform.T);
		CreateModelMat(ObjPos, ObjRot, ObjScale, Objects[Obj]->Transform.T);
		ObjPos = ObjPos * ObjRot * ObjScale;
		shader.SetMat4("transform", glm::value_ptr(ObjPos));
		glDrawArrays(GL_TRIANGLES, 0, Objects[Obj]->mesh.IndiceCount);
	}
}

void RenderScene(Shader& shader, std::string NoTextureShader, std::string TextureShader)
{
	glm::mat4 ObjPos;
	glm::mat4 ObjRot;
	glm::mat4 ObjScale;

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
		if (Count > ObjCount)
			break;
		Count++;

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
		CreateModelMat(ObjPos, ObjRot, ObjScale, Objects[Obj]->Transform.T);
		ObjPos = ObjPos * ObjRot * ObjScale;
		shader.SetMat4("transform", glm::value_ptr(ObjPos));
		shader.SetVec3("MeshColor", Objects[Obj]->mesh.Color);
		glDrawArrays(GL_TRIANGLES, 0, Objects[Obj]->mesh.IndiceCount);
	}
}