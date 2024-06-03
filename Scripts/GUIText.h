#pragma once
#ifndef GUITEXT
#define GUITEXT
#include "EngineBase.h"
#include "IntializeShaders.h"

void WriteText(unsigned int& Texture, float Resolution, Vector3 Color, float Scale, float JumpScale, bool UI, std::string Text, float EndPos, Vector2 Size);

#ifdef GUITEXTMAIN

#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <math.h>
#include <iostream>
#include "Vector.h"
#include "GlStuff.h"
#include "ModelLoading.h"
#include "KeyPresses.h"
#include "Constants.h"
#include "List.h"
#include "AObjectDraw.h"
#include "GlobalVars.h"

struct Character
{
    Vector2 Origin;
    Vector2 Bearing;
    Vector2 Size;
    float Advance;

    Character() : Origin(0, 0), Bearing(0, 0), Size(0, 0), Advance(0) {}
};
#endif

namespace GUIText
{
    void Start();
    void Update();
    void End();
}


struct TextBox : public ObjectBehavior
{
    unsigned int Texture = 0;
    std::string Text = "";
    float EndPos = 1;
    float Scale = 1;
    float JumpScale = 1;
    float Resolution = 5;
    Vector3 Color = { 1, 1, 1 };

    void Start() override
    {
        Vector2 objScale = thisObj->Transform.Scale();
        WriteText(Texture, Resolution, Color, Scale, JumpScale, thisObj->mesh.UI, Text, EndPos, objScale);
        thisObj->mesh.Texture = Texture;
        CacheScale = objScale;
        CacheText = Text;
    }
    void Update() override
    {
        Vector2 objScale = thisObj->Transform.Scale();
        Vector2 Ratio = objScale / CacheScale;
        if (!(Ratio.x >= Ratio.y - 0.01 && Ratio.x <= Ratio.y + 0.01) || CacheText != Text)
        {
            WriteText(Texture, Resolution, Color, Scale, JumpScale, thisObj->mesh.UI, Text, EndPos, objScale);
            thisObj->mesh.Texture = Texture;
            CacheScale = objScale;
            CacheText = Text;
        }
    }

    void DeleteHeap() override
    {

    }

private:
    Vector2 CacheScale = { 1, 1 };
    std::string CacheText = "";
};

#endif