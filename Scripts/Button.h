#pragma once
#ifndef BUTTON
#define BUTTON
#include "EngineBase.h"
#include "GUIText.h"

struct Button : ObjectBehavior
{
	Object* TextObj = new Object(0);
	TextBox* Text = nullptr;

	void DestroyHeap() override
	{

	}
	void Start() override
	{
		Text = new TextBox;
		TextObj->AddScript((void*)Text);
		Text->SetText("Button");
	}
	void Update() override
	{

	}
};

#endif