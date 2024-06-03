/*
* Creates shader stuff for opening and changing the current shader
*/

#ifndef INTIALIZESHADERS
#define INTIALIZESHADERS
#include <iostream>
#include <fstream>
#include <sstream>
#include "GlStuff.h"
#include "Vector.h"
#include "List.h"
#include <map>

struct Shader
{	
	unsigned int ID;

	void Use(unsigned int &Shader)
	{
		ID = Shader;
		glUseProgram(ID);
	}
	void SetInt(const char *Pos, int value)
	{
		glUniform1i(glGetUniformLocation(ID, Pos), value);
	}
	void SetFloat(const char *Pos, float value)
	{
		glUniform1f(glGetUniformLocation(ID, Pos), value);
	}
	void SetMat4(const char *Pos, float *value)
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, Pos), 1, GL_FALSE, value);
	}
	void SetVec4(const char* Pos, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(ID, Pos), x, y, z, w);
	}
	void SetVec3(const char *Pos, float x, float y, float z)
	{
		glUniform3f(glGetUniformLocation(ID, Pos), x, y, z);
	}
	void SetVec3(const char* Pos, Vector3 Vec)
	{
		glUniform3f(glGetUniformLocation(ID, Pos), Vec.x, Vec.y, Vec.z);
	}
	void SetVec2(const char* Pos, float x, float y)
	{
		glUniform2f(glGetUniformLocation(ID, Pos), x, y);
	}
	void SetVec2(const char* Pos, Vector2 Vec)
	{
		glUniform2f(glGetUniformLocation(ID, Pos), Vec.x, Vec.y);
	}

	void CreateShader(std::map <std::string, std::string> &Shaders , unsigned int& program)
	{
		program = glCreateProgram();

		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;

		const char* vShaderCode = nullptr;
		const char* fShaderCode = nullptr;
		const char* gShaderCode = nullptr;

		std::string vShaderString;
		std::string fShaderString;
		std::string gShaderString;

		try
		{
			vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			std::stringstream vShaderStream;
			std::stringstream fShaderStream;
			std::stringstream gShaderStream;

			if (Shaders[VERTFILE] != "")
			{
				vShaderFile.open("./Shaders/" + Shaders[VERTFILE] + VERTFILE);
				vShaderStream << vShaderFile.rdbuf();
				vShaderString = vShaderStream.str();
				vShaderCode = vShaderString.c_str();
			}
			if (Shaders[FRAGFILE] != "")
			{
				fShaderFile.open("./Shaders/" + Shaders[FRAGFILE] + FRAGFILE);
				fShaderStream << fShaderFile.rdbuf();
				fShaderString = fShaderStream.str();
				fShaderCode = fShaderString.c_str();
			}
			if(Shaders[GEOMFILE] != "")
			{
				gShaderFile.open("./Shaders/" + Shaders[GEOMFILE] + GEOMFILE);
				gShaderStream << gShaderFile.rdbuf();
				gShaderString = gShaderStream.str();
				gShaderCode = gShaderString.c_str();
			}
		}
		catch (const std::ios_base::failure& fail)
		{
			vShaderFile.close();
			fShaderFile.close();
			gShaderFile.close();
			std::cout << "Failed to open shader files\n";
		}
		int Success;
		unsigned int VertexShader = glCreateShader(GL_VERTEX_SHADER);
		unsigned int FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		unsigned int GeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		if (vShaderCode != nullptr) 
		{
			glShaderSource(VertexShader, 1, &vShaderCode, NULL);
			glCompileShader(VertexShader);
			glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Success);
			if (!Success)
			{
				std::cout << "Failed to load VertexShader : " << Shaders[VERTFILE] + VERTFILE << "\n";
			}
			glAttachShader(program, VertexShader);
			vShaderFile.close();
		}
		if (fShaderCode != nullptr) 
		{
			glShaderSource(FragmentShader, 1, &fShaderCode, NULL);
			glCompileShader(FragmentShader);
			glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Success);
			if (!Success) 
			{
				std::cout << "Failed to load FragmentShader : " << Shaders[FRAGFILE] + FRAGFILE << "\n";
			}
			glAttachShader(program, FragmentShader);
			fShaderFile.close();
		}
		if (gShaderCode != nullptr)
		{
			glShaderSource(GeometryShader, 1, &gShaderCode, NULL);
			glCompileShader(GeometryShader);
			glGetShaderiv(GeometryShader, GL_COMPILE_STATUS, &Success);
			if (!Success)
			{
				std::cout << "Failed to load GeometryShader : " << Shaders[GEOMFILE] + GEOMFILE << "\n";
			}
			glAttachShader(program, GeometryShader);
			gShaderFile.close();
		}
		glLinkProgram(program);

		glDeleteShader(VertexShader);
		glDeleteShader(FragmentShader);
		glDeleteShader(GeometryShader);

	}
};
struct ComputeShader
{
	ComputeShader() : ID(0){}

	unsigned int ID;

	void Use(unsigned int& Shader)
	{
		ID = Shader;
		glUseProgram(ID);
	}
	void SetInt(const char* Pos, int value)
	{
		glUniform1i(glGetUniformLocation(ID, Pos), value);
	}
	void SetFloat(const char* Pos, float value)
	{
		glUniform1f(glGetUniformLocation(ID, Pos), value);
	}
	void SetMat4(const char* Pos, float* value)
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, Pos), 1, GL_FALSE, value);
	}
	void SetVec4(const char* Pos, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(ID, Pos), x, y, z, w);
	}
	void SetVec3(const char* Pos, float x, float y, float z)
	{
		glUniform3f(glGetUniformLocation(ID, Pos), x, y, z);
	}
	void SetVec3(const char* Pos, Vector3 Vec)
	{
		glUniform3f(glGetUniformLocation(ID, Pos), Vec.x, Vec.y, Vec.z);
	}
	void SetVec2(const char* Pos, float x, float y)
	{
		glUniform2f(glGetUniformLocation(ID, Pos), x, y);
	}
	void SetVec2(const char* Pos, Vector2 Vec)
	{
		glUniform2f(glGetUniformLocation(ID, Pos), Vec.x, Vec.y);
	}

	void CreateShader(std::string ShaderPos, unsigned int& program)
	{
		program = glCreateProgram();

		std::ifstream ShaderFile;

		const char* ShaderCode = nullptr;

		std::string ShaderString;

		try
		{
			ShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			std::stringstream ShaderStream;
			ShaderFile.open("./Shaders/" + ShaderPos + COMPFILE);
			ShaderStream << ShaderFile.rdbuf();
			ShaderString = ShaderStream.str();
			ShaderCode = ShaderString.c_str();
		}
		catch (const std::ios_base::failure& fail)
		{
			ShaderFile.close();
			std::cout << "Failed to open compute shader file\n";
		}
		int Success;
		unsigned int Shader = glCreateShader(GL_COMPUTE_SHADER);
		glShaderSource(Shader, 1, &ShaderCode, NULL);
		glCompileShader(Shader);
		glGetShaderiv(Shader, GL_COMPILE_STATUS, &Success);
		if (!Success)
		{
			std::cout << "Failed to load ComputeShader : " << ShaderPos + COMPFILE << "\n";
		}
		glAttachShader(program, Shader);
		glLinkProgram(program);

		glDeleteShader(Shader);

		ShaderFile.close();
	}
};
#endif