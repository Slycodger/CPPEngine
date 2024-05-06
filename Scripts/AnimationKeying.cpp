#define ANIMATIONKEYINGMAIN
#include "AnimationKeying.h"

List<Animation> AllAnimations = 0;

List<Animation> RunningAnimations = 0;

AnimationKey Test;

void TestFunc(int Output)
{
	std::cout << "e" << Output << "\n";
}

void AdvanceAnimations()
{
	//double CurrentTime = glfwGetTime();
	//for (int i = 0; i < RunningAnimations.Length; i++)
	//{
	//	Animation anim = *RunningAnimations[i];
	//	if (CurrentTime - anim.TimeAtLastFrame >= anim.TimeBetweenFrames)
	//	{
	//		AnimationKey Key = anim.Frames[anim.CurrentFrame];
	//		for (unsigned int Action = 0; Action < Key.Actions.Length; Action++)
	//		{
	//			((AnimationAction*)(Key.Actions[Action]))->RunFrame();
	//		}
	//		anim.CurrentFrame++;
	//		anim.TimeAtLastFrame = glfwGetTime();
	//	}
	//}
	Test.Actions.AddToList(TestFunc);
	(*Test.Actions[0])(3);
}




void End()
{
	//for (int Animation = 0; Animation < AllAnimations.Length; Animation++)
	//{
	//	for (int Frame = 0; Frame < AllAnimations[Animation]->FrameCount; Frame++)
	//	{
	//		for (int Action = 0; Action < AllAnimations[Animation]->Frames[Frame].Actions.Length; Action++)
	//		{
	//			delete(*AllAnimations[Animation]->Frames[Frame].Actions[Action]);
	//		}
	//		AllAnimations[Animation]->Frames[Frame].Actions.Delete();
	//	}
	//	delete[](AllAnimations[Animation]->Frames);
	//}
	//AllAnimations.Delete();
}