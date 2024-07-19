#pragma once

#include <iostream>

constexpr float IDENTITYMAT[16] =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};
constexpr int MAXOBJECTS = 10500;
constexpr int VERTLENGTH = 9;
constexpr char VERTFILE[] = ".vert";
constexpr char FRAGFILE[] = ".frag";
constexpr char GEOMFILE[] = ".geom";
constexpr char COMPFILE[] = ".comp";
constexpr float LIGHTEXTENDDISTANCE = 100;
constexpr unsigned int MAXMODELCOUNT = 5;
extern std::string ExePath;