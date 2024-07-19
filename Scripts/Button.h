#pragma once

#include "TextCreation.h"
#include "Mouse.h"
#include "KeyPresses.h"

struct ButtonScr : public ObjectBehavior
{
	void(*PressFunc)(void*) = nullptr;
	void(*HoldFunc)(void*) = nullptr;
	void(*ReleaseFunc)(void*) = nullptr;
	void* PressFuncParam = nullptr;
	void* HoldFuncParam = nullptr;
	void* ReleaseFuncParam = nullptr;
	std::string Text = "Button";
	Object* textObj = nullptr;
	TextBox* textScr = new TextBox;
	Model SquareModel;
	bool CreateText = true;

	void start() override
	{
		if (!CreateText)
			return;
		loadModel(SquareModel, "Square", "MYOBJ");
		textScr->Text = Text;
		textScr->Color = { 0, 0, 0 };
		textObj = new Object(SquareModel);
		textObj->addScript((void*)textScr);
		textObj->mesh.UI = true;
		textObj->setParent(thisObj);
		textObj->RelativeTransform.Position = { 0, 0, -0.05f };
		textObj->RelativeTransform.Scale = { 1, 1, 1 };
	}

	Vector2 BL = { -1, -1 };
	Vector2 TR = { 1, 1 };

	bool Pressed = false;

	void update() override
	{
		Vector2 Pos = { thisObj->Transform.Position.x, thisObj->Transform.Position.y };
		Vector2 Size = { thisObj->Transform.Scale.x, thisObj->Transform.Scale.y };

		if (CreateText)
			textScr->Text = Text;

		if (ActiveMouse.Pos >= BL * Size + Pos && ActiveMouse.Pos <= TR * Size + Pos)
		{
			if (PressFunc != nullptr)
				if (Keys::keyPressed(GLFW_MOUSE_BUTTON_1))
				{
					PressFunc(PressFuncParam);
					Pressed = true;
				}
			if (Keys::keyHeld(GLFW_MOUSE_BUTTON_1))
				Pressed = true;
		}

		if (Pressed)
			if (Keys::keyReleased(GLFW_MOUSE_BUTTON_1))
			{
				if (ReleaseFunc != nullptr)
					ReleaseFunc(ReleaseFuncParam);
				Pressed = false;
			}
			else
				if (HoldFunc != nullptr)
					HoldFunc(HoldFuncParam);

	}

	void deleteHeap() override
	{
		delete(PressFuncParam);
		delete(ReleaseFuncParam);
		delete(HoldFuncParam);
	}
};