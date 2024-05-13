#include <ft2build.h>
#include FT_FREETYPE_H
#define GUITEXTMAIN
#include "GUIText.h"

const char* CharObj = "Square";

using namespace GUIText;

Model Char;

namespace GUIText
{
    float RectVerts[] =
    {
        1, -1, 0,   1, 0,   0, 0, 0 , 0,
        -1, 1, 0,   0, 1,   0, 0, 0 , 0,
        -1, -1, 0,  0, 0,   0, 0, 0 , 0,

         1, 1, 0,   1, 1,   0, 0, 0 , 0,
        -1, 1, 0,   0, 1,   0, 0, 0 , 0,
        1, -1, 0,   1, 0,   0, 0, 0 , 0,
    };
    int RectIndi[] =
    {
        0, 1, 2,
        3, 4, 5
    };

	std::map<char, Character> Characters;
    unsigned int VAO, RVBO, REBO;
    unsigned int Bitmap;
    unsigned int FBO;

    glm::mat4 ViewportScale = glm::mat4(1);

    void CreateBitmap(float Definition, float ScreenScale, int TexRes, Shader &shader, float TextScale, float BitXOff, float BitYOff)
    {
        ViewportScale = glm::scale(ViewportScale, glm::vec3(1 / ScreenSpace::xGridScale, 1 / ScreenSpace::yGridScale, 1));


        FT_Library FontLibrary;
        if (FT_Init_FreeType(&FontLibrary))
        {
            Fails::Break = true;
            Fails::BreakCode = -4;
        }
        FT_Face FontFace;
        if (FT_New_Face(FontLibrary, "./Fonts/arial.ttf", 0, &FontFace))
        {
            Fails::Break = true;
            Fails::BreakCode = -5;
        }

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &RVBO);
        glGenBuffers(1, &REBO);

        glBindBuffer(GL_ARRAY_BUFFER, RVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(RectVerts), RectVerts, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, REBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(RectIndi), RectIndi, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * (sizeof(float)), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 9 * (sizeof(float)), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 9 * (sizeof(float)), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);

        FT_Set_Pixel_Sizes(FontFace, 0, TexRes * Definition);

        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glViewport(0, 0, ScreenSpace::Width * ScreenScale, ScreenSpace::Height * ScreenScale);

        glGenTextures(1, &Bitmap);
        glBindTexture(GL_TEXTURE_2D, Bitmap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ScreenSpace::Width * ScreenScale, ScreenSpace::Height * ScreenScale, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Bitmap, 0);

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.SetInt("Texture", 0);
        shader.SetMat4("perp", (float*)IDENTITYMAT);
        shader.SetMat4("view", (float*)IDENTITYMAT);
        shader.SetVec3("MeshColor", 1, 1, 1);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, RVBO);
        glm::mat4 transf = glm::mat4(1);
        transf = glm::translate(transf, glm::vec3(-1, -1, 0));
        transf = glm::scale(transf, glm::vec3(2 / (ScreenSpace::Width * Definition), 2 / (ScreenSpace::Height * Definition), 1));

        float x = 0;
        float y = ScreenSpace::Height * (Definition / TextScale) - (Definition * TexRes);
        for (unsigned char c = 32; c < 123; c++)
        {
            // load character glyph 
            if (FT_Load_Char(FontFace, c, FT_LOAD_RENDER))
            {
                Fails::Break = true;
                Fails::BreakCode = -6;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                FontFace->glyph->bitmap.width,
                FontFace->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                FontFace->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                Vector2(FontFace->glyph->bitmap_left, FontFace->glyph->bitmap_top),
                FontFace->glyph->advance.x,
                Vector2(FontFace->glyph->bitmap.width, FontFace->glyph->bitmap.rows),
            };
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);

            float w = character.Size.x * TextScale;
            float h = character.Size.y * TextScale;

            if ((character.Advance / 64) + x > ScreenSpace::Width * (Definition / TextScale))
            {
                x = 0;
                y -= Definition * BitYOff;
            }

            float xpos = (x + character.Bearing.x) * TextScale;
            float ypos = (y - (character.Size.y - character.Bearing.y)) * TextScale;


            if ((int)c == 96)
                ypos -= 10 * TextScale;

            float Verts[] =
            {
                xpos, (ypos + h), 0,      0, 0,     0, 0, 0, 1,
                xpos, ypos, 0,            0, 1,     0, 0, 0, 1,
                (xpos + w), ypos, 0,      1, 1,     0, 0, 0, 1,

                xpos, (ypos + h), 0,        0, 0,   0, 0, 0, 1,
                (xpos + w), ypos, 0,        1, 1,   0, 0, 0, 1,
                (xpos + w), (ypos + h), 0,  1, 0,    0, 0, 0, 1,
            };
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Verts), Verts);

            shader.SetMat4("transform", glm::value_ptr(transf));
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindTexture(GL_TEXTURE_2D, 0);

            glDeleteTextures(1, &texture);

            x += (character.Advance / 64);
            x += BitXOff;

            character.TR.x = (xpos + w) / (ScreenSpace::Width * Definition);
            character.TR.y = (ypos + h) / (ScreenSpace::Height * Definition);

            character.BL.x = xpos / (ScreenSpace::Width * Definition);
            character.BL.y = ypos / (ScreenSpace::Height * Definition);

            character.Pos.x = ((character.TR.x * 2 - 1) - (character.BL.x * 2 - 1)) / TextScale;
            character.Pos.y = ((character.TR.y * 2 - 1) - (character.BL.y * 2 - 1)) / TextScale;
            character.Advance /= 32 * ScreenSpace::Width * Definition;
            character.Bearing.x *= 2 / (ScreenSpace::Width * Definition);
            character.Bearing.y *= 2 / (ScreenSpace::Height * Definition);
            Characters.insert(std::pair<char, Character>(c, character));
        }
        FT_Done_Face(FontFace);
        FT_Done_FreeType(FontLibrary);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glBindVertexArray(0);

        //glDeleteBuffers(1, &RVBO);
        //glDeleteBuffers(1, &REBO);
        //glDeleteVertexArrays(1, &VAO);

        glDeleteFramebuffers(1, &FBO);

        glViewport(0, 0, ScreenSpace::Width, ScreenSpace::Height);
    }

	void Start(Shader &shader)
	{
        LoadModel(Char, CharObj, "MYOBJ");
        CreateBitmap(8, 4, 16, shader, 6.7, 16, 16);
	}
    std::string OutputText;

    void Update(Shader& shader)
    {
        //DrawBitmap(shader);
    }

    void CreateText(std::string Text, float TextSize, Vector2 BoxSize, float LineJump, Vector3 Color, List<Object*> &ObjList, bool UIElement, Object*& TextBox)
    {
        Vector3 ObjPos;
        float x = 0;
        float y = BoxSize.y;

        std::string::const_iterator c;
        
        for(int i = 0; i < ObjList.Length; i++)
        {
            DeleteObj(*ObjList[i]);
        }
        ObjList.Delete();

        for (c = Text.begin(); c != Text.end(); c++)
        {
            Character ch = Characters[*c];
            if (*c == ' ')
            {
                ObjPos.x += TextSize * ch.Advance;
                continue;
            }
            if(*c == '\n')
            {
                ObjPos.x = x;
                y -= LineJump;
                continue;
            }

            ObjList.AddToList(new Object*(new Object(Char)));
            (*ObjList[ObjList.Length - 1])->mesh.Texture = Bitmap;
            if (UIElement)
                (*ObjList[ObjList.Length - 1])->mesh.UI = true;

            if (ObjPos.x + (ch.Pos.x * TextSize) + (ch.Advance * TextSize) >= BoxSize.x)
            {
                ObjPos.x = x;
                y -= LineJump;
            }
            ObjPos.y = -(ch.Pos.y - ch.Bearing.y) * TextSize + y;

            float Verts[] =
            {
                //Pos                    //UV                    //Color
                ch.Pos.x + ch.Bearing.x, ch.Pos.y, 0,   ch.TR.x, ch.TR.y,         Color.x, Color.y, Color.z, 1,
                ch.Pos.x + ch.Bearing.x, 0, 0,          ch.TR.x, ch.BL.y,         Color.x, Color.y, Color.z, 1,
                ch.Bearing.x, ch.Pos.y, 0,          ch.BL.x, ch.TR.y,         Color.x, Color.y, Color.z, 1,
                ch.Bearing.x, 0, 0,                 ch.BL.x,  ch.BL.y,        Color.x, Color.y, Color.z, 1
            };
            for(int i = 0; i < 4 * VERTLENGTH; i++)
            {
                (*ObjList[ObjList.Length - 1])->mesh.Vertices[i] = Verts[i];
            }
            (*ObjList[ObjList.Length - 1])->Transform.SetPosition(ObjPos);
            (*ObjList[ObjList.Length - 1])->Transform.SetScale(TextSize);
            (*ObjList[ObjList.Length - 1])->SetParent(TextBox);
            ObjPos.x += TextSize * ch.Advance;
        }
    }

    //Debugging thing
    void DrawBitmap(Shader &shader)
    {
        shader.SetMat4("perp", (float*)glm::value_ptr(glm::mat4(1)));
        shader.SetMat4("view", (float*)IDENTITYMAT);
        shader.SetMat4("transform", (float*)IDENTITYMAT);
        shader.SetInt("Texture", 0);
        shader.SetVec3("MeshColor", 1, 0, 0);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, RVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, REBO);
        float Verts[] =
        {
            1, -1, 0,   1, 0,   0, 0, 0, 1,
            -1, 1, 0,   0, 1,   0, 0, 0, 1,
            -1, -1, 0,  0, 0,   0, 0, 0, 1,

             1, 1, 0,   1, 1,   0, 0, 0, 1,
            -1, 1, 0,   0, 1,   0, 0, 0, 1,
            1, -1, 0,   1, 0,   0, 0, 0, 1,
        };

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Verts), Verts);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Bitmap);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}
