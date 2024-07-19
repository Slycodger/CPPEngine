#pragma once

#include "ObjectClasses.h"
struct ObjectBehavior
{
	virtual void deleteHeap() { }
	virtual void start() { }
	virtual void update(){}
	Object* thisObj = nullptr;
	void setObject(Object* obj)
	{
		this->thisObj = obj;
	}
};