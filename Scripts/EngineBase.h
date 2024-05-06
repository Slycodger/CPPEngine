#pragma once
#ifndef ENGINEBASE
#define ENGINEBASE
#include "ObjectClasses.h"
struct ObjectBehavior
{
	virtual void DestroyHeap() { }
	virtual void Start() { }
	virtual void Update(){}
	Object* thisObj = nullptr;
	void SetObject(Object* obj)
	{
		this->thisObj = obj;
	}
};

#endif