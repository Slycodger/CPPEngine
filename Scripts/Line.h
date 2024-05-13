#pragma once
#ifndef LINE
#define LINE
#include "ObjectClasses.h"
#include "MathFunctions.h"

struct Line
{
	~Line()
	{
		for (int i = 0; i < Rectangles.Length; i++)
		{
			DeleteObj((*Rectangles[Points.Length - 1]));
			*Rectangles[i] = nullptr;
		}
		Rectangles.Delete();
		Points.Delete();
		DeleteObj(StartCap);
		DeleteObj(EndCap);
	}
	float LineWidth = 1;
	float Detail = 0.4;
	void CheckPoint(Vector3 Point)
	{
		if(StartCap == nullptr)
		{
			StartCap = new Object(capObjName);
			StartCap->Transform.Translate(Point);
			StartCap->Transform.Scale(LineWidth);
			return;
		}
		float Distance = Magnitude(Rectangles.Length == 0 ? Point - StartCap->Transform.Position() : Point - (*Rectangles[Rectangles.Length - 1])->Transform.Position());
		if(Distance < Detail)
		{
			return;
		}
		int Increments = Distance / Detail;
		Vector3 Dir = Normalize(Point - (Rectangles.Length == 0 ? StartCap->Transform.Position() : (*Rectangles[Rectangles.Length - 1])->Transform.Position()));
		Point = Rectangles.Length == 0 ? StartCap->Transform.Position() : (*Rectangles[Rectangles.Length - 1])->Transform.Position();
		for(int i = 1; i <= Increments; i++)
		{
			Points.AddToList(new Vector3(Point + (Dir * Detail)));
			Rectangles.AddToList(new Object*(new Object(squareObjName)));
			(*Rectangles[Rectangles.Length - 1])->Transform.T.Position = (Point + (Dir * Detail));
			(*Rectangles[Rectangles.Length - 1])->Transform.T.Scale = Vector3(LineWidth);
		}
	}
	void EndPoint(Vector3 Point)
	{
		if (StartCap == nullptr)
			return;

		EndCap = new Object(capObjName);
		EndCap->Transform.Translate(Point);
		EndCap->Transform.Scale(LineWidth);

		if (Points.Length == 0)
		{
			Points.AddToList(new Vector3((EndCap->Transform.Position() + StartCap->Transform.Position()) / 2));
			Rectangles.AddToList(new Object * (new Object(squareObjName)));
			(*Rectangles[Rectangles.Length - 1])->Transform.Translate((EndCap->Transform.Position() + StartCap->Transform.Position()) / 2);
			Detail = Magnitude((EndCap->Transform.Position() + StartCap->Transform.Position()) / 2);
		}
		else
		{
			CheckPoint(Point);
			if (*Points[Points.Length - 1] == EndCap->Transform.Position())
			{
				Points.RemoveFromList(Points.Length - 1);
				DeleteObj((*Rectangles[Points.Length - 1]));
				Rectangles.RemoveFromList(Points.Length - 1);
			}
		}
		float Angle = 0;
		float Mag = Magnitude(EndCap->Transform.Position() - *(Points[Points.Length - 1]));
		for(int point = Points.Length - 1; point >= 0; point--)
		{
			if (point == Points.Length - 1)
			{
				(*Rectangles[point])->Transform.T.Scale = Vector3(Mag / 2, LineWidth, 1);
				Angle = AngleBetween(EndCap->Transform.Position(), (*Rectangles[point])->Transform.Position());
			}
			else 
			{
				(*Rectangles[point])->Transform.T.Scale = Vector3(Detail / 2, LineWidth, 1);
				Angle = AngleBetween(*Points[point + 1], (*Rectangles[point])->Transform.Position());
			}
			(*Rectangles[point])->Transform.Rotate(0, 0, Angle);
		}
		Angle = AngleBetween((*Rectangles[0])->Transform.Position(), StartCap->Transform.Position()) + 90;
		StartCap->Transform.Rotate(0, 0, Angle);
		Angle = AngleBetween(EndCap->Transform.Position(), (*Rectangles[Points.Length - 1])->Transform.Position()) - 90;
		EndCap->Transform.Rotate(0, 0, Angle);

		Rectangles.AddToList(new Object * (new Object(squareObjName)));
		(*Rectangles[Rectangles.Length - 1])->Transform.Translate(StartCap->Transform.Position());
		(*Rectangles[Rectangles.Length - 1])->Transform.T.Scale = Vector3(Detail / 2, LineWidth, 1);
		Angle = AngleBetween((*Rectangles[0])->Transform.Position(), StartCap->Transform.Position());
		(*Rectangles[Rectangles.Length - 1])->Transform.Rotate(0, 0, Angle);
	}

private:
	const char* capObjName = "Semicircle";
	const char* squareObjName = "Square";
	Object* StartCap = nullptr;
	Object* EndCap = nullptr;
	List<Object*> Rectangles = 0;
	List<Vector3> Points = 0;
};

#endif