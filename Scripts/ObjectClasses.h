#pragma once
#ifndef CLASSES
#define CLASSES
#include "ModelLoading.h"
#include <iostream>
#include "ApplicationMain.h"
#include "List.h"
#include "Vector.h"
#include "Constants.h"
#include "GlStuff.h"

struct Edge
{
	int Vert1 = -1;
	int Vert2 = -1;

	Edge(int Val) : Vert1(Val), Vert2(Val){}
	Edge(int x, int y) : Vert1(x), Vert2(y){}
	Edge() : Vert1(0), Vert2(0) {}

	bool operator == (Edge& Obj)
	{
		if ((Obj.Vert1 == Vert1 && Obj.Vert2 == Vert2) || (Obj.Vert1 == Vert2 && Obj.Vert2 == Vert1))
			return true;
		return false;
	}
	bool operator != (Edge& Obj)
	{
		if ((Obj.Vert1 == Vert1 && Obj.Vert2 == Vert2) || (Obj.Vert1 == Vert2 && Obj.Vert2 == Vert1))
			return false;
		return true;
	}
};

struct transform
{
	Vector3 Position = Vector3(0, 0, 0);
	Vector3 Scale = Vector3(1, 1, 1);
	Vector3 Rotation = Vector3(0, 0, 0);

	transform() : Position(0), Scale(1), Rotation(0) {}

	glm::mat4 make_glm()
	{
		glm::mat4 ret = glm::mat4(1);
		ret = glm::translate(ret, Position.make_glmVec3());
		ret = glm::rotate(ret, glm::radians<float>(Rotation.x), glm::vec3(1,0,0));
		ret = glm::rotate(ret, glm::radians<float>(Rotation.y), glm::vec3(0,1,0));
		ret = glm::rotate(ret, glm::radians<float>(Rotation.z), glm::vec3(0,0,1));
		ret = glm::scale(ret, Scale.make_glmVec3());
		return ret;
	}
};

struct scriptObj
{
	scriptObj() : Script(nullptr), Active(true) {}
	void(*Script) = nullptr;
	bool Active = true;
};

struct ObjP
{
	void Translate(Vector3 Vec)
	{
		if (this == nullptr)
			return;
		T.Position += Vec;
	}
	void Translate(float x, float y, float z)
	{
		T.Position.x += x;
		T.Position.y += y;
		T.Position.z += z;
	}

	void SetPosition(Vector3 Vec)
	{
		T.Position = Vec;
	}
	void SetPosition(Vector2 Vec)
	{
		T.Position = Vector3(Vec.x, Vec.y, 0);
	}
	void SetPosition(float x, float y, float z)
	{
		T.Position.x = x;
		T.Position.y = y;
		T.Position.z = z;
	}

	void Rotate(Vector3 Vec)
	{
		T.Rotation.x += Vec.x;
		T.Rotation.y += Vec.y;
		T.Rotation.z += Vec.z;
	}
	void Rotate(float x, float y, float z)
	{
		T.Rotation.x += x;
		T.Rotation.y += y;
		T.Rotation.z += z;
	}
	void Scale(Vector3 Vec)
	{
		T.Scale.x *= Vec.x;
		T.Scale.y *= Vec.y;
		T.Scale.z *= Vec.z;
	}
	void Scale(float x, float y, float z)
	{
		T.Scale.x *= x;
		T.Scale.y *= y;
		T.Scale.z *= z;
	}
	void Scale(float Scale)
	{
		T.Scale.x *= Scale;
		T.Scale.y *= Scale;
		T.Scale.z *= Scale;
	}
	void SetScale(float Scale)
	{
		T.Scale.x = Scale;
		T.Scale.y = Scale;
		T.Scale.z = Scale;
	}
	void SetScale(Vector2 Scale)
	{
		T.Scale.x = Scale.x;
		T.Scale.y = Scale.y;
		T.Scale.z = 1;
	}
	Vector3 Position()
	{
		return T.Position;
	}
	Vector3 Rotation()
	{
		return T.Rotation;
	}
	Vector3 Scale()
	{
		return T.Scale;
	}

	transform T;
};

struct Mesh
{
	Vector3 Color = Vector3(1);
	float* Vertices = nullptr;
	unsigned int* Indices = nullptr;
	unsigned int VBO = 0, EBO = 0, VAO = 0;

	unsigned int VertCount = 0;
	unsigned int IndiceCount = 0;

	int Texture = -1;
	bool UI = false;
	bool Active = true;

	int Index = -1;
};

struct Object
{
	ObjP Transform;
	List<scriptObj>Scripts = 0;

	std::string ObjName;
	Mesh mesh;
	List<Object*> Children = 0;
	Object* Parent = nullptr;
	int ChildIndex = -1;
	bool BranchHead = true;

	transform RelativeTransform;


	Object(Model &ActiveModel)
	{
		void SetToTransform(Object*);
		SetToTransform(this);
		mesh.Vertices = ActiveModel.GetVertices();
		mesh.Indices = ActiveModel.GetIndices();
		mesh.VertCount = ActiveModel.VertCount;
		mesh.IndiceCount = ActiveModel.IndiceCount;
		ObjName = ActiveModel.Name;

		glDeleteBuffers(1, &mesh.VBO);
		glDeleteBuffers(1, &mesh.EBO);
		glDeleteBuffers(1, &mesh.VAO);
		glGenVertexArrays(1, &mesh.VAO);
		glBindVertexArray(mesh.VAO);


		glGenBuffers(1, &mesh.VBO);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh.VertCount * 4, mesh.Vertices, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &mesh.EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.IndiceCount * 4, mesh.Indices, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * (sizeof(float)), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 9 * (sizeof(float)), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 9 * (sizeof(float)), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}
	Object(int) : ObjName("None") { void SetToTransform(Object*); SetToTransform(this); }
	Object() : ObjName("None") {}
	~Object()
	{
		Children.Delete();
	}

	void AddScript(void* Scr)
	{
		scriptObj tmp;
		tmp.Script = Scr;
		Scripts.AddToList(tmp);
	}
	
	void SetParent(Object*&Obj)
	{
		if (Obj == nullptr)
			return;
		if (Parent != nullptr)
			Parent->Children.RemoveFromList(ChildIndex);
		Obj->Children.AddToList(this);
		ChildIndex = Obj->Children.Length - 1;
		Parent = Obj;
		SetRelativePosition();
		BranchHead = false;
	}
	void SetHeadOfBranch()
	{
		if (BranchHead)
			return;
		if (Parent == nullptr)
			return;
		Parent->Children.RemoveFromList(ChildIndex);
		Parent = nullptr;
		ChildIndex = -1;
		BranchHead = true;
		SetRelativePosition();
	}
	void SetRelativePosition()
	{
		if (Parent == nullptr)
		{
			RelativeTransform = transform();
			return;
		}
		RelativeTransform.Position = Transform.Position() - Parent->Transform.Position();
		RelativeTransform.Scale = Transform.Scale() / Parent->Transform.Scale();
		RelativeTransform.Rotation = Transform.Rotation() - Parent->Transform.Rotation();
	}
	void SetToRelativePosition()
	{
		if (Parent == nullptr)
			return;
		glm::mat4 Translation(1);
		transform newTransform;
		RelativeTransform.Position.Scale(Parent->Transform.Scale() / Transform.Scale() * RelativeTransform.Scale);
		Translation = glm::rotate(Translation, glm::radians<float>(Parent->Transform.Rotation().x), glm::vec3(1, 0, 0));
		Translation = glm::rotate(Translation, glm::radians<float>(Parent->Transform.Rotation().y), glm::vec3(0, 1, 0));
		Translation = glm::rotate(Translation, glm::radians<float>(Parent->Transform.Rotation().z), glm::vec3(0, 0, 1));
		Translation = glm::translate(Translation, (RelativeTransform.Position - Parent->Transform.Position()).make_glmVec3());

		Transform.T.Position = Translation * Parent->Transform.Position().make_glmVec4();
		Transform.T.Position += Parent->Transform.Position();
		Transform.T.Rotation = Parent->Transform.Rotation();
		Transform.T.Scale = Parent->Transform.Scale() * RelativeTransform.Scale;
	}
	void UpdateChildren()
	{
		for(int i = 0; i < Children.Length; i++)
		{
			(*Children[i])->SetToRelativePosition();
			(*Children[i])->UpdateChildren();
		}
	}
};
void DeleteObj(Object*&);

#endif