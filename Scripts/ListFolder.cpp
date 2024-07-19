/*
* deprecated script
* Creates buttons for every file within a folder that when clicked will open in visual studio
*/


#define ListFolder_MAIN
#include "ListFolder.h"



namespace ListFolder
{
	Model SquareModel;
	void start()
	{
		loadModel(SquareModel, "Square", "MYOBJ");
	}
}
using namespace ListFolder;

//Opens a file in visual studio
//I deleted OpenVSFile.bat
void openFile(void* Data)
{
	std::string String = *(std::string*)Data;
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);

	CreateProcessA("C:\\Windows\\system32\\cmd.exe",
		LPSTR(std::string("cmd /c cd \"" + ExePath + "\" & OpenVSFile.bat \"" + String + "\"").c_str()),
		nullptr,
		nullptr,
		false,
		CREATE_NO_WINDOW,
		nullptr,
		nullptr,
		&si,
		&pi);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}

//The starting point to listing a folder with objects
void visualFolder(std::string Folder, Object*& Target)
{
	WIN32_FIND_DATAA data;
	HANDLE File = FindFirstFileA(("./" + Folder).c_str(), &data);
	if (File == INVALID_HANDLE_VALUE)
		return;
	Folder.pop_back();

	bool UI = Target->mesh.UI;
	float Scale = 0.2f;
	float Inverse = 0.5f / Scale;

	Vector3 Pos = Vector3(0.5f - Inverse, Inverse - 0.5f, -0.05f);

	do
	{
		if (!strcmp(data.cFileName, ".") || !strcmp(data.cFileName, ".."))
			continue;
		Object* newObj = new Object(SquareModel);
		ButtonScr* Button = new ButtonScr;
		Button->PressFunc = openFile;
		Button->PressFuncParam = (void*)new std::string(ExePath + Folder + data.cFileName);
		Button->Text = data.cFileName;
		newObj->addScript((void*)Button);
		newObj->setParent(Target);
		newObj->RelativeTransform.Position = Pos;
		newObj->RelativeTransform.Scale = Vector3(Scale);
		newObj->mesh.UI = UI;
		Pos.y += 6;
	} while (FindNextFileA(File, &data));
	FindClose(File);
}