/*
* Script
*/
#define DRAWINGPADMAIN
#include "DrawingPad.h"

Model CubeModel;
Model SquareModel;

struct ButtonScr : public ObjectBehavior
{
	void(*Func)(void) = nullptr;
	std::string Text = "Button";
	Object* textObj = nullptr;
	TextBox* textScr = new TextBox;
	Model SquareModel;

	void Start() override
	{
		LoadModel(SquareModel, "Square", "MYOBJ");
		textScr->Text = Text;
		textScr->Color = { 0, 0, 0 };
		textObj = new Object(SquareModel);
		textObj->AddScript((void*)textScr);
		textObj->mesh.UI = true;
		textObj->SetParent(thisObj);
		textObj->RelativeTransform.Position = { 0, 0, -0.1 };
		textObj->RelativeTransform.Scale = { 1, 1, 1 };
	}

	Vector2 BL = { -1, -1 };
	Vector2 TR = { 1, 1 };

	void Update() override
	{
		Vector2 Pos = thisObj->Transform.Position();
		Vector2 Size = thisObj->Transform.Scale();

		textScr->Text = Text;

		if (ActiveMouse.Pos >= BL * Size + Pos && ActiveMouse.Pos <= TR * Size + Pos)
			if (Func != nullptr)
				if(Keys::KeyPressed(GLFW_MOUSE_BUTTON_1))
					Func();
	}
};



float Speed = 10;
float RotSpeed = 45;


List<Object*> Objects = 100;
Object* Floor = nullptr;
Object* Cube = nullptr;
Object* TextObj = nullptr;
Object* ButtonObj = nullptr;
TextBox* TextScript = new TextBox;
ButtonScr* ButtonScript = new ButtonScr;


void ButtonFunc()
{
	std::getline(std::cin, ButtonScript->Text);
	std::cin.ignore(256, '\n');
}

namespace DrawingPad
{
	void Start()
	{
		LoadModel(CubeModel, "Cube", "MYOBJ");
		LoadModel(SquareModel, "Square", "MYOBJ");
		Cube = new Object(CubeModel);
		Floor = new Object(CubeModel);
		TextObj = new Object(SquareModel);
		ButtonObj = new Object(SquareModel);

		std::string Text = "Hey, all you Goobers\nIt's time to say howdy to your favorite undersea peanut, Goofy Goober\n(Yeah)\n(Yeah)\n(Yeah)\nAlright, folks, this one goes out to my two bestest friends in the whole world\nPatrick and this big peanut guy\nIt's a little ditty called\nGoofy Goober (yeah)\nOh, I'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah (yeah)\nI'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah (yeah)\nDJ (yeah), time for the test (yeah)\nNo baby can resist singin' along to this (yeah)\n(Yeah, yeah)\n(Yeah, yeah)\n(Yeah, yeah)\nSpongeBob, it's the Goofy Goober theme song\nI know\nOh, I'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah (yeah)\nI'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah\nAnd here's your Triple Gooberberry Sunrise, sir\nOoh\nOh, Triple Gooberberry Sunrise, huh?\nI guess I could use one of those\nThere you go\nBoy, Pat, that hit the spot\nI'm feeling better already\nYeah\nWaiter, let's get another round over here\nOh, I'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah (yeah)\nI'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah (yeah)";

		Cube->Transform.Scale(0.01);
		for(int i = 0; i < 100; i++)
		{
			Objects.AddToList(new Object(CubeModel));
			(*Objects[i])->Transform.T.Position = Vector3(((i % 10) * 5) -9, -10, (int)((i / 10) * 5) - 9.5);
		}
		Floor->Transform.T.Position = Vector3(0, -20, 40);
		Floor->Transform.Scale(50, 1, 50);

		TextScript->Text = Text;
		TextScript->Scale = 0.5;
		TextScript->Color = { 1, 1, 0.5 };
		TextObj->Transform.SetPosition(0, 0, -20);
		TextObj->AddScript((void*)TextScript);
		TextObj->Transform.Scale(10, 80, 1);

		ButtonScript->Func = ButtonFunc;
		ButtonObj->mesh.UI = true;
		ButtonObj->Transform.Scale(0.3, 0.2, 1);
		ButtonObj->Transform.SetPosition(0.7, 0.8, 0);
		ButtonObj->AddScript((void*)ButtonScript);
	}
	void Update(GLFWwindow* window)
	{
		Cube->Transform.T.Position = MainCamera.Position + MainCamera.DirectionFacing();

		if (Keys::KeyHeld(GLFW_KEY_Q))
			TextObj->Transform.Scale(0.99, 1, 1);
		if (Keys::KeyHeld(GLFW_KEY_E))
			TextObj->Transform.Scale(1.01, 1, 1);

		if (Keys::KeyHeld(GLFW_KEY_UP))
			ButtonObj->Transform.Translate(0, 0.4 * Time::deltaTime, 0);
		if (Keys::KeyHeld(GLFW_KEY_DOWN))
			ButtonObj->Transform.Translate(0, -0.4 * Time::deltaTime, 0);
		if (Keys::KeyHeld(GLFW_KEY_LEFT))
			ButtonObj->Transform.Translate(-0.4 * Time::deltaTime, 0, 0);
		if (Keys::KeyHeld(GLFW_KEY_RIGHT))
			ButtonObj->Transform.Translate(0.4 * Time::deltaTime, 0, 0);
		if (Keys::KeyHeld(GLFW_KEY_PAGE_UP))
			ButtonObj->Transform.Scale(1.01);
		if (Keys::KeyHeld(GLFW_KEY_PAGE_DOWN))
			ButtonObj->Transform.Scale(0.99);

		if(Keys::KeyHeld(GLFW_KEY_W))
		{
			MainCamera.Position += MainCamera.DirectionFacing() * Speed * Time::deltaTime;
		}
		if (Keys::KeyHeld(GLFW_KEY_A))
		{
			MainCamera.Position -= MainCamera.DirectionRight() * Speed * Time::deltaTime;
		}
		if (Keys::KeyHeld(GLFW_KEY_S))
		{
			MainCamera.Position -= MainCamera.DirectionFacing() * Speed * Time::deltaTime;
		}
		if (Keys::KeyHeld(GLFW_KEY_D))
		{
			MainCamera.Position += MainCamera.DirectionRight() * Speed * Time::deltaTime;
		}
		if (Keys::KeyHeld(GLFW_KEY_SPACE))
		{
			MainCamera.Translate(0, Speed * Time::deltaTime, 0);
		}
		if (Keys::KeyHeld(GLFW_KEY_LEFT_SHIFT))
		{
			MainCamera.Translate(0, -Speed * Time::deltaTime, 0);
		}

		if(Keys::KeyPressed(GLFW_KEY_RIGHT_SHIFT))
		{
			ActiveMouse.Visibility = MouseView::Shown;
		}
		if (Keys::KeyPressed(GLFW_KEY_RIGHT_CONTROL))
		{
			ActiveMouse.Visibility = MouseView::Hidden;
		}
		if(Keys::KeyPressed(GLFW_KEY_ESCAPE))
		{
			App::Stop = true;
		}
		if(ActiveMouse.Visibility == MouseView::Hidden)
			MainCamera.Rotate(-ActiveMouse.Movement.y * RotSpeed, -ActiveMouse.Movement.x * RotSpeed, 0);
		//std::cout << MainCamera.RotToVec() << "\n";
		if (MainCamera.Rotation().x > 90 && MainCamera.Rotation().x < 180)
			MainCamera.SetRotation(90, MainCamera.Rotation().y, MainCamera.Rotation().z);
		if (MainCamera.Rotation().x < 270 && MainCamera.Rotation().x > 180)
			MainCamera.SetRotation(270, MainCamera.Rotation().y, MainCamera.Rotation().z);
	}
	void End()
	{
		Objects.Delete();
	}
}