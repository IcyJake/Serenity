/*
	Credits:
		- IcyJake (Developer)
		- Qoazi (Helper/good friend)
		- Brandon/Chirality (Serenity's first return check bypass)
		- Eternal (Serenity's current return check bypass)
*/

// Note: All addresses, all aobs/signatures, all calling conventions, and the Lua state is outdated in this source

#define _WIN32_WINNT 0x0500

#include "windows.h"
#include "detours.h"
#include "Addresses.h"
#include <conio.h>
#include <cstdio>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <iterator>
#include <windows.h>
#include <sstream>
#include <istream>
#include <fstream>

int ConsoleCheck = (int)GetProcAddress(GetModuleHandle("Kernel32.dll"), "FreeConsole");

int patch()
{
	return 0;
}

DWORD VFTable2 = Format(0x1171244); // The ScriptContext address goes here

int ScriptContext = memory::Scan(PAGE_READWRITE, (char*)&VFTable2, "xxxx");
int rState = *(DWORD *)(ScriptContext + 56 * 1 + 164) - (ScriptContext + 56 * 1 + 164); // The Lua state goes here

void audio(std::string id)
{
	getglobal(rState, "workspace");
	getglobal(rState, "Instance");
	getfield(rState, -1, "new");
	pushstring(rState, "Sound");
	pushvalue(rState, -4);
	pcall(rState, 2, 1, 0);
	pushstring(rState, ("rbxassetid://" + id).c_str());
	setfield(rState, -2, "SoundId");
	pushstring(rState, "POSITRON_AUDIO");
	setfield(rState, -2, "Name");
	pushnumber(rState, 1);
	setfield(rState, -2, "Volume");
	getfield(rState, -1, "Play");
	pushvalue(rState, -2);
	pcall(rState, 1, 0, 0);
}

void StartFunction()
{
	DWORD oldProt;

	VirtualProtect(&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &oldProt);

	*(BYTE *)&FreeConsole = 0xC3;

	VirtualProtect(&FreeConsole, 1, oldProt, &oldProt);
	AllocConsole();

	SetConsoleTitle("Serenity v1.4 - Made by IcyJake");

	FILE * pFile;

	freopen_s(&pFile, "CONOUT$", "w", stdout);
	freopen_s(&pFile, "CONIN$", "r", stdin);
	system("color B");

	std::cout << "Scanning memory...\n\n";

	VFTable2 = Format(0x1171244); // The ScriptContext address goes here

	ScriptContext = memory::Scan(PAGE_READWRITE, (char*)&VFTable2, "xxxx");
	rState = *(DWORD*)(ScriptContext + 56 * 1 + 164) - (ScriptContext + 56 * 1 + 164); // The Lua state goes here

	std::cout << "Welcome to Serenity, an exploit made by IcyJake!\n\n";
	std::cout << "To see the current commands, just type 'cmds'!\n\n\n\n";

	STD_INPUT_HANDLE;

	HANDLE ConsoleHandle;

	ConsoleHandle = GetStdHandle;
	ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	char Tmp[100];

	while (true)
	{
		std::cout << "->  ";
		std::cin >> Tmp;
		// std::cout << "Tmp: " << Tmp << " \n";

		if ((strcmp(Tmp, "cmds") == 0))
		{
			std::cout << "\nYou must use 'me' or the player's full name (case sensitive)!\n\n"
				"colorcmds -> Gives all the color commands you can use\n"
				"credits -> Shows the developers of Serenity\n"
				"clear -> Clears the console\n"
				"checkfe -> Prints boolean value of the FilteringEnabled property\n"
				"printidentity -> Prints Serenity's context level\n"
				"print -> print [t]\n"
				"warn -> warn [t]\n"
				"kill -> kill [p]\n"
				"god -> god [p]\n"
				"fire -> fire [p]\n"
				"sparkles -> sparkles [p]\n"
				"ff -> ff [p]\n"
				"smoke -> smoke [p]\n"
				"bighead -> bighead [p]\n"
				"longhead -> longhead [p]\n"
				"fastwalk -> fastwalk [p]\n"
				"highjump -> highjump [p]\n"
				"float -> float [p]\n"
				"ghost -> ghost [p]\n"
				"sit -> sit [p]\n"
				"nograv -> Sets the server's gravity to 0\n"
				"grav -> Sets the server's gravity to 196.2 (the default value)\n"
				"day -> Sets the server's time to be day\n"
				"night -> Sets the server's time to be night\n"
				"purge -> Plays a purge audio in the server\n"
				"moonman -> Plays a moon man audio in the server\n"
				"earrape -> Plays an ear raping audio in the server\n"
				"stop -> Stops all audios in the server from playing\n"
				"btools -> btools [p]\n\n";
		}

		if ((strcmp(Tmp, "colorcmds") == 0))
		{
			std::cout << "\nred\n"
				"green\n"
				"magenta\n"
				"blue\n"
				"yellow\n"
				"white\n"
				"default\n\n";
		}

		if ((strcmp(Tmp, "red") == 0))
		{
			SetConsoleTextAttribute(ConsoleHandle, 12);

			std::cout << "\nThe text color is now red!\n";
		}

		if ((strcmp(Tmp, "green") == 0))
		{
			SetConsoleTextAttribute(ConsoleHandle, 10);

			std::cout << "\nThe text color is now green!\n";
		}

		if ((strcmp(Tmp, "magenta") == 0))
		{
			SetConsoleTextAttribute(ConsoleHandle, 13);

			std::cout << "\nThe text color is now magenta!\n";
		}

		if ((strcmp(Tmp, "blue") == 0))
		{
			SetConsoleTextAttribute(ConsoleHandle, 9);

			std::cout << "\nThe text color is now blue!\n";
		}

		if ((strcmp(Tmp, "yellow") == 0))
		{
			SetConsoleTextAttribute(ConsoleHandle, 14);

			std::cout << "\nThe text color is now yellow!\n";
		}

		if ((strcmp(Tmp, "white") == 0))
		{
			SetConsoleTextAttribute(ConsoleHandle, 15);

			std::cout << "\nThe text color is now white!\n";
		}

		if ((strcmp(Tmp, "default") == 0))
		{
			system("color B");

			std::cout << "\nThe text color is now restored to default!\n";
		}

		if ((strcmp(Tmp, "clear") == 0))
		{
			system("cls");
		}

		if ((strcmp(Tmp, "checkfe") == 0))
		{
			getglobal(rState, "workspace");
			getfield(rState, -1, "FilteringEnabled");
			getglobal(rState, "print");
			pushvalue(rState, -2);
			pcall(rState, 1, 0, 0);
		}

		if ((strcmp(Tmp, "credits") == 0))
		{
			std::cout << "IcyJake - I developed Serenity, lol\n"
			"Qoazi - Good friend who taught me what was necessary and helped out when I needed it"
			"Eternal - Thanks for your return check bypass, noob";
		}

		if ((strcmp(Tmp, "printidentity") == 0))
		{
			getglobal(rState, "print");
			pushstring(rState, "Serenity has an identity of 6");
			pcall(rState, 1, 0, 0);
		}

		if ((strcmp(Tmp, "print") == 0))
		{
			std::string print = "";

			std::getline(std::cin, print);

			getglobal(rState, "print");
			pushstring(rState, print.c_str());
			pcall(rState, 1, 0, 0);
		}

		if ((strcmp(Tmp, "warn") == 0))
		{
			std::string warn = "";

			std::getline(std::cin, warn);

			getglobal(rState, "warn");
			pushstring(rState, warn.c_str());
			pcall(rState, 1, 0, 0);
		}

		if ((strcmp(Tmp, "nograv") == 0))
		{
			getglobal(rState, "workspace");
			pushnumber(rState, 0);
			setfield(rState, -2, "Gravity");
		}

		if ((strcmp(Tmp, "grav") == 0))
		{
			getglobal(rState, "workspace");
			pushnumber(rState, 196.2);
			setfield(rState, -2, "Gravity");
		}

		if ((strcmp(Tmp, "day") == 0))
		{
			getglobal(rState, "game");
			getfield(rState, -1, "Lighting");
			pushnumber(rState, 7);
			setfield(rState, -2, "TimeOfDay");
		}

		if ((strcmp(Tmp, "night") == 0))
		{
			getglobal(rState, "game");
			getfield(rState, -1, "Lighting");
			pushnumber(rState, 0);
			setfield(rState, -2, "TimeOfDay");
		}

		if ((strcmp(Tmp, "purge") == 0))
		{
			audio("151204442");
		}

		if ((strcmp(Tmp, "moonman") == 0))
		{
			audio("701552854");
		}

		if ((strcmp(Tmp, "earrape") == 0))
		{
			audio("533248462");
		}

		if ((strcmp(Tmp, "stop") == 0))
		{
			getglobal(rState, "game");
			getfield(rState, -1, "Workspace");
			getfield(rState, -1, "FindFirstChild");
			pushvalue(rState, -2);
			pushstring(rState, "POSITRON_AUDIO");
			pcall(rState, 2, 1, 0);
			getfield(rState, -1, "Destroy");
			pushvalue(rState, -2);
			pcall(rState, 1, 0, 0);
		}

		if ((strcmp(Tmp, "fire") == 0))
		{
			char Nome[100];

			std::cin >> Nome;

			std::string Name = Nome;

			if ((strcmp(Name.c_str(), "me") == 0))
			{
				std::cout << "\nSet fire to -> " << "LocalPlayer" << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, "LocalPlayer");
				getfield(rState, -1, "Character");
				getfield(rState, -1, "Torso");
				getglobal(rState, "Instance");
				getfield(rState, -1, "new");
				pushstring(rState, "Fire");
				pushvalue(rState, -4);
				pcall(rState, 2, 0, 0);
			}
			else
			{
				std::cout << "\nSet fire to -> " << Name.c_str() << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, Name.c_str());
				getfield(rState, -1, "Character");
				getfield(rState, -1, "Torso");
				getglobal(rState, "Instance");
				getfield(rState, -1, "new");
				pushstring(rState, "Fire");
				pushvalue(rState, -4);
				pcall(rState, 2, 0, 0);
			}
		}
		else if ((strcmp(Tmp, "kill") == 0))
		{
			char Nome[100];

			std::cin >> Nome;

			std::string Name = Nome;

			if ((strcmp(Name.c_str(), "me") == 0))
			{
				std::cout << "\nKilled -> " << "LocalPlayer" << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, "LocalPlayer");
				getfield(rState, -1, "Character");
				getfield(rState, -1, "Humanoid");
				pushnumber(rState, 0);
				setfield(rState, -2, "Health");
			}
			else
			{
				std::cout << "\nKilled -> " << Name.c_str() << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, Name.c_str());
				getfield(rState, -1, "Character");
				getfield(rState, -1, "Humanoid");
				pushnumber(rState, 0);
				setfield(rState, -2, "Health");
			}
		}
		else if ((strcmp(Tmp, "god") == 0))
		{
			char Nome[100];

			std::cin >> Nome;

			std::string Name = Nome;

			if ((strcmp(Name.c_str(), "me") == 0))
			{
				std::cout << "\nGodded -> " << "LocalPlayer" << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, "LocalPlayer");
				getfield(rState, -1, "Character");
				getfield(rState, -1, "Humanoid");
				pushnumber(rState, 99999999);
				setfield(rState, -2, "MaxHealth");

				Sleep(100);

				pushnumber(rState, 99999999);
				setfield(rState, -2, "Health");
			}
			else
			{
				std::cout << "\nGodded -> " << Name.c_str() << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, Name.c_str());
				getfield(rState, -1, "Character");
				getfield(rState, -1, "Humanoid");
				pushnumber(rState, 99999999);
				setfield(rState, -2, "MaxHealth");

				Sleep(1000);

				pushnumber(rState, 99999999);
				setfield(rState, -2, "Health");
			}
		}
		else if ((strcmp(Tmp, "fastwalk") == 0))
		{
			char Nome[100];

			std::cin >> Nome;

			std::string Name = Nome;

			if ((strcmp(Name.c_str(), "me") == 0))
			{
				std::cout << "\nSet walk-speed to 100 for -> " << "LocalPlayer" << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, "LocalPlayer");
				getfield(rState, -1, "Character");
				getfield(rState, -1, "Humanoid");
				pushnumber(rState, 100);
				setfield(rState, -2, "WalkSpeed");
			}
			else
			{
				std::cout << "\nSet walk-speed to 100 for -> " << Name.c_str() << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, Name.c_str());
				getfield(rState, -1, "Character");
				getfield(rState, -1, "Humanoid");
				pushnumber(rState, 100);
				setfield(rState, -2, "WalkSpeed");
			}
		}
		else if ((strcmp(Tmp, "highjump") == 0))
		{
			char Nome[100];

			std::cin >> Nome;

			std::string Name = Nome;

			if ((strcmp(Name.c_str(), "me") == 0))
			{
				std::cout << "\nSet jump-power to 100 for -> " << "LocalPlayer" << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, "LocalPlayer");
				getfield(rState, -1, "Character");
				getfield(rState, -1, "Humanoid");
				pushnumber(rState, 100);
				setfield(rState, -2, "JumpPower");
			}
			else
			{
				std::cout << "\nSet jump-power to 100 for -> " << Name.c_str() << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, Name.c_str());
				getfield(rState, -1, "Character");
				getfield(rState, -1, "Humanoid");
				pushnumber(rState, 100);
				setfield(rState, -2, "JumpPower");
			}
		}
		else if ((strcmp(Tmp, "float") == 0))
		{
			char Nome[100];

			std::cin >> Nome;

			std::string Name = Nome;

			if ((strcmp(Name.c_str(), "me") == 0))
			{
				std::cout << "\nFloated -> " << "LocalPlayer" << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, "LocalPlayer");
				getfield(rState, -1, "Character");
				getfield(rState, -1, "Torso");
				getglobal(rState, "Instance");
				getfield(rState, -1, "new");
				pushstring(rState, "BodyVelocity");
				pushvalue(rState, -4);
				pcall(rState, 2, 1, 0);
			}
			else
			{
				std::cout << "\nFloated -> " << Name.c_str() << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, Name.c_str());
				getfield(rState, -1, "Character");
				getfield(rState, -1, "Torso");
				getglobal(rState, "Instance");
				getfield(rState, -1, "new");
				pushstring(rState, "BodyVelocity");
				pushvalue(rState, -4);
				pcall(rState, 2, 1, 0);
			}
		}
		else if ((strcmp(Tmp, "sit") == 0))
		{
			char Nome[100];

			std::cin >> Nome;

			std::string Name = Nome;

			if ((strcmp(Name.c_str(), "me") == 0))
			{
				std::cout << "\nSat -> " << "LocalPlayer" << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, "LocalPlayer");
				getfield(rState, -1, "Character");
				getfield(rState, -1, "Humanoid");
				pushnumber(rState, 1);
				setfield(rState, -2, "Sit");
			}
			else
			{
				std::cout << "\nSat -> " << Name.c_str() << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, Name.c_str());
				getfield(rState, -1, "Character");
				getfield(rState, -1, "Humanoid");
				pushnumber(rState, 1);
				setfield(rState, -2, "Sit");
			}
		}
		else if ((strcmp(Tmp, "ghost") == 0))
		{
			char Nome[100];

			std::cin >> Nome;

			std::string Name = Nome;

			if ((strcmp(Name.c_str(), "me") == 0))
			{
				std::cout << "\nGhosted -> " << "LocalPlayer" << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, "LocalPlayer");
				getfield(rState, -1, "Character");
				getfield(rState, -1, "Head");
				pushnumber(rState, .5);
				setfield(rState, -2, "Transparency");
				getfield(rState, -2, "Torso");
				pushnumber(rState, .5);
				setfield(rState, -2, "Transparency");
				getfield(rState, -3, "Right Leg");
				pushnumber(rState, .5);
				setfield(rState, -2, "Transparency");
				getfield(rState, -4, "Left Leg");
				pushnumber(rState, .5);
				setfield(rState, -2, "Transparency");
				getfield(rState, -5, "Right Arm");
				pushnumber(rState, .5);
				setfield(rState, -2, "Transparency");
				getfield(rState, -6, "Left Arm");
				pushnumber(rState, .5);
				setfield(rState, -2, "Transparency");
			}
			else
			{
				std::cout << "\nGhosted -> " << Name.c_str() << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, Name.c_str());
				getfield(rState, -1, "Character");
				getfield(rState, -1, "Head");
				pushnumber(rState, .5);
				setfield(rState, -2, "Transparency");
				getfield(rState, -2, "Torso");
				pushnumber(rState, .5);
				setfield(rState, -2, "Transparency");
				getfield(rState, -3, "Right Leg");
				pushnumber(rState, .5);
				setfield(rState, -2, "Transparency");
				getfield(rState, -4, "Left Leg");
				pushnumber(rState, .5);
				setfield(rState, -2, "Transparency");
				getfield(rState, -5, "Right Arm");
				pushnumber(rState, .5);
				setfield(rState, -2, "Transparency");
				getfield(rState, -6, "Left Arm");
				pushnumber(rState, .5);
				setfield(rState, -2, "Transparency");
			}
		}
		else if ((strcmp(Tmp, "bighead") == 0))
		{
			char Nome[100];

			std::cin >> Nome;

			std::string Name = Nome;

			if ((strcmp(Name.c_str(), "me") == 0))
			{
				std::cout << "\nGave a big head to -> " << "LocalPlayer" << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, "LocalPlayer");
				getfield(rState, -1, "Character");
				getfield(rState, -1, "Head");
				getglobal(rState, "Vector3");
				getfield(rState, -1, "new");
				pushnumber(rState, 5);
				pushnumber(rState, 5);
				pushnumber(rState, 5);
				pushnumber(rState, 5);
				pushnumber(rState, 5);
				pushnumber(rState, 5);
				pcall(rState, 6, 1, 0);
				setfield(rState, -3, "Size");
			}
			else
			{
				std::cout << "\nGave a big head to -> " << Name.c_str() << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, Name.c_str());
				getfield(rState, -1, "Character");
				getfield(rState, -1, "Head");
				getglobal(rState, "Vector3");
				getfield(rState, -1, "new");
				pushnumber(rState, 5);
				pushnumber(rState, 5);
				pushnumber(rState, 5);
				pushnumber(rState, 5);
				pushnumber(rState, 5);
				pushnumber(rState, 5);
				pcall(rState, 6, 1, 0);
				setfield(rState, -3, "Size");
			}
		}
		else if ((strcmp(Tmp, "longhead") == 0))
		{
			char Nome[100];

			std::cin >> Nome;

			std::string Name = Nome;

			if ((strcmp(Name.c_str(), "me") == 0))
			{
				std::cout << "\nGave a long head to -> " << "LocalPlayer" << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, "LocalPlayer");
				getfield(rState, -1, "Character");
				getfield(rState, -1, "Head");
				getglobal(rState, "Vector3");
				getfield(rState, -1, "new");
				pushnumber(rState, 1);
				pushnumber(rState, 5);
				pushnumber(rState, 2);
				pushnumber(rState, 3);
				pushnumber(rState, 6);
				pushnumber(rState, 5);
				pcall(rState, 6, 1, 0);
				setfield(rState, -3, "Size");
			}
			else
			{
				std::cout << "\nGave a long head to -> " << Name.c_str() << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, Name.c_str());
				getfield(rState, -1, "Character");
				getfield(rState, -1, "Head");
				getglobal(rState, "Vector3");
				getfield(rState, -1, "new");
				pushnumber(rState, 1);
				pushnumber(rState, 5);
				pushnumber(rState, 2);
				pushnumber(rState, 3);
				pushnumber(rState, 6);
				pushnumber(rState, 5);
				pcall(rState, 6, 1, 0);
				setfield(rState, -3, "Size");
			}
		}
		else if ((strcmp(Tmp, "sparkles") == 0))
		{
			char Nome[100];

			std::cin >> Nome;

			std::string Name = Nome;

			if ((strcmp(Name.c_str(), "me") == 0))
			{
				std::cout << "\nGave sparkles to -> " << "LocalPlayer" << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "GetService");
				pushvalue(rState, -2);
				pushstring(rState, "Players");
				pcall(rState, 2, 1, 0);
				getfield(rState, -1, "LocalPlayer");
				getfield(rState, -1, "Character");
				getfield(rState, -1, "Torso");
				getglobal(rState, "Instance");
				getfield(rState, -1, "new");
				pushstring(rState, "Sparkles");
				pushvalue(rState, -4);
				pcall(rState, 2, 1, 0);
			}
			else
			{
				std::cout << "\nGave sparkles to -> " << Name.c_str() << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "GetService");
				pushvalue(rState, -2);
				pushstring(rState, "Players");
				pcall(rState, 2, 1, 0);
				getfield(rState, -1, Name.c_str());
				getfield(rState, -1, "Character");
				getfield(rState, -1, "Torso");
				getglobal(rState, "Instance");
				getfield(rState, -1, "new");
				pushstring(rState, "Sparkles");
				pushvalue(rState, -4);
				pcall(rState, 2, 1, 0);
			}
		}
		else if ((strcmp(Tmp, "ff") == 0))
		{
			char Nome[100];

			std::cin >> Nome;

			std::string Name = Nome;

			if ((strcmp(Name.c_str(), "me") == 0))
			{
				std::cout << "\nGave forcefield to -> " << "LocalPlayer" << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "GetService");
				pushvalue(rState, -2);
				pushstring(rState, "Players");
				pcall(rState, 2, 1, 0);
				getfield(rState, -1, "LocalPlayer");
				getfield(rState, -1, "Character");
				getglobal(rState, "Instance");
				getfield(rState, -1, "new");
				pushstring(rState, "ForceField");
				pushvalue(rState, -4);
				pcall(rState, 2, 1, 0);
			}
			else
			{
				std::cout << "\nGave forcefield to -> " << Name.c_str() << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "GetService");
				pushvalue(rState, -2);
				pushstring(rState, "Players");
				pcall(rState, 2, 1, 0);
				getfield(rState, -1, Name.c_str());
				getfield(rState, -1, "Character");
				getglobal(rState, "Instance");
				getfield(rState, -1, "new");
				pushstring(rState, "ForceField");
				pushvalue(rState, -4);
				pcall(rState, 2, 1, 0);
			}
		}
		else if ((strcmp(Tmp, "smoke") == 0))
		{
			char Nome[100];

			std::cin >> Nome;

			std::string Name = Nome;

			if ((strcmp(Name.c_str(), "me") == 0))
			{
				std::cout << "\nGave smoke to -> " << "LocalPlayer" << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "GetService");
				pushvalue(rState, -2);
				pushstring(rState, "Players");
				pcall(rState, 2, 1, 0);
				getfield(rState, -1, "LocalPlayer");
				getfield(rState, -1, "Character");
				getfield(rState, -1, "Torso");
				getglobal(rState, "Instance");
				getfield(rState, -1, "new");
				pushstring(rState, "Smoke");
				pushvalue(rState, -4);
				pcall(rState, 2, 1, 0);
			}
			else
			{
				std::cout << "\nGave smoke to -> " << Name.c_str() << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "GetService");
				pushvalue(rState, -2);
				pushstring(rState, "Players");
				pcall(rState, 2, 1, 0);
				getfield(rState, -1, Name.c_str());
				getfield(rState, -1, "Character");
				getfield(rState, -1, "Torso");
				getglobal(rState, "Instance");
				getfield(rState, -1, "new");
				pushstring(rState, "Smoke");
				pushvalue(rState, -4);
				pcall(rState, 2, 1, 0);
			}
		}
		if ((strcmp(Tmp, "btools") == 0))
		{
			char Nome[100];

			std::cin >> Nome;

			std::string Name = Nome;

			if ((strcmp(Name.c_str(), "me") == 0))
			{
				std::cout << "\nGave B-Tools to -> " << "LocalPlayer" << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, "LocalPlayer");
				getfield(rState, -1, "Backpack");
				getglobal(rState, "Instance");
				getfield(rState, -1, "new");
				pushstring(rState, "HopperBin");
				pushvalue(rState, -4);
				pcall(rState, 2, 1, 0);
				pushnumber(rState, 1);
				setfield(rState, -2, "BinType");
				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, "LocalPlayer");
				getfield(rState, -1, "Backpack");
				getglobal(rState, "Instance");
				getfield(rState, -1, "new");
				pushstring(rState, "HopperBin");
				pushvalue(rState, -4);
				pcall(rState, 2, 1, 0);
				pushnumber(rState, 2);
				setfield(rState, -2, "BinType");
				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, "LocalPlayer");
				getfield(rState, -1, "Backpack");
				getglobal(rState, "Instance");
				getfield(rState, -1, "new");
				pushstring(rState, "HopperBin");
				pushvalue(rState, -4);
				pcall(rState, 2, 1, 0);
				pushnumber(rState, 3);
				setfield(rState, -2, "BinType");
				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, "LocalPlayer");
				getfield(rState, -1, "Backpack");
				getglobal(rState, "Instance");
				getfield(rState, -1, "new");
				pushstring(rState, "HopperBin");
				pushvalue(rState, -4);
				pcall(rState, 2, 1, 0);
				pushnumber(rState, 4);
				setfield(rState, -2, "BinType");
			}
			else
			{
				std::cout << "\nGave B-Tools to -> " << Name.c_str() << " \n";

				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, Name.c_str());
				getfield(rState, -1, "Backpack");
				getglobal(rState, "Instance");
				getfield(rState, -1, "new");
				pushstring(rState, "HopperBin");
				pushvalue(rState, -4);
				pcall(rState, 2, 1, 0);
				pushnumber(rState, 1);
				setfield(rState, -2, "BinType");
				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, Name.c_str());
				getfield(rState, -1, "Backpack");
				getglobal(rState, "Instance");
				getfield(rState, -1, "new");
				pushstring(rState, "HopperBin");
				pushvalue(rState, -4);
				pcall(rState, 2, 1, 0);
				pushnumber(rState, 2);
				setfield(rState, -2, "BinType");
				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, Name.c_str());
				getfield(rState, -1, "Backpack");
				getglobal(rState, "Instance");
				getfield(rState, -1, "new");
				pushstring(rState, "HopperBin");
				pushvalue(rState, -4);
				pcall(rState, 2, 1, 0);
				pushnumber(rState, 3);
				setfield(rState, -2, "BinType");
				getglobal(rState, "game");
				getfield(rState, -1, "Players");
				getfield(rState, -1, Name.c_str());
				getfield(rState, -1, "Backpack");
				getglobal(rState, "Instance");
				getfield(rState, -1, "new");
				pushstring(rState, "HopperBin");
				pushvalue(rState, -4);
				pcall(rState, 2, 1, 0);
				pushnumber(rState, 4);
				setfield(rState, -2, "BinType");
			}
		}
	}
}

// The DllMain function is the entry point of a program and it's the first thing that runs when the program starts

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	// This is a switch statement that creates a thread only if the DLL is being injected and not if it's attached to a new thread and/or not injected

	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartFunction, NULL, 0, 0);
		break;

		case DLL_PROCESS_DETACH:
		break;

		case DLL_THREAD_ATTACH:
		break;

		case DLL_THREAD_DETACH:
		break;
	}

	// By returning true in the entry point, you are letting the injector know that it's done

	return TRUE;
}