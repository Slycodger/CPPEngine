#define GUITEXTMAIN
#include "GUIText.h"

using namespace GUIText;

constexpr float BitmapWidth = 1920;
constexpr float BitmapHeight = 1080;
constexpr float BitmapRatio = BitmapWidth / BitmapHeight;
constexpr unsigned int DefaultSize = 64;
constexpr unsigned int CharStart = 32;
constexpr unsigned int CharEnd = 126;

constexpr float Resolution = 10;


unsigned int BitmapTexture = 0, FBO, Texture;

unsigned int VAO, VBO;

unsigned int CharTexture = 0;

float Height = 0, Under = 0;

std::map<char, Character> CharacterMap;

float Verts[] =
{
	-1, -1, 0, 0,
	-1, 1, 0, 1,
	1, -1, 1, 0,

	-1, 1, 0, 1,
	1, 1, 1, 1,
	1, -1, 1, 0
};

void Resize(float Height, FT_Face& Face)
{
	float Layers = 0;
	Vector2 Pos = Vector2(-1, 1 - Height);

	for (unsigned char c = CharStart; c <= CharEnd; c++)
	{
		Character Char = CharacterMap[c];

		if (Pos.x + Char.Size.x > 1)
		{
			Pos.x = -1;
			Pos.y -= Height;
			Layers++;
		}

		Pos.x += Char.Size.x;
	}

	float ToLine = 1 / (((Pos.x + 1) / 2) + Layers);
	int MaxLayers = 2 / (Height * ToLine);
	float ResizeVal = sqrt(MaxLayers - 1) * ToLine;

	FT_Set_Pixel_Sizes(Face, 0, floor(DefaultSize * ResizeVal));

}

float LoadAndReturnHeight(FT_Face& Face)
{
	Height = 0;
	Under = 0;

	CharacterMap.clear();

	for (unsigned char c = CharStart; c <= CharEnd; c++)
	{
		if (FT_Load_Char(Face, c, FT_LOAD_RENDER))
			return 0;
		Character Char;
		Char.Origin = Vector2(0, 0);
		Char.Size = Vector2(Face->glyph->bitmap.width / BitmapWidth, Face->glyph->bitmap.rows / BitmapHeight) * 2;
		if (Char.Size.y > Height)
			Height = Char.Size.y;
		Char.Bearing = Vector2(Face->glyph->bitmap_left / BitmapWidth, Face->glyph->bitmap_top / BitmapHeight) * 2;
		if (Char.Size.y - Char.Bearing.y > Under)
			Under = Char.Size.y - Char.Bearing.y;
		Char.Advance = (Face->glyph->advance.x / 64 / BitmapWidth) * 2;

		CharacterMap.insert({ c, Char });
	}

	return Height;
}

int LoadCharacterSet(Shader &shader)
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	FT_Library Library;
	FT_Face Face;

	if (!FT_Init_FreeType(&Library))
		0;
	if (FT_New_Face(Library, "./Fonts/Arial.ttf", 0, &Face))
		return 0;

	FT_Set_Pixel_Sizes(Face, 0, DefaultSize);

	Resize(LoadAndReturnHeight(Face), Face);
	float Height = LoadAndReturnHeight(Face);


	Vector2 Pos = Vector2(0, 0);

	float* CharVerts = nullptr;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glViewport(0, 0, BitmapWidth * Resolution, BitmapHeight * Resolution);

	glDeleteFramebuffers(1, &FBO);
	glDeleteTextures(1, &BitmapTexture);
	glDeleteTextures(1, &CharTexture);

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glGenTextures(1, &BitmapTexture);
	glBindTexture(GL_TEXTURE_2D, BitmapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, BitmapWidth * Resolution, BitmapHeight * Resolution, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, BitmapTexture, 0);

	Pos = Vector2(-1, 1 - Height);

	for (unsigned char c = CharStart; c <= CharEnd; c++)
	{
		glGenTextures(1, &CharTexture);
		glBindTexture(GL_TEXTURE_2D, CharTexture);

		if (FT_Load_Char(Face, c, FT_LOAD_RENDER))
			return 0;

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Face->glyph->bitmap.width, Face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, Face->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character& Char = CharacterMap[c];

		if (Pos.x + Char.Size.x > 1)
		{
			Pos.x = -1;
			Pos.y -= Height;
		}

		CharVerts = new float[24]
		{
			Pos.x, Pos.y, 0, 1,
			Pos.x, Pos.y + Char.Size.y, 0, 0,
			Pos.x + Char.Size.x, Pos.y, 1, 1,

			Pos.x, Pos.y + Char.Size.y, 0, 0,
			Pos.x + Char.Size.x, Pos.y + Char.Size.y, 1, 0,
			Pos.x + Char.Size.x, Pos.y, 1, 1
		};
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 24, CharVerts);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, CharTexture);
		shader.SetInt("Texture", 0);

		Char.Origin = Vector2((Pos.x + 1) / 2, (Pos.y + 1) / 2);


		glDrawArrays(GL_TRIANGLES, 0, 6);
		glFinish();

		glDeleteTextures(1, &CharTexture);
		delete[](CharVerts);

		Pos.x += Char.Size.x;
	}

	FT_Done_Face(Face);
	FT_Done_FreeType(Library);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, ScreenSpace::Width, ScreenSpace::Height);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	return 1;
}

void ShowBitmap(Shader& shader)
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Verts), Verts);
	shader.Use(Shading::ShaderPrograms["TextShader"]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, BitmapTexture);
	shader.SetInt("Texture", 0);
	shader.SetVec3("inColor", 1, 0, 0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glFinish();
}

Model CharModel;

Vector2 WriteBuffer(std::string Text, float Scale, float JumpScale, Vector2 CurrentPos, float EndPos, Vector2 Size, bool Larger)
{
	float* TextVerts = nullptr;

	float Ratio = 0;
	if (Larger)
		Ratio = Size.x / Size.y;
	else
		Ratio = Size.y / Size.x;

	float ScaleX = Scale;
	float ScaleY = Scale;

	if (Larger)
		ScaleX /= Ratio;
	else
		ScaleY /= Ratio;

	for (int Iter = 0; Iter < Text.length(); Iter++)
	{
		char c = Text[Iter];
		Character Char = CharacterMap[c];
	

		if (CurrentPos.x + (Char.Advance * ScaleX) > EndPos)
		{
			if (c == ' ')
				continue;
			CurrentPos.x = -1;
			CurrentPos.y -= (Height + Under) * ScaleY * JumpScale;
		}

		TextVerts = new float[]
		{
			CurrentPos.x + ((Char.Size.x + Char.Bearing.x) * ScaleX),		 CurrentPos.y + ((Char.Size.y - (Char.Size.y - Char.Bearing.y)) * ScaleY),			 Char.Origin.x + (Char.Size.x / 2), Char.Origin.y + (Char.Size.y / 2),
			CurrentPos.x + ((Char.Size.x + Char.Bearing.x) * ScaleX),		 CurrentPos.y - ((Char.Size.y - Char.Bearing.y) * ScaleY),							 Char.Origin.x + (Char.Size.x / 2), Char.Origin.y,
			CurrentPos.x + (Char.Bearing.x * ScaleX),						 CurrentPos.y + ((Char.Size.y - (Char.Size.y - Char.Bearing.y)) * ScaleY),			 Char.Origin.x, Char.Origin.y + (Char.Size.y / 2),

			CurrentPos.x + (Char.Bearing.x * ScaleX),						 CurrentPos.y - ((Char.Size.y - Char.Bearing.y) * ScaleY), Char.Origin.x,			 Char.Origin.y,
			CurrentPos.x + (Char.Bearing.x * ScaleX),						 CurrentPos.y + ((Char.Size.y - (Char.Size.y - Char.Bearing.y)) * ScaleY),			 Char.Origin.x, Char.Origin.y + (Char.Size.y / 2),
			CurrentPos.x + ((Char.Size.x + Char.Bearing.x) * ScaleX),		 CurrentPos.y - ((Char.Size.y - Char.Bearing.y) * ScaleY),							 Char.Origin.x + (Char.Size.x / 2), Char.Origin.y,
		};
		glBufferSubData(GL_ARRAY_BUFFER, 0, 24 * sizeof(float), TextVerts);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		CurrentPos.x += Char.Advance * ScaleX;

		delete[](TextVerts);
	}

	return CurrentPos;
}

void WriteText(unsigned int& Texture, float Resolution, Vector3 Color, float Scale, float JumpScale, bool UI, std::string Text, float EndPos, Vector2 Size)
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	glDeleteTextures(1, &Texture);
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	Vector2 ScreenSize = 0;
	float Ratio = 0;
	bool Larger = Size.x > Size.y ? true : false;
	if (Larger)
		Ratio = Size.x / Size.y;
	else
		Ratio = Size.y / Size.x;

	float ScaleX = Scale;
	float ScaleY = Scale;

	if (Larger)
		ScaleX /= Ratio;
	else
		ScaleY /= Ratio;

	if (UI)
		ScreenSize = { ScreenSpace::Width, ScreenSpace::Height };
	else
		if(Larger)
			ScreenSize = { BitmapHeight, BitmapHeight / Ratio };
		else
			ScreenSize = { BitmapHeight / Ratio, BitmapHeight };

	ScreenSize /= Scale;
	ScreenSize *= Resolution;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ScreenSize.x, ScreenSize.y, 0, GL_RGBA, GL_FLOAT, nullptr);
	glViewport(0, 0, ScreenSize.x, ScreenSize.y);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Texture, 0);



	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	shader.Use(Shading::ShaderPrograms["TextShader"]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, BitmapTexture);
	shader.SetInt("Texture", 0);
	shader.SetVec3("inColor", Color);


	Vector2 CurrentPos = { -1, 1 };
	CurrentPos.y -= Height * ScaleY;
	Vector2 WordPos = CurrentPos;
	std::string Buffer = "";

	bool Word = false;



	for (int Iter = 0; Iter < Text.length(); Iter++)
	{
		char c = Text[Iter];
		if (c == '\n')
		{
			WriteBuffer(Buffer, Scale, JumpScale, WordPos, EndPos, Size, Larger);
			Buffer.clear();
			Word = false;
			CurrentPos.x = -1;
			CurrentPos.y -= (Height + Under) * ScaleY * JumpScale;
			WordPos = CurrentPos;
			continue;
		}
		Character Char = CharacterMap[c];
		float CharAdvance = Char.Advance;
		Buffer += c;
		if(c == ' ')
		{
			CurrentPos = WriteBuffer(Buffer, Scale, JumpScale, WordPos, EndPos, Size, Larger);
			WordPos = CurrentPos;
			Buffer.clear();
			Word = true;
			continue;
		}
		
		if (CurrentPos.x + (Char.Advance * ScaleX) > EndPos)
		{
			if (Word)
			{
				CurrentPos.x = -1;
				CurrentPos.y -= (Height + Under) * ScaleY * JumpScale;
				WordPos = CurrentPos;
				Word = false;
			}
			CurrentPos = WriteBuffer(Buffer, Scale, JumpScale, WordPos, EndPos, Size, Larger);
			WordPos = CurrentPos;
			Buffer.clear();
		}
		CurrentPos.x += Char.Advance * ScaleX;
	}

	if (Buffer.length() > 0)
		WriteBuffer(Buffer, Scale, JumpScale, WordPos, EndPos, Size, Larger);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glFinish();
	
	glViewport(0, 0, ScreenSpace::Width, ScreenSpace::Height);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Verts), Verts);
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

namespace GUIText
{
	void Start()
	{
		LoadModel(CharModel, "Square", "MYOBJ");
		shader.Use(Shading::ShaderPrograms["TextSetupShader"]);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Verts), Verts, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * (sizeof(float)), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * (sizeof(float)), (void*)(sizeof(float) * 2));
		glEnableVertexAttribArray(1);

		LoadCharacterSet(shader);

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Verts), Verts);
	}
	void Update()
	{
		//ShowBitmap(shader);
	}
	void End()
	{

	}
}