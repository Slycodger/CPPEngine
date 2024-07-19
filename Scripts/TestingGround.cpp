/*
* Script
*/
#define TestingGround_MAIN
#include "TestingGround.h"

Model CubeModel;
Model SquareModel;


float Speed = 10;
float RotSpeed = 45;


List<Object*> Objects = 100;
Object* Floor = nullptr;
Object* Cube = nullptr;


namespace TestingGround
{
	Vector3 CubeRotation = Vector3(0, 0, 0);
	void start()
	{
		loadModel(CubeModel, "Cube", "MYOBJ");
		loadModel(SquareModel, "Square", "MYOBJ");
		Cube = new Object(CubeModel);
		Floor = new Object(CubeModel);

		std::string Text = "Hey, all you Goobers\nIt's time to say howdy to your favorite undersea peanut, Goofy Goober\n(Yeah)\n(Yeah)\n(Yeah)\nAlright, folks, this one goes out to my two bestest friends in the whole world\nPatrick and this big peanut guy\nIt's a little ditty called\nGoofy Goober (yeah)\nOh, I'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah (yeah)\nI'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah (yeah)\nDJ (yeah), time for the test (yeah)\nNo baby can resist singin' along to this (yeah)\n(Yeah, yeah)\n(Yeah, yeah)\n(Yeah, yeah)\nSpongeBob, it's the Goofy Goober theme song\nI know\nOh, I'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah (yeah)\nI'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah\nAnd here's your Triple Gooberberry Sunrise, sir\nOoh\nOh, Triple Gooberberry Sunrise, huh?\nI guess I could use one of those\nThere you go\nBoy, Pat, that hit the spot\nI'm feeling better already\nYeah\nWaiter, let's get another round over here\nOh, I'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah (yeah)\nI'm a goofy goober, yeah\nYou're a goofy goober, yeah\nWe're all goofy goobers, yeah\nGoofy, goofy, goober, goober, yeah (yeah)";

		Cube->Transform.setScale(0.01);
		for(int i = 0; i < 100; i++)
		{
			Objects.addToList(new Object(CubeModel));
			(*Objects[i])->Transform.Position = Vector3(((i % 10) * 5) -9, -10, (int)((i / 10) * 5) - 9.5);
		}
		Floor->Transform.Position = Vector3(0, -20, 40);
		Floor->Transform.setScale(50, 1, 50);
	}


	void update(GLFWwindow* window)
	{
		Cube->Transform.Position = MainCamera.Position + MainCamera.DirectionFacing();

		if (Keys::keyHeld(GLFW_KEY_W))
		{
			MainCamera.Position += MainCamera.DirectionFacing() * Speed * Time::deltaTime;
		}
		if (Keys::keyHeld(GLFW_KEY_A))
		{
			MainCamera.Position -= MainCamera.DirectionRight() * Speed * Time::deltaTime;
		}
		if (Keys::keyHeld(GLFW_KEY_S))
		{
			MainCamera.Position -= MainCamera.DirectionFacing() * Speed * Time::deltaTime;
		}
		if (Keys::keyHeld(GLFW_KEY_D))
		{
			MainCamera.Position += MainCamera.DirectionRight() * Speed * Time::deltaTime;
		}
		if (Keys::keyHeld(GLFW_KEY_SPACE))
		{
			MainCamera.Translate(0, Speed * Time::deltaTime, 0);
		}
		if (Keys::keyHeld(GLFW_KEY_LEFT_SHIFT))
		{
			MainCamera.Translate(0, -Speed * Time::deltaTime, 0);
		}

		if (Keys::keyHeld(GLFW_KEY_KP_2))
		{
			CubeRotation += Vector3(-RotSpeed * Time::deltaTime, 0, 0);
		}
		if (Keys::keyHeld(GLFW_KEY_KP_8))
		{
			CubeRotation += Vector3(RotSpeed * Time::deltaTime, 0, 0);
		}
		if (Keys::keyHeld(GLFW_KEY_KP_4))
		{
			CubeRotation += Vector3(0, RotSpeed * Time::deltaTime, 0);
		}
		if (Keys::keyHeld(GLFW_KEY_KP_6))
		{
			CubeRotation += Vector3(0, -RotSpeed * Time::deltaTime, 0);
		}
		if (Keys::keyHeld(GLFW_KEY_KP_1))
		{
			CubeRotation += Vector3(0, 0, RotSpeed * Time::deltaTime);
		}
		if (Keys::keyHeld(GLFW_KEY_KP_3))
		{
			CubeRotation += Vector3(0, 0, -RotSpeed * Time::deltaTime);
		}
		Cube->Transform.Rotation = CubeRotation;


		if(Keys::keyPressed(GLFW_KEY_RIGHT_SHIFT))
		{
			ActiveMouse.Visibility = MouseView::Shown;
		}
		if (Keys::keyPressed(GLFW_KEY_RIGHT_CONTROL))
		{
			ActiveMouse.Visibility = MouseView::Hidden;
		}
		if(ActiveMouse.Visibility == MouseView::Hidden)
			MainCamera.Rotate(ActiveMouse.Movement.y * RotSpeed, -ActiveMouse.Movement.x * RotSpeed, 0);
		if (MainCamera.Rotation().x > 90 && MainCamera.Rotation().x < 180)
			MainCamera.SetRotation(90, MainCamera.Rotation().y, MainCamera.Rotation().z);
		if (MainCamera.Rotation().x < 270 && MainCamera.Rotation().x > 180)
			MainCamera.SetRotation(270, MainCamera.Rotation().y, MainCamera.Rotation().z);
	}
	void end()
	{
		Objects.deleteList();
	}
}