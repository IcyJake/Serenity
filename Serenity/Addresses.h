/*
	Credits:
		- IcyJake (Developer)
		- Qoazi (Helper/good friend)
		- Brandon/Chirality (Serenity's first return check bypass)
		- Eternal (Serenity's current return check bypass)
*/

// Note: All addresses, all aobs/signatures, all calling conventions, and the Lua state is outdated in this source

#pragma once

#include "windows.h"
#include <vector>
#include <iostream>

HANDLE handle = GetModuleHandle(NULL);

int *baseAddress = (int*)handle;
int bossbase = (int)baseAddress;

DWORD unprotect(DWORD address)
{
	BYTE * tAddr = (BYTE*)address;

	do
	{
		tAddr += 0x10;
	} while (!(tAddr[0] == 0x55 && tAddr[1] == 0x8B && tAddr[2] == 0xEC));

	DWORD funcSz = tAddr - (BYTE*)address;

	PVOID nFunc = VirtualAlloc(NULL, funcSz, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	if (nFunc == NULL)
	{
		return address;
	}

	memcpy(nFunc, (BYTE*)address, funcSz);

	DWORD pos = (DWORD)nFunc;

	BOOL valid = false;

	do
	{
		if (*(BYTE*)pos == 0x72 && * (BYTE*)(pos + 0x2) == 0xA1 && (*(BYTE*)(pos + 0x7)) == 0x8B)
		{
			memcpy((void*)pos, "\xEB", 1);

			DWORD cNFunc = (DWORD)nFunc;

			do
			{
				if (*(BYTE*)cNFunc == 0xE8)
				{
					DWORD tFunc = address + (cNFunc - (DWORD)nFunc);
					DWORD oFunc = (tFunc + *(DWORD*)(tFunc + 1)) + 5;

					if (oFunc % 16 == 0)
					{
						DWORD rebFAddr = oFunc - cNFunc - 5;
						*(DWORD*)(cNFunc + 1) = rebFAddr;
					}
					cNFunc += 5;
				}
				else
				{
					cNFunc += 1;
				}
			} while (cNFunc - (DWORD)nFunc < funcSz);
			valid = true;
		}
		pos += 1;
	} while (pos < (DWORD)nFunc + funcSz);

	if (!valid)
	{
		VirtualFree(nFunc, funcSz, MEM_RELEASE);

		return address;
	}
	return (DWORD)nFunc;
}

DWORD base = (DWORD)GetModuleHandle("RobloxPlayerBeta.exe");

DWORD Format(int address)
{
	return (address - 0x400000 + base);
}

namespace memory
{
	bool Compare(const BYTE * pData, const BYTE * bMask, const char * szMask)
	{
		for (;*szMask; ++szMask, ++pData, ++bMask)
		{
			if (*szMask == 'x' && * pData != *bMask)
			{
				return 0;
			}
		}
		return (*szMask) == NULL;
	}

	DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE * bMask, char * szMask)
	{
		for (int i = 0; i < (int)dwLen; i++)
		{
			if (Compare((BYTE*)(dwAddress + (int)i), bMask, szMask))
			{
				return (int)(dwAddress + i);
			}
		}
		return 0;
	}

	int Scan(DWORD mode, char * content, char * mask)
	{
		SYSTEM_INFO si;
		
		GetSystemInfo(&si);

		MEMORY_BASIC_INFORMATION mi;
		SYSTEM_INFO SystemInfo;

		GetSystemInfo(&SystemInfo);

		DWORD PageSize = SystemInfo.dwPageSize;
		MEMORY_BASIC_INFORMATION meminfo;
		DWORD Start = (DWORD)SystemInfo.lpMinimumApplicationAddress;
		DWORD End = (DWORD)SystemInfo.lpMaximumApplicationAddress;

		for (DWORD lpAddress = Start; lpAddress <= End; lpAddress += PageSize)
		{
			VirtualQuery((LPCVOID*)lpAddress, &meminfo, sizeof(MEMORY_BASIC_INFORMATION));

			if (meminfo.Type == MEM_MAPPED)
			{
				continue;
			}

			if (meminfo.Protect == PAGE_READWRITE)
			{
				DWORD Address = memory::FindPattern(lpAddress, PageSize, (BYTE *)content, "xxxx");

				if (Address != NULL)
				{
					return Address;
				}
			}
		}
	}
}

DWORD ScanAltBit(char* content)
{
	SYSTEM_INFO SystemInfo;

	GetSystemInfo(&SystemInfo);

	DWORD PageSize = SystemInfo.dwPageSize;
	MEMORY_BASIC_INFORMATION meminfo;
	DWORD Start = (DWORD)SystemInfo.lpMinimumApplicationAddress;
	DWORD End = (DWORD)SystemInfo.lpMaximumApplicationAddress;

	for (DWORD lpAddress = Start; lpAddress <= End; lpAddress += PageSize)
	{
		VirtualQuery((LPCVOID*)lpAddress, &meminfo, sizeof(MEMORY_BASIC_INFORMATION));

		if (meminfo.Type == MEM_MAPPED)
		{
			continue;
		}

		if (meminfo.Protect == PAGE_READWRITE)
		{
			DWORD Address = memory::FindPattern(lpAddress, PageSize, (BYTE*)content, "xxxx");

			if (Address != NULL)
			{
				return Address;
			}
		}
	}
}

DWORD sehChainPcall = Format(0x7244E7);
// 74 97 A1 ? ? ? ?

typedef void*(__fastcall *Lua_getfield)(int State, int idx, const char *k);
Lua_getfield lua_getfield = (Lua_getfield)unprotect(Format(0x72B380));
// 55 8B EC 83 EC 10 53 56 57 8B F1 52 56 E8 ? ? ? ? 8B 4D 08 83 C4 08 8B D1 8B F8 8D 5A 01 90 8A 02 42 84 C0 75 F9 2B D3 52 51 56 E8 ? ? ? ? 6A 01

typedef void*(__cdecl *Lua_pushstring)(int State, const char *s);
Lua_pushstring lua_pushstring = (Lua_pushstring)unprotect(Format(0x72C740));
// 55 8B EC 8B 55 0C 85 D2 75 0D

typedef void*(__fastcall *Lua_pushvalue)(int State, int idx);
Lua_pushvalue lua_pushvalue = (Lua_pushvalue)unprotect(Format(0x72C810));
// 55 8B EC 56 57 8B F9 52

typedef int(__cdecl *pCall)(int State, int naArgs, int nresults, int errfunc);
pCall lua_pcall = (pCall)unprotect(Format(0x72C170));
// 55 8B EC 8B 45 14 83 EC 08 57

typedef void*(__fastcall *Lua_Setfield)(int State, int idx, const char *k);
Lua_Setfield lua_setfield = (Lua_Setfield)unprotect(Format(0x72D150));
// 55 8B EC 83 EC 10 53 56 57 8B F1 52 56 E8 ? ? ? ? 8B 4D 08 83 C4 08 8B D1 8B F8 8D 5A 01 90 8A 02 42 84 C0 75 F9 2B D3 52 51 56 E8 ? ? ? ? 89 45 F0

typedef void(__thiscall *Lua_pushnumber)(int State, double number);
Lua_pushnumber lua_pushnumber = (Lua_pushnumber)unprotect(Format(0x72C6B0));
// 55 8B EC 8B 41 18 0F 28 15 ? ? ? ? 

typedef int*(__fastcall *Lua_getmetatable)(int State, int index);
Lua_getmetatable lua_getmetatable = (Lua_getmetatable)unprotect(Format(0x72B650));
// 55 8B EC 56 8B F1 52

void set_jnz(unsigned int address)
{
	DWORD o_buff;

	VirtualProtect((void*)address, 5, PAGE_EXECUTE_READWRITE, &o_buff);

	*(char*)address = 0x75; // The opcode for jnz

	VirtualProtect((void*)address, 5, o_buff, &o_buff);
}

void set_jz(unsigned int address)
{
	DWORD o_buff;

	VirtualProtect((void*)address, 5, PAGE_EXECUTE_READWRITE, &o_buff);

	*(char*)address = 0x74; // The opcode for jz

	VirtualProtect((void*)address, 5, o_buff, &o_buff);
}

void pushboolean(int state, bool boolean)
{
	int top = *(DWORD*)(state + 32);

	*(DWORD*)top = boolean != 0;
	*(DWORD*)(top + 8) = 3;
	*(DWORD*)(state + 32) += 16;
}

void getfield(int state, int index, const char *k)
{
	lua_getfield(state, index, k);
}

void pushstring(int state, const char *s)
{
	lua_pushstring(state, s);
}

void pushvalue(int state, int index)
{
	lua_pushvalue(state, index);
}

typedef DWORD(__cdecl *toboolean)(DWORD state, DWORD index);

void setJbb(DWORD address)
{
	DWORD oldProtection;

	VirtualProtect((LPVOID)address, 0x05, PAGE_EXECUTE_READWRITE, &oldProtection);

	*(char*)address = 0xEB;

	VirtualProtect((LPVOID)address, 0x05, oldProtection, &oldProtection);
}

void restoreJbb(DWORD address)
{
	DWORD oldProtection;

	VirtualProtect((void*)address, 5, PAGE_EXECUTE_READWRITE, &oldProtection);

	*(char*)address = 0x72;

	VirtualProtect((void*)address, 5, oldProtection, &oldProtection);
}

int pcall(int state, int nargs, int nresults, int errfunc)
{
	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(sehChainPcall), "\xEB", 1, 0);

	lua_pcall(state, nargs, nresults, errfunc);

	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(sehChainPcall), "\x74", 1, 0);

	return 0;
}

#define lua_pop(L, n) settop(L, - (n) - 1)

void pushnumber(int state, double number)
{
	lua_pushnumber(state, number);
}

void getglobal(int state, const char *k)
{
	return getfield(state, -10002, k);
}

void settop(int a1, int a2)
{
	int i;

	if (a2 < 0)
	{
		*(DWORD *)(a1 + 32) += 16 * a2 + 16;
	}
	else
	{
		for (i = 16 * a2;*(DWORD *)(a1 + 32) < (unsigned int)(i + *(DWORD *)(a1 + 8));*(DWORD *)(a1 + 32) += 16)
			*(DWORD *)(*(DWORD *)(a1 + 32) + 8) = 0;
		*(DWORD *)(a1 + 32) = i + *(DWORD *)(a1 + 8);
	}
}

void setfield(int state, int index, const char *k)
{
	lua_pushvalue(state, index);

	if (lua_getmetatable(state, -1))
	{
		lua_getfield(state, -1, "__newindex");
		lua_pushvalue(state, -3);
		lua_pushstring(state, k);
		lua_pushvalue(state, -6);
		lua_pcall(state, 3, 1, 0);
		lua_pop(state, 3);
	}
	else
	{
		lua_pop(state, 1);
		lua_setfield(state, index, k);
	}
}