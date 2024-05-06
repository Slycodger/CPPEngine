#pragma once
#ifndef RAYCASTING
#define RAYCASTING
#include "MathFunctions.h"
#include "ObjectClasses.h"
#include "Constants.h"


struct Ray
{
	Vector3 RayDirection;
	float Allowance = 0.01;

	bool Intersecting(Object &Obj, Vector3 Origin)
	{
		RayDirection.x = (float)((int)(RayDirection.x * 10000)) / 10000;
		RayDirection.y = (float)((int)(RayDirection.y * 10000)) / 10000;
		RayDirection.z = (float)((int)(RayDirection.z * 10000)) / 10000;


		for(int TriCount = 0; TriCount < Obj.mesh.Indices[0]; TriCount+=3)
		{
			Vector3 Vert1 = Vector3(Obj.mesh.Vertices[Obj.mesh.Indices[TriCount + 1] * VERTLENGTH + 1], Obj.mesh.Vertices[Obj.mesh.Indices[TriCount + 1] * VERTLENGTH + 2], Obj.mesh.Vertices[Obj.mesh.Indices[TriCount + 1] * VERTLENGTH + 3]);
			Vector3 Vert2 = Vector3(Obj.mesh.Vertices[Obj.mesh.Indices[TriCount + 2] * VERTLENGTH + 1], Obj.mesh.Vertices[Obj.mesh.Indices[TriCount + 2] * VERTLENGTH + 2], Obj.mesh.Vertices[Obj.mesh.Indices[TriCount + 2] * VERTLENGTH + 3]);
			Vector3 Vert3 = Vector3(Obj.mesh.Vertices[Obj.mesh.Indices[TriCount + 3] * VERTLENGTH + 1], Obj.mesh.Vertices[Obj.mesh.Indices[TriCount + 3] * VERTLENGTH + 2], Obj.mesh.Vertices[Obj.mesh.Indices[TriCount + 3] * VERTLENGTH + 3]);

			Vert1 += Obj.Transform.Position();
			Vert2 += Obj.Transform.Position();
			Vert3 += Obj.Transform.Position();


			Vector3 PlaneNormal = -Normalize(CrossProduct(Vert2 - Vert1, Vert3 - Vert1));
			double PlaneConstant = -DotProduct(PlaneNormal, Vert1);

			double Distance = DistanceToPlane(PlaneNormal, Origin, PlaneConstant);
			double T0 = (-Distance) / DotProduct(PlaneNormal, RayDirection);

			Vector3 Intersection = Origin + (RayDirection * T0);

			double CombinedAreaOfTriangles = AreaOfTriangle(Intersection, Vert1, Vert2) + AreaOfTriangle(Intersection, Vert1, Vert3) + AreaOfTriangle(Intersection, Vert2, Vert3);
			double RealAreaOfTriangle = AreaOfTriangle(Vert1, Vert2, Vert3);

			if (T0 > 0 && CombinedAreaOfTriangles >= RealAreaOfTriangle - Allowance && CombinedAreaOfTriangles <= RealAreaOfTriangle + Allowance)
				return true;
		}
		return false;
	}

	float DistanceToPlane(Vector3 Normals, Vector3  ComparePoint, float constant)
	{
		return DotProduct(Normals, ComparePoint) + constant;
	}

	float AreaOfTriangle(Vector3 p1, Vector3 p2, Vector3 p3)
	{
		float Output = 0;

		float s1 = std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2) + std::pow(p1.z - p2.z, 2));
		float s2 = std::sqrt(std::pow(p1.x - p3.x, 2) + std::pow(p1.y - p3.y, 2) + std::pow(p1.z - p3.z, 2));
		float s3 = std::sqrt(std::pow(p2.x - p3.x, 2) + std::pow(p2.y - p3.y, 2) + std::pow(p2.z - p3.z, 2));
		float SemiPer = (s1 + s2 + s3) / 2;

		Output = std::sqrt(SemiPer * ((SemiPer - s1) * (SemiPer - s2) * (SemiPer - s3)));
		return std::abs(Output);
	}

	Ray(Vector3 Dir) : RayDirection(Dir){}
	Ray(){}
};


#endif