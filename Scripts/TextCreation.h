#pragma once

#include "ScriptBase.h"
#include "IntializeShaders.h"

void writeText(unsigned int& Texture, float Resolution, Vector3 Color, float Scale, float JumpScale, bool UI, std::string Text, float EndPos, Vector2 Size);

#ifdef TextCreation_MAIN

#include <ft2build.h>
#include FT_FREETYPE_H
#include <math.h>
#include "ModelLoading.h"
#include "KeyPresses.h"
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

namespace TextCreation
{
    void start();
    void update();
    void end();
}


struct TextBox : public ObjectBehavior
{
    unsigned int Texture = 0;
    std::string Text = "";
    float EndPos = 1;
    float Scale = 1;
    float JumpScale = 1;
    float Resolution = 1;
    Vector3 Color = { 1, 1, 1 };

    void start() override
    {
        Vector2 objScale = { thisObj->Transform.Scale.x, thisObj->Transform.Scale.y };
        writeText(Texture, Resolution, Color, Scale, JumpScale, thisObj->mesh.UI, Text, EndPos, objScale);
        thisObj->mesh.Texture = Texture;
        CacheScale = objScale;
        CacheText = Text;
    }
    void update() override
    {
        Vector2 objScale = { thisObj->Transform.Scale.x, thisObj->Transform.Scale.y };
        Vector2 Ratio = objScale / CacheScale;
        if (!(Ratio.x >= Ratio.y - 0.01 && Ratio.x <= Ratio.y + 0.01) || CacheText != Text)
        {
            writeText(Texture, Resolution, Color, Scale, JumpScale, thisObj->mesh.UI, Text, EndPos, objScale);
            thisObj->mesh.Texture = Texture;
            CacheScale = objScale;
            CacheText = Text;
        }
    }

    void deleteHeap() override
    {

    }

private:
    Vector2 CacheScale = { 1, 1 };
    std::string CacheText = "";
};