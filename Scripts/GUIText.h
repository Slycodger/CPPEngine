#pragma once
#ifndef GUITEXT
#define GUITEXT
#include "EngineBase.h"

#ifdef GUITEXTMAIN
#include <iostream>
#include "Vector.h"
#include "GlStuff.h"
#include "IntializeShaders.h"
#include "KeyPresses.h"
#include "Constants.h"
#include <map>
#include "List.h"

struct Character
{
	Vector2 Bearing;
	float Advance;
	Vector2 Size;
	Vector2 BL;
	Vector2 TR;
	Vector2 Pos;
};
#endif

namespace GUIText
{
    void CreateText(std::string Text, float TextSize, Vector2 BoxSize, float LineJump, Vector3 Color, List<Object*>& ObjList, bool UIElement, Object*& TextBox);
    void DrawBitmap(Shader& shader);
    void Start(Shader& shader);
    void Update(Shader& shader);
    void End();
}


struct TextBox : ObjectBehavior
{
    Vector2 Position = Vector2(0, 0);
    Vector2 Size = Vector2(1, 0.6);
    float TextSize = 5;
    float LineJump = 0.3;
    Vector3 Color = Vector3(1,0,0);
    bool UIElement = true;

    void DestroyHeap() override
    {
        for (unsigned int i = 0; i < TextObjects.Length; i++)
        {
            delete(*TextObjects[i]);
            TextObjects.Delete();
        }
    }

    void Start() override
    {

    }

    void SetText(std::string text)
    {
        Text = text;
        GUIText::CreateText(Text, TextSize, Size, LineJump, Color, TextObjects, UIElement, thisObj);
        thisObj->Transform.SetPosition(Position);
    }

    std::string Text = "";

private:
    List<Object*> TextObjects = 0;
};

#endif