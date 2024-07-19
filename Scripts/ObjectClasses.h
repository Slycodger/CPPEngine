#pragma once

#include "ModelLoading.h"
#include "List.h"
#include "Vector.h"
#include "MathFunctions.h"
#include "Constants.h"
#include "GlStuff.h"

struct transform
{
	Vector3 Position = Vector3(0, 0, 0);
	Vector3 Scale = Vector3(1, 1, 1);
	Vector3 Rotation = Vector3(0, 0, 0);

	transform() : Position(0), Scale(1), Rotation(0) {}

	glm::mat4 make_glm()
	{
		glm::mat4 ret = glm::mat4(1);
		ret = glm::translate(ret, Position.makeGlmVec3());
		ret = glm::rotate(ret, glm::radians<float>(Rotation.x), glm::vec3(1,0,0));
		ret = glm::rotate(ret, glm::radians<float>(Rotation.y), glm::vec3(0,1,0));
		ret = glm::rotate(ret, glm::radians<float>(Rotation.z), glm::vec3(0,0,1));
		ret = glm::scale(ret, Scale.makeGlmVec3());
		return ret;
	}

	void translate(Vector3 Vec)
	{
		if (this == nullptr)
			return;
		Position += Vec;
	}
	void translate(float x, float y, float z)
	{
		Position.x += x;
		Position.y += y;
		Position.z += z;
	}
	void translate(float x, float y)
	{
		Position.x += x;
		Position.y += y;
	}

	void setPosition(Vector3 Vec)
	{
		Position = Vec;
	}

	void setPosition(Vector2 Vec)
	{
		Position = Vector3(Vec.x, Vec.y, 0);
	}

	void setPosition(float x, float y, float z)
	{
		Position.x = x;
		Position.y = y;
		Position.z = z;
	}

	void setPosition(float x, float y)
	{
		Position.x = x;
		Position.y = y;
		Position.z = 0;
	}

	void rotate(Vector3 Vec)
	{
		Rotation.x += Vec.x;
		Rotation.y += Vec.y;
		Rotation.z += Vec.z;
	}

	void rotate(float x, float y, float z)
	{
		Rotation.x += x;
		Rotation.y += y;
		Rotation.z += z;
	}

	void changeScale(Vector3 Vec)
	{
		Scale.x *= Vec.x;
		Scale.y *= Vec.y;
		Scale.z *= Vec.z;
	}

	void changeScale(float x, float y, float z)
	{
		Scale.x *= x;
		Scale.y *= y;
		Scale.z *= z;
	}

	void changeScale(float Val)
	{
		Scale.x *= Val;
		Scale.y *= Val;
		Scale.z *= Val;
	}

	void setScale(float Val)
	{
		Scale.x = Val;
		Scale.y = Val;
		Scale.z = Val;
	}

	void setScale(Vector2 Val)
	{
		Scale.x = Val.x;
		Scale.y = Val.y;
		Scale.z = 1;
	}

	void setScale(Vector3 Val)
	{
		Scale.x = Val.x;
		Scale.y = Val.y;
		Scale.z = Val.z;
	}

	void setScale(float x, float y, float z)
	{
		Scale.x = x;
		Scale.y = y;
		Scale.z = z;
	}

	void setScale(float x, float y)
	{
		Scale.x = x;
		Scale.y = y;
		Scale.z = 1;
	}

	Vector3 getPosition()
	{
		return Position;
	}

	Vector3 getRotation()
	{
		return Rotation;
	}

	Vector3 getScale()
	{
		return Scale;
	}
};

struct scriptObj
{
	scriptObj() : Script(nullptr), Active(true) {}
	void(*Script) = nullptr;
	bool Active = true;
};

struct Mesh
{
	Vector4 Color = Vector4(1);
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
	transform Transform;
	List<scriptObj>Scripts = 0;

	std::string ObjName;
	Mesh mesh;
	List<Object*> Children = 0;
	List<Object*> Dependents = 0;
	Object* Parent = nullptr;
	Object* Dependency = nullptr;
	int ChildIndex = -1;
	int DependentIndex = -1;
	bool BranchHead = true;

	transform RelativeTransform;

	Object(Model &ActiveModel)
	{
		void addObjToList(Object*);
		addObjToList(this);
		mesh.Vertices = ActiveModel.getVertices();
		mesh.Indices = ActiveModel.getIndices();
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

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	Object(int) : ObjName("None") { void SetToTransform(Object*); SetToTransform(this); }
	Object() : ObjName("None") {}
	~Object()
	{
		Children.deleteList();
		Dependents.deleteList();
	}

	//Adds a script to the object
	void addScript(void* Scr)
	{
		scriptObj tmp;
		tmp.Script = Scr;
		Scripts.addToList(tmp);
	}
	
	//Sets the object's parent
	void setParent(Object*&Obj)
	{
		if (Obj == nullptr)
			return;
		if (Parent != nullptr)
			Parent->Children.removeFromList(ChildIndex);
		Obj->Children.addToList(this);
		ChildIndex = Obj->Children.Length - 1;
		Parent = Obj;
		setRelativeTransform();
		BranchHead = false;
	}

	//Removes an object's parent if it has one
	void removeParent()
	{
		if (BranchHead)
			return;
		if (Parent == nullptr)
			return;
		Parent->Children.removeFromList(ChildIndex);
		Parent = nullptr;
		ChildIndex = -1;
		BranchHead = true;
		setRelativeTransform();
	}

	//Sets an objects relative transform to parent
	void setRelativeTransform()
	{
		if (Parent == nullptr)
		{
			RelativeTransform = transform();
			return;
		}
		RelativeTransform.Position = Transform.Position - Parent->Transform.Position;
		RelativeTransform.Scale = Transform.Scale / Parent->Transform.Scale;
		RelativeTransform.Rotation = Transform.Rotation - Parent->Transform.Rotation;
	}

	//Sets an object to be at the relative transform to the parent
	void setToRelativeTransform()
	{
		if (Parent == nullptr)
			return;
		glm::mat4 Translation(1);
		transform newTransform;
		Translation = glm::rotate(Translation, glm::radians<float>(Parent->Transform.Rotation.x), glm::vec3(1, 0, 0));
		Translation = glm::rotate(Translation, glm::radians<float>(Parent->Transform.Rotation.y), glm::vec3(0, 1, 0));
		Translation = glm::rotate(Translation, glm::radians<float>(Parent->Transform.Rotation.z), glm::vec3(0, 0, 1));
		Translation = glm::translate(Translation, (RelativeTransform.Position * RelativeTransform.Scale).makeGlmVec3());

		Transform.Position = Translation * Parent->Transform.Position.makeGlmVec4();
		Transform.Rotation = Parent->Transform.Rotation;
		Transform.Scale = Parent->Transform.Scale * RelativeTransform.Scale;
	}

	//Updates all children to go to their relative transforms
	void updateChildren()
	{
		for(unsigned int i = 0; i < Children.Length; i++)
		{
			(*Children[i])->setToRelativeTransform();
			(*Children[i])->updateChildren();
		}
	}

	//Adds a dependent to the object
	void addDependent(Object*& Obj)
	{
		Obj->DependentIndex = Dependents.Length;
		Obj->Dependency = this;
		Dependents.addToList(Obj);
	}

	//Sets the dependency for the object
	void setDependency(Object*& Obj)
	{
		DependentIndex = Obj->Dependents.Length;
		Dependency = Obj;
		Obj->Dependents.addToList(this);
	}

	//Seperates an object from its dependency if it has one
	void removeDependency()
	{
		if (DependentIndex == -1 || Dependency == nullptr)
			return;
		Dependency->Dependents.removeFromList(DependentIndex);
		DependentIndex = -1;
		Dependency = nullptr;
	}
};
void DeleteObj(Object*&);