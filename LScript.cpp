/******************************************************************************
*
* LScript Introduction:
* LScript is a tiny scripting engine for the 2.6 version of the AAO game.  
* LScript connects a Lua scripting engine up to the 2.6 game engine.  You 
* can learn more about the Lua scripting language here:
* http://www.lua.org
*
* There is a LScript.txt file that describes how to use this code from Lua.
*
* How to build LScript:
*   1. Download the Lua 5.1 source code from the Lua site.
*   2. Copy all the files from lua-5.1/src directory into your build directory.
*   3. Delete the lua.c and luac.c files.
*   4. Copy this file into the build directory.
*   5. Create a project that includes all the files in the build directory.
*   6. Build it.
*
* Picklelicious	1  Apr 2006	Original Version - April fools day
* Temp2			5  Nov 2006 Converted for AA 2.7.0 - Fireworks night
* Temp2			6  Nov 2006 Ported to UT3369 - Melbourne Cup
* SmokeZ		13 Nov 2006 Added Some Extra Features + Fixed Common Compiler Errors
* Temp2			13 Nov 2006 Changed some stuff for VS 2005
* Temp2			15 Nov 2006 New hooks
* Temp2			24 Dec 2006 Converted for AA 2.8.0 - Christmas Eve
* Temp2			4  Apr 2007 Converted for AA 2.8.1 - Easter Parade.
******************************************************************************/

#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_NONSTDC_NO_DEPRECATE 1
//#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1

/* 
	USER CONFIG
*/
	
#define DEFAULT_BOTFILE		"default.lua"
#define DEFAULT_LOGFILE		"default.log"

#define BASEKEY			VK_MENU		//,VK_SHIFT
#define RESTART_KEY		'r'		//BASEKEY + <your_key>
#define OPEN_LOG_KEY		'l'		//BASEKEY + <your_key>
#define OPEN_LUA_CONSOLE	'c'		//BASEKEY + <your_key>

/*
	USER CONFIG END
*/

#include <windows.h>
#include <fcntl.h>
#include <io.h>
#include <process.h>
#include <time.h>

extern "C" 
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#define EngineUFunction "Engine.UFunction"
#define EngineUObject   "Engine.UObject"
#define EngineUProperty "Engine.UProperty"
#define MaxFullNameSize 512
#define MaxNameSize     128
#define NoIndex         0x7FFFFFFF


/******************************************************************************
*
* LogFile
*
******************************************************************************/
FILE* hLogFile;
char szLogFile[MAX_PATH];
bool bLogActive = false;

void Log(char * pFormat, ...)
{
	va_list     ArgList;
	char        Line[32];
	time_t      Time;
	struct tm * TM;

	if(hLogFile && bLogActive)
	{
		Time = time(0);
		TM = localtime(&Time);
		strftime(Line, sizeof(Line), "%H:%M:%S ", TM);
		fwrite(Line, strlen(Line), 1, hLogFile);

		va_start(ArgList, pFormat);
		vfprintf(hLogFile, pFormat, ArgList);
		va_end(ArgList);

		fprintf(hLogFile, "\n");
		fflush(hLogFile);
	}
}


/******************************************************************************
*
* Game Engine
*
******************************************************************************/
template<class Type> struct TArray
{
	Type * pArray;
	DWORD  Count;
	DWORD  Max;
};


typedef DWORD FName;


struct FNameEntry
{
	int   FNameIndex;
	DWORD Unknown0x004;
	DWORD Unknown0x008;
	WCHAR Name[1];
};


struct UClass;
struct UObject
{
	DWORD **     pVMT;				//0x0000
	DWORD        ObjectInternal;			//0x0004
	UObject *    pHash;				//0x0008
	DWORD *      pFrame;				//0x000C
	DWORD *      pLinkerLoad;			//0x0010
	int          PackageNumber;			//0x0014
	DWORD        Unknown0x0018;			//0x0018
	UObject *    pOuter;				//0x001C
	DWORD        ObjectFlags;			//0x0020
	FName        Name;				//0x0024
	UClass *     pClass;				//0x0028

	FName        LastStateName;			//0x002C
	FName        LastLabelName;			//0x0030
	unsigned int bDisableStateChange : 1;		//0x0034


	void GetCPPName(char * pBuffer);
	void GetFullName(char * pBuffer);
	void GetName(char * pBuffer);
	int  IsA(UClass * pClass);
};


struct UField : UObject 
{
	UField *           pSuper;			//0x0038
	UField *           pNext;			//0x003C
	DWORD              Unknown0x003C;		//0x0040
};

struct ULevel : UObject
{
	char               Unknown0x0034;		//0x0038
	TArray<UObject *>  ActorArray;			//0x003C
};

struct UProperty : UField 
{
	DWORD              ElementCount;		//0x0044
	DWORD              ElementSize;			//0x0048
	DWORD              Flags;			//0x004C
	WORD               ReplicationOffset;		//0x0050
	WORD               Unknown0x0050;		//0x0052
	WORD               Unknown0x0052;		//0x0054
	DWORD              CStructOffset;		//0x0058
	char               Unknown0x005C[0x20];
	UClass *           pRelatedClass;		//0x007C
};

struct UStruct : UField 
{
	DWORD              Unknown0x0040;		//0x0044
	DWORD              Unknown0x0044;		//0x0048
	UField *           pChildren;			//0x004C
	DWORD              Size;			//0x0050
	DWORD              Unknown0x0050;		//0x0054
	TArray<BYTE>       Script;			//0x0058
	char               Unknown0x0060[0x02C];
};


struct UFunction : UStruct 
{
	DWORD              Flags;			//0x0090
	WORD               NativeIndex;			//0x0094
	WORD               ReplicationOffset;		//0x0096
	DWORD              Unknown0x094;		//0x0098
	DWORD              Unknown0x098;		//0x009C
	DWORD              pExecFunction;		//0x00A0
};


struct UState : UStruct 
{
};


struct UClass : UState 
{
};


struct AActor : UObject
{
	char               Unknown0x0034[0x104];
	ULevel *           XLevel;			//0x0134 013c
};


struct UViewport : UObject
{
	char               Unknown0x0034[0x008];
	AActor *           Actor;			//0x003C
};


TArray<UObject *> *    pAActorArray;
TArray<FNameEntry *> * pFNameEntryArray;
TArray<UObject *> *    pUObjectArray;
UObject **             pUObjectHash;

UViewport *            pViewport;

UClass *               pActorClass;
UClass *               pClassClass;
UClass *               pFunctionClass;
UClass *               pStructClass;

UClass *               pArrayPropertyClass;
UClass *               pBoolPropertyClass;
UClass *               pBytePropertyClass;
UClass *               pClassPropertyClass;
UClass *               pDelegatePropertyClass;
UClass *               pFixedArrayPropertyClass;
UClass *               pFloatPropertyClass;
UClass *               pIntPropertyClass;
UClass *               pMapPropertyClass;
UClass *               pNamePropertyClass;
UClass *               pObjectPropertyClass;
UClass *               pPointerPropertyClass;
UClass *               pPropertyClass;
UClass *               pStructPropertyClass;
UClass *               pStrPropertyClass;

void (__stdcall *      pFString_ConstructPChar)(char *);
void (__stdcall *      pFString_Destruct)(void);


UObject * FindActorByFullName(const char * pFullName) 
{
	char       Name[MaxFullNameSize];
	int        ObjectCount;
	UObject *  pObject;
	UObject ** ppObject;


	ObjectCount = pAActorArray->Count;
	ppObject = (UObject **)pAActorArray->pArray;
	while (ObjectCount)
	{
		pObject = *ppObject;
		if (pObject)
		{
			pObject->GetFullName(Name);
			if (!strcmp(Name, pFullName))
				return(pObject);
		}

		++ppObject;
		--ObjectCount;
	}

	return(0);
}


UObject * FindObjectByFullName(const char * pFullName) 
{
	char       Name[MaxFullNameSize];
	int        ObjectCount;
	UObject *  pObject;
	UObject ** ppObject;


	ObjectCount = pUObjectArray->Count;
	ppObject = (UObject **)pUObjectArray->pArray;
	while (ObjectCount)
	{
		pObject = *ppObject;
		if (pObject)
		{
			pObject->GetFullName(Name);
			if (!strcmp(Name, pFullName))
				return(pObject);
		}

		++ppObject;
		--ObjectCount;
	}

	return(0);
}


WCHAR * GetFName(FName Name)
{
	if ((Name >= pFNameEntryArray->Count) || !pFNameEntryArray->pArray[Name])
		return(L"[empty]");

	return(pFNameEntryArray->pArray[Name]->Name);
}


void UObject::GetCPPName(char * pBuffer)
{
	if (pClass == pStructClass)
	{
		*pBuffer = 'F';
	}
	else
	{
		UClass * pNextClass;

		*pBuffer = 'U';

		if (pClass == pClassClass)
		        pNextClass = (UClass *)this;
		else
			pNextClass = this->pClass;
		while (pNextClass)
		{
			if (pNextClass == pActorClass)
			{
				*pBuffer = 'A';
				break;
			}

			if (pNextClass == pNextClass->pSuper)
				break;

			pNextClass = (UClass *)pNextClass->pSuper;
		}
	}

	sprintf(&pBuffer[1], "%S", GetFName(Name));
}


void AddPath(UObject * pObject, char * pBuffer)
{
	if (pObject->pOuter)
		AddPath(pObject->pOuter, pBuffer);

	sprintf(pBuffer, "%S.", GetFName(pObject->Name));
}


void UObject::GetFullName(char * pBuffer)
{
	sprintf(pBuffer, "%S ", GetFName(this->pClass->Name));
	if (this->pOuter)
		AddPath(this->pOuter, &pBuffer[strlen(pBuffer)]);
	sprintf(&pBuffer[strlen(pBuffer)], "%S", GetFName(this->Name));
}


void UObject::GetName(char * pBuffer)
{
	sprintf(pBuffer, "%S", GetFName(Name));
}


int UObject::IsA(UClass * pClass)
{
	UClass * pNextClass;


	pNextClass = this->pClass;
	while (pNextClass)
	{
		if (pNextClass == pClass)
			return(1);

		if (pNextClass == pNextClass->pSuper)
			return(0);

		pNextClass = (UClass *)pNextClass->pSuper;
	}

	return(0);
}


void AppendType(char * pString, UProperty * pProperty)
{
	if (pProperty->IsA(pArrayPropertyClass))
	{
		strcat(pString, "TArray<");
		AppendType(pString, (UProperty *)pProperty->pRelatedClass);
		strcat(pString, ">");
	}

	else if (pProperty->IsA(pBoolPropertyClass))
	{
		strcat(pString, "bool");
	}

	else if (pProperty->IsA(pBytePropertyClass))
	{
		strcat(pString, "byte");
	}

	else if (pProperty->IsA(pClassPropertyClass))
	{
		strcat(pString, "UClass *");
	}

	else if (pProperty->IsA(pDelegatePropertyClass))
	{
		strcat(pString, "delegate");
	}

	else if (pProperty->IsA(pFixedArrayPropertyClass))
	{
		strcat(pString, "fixed");
	}

	else if (pProperty->IsA(pFloatPropertyClass))
	{
		strcat(pString, "float");
	}

	else if (pProperty->IsA(pIntPropertyClass))
	{
		strcat(pString, "int");
	}

	else if (pProperty->IsA(pMapPropertyClass))
	{
		strcat(pString, "map");
	}

	else if (pProperty->IsA(pNamePropertyClass))
	{
		strcat(pString, "FName");
	}

	else if (pProperty->IsA(pObjectPropertyClass))
	{
		pProperty->pRelatedClass->GetCPPName(&pString[strlen(pString)]);
		strcat(pString, " *");
	}

	else if (pProperty->IsA(pPointerPropertyClass))
	{
		strcat(pString, "void *");
	}

	else if (pProperty->IsA(pStrPropertyClass))
	{
		strcat(pString, "FString");
	}

	else if (pProperty->IsA(pStructPropertyClass))
	{
		pProperty->pRelatedClass->GetCPPName(&pString[strlen(pString)]);
	}

	else
	{
		strcat(pString, "[unknown]");
	}
}


void AppendValue(char * pString, UProperty * pProperty, char * pData, int Index)
{
	if (pProperty->IsA(pArrayPropertyClass))
	{
		TArray<void> * pValue;

		pValue = (TArray<void> *)(pData + pProperty->CStructOffset);
		if (!pValue->Count || !pValue->pArray)
		{
			sprintf(&pString[strlen(pString)], "[%d][%d][nil]", pValue->Count, pValue->Max);
		}
		else
		{
			sprintf(&pString[strlen(pString)], "[%d][%d]", pValue->Count, pValue->Max);
			AppendValue(pString, (UProperty *)pProperty->pRelatedClass, (char *)pValue->pArray, 0);
		}
	}

	else if (pProperty->IsA(pBoolPropertyClass))
	{
		int Value;

		Value = *(int *)(pData + pProperty->CStructOffset +
			(pProperty->ElementSize * Index));
		Value &= (DWORD)pProperty->pRelatedClass;
		if (Value)
			Value = 1;
		sprintf(&pString[strlen(pString)], "[%d]", Value);
	}

	else if (pProperty->IsA(pBytePropertyClass))
	{
		int Value;

		Value = *(BYTE *)(pData + pProperty->CStructOffset +
			(pProperty->ElementSize * Index));
		sprintf(&pString[strlen(pString)], "[%d]", Value);
	}

	else if (pProperty->IsA(pClassPropertyClass))
	{
		int Value;

		Value = *(int *)(pData + pProperty->CStructOffset +
			(pProperty->ElementSize * Index));
		sprintf(&pString[strlen(pString)], "[0x08%X]", Value);
	}

	else if (pProperty->IsA(pFloatPropertyClass))
	{
		float Value;

		Value = *(float *)(pData + pProperty->CStructOffset +
			(pProperty->ElementSize * Index));
		sprintf(&pString[strlen(pString)], "[%0.3f]", Value);
	}

	else if (pProperty->IsA(pIntPropertyClass))
	{
		int Value;

		Value = *(int *)(pData + pProperty->CStructOffset +
			(pProperty->ElementSize * Index));
		sprintf(&pString[strlen(pString)], "[%d]", Value);
	}

	else if (pProperty->IsA(pNamePropertyClass))
	{
		FName Value;

		Value = *(FName *)(pData + pProperty->CStructOffset +
			(pProperty->ElementSize * Index));
		sprintf(&pString[strlen(pString)], "[%S]", GetFName(Value));
	}

	else if (pProperty->IsA(pObjectPropertyClass))
	{
		UObject * pValue;

		pValue = *(UObject **)(pData + pProperty->CStructOffset +
			(pProperty->ElementSize * Index));
		if (pValue)
			sprintf(&pString[strlen(pString)], "[%S]", GetFName(pValue->Name));
		else
			strcat(pString, "[nil]");
	}

	else if (pProperty->IsA(pPointerPropertyClass))
	{
		int Value;

		Value = *(int *)(pData + pProperty->CStructOffset +
			(pProperty->ElementSize * Index));
		sprintf(&pString[strlen(pString)], "[0x08%X]", Value);
	}

	else if (pProperty->IsA(pStrPropertyClass))
	{
		TArray<WCHAR> * Value;

		Value = (TArray<WCHAR> *)(pData + pProperty->CStructOffset +
			(pProperty->ElementSize * Index));
		if (Value->pArray)
			sprintf(&pString[strlen(pString)], "[%S]", Value->pArray);
		else
			strcat(pString, "[nil]");
	}

	else if (pProperty->IsA(pStructPropertyClass))
	{
		UProperty * pSubProp;

		pSubProp = (UProperty *)pProperty->pRelatedClass->pChildren;
		while (pSubProp)
		{
			if (pSubProp->IsA(pPropertyClass))
			{
				pSubProp->GetName(&pString[strlen(pString)]);
				AppendValue(pString, pSubProp, pData + pProperty->CStructOffset +
					(pProperty->ElementSize * Index), 0);
				strcat(pString, " ");
			}

			pSubProp = (UProperty *)pSubProp->pNext;
		}
	}

	else if (pProperty->IsA(pDelegatePropertyClass))
	{
		UObject * pValue;

		pValue = *(UObject **)(pData + pProperty->CStructOffset +
			(pProperty->ElementSize * Index));
		if (pValue)
			sprintf(&pString[strlen(pString)], "[%S]", GetFName(pValue->Name));
		else
			strcat(pString, "[nil]");
	}

	//pDelegatePropertyClass
	//pFixedArrayPropertyClass
	//pMapPropertyClass

}


void FStringConstructor(void * pThis, const char * pString)
{
	__asm mov  eax,pString;
	__asm push eax
		__asm mov  ecx,pThis;
	__asm call pFString_ConstructPChar;
}

void FStringDestructor(void * pThis) 
{
	__asm mov  ecx,pThis;
	__asm call pFString_Destruct;
}

void EngineInit(void)
{
	if (!pUObjectArray)
	{
		HMODULE hCore;
		hCore = GetModuleHandle("core.dll");

		pFNameEntryArray = (TArray<FNameEntry *> *)GetProcAddress(hCore, "?Names@FName@@0V?$TArray@PAUFNameEntry@@@@A");
		pFString_ConstructPChar = (void (__stdcall *)(char *))GetProcAddress(hCore, "??0FString@@QAE@PBD@Z");
		pFString_Destruct = (void (__stdcall *)(void))GetProcAddress(hCore, "??1FString@@QAE@XZ");
		pUObjectArray = (TArray<UObject *> *)GetProcAddress(hCore, "?GObjObjects@UObject@@0V?$TArray@PAVUObject@@@@A");
		pUObjectHash = (UObject **)GetProcAddress(hCore, "?GObjHash@UObject@@0PAPAV1@A");
		pViewport = (UViewport *)FindObjectByFullName("WindowsViewport WindowsClient.WindowsViewport");

		pActorClass    = (UClass *)FindObjectByFullName("Class Engine.Actor");
		pClassClass    = (UClass *)FindObjectByFullName("Class Core.Class");
		pFunctionClass = (UClass *)FindObjectByFullName("Class Core.Function");
		pStructClass   = (UClass *)FindObjectByFullName("Class Core.Struct");

		pArrayPropertyClass      = (UClass *)FindObjectByFullName("Class Core.ArrayProperty");
		pBoolPropertyClass       = (UClass *)FindObjectByFullName("Class Core.BoolProperty");
		pBytePropertyClass       = (UClass *)FindObjectByFullName("Class Core.ByteProperty");
		pClassPropertyClass      = (UClass *)FindObjectByFullName("Class Core.ClassProperty");
		pDelegatePropertyClass   = (UClass *)FindObjectByFullName("Class Core.DelegateProperty");
		pFixedArrayPropertyClass = (UClass *)FindObjectByFullName("Class Core.FixedArrayProperty");
		pFloatPropertyClass      = (UClass *)FindObjectByFullName("Class Core.FloatProperty");
		pIntPropertyClass        = (UClass *)FindObjectByFullName("Class Core.IntProperty");
		pMapPropertyClass        = (UClass *)FindObjectByFullName("Class Core.MapProperty");
		pNamePropertyClass       = (UClass *)FindObjectByFullName("Class Core.NameProperty");
		pObjectPropertyClass     = (UClass *)FindObjectByFullName("Class Core.ObjectProperty");
		pPointerPropertyClass    = (UClass *)FindObjectByFullName("Class Core.PointerProperty");
		pPropertyClass           = (UClass *)FindObjectByFullName("Class Core.Property");
		pStructPropertyClass     = (UClass *)FindObjectByFullName("Class Core.StructProperty");
		pStrPropertyClass        = (UClass *)FindObjectByFullName("Class Core.StrProperty");
	}
}


/******************************************************************************
*
* Lua Engine Functions
*
******************************************************************************/
struct UFunctionData
{
	UObject *   pObject;
	UFunction * pFunction;
};

struct UObjectData
{
	UObject *   pObject;
	UClass *    pClass;
	char *      pData;
	DWORD       ActorIndex;
};

struct UObjectProp
{
	UObject *   pObject;
	UProperty * pProperty;
	char *      pData;
};


HANDLE    hConsole;
HANDLE    hInputEvent;
char      Input[MaxFullNameSize];
char      Path[MAX_PATH];
bool      RestartLua;
HANDLE    StdErr;
HANDLE    StdIn;
HANDLE    StdOut;


int CloseConsole(lua_State * /*L*/) 
{
	if (hConsole)
	{
		SetStdHandle(STD_INPUT_HANDLE, StdIn);
		SetStdHandle(STD_OUTPUT_HANDLE, StdOut);
		SetStdHandle(STD_ERROR_HANDLE, StdErr);

		CloseHandle(hConsole);
		FreeConsole();

		hConsole = 0;
		SetEvent(hInputEvent);
	}

	return(0);
}

BOOL WINAPI ConsoleHandlerRoutine(DWORD	/*dwCtrlType*/)
{
	FreeConsole();
	return(1);
}

void DumpProperty(UProperty * pProperty, char * pData, char * pName)
{
	int Len;

	if (pProperty->IsA(pPropertyClass))
	{
		AppendType(pName, pProperty);

		strcat(pName, " ");
		Len = strlen(pName);
		if (Len < 40)
		{
			memset(&pName[Len], ' ', 40 - Len);
			pName[40] = 0;
		}

		pProperty->GetName(&pName[strlen(pName)]);

		if (pProperty->ElementCount > 1)
		{
			sprintf(&pName[strlen(pName)], "[%d]", 
				pProperty->ElementCount);
		}

		strcat(pName, " ");
		Len = strlen(pName);
		if (Len < 80)
		{
			memset(&pName[Len], ' ', 80 - Len);
			pName[80] = 0;
		}

		AppendValue(pName, pProperty, pData, 0);
	}
}

int Dump(lua_State * L) 
{
	int           Found;
	DWORD         Index;
	char          Name[MaxFullNameSize*8];
	char *        pData;
	UClass *      pClass;
	UObject *     pObject;
	UObjectData * pObjectData;
	UObjectProp * pObjectProp;
	UProperty *   pProperty;

	//Plain data type.
	if (!lua_touserdata(L, 1))
	{
		pData = Name;
		if (lua_isnoneornil(L, 1))
			pData = "[nil]";
		else if (lua_isnumber(L, 1))
			sprintf(pData, "[%0.3f]", (float)lua_tonumber(L, 1));
		else
			sprintf(pData, "[%s]", lua_tostring(L, 1));

		fputs("\n", stdout);
		fputs(pData, stdout);
		fputs("\n\n", stdout);
		Log("Dump: %s", pData);
	}

	//Check for UObject
	lua_getfield(L, LUA_REGISTRYINDEX, EngineUObject);
	lua_getmetatable(L, 1);
	Found = lua_rawequal(L, -1, -2);
	lua_pop(L, 2);
	if (Found)
	{
		pObjectData = (UObjectData *)luaL_checkudata(L, 1, EngineUObject);
		pObject   = pObjectData->pObject;
		pData     = pObjectData->pData;
		pClass    = pObjectData->pClass;

		if ((char *)pObject == pData)
		{
			pObject->GetFullName(Name);
		}
		else
		{
			pClass->GetFullName(Name);
			if (pObject)
			{
				strcat(Name, " part of [");
				pObject->GetFullName(&Name[strlen(Name)]);
				strcat(Name, "]");
			}
		}

		fputs("\n", stdout);
		fputs(Name, stdout);
		fputs("\n", stdout);
		Log("\n\n\nDump: %s", Name);

		while (pClass)
		{
			strcpy(Name, "  ");
			pClass->GetCPPName(&Name[strlen(Name)]);
			strcat(Name, "  [");
			pClass->GetFullName(&Name[strlen(Name)]);
			strcat(Name, "]");
			fputs(Name, stdout);
			fputs("\n", stdout);
			Log("Dump: %s", Name);

			pProperty = (UProperty *)pClass->pChildren;
			while (pProperty)
			{
				if (pProperty->IsA(pPropertyClass))
				{
					strcpy(Name, "    ");

					DumpProperty(pProperty, pData, Name);

					fputs(Name, stdout);
					fputs("\n", stdout);
					Log("Dump: %s", Name);
				}

				pProperty = (UProperty *)pProperty->pNext;
			}

			pClass = (UClass *)pClass->pSuper;
		}
		fputs("\n", stdout);

		return(0);
	}

	//Check for UProperty
	lua_getfield(L, LUA_REGISTRYINDEX, EngineUProperty);
	lua_getmetatable(L, 1);
	Found = lua_rawequal(L, -1, -2);
	lua_pop(L, 2);
	if (!Found)
		return(0);

	pObjectProp = (UObjectProp *)luaL_checkudata(L, 1, EngineUProperty);
	pObject     = pObjectProp->pObject;
	pProperty   = pObjectProp->pProperty;
	pData       = pObjectProp->pData;

	if ((char *)pObject == pData)
	{
		pObject->GetFullName(Name);
	}
	else
	{
		pProperty->GetFullName(Name);
		if (pObject)
		{
			strcat(Name, " part of [");
			pObject->GetFullName(&Name[strlen(Name)]);
			strcat(Name, "]");
		}
	}

	fputs("\n", stdout);
	fputs(Name, stdout);
	fputs("\n", stdout);
	Log("\n\n\nDump: %s", Name);

	if (pProperty->ElementCount > 1)
	{
		strcpy(Name, "  ");

		AppendType(Name, pProperty);
		strcat(Name, " ");
		pProperty->GetName(&Name[strlen(Name)]);
		sprintf(&Name[strlen(Name)], "[%d]", pProperty->ElementCount);

		fputs(Name, stdout);
		fputs("\n", stdout);
		Log("Dump: %s", Name);

		for (Index = 0; Index < pProperty->ElementCount; ++Index)
		{
			sprintf(Name, "    [%d] ", Index);
			AppendValue(Name, pProperty, pData, Index);

			fputs(Name, stdout);
			fputs("\n", stdout);
			Log("Dump: %s", Name);
		}
	}

	else if (pProperty->IsA(pStructPropertyClass))
	{
		pData += pProperty->CStructOffset;
		pClass = pProperty->pRelatedClass;
		while (pClass)
		{
			strcpy(Name, "  ");
			pClass->GetCPPName(&Name[strlen(Name)]);
			fputs(Name, stdout);
			fputs("\n", stdout);
			Log("Dump: %s", Name);

			pProperty = (UProperty *)pClass->pChildren;
			while (pProperty)
			{
				if (pProperty->IsA(pPropertyClass))
				{
					strcpy(Name, "    ");

					DumpProperty(pProperty, pData, Name);

					fputs(Name, stdout);
					fputs("\n", stdout);
					Log("Dump: %s", Name);
				}

				pProperty = (UProperty *)pProperty->pNext;
			}

			pClass = (UClass *)pClass->pSuper;
		}
	}

	else if (pProperty->IsA(pArrayPropertyClass))
	{
		TArray<void> * pArray;


		pArray = (TArray<void> *)(pData + pProperty->CStructOffset); 
		strcpy(Name, "  ");

		AppendType(Name, pProperty);
		strcat(Name, " ");
		pProperty->GetName(&Name[strlen(Name)]);
		sprintf(&Name[strlen(Name)], "[%d][%d]", pArray->Count, pArray->Max);

		fputs(Name, stdout);
		fputs("\n", stdout);
		Log("Dump: %s", Name);

		pProperty = (UProperty *)pProperty->pRelatedClass;
		for (Index = 0; Index < pArray->Count; ++Index)
		{
			sprintf(Name, "    [%d] ", Index);

			pData = (char *)((DWORD)pArray->pArray + (Index * pProperty->ElementSize));
			AppendValue(Name, pProperty, pData, 0);

			fputs(Name, stdout);
			fputs("\n", stdout);
			Log("Dump: %s", Name);
		}
	}

	else
	{
		pClass = (UClass *)pProperty;
		while (pClass)
		{
			strcpy(Name, "  ");
			pClass->GetCPPName(&Name[strlen(Name)]);
			strcat(Name, "  [");
			pClass->GetFullName(&Name[strlen(Name)]);
			strcat(Name, "]");
			fputs(Name, stdout);
			fputs("\n", stdout);
			Log("Dump: %s", Name);

			pProperty = (UProperty *)pClass->pChildren;
			while (pProperty)
			{
				if (pProperty->IsA(pPropertyClass))
				{
					strcpy(Name, "    ");

					DumpProperty(pProperty, pData, Name);

					fputs(Name, stdout);
					fputs("\n", stdout);
					Log("Dump: %s", Name);
				}

				pProperty = (UProperty *)pProperty->pNext;
			}

			pClass = (UClass *)pClass->pSuper;
		}
	}
	fputs("\n", stdout);

	return(0);
}


int DumpClass(lua_State * L) 
{
	int           Found;
	char          Name[MaxFullNameSize*8];
	UClass *      pClass;
	UObjectData * pObjectData;
	UObjectProp * pObjectProp;

	//Plain data type.
	if (!lua_touserdata(L, 1))
		luaL_argcheck(L, 0, 1, "Expecting a UObject or UProperty.");

	//Check for UObject
	lua_getfield(L, LUA_REGISTRYINDEX, EngineUObject);
	lua_getmetatable(L, 1);
	Found = lua_rawequal(L, -1, -2);
	lua_pop(L, 2);
	if (Found)
	{
		pObjectData = (UObjectData *)luaL_checkudata(L, 1, EngineUObject);
		if (!pObjectData->pObject)
			luaL_argcheck(L, 0, 1, "Expecting a UObject or UProperty.");

		pClass = pObjectData->pClass;
		pClass->GetFullName(Name);
		pClass = (UClass *)pClass->pSuper;

		if (pObjectData->pObject && pObjectData->pObject->IsA(pClassClass))
		{
			strcat(Name, " [");
			pObjectData->pObject->GetFullName(&Name[strlen(Name)]);
			strcat(Name, "]");
		}
	}

	//Check for UProperty
	else
	{
		lua_getfield(L, LUA_REGISTRYINDEX, EngineUProperty);
		lua_getmetatable(L, 1);
		Found = lua_rawequal(L, -1, -2);
		lua_pop(L, 2);
		if (!Found)
			return(0);

		pObjectProp = (UObjectProp *)luaL_checkudata(L, 1, EngineUProperty);
		pObjectProp->pProperty->GetFullName(Name);

		pClass = 0;
		if (pObjectProp->pProperty->IsA(pArrayPropertyClass) ||
			pObjectProp->pProperty->IsA(pStructPropertyClass))
		{
			pClass = pObjectProp->pProperty->pRelatedClass;
		}
	}

	while (pClass)
	{
		strcat(Name, "\n  ");
		pClass->GetFullName(&Name[strlen(Name)]);

		pClass = (UClass *)pClass->pSuper;
	}

	fputs(Name, stdout);
	fputs("\n", stdout);
	Log("DumpClass: %s", Name);

	return(0);
}


int FindFirst(lua_State * L) 
{
	FName         Name;
	UClass *      pClass;
	UObject *     pObject;
	UObjectData * pObjectData;


	if (lua_type(L, 1) == LUA_TUSERDATA)
	{
		pObjectData = (UObjectData *)luaL_checkudata(L, 1, EngineUObject);
		if ((char *)pObjectData->pObject != pObjectData->pData)
			luaL_argcheck(L, 0, 1, "Expecting a UObject or FName, UClass");

		pObject = pObjectData->pObject;
		pClass  = pObject->pClass;
		Name    = pObject->Name;
	}
	else
	{
		Name = luaL_checkinteger(L, 1);
		pObjectData = (UObjectData *)luaL_checkudata(L, 2, EngineUObject);
		if ((char *)pObjectData->pObject != pObjectData->pData)
			luaL_argcheck(L, 0, 2, "Expecting a UObject or FName, UClass");
		if (!pObjectData->pObject->IsA(pClassClass))
			luaL_argcheck(L, 0, 2, "Expecting a UObject or FName, UClass");

		pClass = (UClass *)pObjectData->pObject;
	}

	pObject = pUObjectHash[Name & 0xFFF];
	while (pObject && !pObject->IsA(pClass))
		pObject = pObject->pHash;

	if (pObject)
	{
		pObjectData = (UObjectData *)lua_newuserdata(L, sizeof(*pObjectData));
		pObjectData->pObject    = pObject;
		pObjectData->pClass     = pObject->pClass;
		pObjectData->pData      = (char *)pObject;
		pObjectData->ActorIndex = (DWORD)-1;

		luaL_getmetatable(L, EngineUObject);
		lua_setmetatable(L, -2);
	}
	else
	{
		lua_pushnil(L);
	}

	return(1);
}


int FindFirstAActor(lua_State * L) 
{
	DWORD         Index;
	UClass *      pClass;
	UObject *     pObject;
	UObjectData * pObjectData;

	pObjectData = (UObjectData *)luaL_checkudata(L, 1, EngineUObject);
	if ((char *)pObjectData->pObject != pObjectData->pData)
		luaL_argcheck(L, 0, 1, "Expecting a UClass");
	if (!pObjectData->pObject->IsA(pClassClass))
		luaL_argcheck(L, 0, 1, "Expecting a UClass");

	pClass = (UClass *)pObjectData->pObject;

	Index = 0;
	pObject = 0;
	while (Index < pAActorArray->Count)
	{
		pObject = pAActorArray->pArray[Index];
		if (pObject && (pObject->IsA(pClass)))
			break;

		++Index;
	}

	if (pObject && (Index < pAActorArray->Count))
	{
		pObjectData = (UObjectData *)lua_newuserdata(L, sizeof(*pObjectData));
		pObjectData->pObject    = pObject;
		pObjectData->pClass     = pObject->pClass;
		pObjectData->pData      = (char *)pObject;
		pObjectData->ActorIndex = Index;

		luaL_getmetatable(L, EngineUObject);
		lua_setmetatable(L, -2);
	}
	else
	{
		lua_pushnil(L);
	}

	return(1);
}


int FindNext(lua_State * L) 
{
	UClass *      pClass;
	UObject *     pObject;
	UObjectData * pObjectData;

	pObjectData = (UObjectData *)luaL_checkudata(L, 1, EngineUObject);
	if ((char *)pObjectData->pObject != pObjectData->pData)
		luaL_argcheck(L, 0, 1, "Expecting a UObject");

	pObject = pObjectData->pObject;
	pClass = pObject->pClass;

	pObject = pObject->pHash;
	while (pObject && (pObject->pClass != pClass))
		pObject = pObject->pHash;

	if (pObject)
	{
		pObjectData = (UObjectData *)lua_newuserdata(L, sizeof(*pObjectData));
		pObjectData->pObject    = pObject;
		pObjectData->pClass     = pObject->pClass;
		pObjectData->pData      = (char *)pObject;
		pObjectData->ActorIndex = (DWORD)-1;

		luaL_getmetatable(L, EngineUObject);
		lua_setmetatable(L, -2);
	}
	else
	{
		lua_pushnil(L);
	}

	return(1);
}


int FindNextAActor(lua_State * L) 
{
	DWORD         Index;
	UClass *      pClass;
	UObject *     pObject;
	UObjectData * pObjectData;

	pObjectData = (UObjectData *)luaL_checkudata(L, 1, EngineUObject);
	if ((char *)pObjectData->pObject != pObjectData->pData)
		luaL_argcheck(L, 0, 1, "Expecting a UObject, UClass");

	Index = pObjectData->ActorIndex + 1;

	pObjectData = (UObjectData *)luaL_checkudata(L, 2, EngineUObject);
	if ((char *)pObjectData->pObject != pObjectData->pData)
		luaL_argcheck(L, 0, 2, "Expecting a UObject, UClass");
	if (!pObjectData->pObject->IsA(pClassClass))
		luaL_argcheck(L, 0, 2, "Expecting a UObject, UClass");

	pClass = (UClass *)pObjectData->pObject;

	pObject = 0;
	while (Index < pAActorArray->Count)
	{
		pObject = pAActorArray->pArray[Index];
		if (pObject && (pObject->IsA(pClass)))
			break;

		++Index;
	}

	if (pObject && (Index < pAActorArray->Count))
	{
		pObjectData = (UObjectData *)lua_newuserdata(L, sizeof(*pObjectData));
		pObjectData->pObject    = pObject;
		pObjectData->pClass     = pObject->pClass;
		pObjectData->pData      = (char *)pObject;
		pObjectData->ActorIndex = Index;

		luaL_getmetatable(L, EngineUObject);
		lua_setmetatable(L, -2);
	}
	else
	{
		lua_pushnil(L);
	}

	return(1);
}


int FullName(lua_State * L) 
{
	int             Found;
	char            Name[MaxFullNameSize];
	UObjectData *   pObjectData;
	UFunctionData * pFunctionData;


	lua_getfield(L, LUA_REGISTRYINDEX, EngineUObject);
	lua_getmetatable(L, 1);
	Found = lua_rawequal(L, -1, -2);
	lua_pop(L, 2);
	if (Found)
	{
		pObjectData = (UObjectData *)luaL_checkudata(L, 1, EngineUObject);
		if ((char *)pObjectData->pObject != pObjectData->pData)
			luaL_argcheck(L, 0, 1, "Expecting a UObject or UFunction");
		pObjectData->pObject->GetFullName(Name);
	}
	else
	{
		pFunctionData = (UFunctionData *)luaL_checkudata(L, 1, EngineUFunction);
		pFunctionData->pFunction->GetFullName(Name);
	}

	lua_pushstring(L, Name);

	return(1);
}


unsigned int __stdcall InputThread(void *)
{
	Input[0] = 0;
	ResetEvent(hInputEvent);
	while (hConsole)
	{
		fgets(&Input[1], sizeof(Input) - 1, stdin);
		Input[0] = 1;
		WaitForSingleObject(hInputEvent, INFINITE);
	}

	return(0);
}


int IsA(lua_State * L) 
{
	UClass *      pClass;
	UObjectData * pClassData;
	UObjectData * pObjectData;


	pObjectData = (UObjectData *)luaL_checkudata(L, 1, EngineUObject);
	if ((char *)pObjectData->pObject != pObjectData->pData)
		luaL_argcheck(L, 0, 1, "Expecting a UObject, UClass");

	pClassData = (UObjectData *)luaL_checkudata(L, 2, EngineUObject);
	if ((char *)pClassData->pObject != pClassData->pData)
		luaL_argcheck(L, 0, 2, "Expecting a UObject, UClass");
	if (!pClassData->pObject->IsA(pClassClass))
		luaL_argcheck(L, 0, 2, "Expecting a UObject, UClass");

	pClass = (UClass *)pClassData->pObject;

	lua_pushboolean(L, pObjectData->pObject->IsA(pClass));


	return(1);
}


extern HWND GetConsoleWindowA(void);

int OpenConsole(lua_State *	/*L*/) 
{
	int          ConsoleHandle;
	FILE *       File;
	unsigned int ID;


	if (hConsole)
		return(0);

	AllocConsole();
	SetConsoleTitle("Command Prompt");
	SetConsoleCtrlHandler(ConsoleHandlerRoutine, 1);


	//Fix up stdin/stdout/stderr
	StdIn = GetStdHandle(STD_INPUT_HANDLE);
	ConsoleHandle = _open_osfhandle((long)StdIn, _O_TEXT);
	File = _fdopen(ConsoleHandle, "r");
	*stdin = *File;
	setvbuf(stdin, NULL, _IONBF, 0);

	StdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleHandle = _open_osfhandle((long)StdOut, _O_TEXT);
	File = _fdopen(ConsoleHandle, "w");
	*stdout = *File;
	setvbuf(stdout, NULL, _IONBF, 0);

	StdErr = GetStdHandle(STD_ERROR_HANDLE);
	ConsoleHandle = _open_osfhandle((long)StdErr, _O_TEXT);
	File = _fdopen(ConsoleHandle, "w");
	*stdout = *File;
	setvbuf(stdout, NULL, _IONBF, 0);

	fputs("\n> ", stdout);
	hConsole = (HANDLE)_beginthreadex(0, 0, InputThread, 0, 0, &ID);

	return(0);
}


int Load(lua_State * L) 
{
	char * pFileName;
	char   Script[MAX_PATH];

	strcpy(Script, Path);
	pFileName = (char *)luaL_checkstring(L, 1);
	strcat(Script, pFileName);

	fputs("Loading[", stdout);
	fputs(Script, stdout);
	fputs("]\n", stdout);
	Log("System: Loading[%s]", Script);
	if (luaL_dofile(L, Script))
	{
		const char * pError = lua_tostring(L, -1);

		fputs("Script Error: ", stdout);
		if (pError)
		{
			fputs(pError, stdout);
			Log("Script Error: %s", pError);
		}
		else
		{
			fputs("[unknown]", stdout);
			Log("Script Error: [unknown]");
		}

		fputs("\n", stdout);
	}
	fputs("\n", stdout);

	return(0);
}


int LogString(lua_State * L) 
{
	//Log("Script: %s", (char *)luaL_checkstring(L, 1));
	Log("%s", (char *)luaL_checkstring(L, 1));
	return(0);
}


int New(lua_State * L) 
{
	UObjectData * pObjectData;
	UObjectData * pNewData;
	UStruct *     pStruct;

	pObjectData = (UObjectData *)luaL_checkudata(L, 1, EngineUObject);

	if (!pObjectData->pObject->IsA(pStructClass))
		luaL_argcheck(L, 0, 1, "Expecting a Struct.");

	pStruct = (UStruct *)pObjectData->pObject;

	pNewData = (UObjectData *)lua_newuserdata(L, sizeof(*pNewData) +
		pStruct->Size);
	pNewData->pObject = 0;
	pNewData->pClass  = (UClass *)pStruct;
	pNewData->pData   = (char *)pNewData + sizeof(*pNewData);

	luaL_getmetatable(L, EngineUObject);
	lua_setmetatable(L, -2);

	memset(pNewData->pData, 0, pStruct->Size);

	return(1);
}


int Restart(lua_State *	/*L*/) 
{
	RestartLua = true;
	return(0);
}


/******************************************************************************
*
* Lua Common Functions to find UScript properties.
*
******************************************************************************/
#define MaxPropCache 256

struct PropertyCacheStruct
{
	UClass *    pClass;
	UProperty * pProperty;
	char        Name[MaxNameSize];
};

PropertyCacheStruct * Cache[MaxPropCache];
PropertyCacheStruct   NewCacheData[MaxPropCache]; //fixed


void ResetPropertyCache(void)
{
	int Index;

	for (Index = 0; Index < MaxPropCache; ++Index)
		Cache[Index] = &NewCacheData[Index];

	memset(NewCacheData, 0, sizeof(NewCacheData));
}

UProperty * FindPropertySlow(UClass * pClass, const char * pPropertyName)
{
	UField *    pField;
	UProperty * pProperty;
	WCHAR       WName[MaxNameSize];

	wsprintfW(WName, L"%S", pPropertyName);

	pProperty = 0;
	while (pClass && !pProperty)
	{
		pField = pClass->pChildren;
		while (pField)
		{
			if (pField->pClass == pFunctionClass)
			{
				if (!wcscmp(WName, GetFName(pField->Name)))
				{
					pProperty = (UProperty *)pField;
					break;
				}
			}

			else if (pField->IsA(pPropertyClass))
			{
				if (!wcscmp(WName, GetFName(pField->Name)))
				{
					pProperty = (UProperty *)pField;
					break;
				}
			}

			pField = pField->pNext;
		}

		pClass = (UClass *)pClass->pSuper;
	}

	return(pProperty);
}


UProperty * FindProperty(lua_State * L, UClass * pClass, const char * pPropertyName)
{
	int                    Index;
    PropertyCacheStruct *  pCache;
	UProperty *            pProperty;
    PropertyCacheStruct ** ppCache;

	//Search the cache.
	ppCache = Cache;
	for (Index = 0; Index < MaxPropCache; ++Index)
	{
		if ((*ppCache)->pClass &&
			((*ppCache)->pClass == pClass) &&
			!strcmp((*ppCache)->Name, pPropertyName))
		{
			break;
		}
		++ppCache;
	}


	//We don't have a cache hit, so look it up the old way.
	if ((Index >= MaxPropCache) || !(*ppCache)->pClass)
	{
		pProperty = FindPropertySlow(pClass, pPropertyName);
		if (!pProperty)
		{
			char Line[512];
			sprintf(Line, "[%s] is not a valid property of [%S]",
				pPropertyName, GetFName(pClass->Name));
			luaL_argcheck(L, pProperty, 2, Line);
			return(0);
		}

		Log("System: Caching Property[%s] Class[%S]", 
			pPropertyName, GetFName(pClass->Name));

		Index = MaxPropCache - 1;
 		pCache = Cache[Index];
 		pCache->pClass = pClass;
 		pCache->pProperty = pProperty;
 		strcpy(pCache->Name, pPropertyName);
	}


	//Move the item to the top.
	pCache = Cache[Index];
	memmove(&Cache[1], &Cache[0], sizeof(Cache[0]) * Index);
	Cache[0] = pCache;


	return(pCache->pProperty);
}


/******************************************************************************
*
* Lua Common Functions to Get/Set properties and to create
* parameter blocks for calling UScript functions.
*
******************************************************************************/
void CreateReturn(lua_State * L, UProperty * pProperty, char * pParam)
{
	if (pProperty->IsA(pBoolPropertyClass))
	{
		lua_pushboolean(L, *(bool *)pParam);
	}

	else if (pProperty->IsA(pBytePropertyClass))
	{
		lua_pushinteger(L, *(BYTE *)pParam);
	}

	else if (pProperty->IsA(pClassPropertyClass) ||
		pProperty->IsA(pObjectPropertyClass))
	{
		UObjectData * pObjectData;


		pObjectData = (UObjectData *)lua_newuserdata(L, sizeof(*pObjectData));
		pObjectData->pObject    = *(UClass **)pParam;
		pObjectData->pClass     = pObjectData->pObject->pClass;
		pObjectData->pData      = (char *)pObjectData->pObject;
		pObjectData->ActorIndex = (DWORD)-1;

		luaL_getmetatable(L, EngineUObject);
		lua_setmetatable(L, -2);
	}

	else if (pProperty->IsA(pFloatPropertyClass))
	{
		lua_pushnumber(L, *(float *)pParam);
	}

	else if (pProperty->IsA(pIntPropertyClass))
	{
		lua_pushinteger(L, *(int *)pParam);
	}

	else if (pProperty->IsA(pNamePropertyClass))
	{
		lua_pushinteger(L, *(FName *)pParam);
	}

	else if (pProperty->IsA(pStrPropertyClass))
	{
		TArray<WCHAR> * Value;

		Value = (TArray<WCHAR> *)pParam;
		if (Value->pArray)
		{
			char * pName = new char[Value->Count];
			sprintf(pName, "%S", Value->pArray);
			lua_pushstring(L, pName);
			delete[] pName;
		}
		else
		{
			lua_pushnil(L);
		}
	}

	else if (pProperty->IsA(pStructPropertyClass))
	{
		UObjectData * pObjectData;


		pObjectData = (UObjectData *)lua_newuserdata(L, sizeof(*pObjectData) +
			pProperty->ElementSize);
		pObjectData->pObject = 0;
		pObjectData->pClass  = pProperty->pRelatedClass;
		pObjectData->pData   = (char *)pObjectData + sizeof(*pObjectData);

		luaL_getmetatable(L, EngineUObject);
		lua_setmetatable(L, -2);

		memcpy(pObjectData->pData, pParam, pProperty->ElementSize);
	}

	else
	{
		luaL_argcheck(L, 0, 0, "Return property type not implemented.");
	}
}


void GetProperty(lua_State * L, UObject * pObject, UProperty * pProperty, 
	char * pData, int Index)
{
	UObjectData *   pObjectData;
	UObjectProp *   pObjectProp;
	UFunctionData * pFunctionData;


	if (pProperty->IsA(pFunctionClass))
	{
		pFunctionData = (UFunctionData *)lua_newuserdata(L, sizeof(*pFunctionData));
		pFunctionData->pObject   = pObject;
		pFunctionData->pFunction = (UFunction *)pProperty;

		luaL_getmetatable(L, EngineUFunction);
		lua_setmetatable(L, -2);
		return;
	}

	if ((Index == NoIndex) && ((pProperty->ElementCount > 1) || 
		pProperty->IsA(pArrayPropertyClass)))
	{
		pObjectProp = (UObjectProp *)lua_newuserdata(L, sizeof(*pObjectProp));
		pObjectProp->pObject   = pObject;
		pObjectProp->pProperty = pProperty;
		pObjectProp->pData     = pData;

		luaL_getmetatable(L, EngineUProperty);
		lua_setmetatable(L, -2);
		return;
	}

	if ((Index != NoIndex) && (pProperty->ElementCount == 1))
	{
		luaL_argcheck(L, 0, 2, "Property cannot be indexed.");
		return;
	}

	if (Index == NoIndex)
		Index = 0;

	if ((Index < 0) || ((DWORD)Index >= pProperty->ElementCount))
	{
		luaL_argcheck(L, 0, 2, "Index out of range.");
		return;
	}


	if (pProperty->IsA(pArrayPropertyClass))
	{
		TArray<void> * pValue;

		pValue = (TArray<void> *)(pData + pProperty->CStructOffset); 
		if ((DWORD)Index >= pValue->Count)
		{
			luaL_argcheck(L, 0, 2, "Index out of range.");
			return;
		}

		pProperty = (UProperty *)pProperty->pRelatedClass;
		GetProperty(L, pObject, pProperty, (char *)((DWORD)pValue->pArray + 
			(Index * pProperty->ElementSize)), NoIndex);
	}

	else if (pProperty->IsA(pBoolPropertyClass))
	{
		int Value;

		Value = *(int *)(pData + pProperty->CStructOffset + 
			(pProperty->ElementSize * Index));
		Value &= (DWORD)pProperty->pRelatedClass;
		if (Value)
			Value = 1;
		lua_pushboolean(L, Value);
	}

	else if (pProperty->IsA(pBytePropertyClass))
	{
		int Value;

		Value = *(BYTE *)(pData + pProperty->CStructOffset +
			(pProperty->ElementSize * Index));
		lua_pushinteger(L, Value);
	}

	else if (pProperty->IsA(pClassPropertyClass) ||
		pProperty->IsA(pObjectPropertyClass))
	{
		UObject * pValue;

		pValue = *(UObject **)(pData + pProperty->CStructOffset +
			(pProperty->ElementSize * Index));

		if (pValue)
		{
			pObjectData = (UObjectData *)lua_newuserdata(L, sizeof(*pObjectData));
			pObjectData->pObject    = pValue;
			pObjectData->pClass     = pValue->pClass;
			pObjectData->pData      = (char *)pValue;
			pObjectData->ActorIndex = (DWORD)-1;

			luaL_getmetatable(L, EngineUObject);
			lua_setmetatable(L, -2);
		}
		else
		{
			lua_pushnil(L);
		}
	}

	else if (pProperty->IsA(pFloatPropertyClass))
	{
		float Value;

		Value = *(float *)(pData + pProperty->CStructOffset +
			(pProperty->ElementSize * Index));
		lua_pushnumber(L, Value);
	}

	else if (pProperty->IsA(pIntPropertyClass))
	{
		int Value;

		Value = *(int *)(pData + pProperty->CStructOffset +
			(pProperty->ElementSize * Index));
		lua_pushinteger(L, Value);
	}

	else if (pProperty->IsA(pNamePropertyClass))
	{
		FName Value;

		Value = *(FName *)(pData + pProperty->CStructOffset +
			(pProperty->ElementSize * Index));
		lua_pushinteger(L, Value);
	}

	else if (pProperty->IsA(pPointerPropertyClass))
	{
		int Value;

		Value = *(int *)(pData + pProperty->CStructOffset +
			(pProperty->ElementSize * Index));
		lua_pushinteger(L, Value);
	}

	else if (pProperty->IsA(pStrPropertyClass))
	{
		TArray<WCHAR> * Value;

		Value = (TArray<WCHAR> *)(pData + pProperty->CStructOffset +
			(pProperty->ElementSize * Index));
		if (Value->pArray)
		{
			char * pName = new char[Value->Count];
			sprintf(pName, "%S", Value->pArray);
			lua_pushstring(L, pName);
			delete[] pName;
		}
		else
		{
			lua_pushnil(L);
		}
	}

	else if (pProperty->IsA(pStructPropertyClass))
	{
		pObjectData = (UObjectData *)lua_newuserdata(L, sizeof(*pObjectData));
		pObjectData->pObject    = pObject;
		pObjectData->pClass     = pProperty->pRelatedClass;
		pObjectData->pData      = pData + pProperty->CStructOffset +
			(pProperty->ElementSize * Index);
		pObjectData->ActorIndex = (DWORD)-1;

		luaL_getmetatable(L, EngineUObject);
		lua_setmetatable(L, -2);
	}

	//pDelegatePropertyClass
	//pFixedArrayPropertyClass
	//pMapPropertyClass
	else
	{
		luaL_argcheck(L, 0, 2, "Can't get this property type.");
	}
}


void InParam(lua_State * L, UProperty * pProperty, int ParamIndex, char * pParam)
{
	if (pProperty->IsA(pBoolPropertyClass))
	{
		int Value;

		luaL_checktype(L, ParamIndex, LUA_TBOOLEAN);
		Value = lua_toboolean(L, ParamIndex);
		*(int *)pParam = Value;
	}

	else if (pProperty->IsA(pBytePropertyClass))
	{
		int Value;

		Value = luaL_checkinteger(L, ParamIndex);
		*(BYTE *)pParam = (BYTE)Value;
	}


	else if (pProperty->IsA(pClassPropertyClass) ||
		pProperty->IsA(pObjectPropertyClass))
	{
		int           Found;
		UObjectData * pObjectData;
		UObjectProp * pObjectProp;


		lua_getfield(L, LUA_REGISTRYINDEX, EngineUObject);
		lua_getmetatable(L, ParamIndex);
		Found = lua_rawequal(L, -1, -2);
		lua_pop(L, 2);
		if (Found)
		{
			pObjectData = (UObjectData *)luaL_checkudata(L, ParamIndex, EngineUObject);
			*(UClass **)pParam = (UClass *)pObjectData->pData;

			return;
		}

		lua_getfield(L, LUA_REGISTRYINDEX, EngineUProperty);
		lua_getmetatable(L, ParamIndex);
		Found = lua_rawequal(L, -1, -2);
		lua_pop(L, 2);
		if (!Found)
			return;

		pObjectProp = (UObjectProp *)luaL_checkudata(L, ParamIndex, EngineUProperty);
		memcpy(pParam, pObjectProp->pData, 4);
	}

	else if (pProperty->IsA(pFloatPropertyClass))
	{
		float Value;

		Value = (float)luaL_checknumber(L, ParamIndex);
		*(float *)pParam = Value;
	}

	else if (pProperty->IsA(pIntPropertyClass))
	{
		int Value;

		Value = luaL_checkinteger(L, ParamIndex);
		*(int *)pParam = Value;
	}

	else if (pProperty->IsA(pNamePropertyClass))
	{
		FName Value;

		Value = luaL_checkinteger(L, ParamIndex);
		*(FName *)pParam = Value;
	}

	else if (pProperty->IsA(pStrPropertyClass))
	{
		const char * pValue;

		pValue = luaL_checkstring(L, ParamIndex);
		FStringConstructor(pParam, pValue);
	}

	else if (pProperty->IsA(pStructPropertyClass))
	{
		int           Found;
		UObjectData * pObjectData;
		UObjectProp * pObjectProp;


		lua_getfield(L, LUA_REGISTRYINDEX, EngineUObject);
		lua_getmetatable(L, ParamIndex);
		Found = lua_rawequal(L, -1, -2);
		lua_pop(L, 2);
		if (Found)
		{
			pObjectData = (UObjectData *)luaL_checkudata(L, ParamIndex, EngineUObject);
			memcpy(pParam, pObjectData->pData, pObjectData->pClass->Size);

			return;
		}

		lua_getfield(L, LUA_REGISTRYINDEX, EngineUProperty);
		lua_getmetatable(L, ParamIndex);
		Found = lua_rawequal(L, -1, -2);
		lua_pop(L, 2);
		if (!Found)
			return;

		pObjectProp = (UObjectProp *)luaL_checkudata(L, ParamIndex, EngineUProperty);
		memcpy(pParam, pObjectProp->pData, pProperty->ElementSize);
	}
}


void SetProperty(lua_State * L, UProperty * pProperty, char * pData, int Index)
{
	if ((Index == NoIndex) && (pProperty->ElementCount > 1))
	{
		luaL_argcheck(L, 0, 2, "Can't set multiple array elements.");
		return;
	}

	if ((Index != NoIndex) && (pProperty->ElementCount == 1) &&
		!pProperty->IsA(pArrayPropertyClass))
	{
		luaL_argcheck(L, 0, 2, "Property cannot be indexed.");
		return;
	}

	if (Index == NoIndex)
		Index = 0;

	if ((Index < 0) || ((DWORD)Index >= pProperty->ElementCount))
	{
		luaL_argcheck(L, 0, 2, "Index out of range.");
		return;
	}

	if (pProperty->IsA(pArrayPropertyClass))
	{
		TArray<void> * pArray;

		pArray = (TArray<void> *)(pData + pProperty->CStructOffset); 
		if ((DWORD)Index >= pArray->Count)
		{
			luaL_argcheck(L, 0, 2, "Index out of range.");
			return;
		}

		pProperty = (UProperty *)pProperty->pRelatedClass;
		SetProperty(L, pProperty, (char *)((DWORD)pArray->pArray + 
			(Index * pProperty->ElementSize)), NoIndex);
	}

	else if (pProperty->IsA(pBoolPropertyClass))
	{
		int Value;


		Value = (DWORD)pProperty->pRelatedClass;
		luaL_checktype(L, 3, LUA_TBOOLEAN);
		if (lua_toboolean(L, 3))
		{
			*(int *)(pData + pProperty->CStructOffset + 
				(pProperty->ElementSize * Index)) |= Value;
		}

		else
		{
			*(int *)(pData + pProperty->CStructOffset + 
				(pProperty->ElementSize * Index)) &= ~Value;
		}
	}

	else if (pProperty->IsA(pBytePropertyClass))
	{
		BYTE Value;

		Value = (BYTE)luaL_checkinteger(L, 3);
		*(BYTE *)(pData + pProperty->CStructOffset +
			(pProperty->ElementSize * Index)) = Value;
	}

	else if (pProperty->IsA(pClassPropertyClass) ||
	    pProperty->IsA(pObjectPropertyClass))
	{
		UObjectData * pValue;

		pValue = (UObjectData *)luaL_checkudata(L, 3, EngineUObject);
		*(UClass **)(pData + pProperty->CStructOffset +
			(pProperty->ElementSize * Index)) = (UClass *)pValue->pData;
	}

	else if (pProperty->IsA(pFloatPropertyClass))
	{
		float Value;

		Value = (float)luaL_checknumber(L, 3);
		*(float *)(pData + pProperty->CStructOffset +
			(pProperty->ElementSize * Index)) = Value;
	}

	else if (pProperty->IsA(pIntPropertyClass))
	{
		int Value;

		Value = luaL_checkinteger(L, 3);
		*(int *)(pData + pProperty->CStructOffset +
			(pProperty->ElementSize * Index)) = Value;
	}

	else if (pProperty->IsA(pNamePropertyClass))
	{
		FName Value;

		Value = (FName)luaL_checkinteger(L, 3);
		*(FName *)(pData + pProperty->CStructOffset +
			(pProperty->ElementSize * Index)) = Value;
	}

	else if (pProperty->IsA(pPointerPropertyClass))
	{
		int Value;

		Value = luaL_checkinteger(L, 3);
		*(int *)(pData + pProperty->CStructOffset +
			(pProperty->ElementSize * Index)) = Value;
	}

	else if (pProperty->IsA(pStrPropertyClass))
	{
		TArray<WCHAR> * Value;

		Value = (TArray<WCHAR> *)(pData + pProperty->CStructOffset +
			(pProperty->ElementSize * Index));
		if (Value->pArray)
			FStringDestructor(Value);

		if (!lua_isnil(L, 3))
			FStringConstructor(Value, luaL_checkstring(L, 3));
	}

	else if (pProperty->IsA(pStructPropertyClass))
	{
		UObjectData * pValue;

		pValue = (UObjectData *)luaL_checkudata(L, 3, EngineUObject);
		memcpy(pData + pProperty->CStructOffset + 
			(pProperty->ElementSize * Index), pValue->pData, 
			pProperty->ElementSize);
	}

	//pDelegatePropertyClass
	//pFixedArrayPropertyClass
	//pMapPropertyClass
	else
	{
		luaL_argcheck(L, 0, 2, "Can't set this property type.");
	}
}

void ValidateParam(lua_State * L, UProperty * pProperty, int ParamIndex)
{
	if (pProperty->IsA(pBoolPropertyClass))
	{
		luaL_checktype(L, ParamIndex, LUA_TBOOLEAN);
	}

	else if (pProperty->IsA(pBytePropertyClass))
	{
		luaL_checkinteger(L, ParamIndex);
	}

	else if (pProperty->IsA(pClassPropertyClass))
	{
		int           Found;
		UObjectData * pObjectData;
		UObjectProp * pObjectProp;


		lua_getfield(L, LUA_REGISTRYINDEX, EngineUObject);
		lua_getmetatable(L, ParamIndex);
		Found = lua_rawequal(L, -1, -2);
		lua_pop(L, 2);
		if (Found)
		{
			pObjectData = (UObjectData *)luaL_checkudata(L, ParamIndex, EngineUObject);
			if (!pObjectData->pObject || 
				!pObjectData->pObject->IsA(pClassClass) ||
				((char *)pObjectData->pObject != pObjectData->pData))
			{
				luaL_argcheck(L, 0, ParamIndex, "Expecting a UClass.");
			}

			return;
		}

		lua_getfield(L, LUA_REGISTRYINDEX, EngineUProperty);
		lua_getmetatable(L, ParamIndex);
		Found = lua_rawequal(L, -1, -2);
		lua_pop(L, 2);
		if (!Found)
			return;

		pObjectProp = (UObjectProp *)luaL_checkudata(L, ParamIndex, EngineUProperty);
		if (!pObjectProp->pProperty->IsA(pClassPropertyClass))
			luaL_argcheck(L, 0, ParamIndex, "Expecting a Class.");
	}

	else if (pProperty->IsA(pFloatPropertyClass))
	{
		luaL_checknumber(L, ParamIndex);
	}

	else if (pProperty->IsA(pIntPropertyClass))
	{
		luaL_checkinteger(L, ParamIndex);
	}

	else if (pProperty->IsA(pNamePropertyClass))
	{
		luaL_checkinteger(L, ParamIndex);
	}

	else if (pProperty->IsA(pObjectPropertyClass))
	{
		int           Found;
		UObjectData * pObjectData;
		UObjectProp * pObjectProp;

		lua_getfield(L, LUA_REGISTRYINDEX, EngineUObject);
		lua_getmetatable(L, ParamIndex);
		Found = lua_rawequal(L, -1, -2);
		lua_pop(L, 2);
		if (Found)
		{
			pObjectData = (UObjectData *)luaL_checkudata(L, ParamIndex, EngineUObject);
			if (!pObjectData->pObject || 
				((char *)pObjectData->pObject != pObjectData->pData))
			{
				luaL_argcheck(L, 0, ParamIndex, "Expecting a UObject.");
			}

			return;
		}

		lua_getfield(L, LUA_REGISTRYINDEX, EngineUProperty);
		lua_getmetatable(L, ParamIndex);
		Found = lua_rawequal(L, -1, -2);
		lua_pop(L, 2);
		if (!Found)
			return;

		pObjectProp = (UObjectProp *)luaL_checkudata(L, ParamIndex, EngineUProperty);
		if (!pObjectProp->pProperty->IsA(pObjectPropertyClass))
			luaL_argcheck(L, 0, ParamIndex, "Expecting a UObject.");
	}

	else if (pProperty->IsA(pStrPropertyClass))
	{
		luaL_checkstring(L, ParamIndex);
	}

	else if (pProperty->IsA(pStructPropertyClass))
	{
		int           Found;
		UObjectData * pObjectData;
		UObjectProp * pObjectProp;


		lua_getfield(L, LUA_REGISTRYINDEX, EngineUObject);
		lua_getmetatable(L, ParamIndex);
		Found = lua_rawequal(L, -1, -2);
		lua_pop(L, 2);
		if (Found)
		{
			pObjectData = (UObjectData *)luaL_checkudata(L, ParamIndex, EngineUObject);
			if (!pObjectData->pClass->IsA(pStructClass))
				luaL_argcheck(L, 0, ParamIndex, "Expecting a Struct.");

			return;
		}

		lua_getfield(L, LUA_REGISTRYINDEX, EngineUProperty);
		lua_getmetatable(L, ParamIndex);
		Found = lua_rawequal(L, -1, -2);
		lua_pop(L, 2);
		if (!Found)
			return;

		pObjectProp = (UObjectProp *)luaL_checkudata(L, ParamIndex, EngineUProperty);
		if (!pObjectProp->pProperty->IsA(pStructPropertyClass))
			luaL_argcheck(L, 0, ParamIndex, "Expecting a Struct.");
	}

	//pArrayPropertyClass
	//pDelegatePropertyClass
	//pFixedArrayPropertyClass
	//pMapPropertyClass
	//pPointerPropertyClass
	else
	{
		luaL_argcheck(L, 0, ParamIndex, "Property type not implemented.");
	}
}


/******************************************************************************
*
* Lua AActors Functions
*
******************************************************************************/
int AActorsIndex(lua_State * L) 
{
	DWORD         Index;
	UObject *     pObject;
	UObjectData * pObjectData;


	if (lua_type(L, 2) == LUA_TSTRING)
	{
		pObject = FindActorByFullName(luaL_checkstring(L, 2));
		if (pObject)
		{
			pObjectData = (UObjectData *)lua_newuserdata(L, sizeof(*pObjectData));
			pObjectData->pObject    = pObject;
			pObjectData->pClass     = pObject->pClass;
			pObjectData->pData      = (char *)pObject;
			pObjectData->ActorIndex = (DWORD)-1;

			luaL_getmetatable(L, EngineUObject);
			lua_setmetatable(L, -2);
		}
		else
		{
			lua_pushnil(L);
		}
	}

	else
	{
		Index = luaL_checkinteger(L, 2);
		if (Index < pAActorArray->Count)
		{
			if (pAActorArray->pArray[Index])
			{
				pObject = pAActorArray->pArray[Index];

				pObjectData = (UObjectData *)lua_newuserdata(L, sizeof(*pObjectData));
				pObjectData->pObject    = pObject;
				pObjectData->pClass     = pObject->pClass;
				pObjectData->pData      = (char *)pObject;
				pObjectData->ActorIndex = (DWORD)-1;

				luaL_getmetatable(L, EngineUObject);
				lua_setmetatable(L, -2);
			}
			else
			{
				lua_pushnil(L);
			}
		}
		else
		{
			luaL_argcheck(L, 0, 2, "Index out of range.");
		}
	}

	return(1);
}


int AActorsLen(lua_State * L) 
{
	lua_pushinteger(L, pAActorArray->Count);

	return(1);
}



/******************************************************************************
*
* Lua FNames Functions
*
******************************************************************************/
int FNamesIndex(lua_State * L) 
{
	DWORD  Index;
	char   Name[MaxNameSize];
	WCHAR  WName[MaxNameSize];

	if (lua_type(L, 2) == LUA_TSTRING)
	{
		wsprintfW(WName, L"%S", luaL_checkstring(L, 2));
		for (Index = 0; Index < pFNameEntryArray->Count; ++Index)
		{
			if (pFNameEntryArray->pArray[Index])
			{
				if (!wcscmp(WName, pFNameEntryArray->pArray[Index]->Name))
					break;
			}
		}

		if (Index < pFNameEntryArray->Count)
			lua_pushinteger(L, Index);
		else
			lua_pushnil(L);
	}

	else
	{
		Index = luaL_checkinteger(L, 2);
		if (Index < pFNameEntryArray->Count)
		{
			if (pFNameEntryArray->pArray[Index])
			{
				sprintf(Name, "%S", GetFName(Index));
				lua_pushstring(L, Name);
			}
			else
			{
				lua_pushnil(L);
			}
		}
		else
		{
			luaL_argcheck(L, 0, 2, "Index out of range.");
		}
	}

	return(1);
}


int FNamesLen(lua_State * L) 
{
	lua_pushinteger(L, pFNameEntryArray->Count);

	return(1);
}


/******************************************************************************
*
* Lua UFunction Functions
*
******************************************************************************/
int UFunctionCall(lua_State * L) 
{
	WORD            OldNativeIndex;
	UFunction *     pFunction;
	UFunctionData * pFunctionData;
	UObject *       pObject;
	char *          pParam;
	char *          pParamBlock;
	DWORD *         pProcessEvent;
	UProperty *     pProperty;
	int             ParamBlockSize;
	int             ParamIndex;
	int             Results;

	Results = 0;
	try
	{
		pFunctionData = (UFunctionData *)luaL_checkudata(L, 1, EngineUFunction);

		//Validate the parameters.
		ParamIndex = 2;
		ParamBlockSize = 0;
		pProperty  = (UProperty *)pFunctionData->pFunction->pChildren;
		while (pProperty)
		{
			if (pProperty->IsA(pPropertyClass) && (pProperty->Flags & 0x80))
			{
				ParamBlockSize += pProperty->ElementSize;

				if (!(pProperty->Flags & 0x400))
				{
					if (!(pProperty->Flags & 0x10) || (ParamIndex <= lua_gettop(L)))
						ValidateParam(L, pProperty, ParamIndex);

					++ParamIndex;
				}
			}

			pProperty = (UProperty *)pProperty->pNext;
		}


		//Fill out the parameter block.
		pParamBlock = new char[ParamBlockSize];
		memset(pParamBlock, 0, ParamBlockSize);

		try
		{
			pParam = pParamBlock;
			ParamIndex = 2;
			pProperty  = (UProperty *)pFunctionData->pFunction->pChildren;
			while (pProperty)
			{
				if (pProperty->IsA(pPropertyClass) && (pProperty->Flags & 0x80))
				{
					if (!(pProperty->Flags & 0x400))
					{
						if (!(pProperty->Flags & 0x10) || (ParamIndex <= lua_gettop(L)))
							InParam(L, pProperty, ParamIndex, pParam);

						++ParamIndex;
					}

					pParam += pProperty->ElementSize;
				}

				pProperty = (UProperty *)pProperty->pNext;
			}


			//Call the function.
			OldNativeIndex = pFunctionData->pFunction->NativeIndex;
			pFunctionData->pFunction->NativeIndex = 0;

			pObject = pFunctionData->pObject;
			pFunction = pFunctionData->pFunction;
			pProcessEvent = pObject->pVMT[4];

			__asm xor  eax,eax;
			__asm push eax;
			__asm mov  eax,pParamBlock;
			__asm push eax;
			__asm mov  eax,pFunction;
			__asm push eax;
			__asm mov  ecx,pObject;
			__asm call pProcessEvent;

			pFunctionData->pFunction->NativeIndex = OldNativeIndex;


			//Push the return value.
			pParam = pParamBlock;
			pProperty  = (UProperty *)pFunctionData->pFunction->pChildren;
			while (pProperty)
			{
				if (pProperty->IsA(pPropertyClass) && (pProperty->Flags & 0x80))
				{
					if (pProperty->Flags & 0x400)
					{
						++Results;
						CreateReturn(L, pProperty, pParam);
					}

					pParam += pProperty->ElementSize;
				}

				pProperty = (UProperty *)pProperty->pNext;
			}


			//Push the out parameters, in order
			pParam = pParamBlock;
			pProperty  = (UProperty *)pFunctionData->pFunction->pChildren;
			while (pProperty)
			{
				if (pProperty->IsA(pPropertyClass) && (pProperty->Flags & 0x80))
				{
					if (pProperty->Flags & 0x400)
					{
					}

					else if (pProperty->Flags & 0x100)
					{
						++Results;
						CreateReturn(L, pProperty, pParam);
					}

					if (pProperty->IsA(pStrPropertyClass))
						FStringDestructor(pParam);

					pParam += pProperty->ElementSize;
				}

				pProperty = (UProperty *)pProperty->pNext;
			}
		}
		catch (...)
		{
			luaL_argcheck(L, 0, 1, "GPF in Call2");
		}

		delete[] pParamBlock;
	}
	catch(...)
	{
		luaL_argcheck(L, 0, 1, "GPF in Call");
		ResetPropertyCache();
	}

	return(Results);
}


int UFunctionIndex(lua_State * L) 
{
	DWORD           Index;
	UFunctionData * pFunctionData;


	pFunctionData = (UFunctionData *)luaL_checkudata(L, 1, EngineUFunction);
	Index = luaL_checkinteger(L, 2);
	if (Index < pFunctionData->pFunction->Script.Count)
		lua_pushinteger(L, pFunctionData->pFunction->Script.pArray[Index]);
	else
		luaL_argcheck(L, 0, 2, "Index out of range.");

	return(1);
}


int UFunctionNewIndex(lua_State * L) 
{
	DWORD           Index;
	UFunctionData * pFunctionData;
	DWORD           Value;


	pFunctionData = (UFunctionData *)luaL_checkudata(L, 1, EngineUFunction);
	Index = luaL_checkinteger(L, 2);
	Value = luaL_checkinteger(L, 3);
	if (Index < pFunctionData->pFunction->Script.Count)
		pFunctionData->pFunction->Script.pArray[Index] = (BYTE)Value;
	else
		luaL_argcheck(L, 0, 2, "Index out of range.");

	return(0);
}


int UFunctionLen(lua_State * L) 
{
	UFunctionData * pFunctionData;


	pFunctionData = (UFunctionData *)luaL_checkudata(L, 1, EngineUFunction);
	lua_pushinteger(L, pFunctionData->pFunction->Script.Count);

	return(1);
}


/******************************************************************************
*
* Lua UObject Functions
*
******************************************************************************/
int UObjectIndex(lua_State * L) 
{
	UProperty *   pProperty;
	const char *  pPropertyName;
	UObjectData * pObjectData;


	pObjectData = (UObjectData *)luaL_checkudata(L, 1, EngineUObject);
	pPropertyName = lua_tostring(L, 2);

	pProperty = FindProperty(L, pObjectData->pClass, pPropertyName);
	GetProperty(L, pObjectData->pObject, pProperty, pObjectData->pData, NoIndex);

	return(1);
}


int UObjectNewIndex(lua_State * L) 
{
	UObjectData * pObjectData;
	UProperty *   pProperty;
	const char *  pPropertyName;


	pObjectData = (UObjectData *)luaL_checkudata(L, 1, EngineUObject);
	pPropertyName = lua_tostring(L, 2);

	pProperty = FindProperty(L, pObjectData->pClass, pPropertyName);
	SetProperty(L, pProperty, pObjectData->pData, NoIndex);

	return(0);
}


int UObjectLen(lua_State * L) 
{
	UObjectData * pObjectData;

	pObjectData = (UObjectData *)luaL_checkudata(L, 1, EngineUObject);
	if ((char *)pObjectData->pObject != pObjectData->pData)
		luaL_argcheck(L, 0, 1, "Expecting a UObject");

	lua_pushinteger(L, pObjectData->pObject->ObjectInternal);

	return(1);
}


/******************************************************************************
*
* Lua UObjects Functions
*
******************************************************************************/
int UObjectsIndex(lua_State * L) 
{
	DWORD           Index;
	UFunctionData * pFunctionData;
	UObject *       pObject;
	UObjectData *   pObjectData;


	pObject = 0;
	if (lua_type(L, 2) == LUA_TSTRING)
	{
		pObject = FindObjectByFullName(luaL_checkstring(L, 2));
	}

	else
	{
		Index = luaL_checkinteger(L, 2);
		if (Index < pUObjectArray->Count)
			pObject = pUObjectArray->pArray[Index];
		else
			luaL_argcheck(L, 0, 2, "Index out of range.");
	}

	if (pObject)
	{
		if (pObject->IsA(pFunctionClass))
		{
			pFunctionData = (UFunctionData *)lua_newuserdata(L, sizeof(*pFunctionData));
			pFunctionData->pObject   = pObject;
			pFunctionData->pFunction = (UFunction *)pObject;

			luaL_getmetatable(L, EngineUFunction);
			lua_setmetatable(L, -2);
		}

		else
		{
			pObjectData = (UObjectData *)lua_newuserdata(L, sizeof(*pObjectData));
			pObjectData->pObject    = pObject;
			pObjectData->pClass     = pObject->pClass;
			pObjectData->pData      = (char *)pObject;
			pObjectData->ActorIndex = (DWORD)-1;

			luaL_getmetatable(L, EngineUObject);
			lua_setmetatable(L, -2);
		}
	}
	else
	{
		lua_pushnil(L);
	}

	return(1);
}


int UObjectsLen(lua_State * L) 
{
	lua_pushinteger(L, pUObjectArray->Count);

	return(1);
}


/******************************************************************************
*
* Lua UProperty Functions
*
******************************************************************************/
int UPropertyIndex(lua_State * L) 
{
	int           Index;
	UObjectProp * pObjectProp;
	UProperty *   pProperty;
	const char *  pPropertyName;



	pObjectProp = (UObjectProp *)luaL_checkudata(L, 1, EngineUProperty);

	if (lua_type(L, 2) == LUA_TSTRING)
	{
		pPropertyName = lua_tostring(L, 2);

		pProperty = FindProperty(L, pObjectProp->pProperty->pRelatedClass, 
			pPropertyName);
		GetProperty(L, pObjectProp->pObject, pProperty, 
			pObjectProp->pData + pObjectProp->pProperty->CStructOffset, NoIndex);
	}

	else
	{
		Index = luaL_checkinteger(L, 2);
		GetProperty(L, pObjectProp->pObject, pObjectProp->pProperty, 
			pObjectProp->pData, Index);
	}

	return(1);
}


int UPropertyNewIndex(lua_State * L) 
{
	int           Index;
	UObjectProp * pObjectProp;
	UProperty *   pProperty;
	const char *  pPropertyName;



	pObjectProp = (UObjectProp *)luaL_checkudata(L, 1, EngineUProperty);

	if (lua_type(L, 2) == LUA_TSTRING)
	{
		pPropertyName = lua_tostring(L, 2);

		pProperty = FindProperty(L, pObjectProp->pProperty->pRelatedClass, 
			pPropertyName);
		SetProperty(L, pProperty, pObjectProp->pData + 
			pObjectProp->pProperty->CStructOffset, NoIndex);
	}

	else
	{
		Index = luaL_checkinteger(L, 2);
		SetProperty(L, pObjectProp->pProperty, pObjectProp->pData, Index);
	}

	return(0);
}


/******************************************************************************
*
* Lua Engine Library
*
******************************************************************************/
lua_State* L;


const struct luaL_reg EngineLib[] = 
{
	{"CloseConsole", CloseConsole},
	{"Dump", Dump},
	{"DumpClass", DumpClass},
	{"FindFirst", FindFirst},
	{"FindFirstAActor", FindFirstAActor},
	{"FindNext", FindNext},
	{"FindNextAActor", FindNextAActor},
	{"FullName", FullName},
	{"IsA", IsA},
	{"Load", Load},
	{"Log", LogString},
	{"New", New},
	{"OpenConsole", OpenConsole},
	{"Restart", Restart},
	{NULL, NULL}
};


int lua_openEngine(lua_State *L) 
{
	luaL_openlib(L, "Engine", EngineLib, 0);

	lua_pushstring(L, "AActors");
	lua_newuserdata(L, 1);
	luaL_newmetatable(L, "Engine.AActors");
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, AActorsIndex);
	lua_settable(L, -3);
	lua_pushstring(L, "__len");
	lua_pushcfunction(L, AActorsLen);
	lua_settable(L, -3);
	lua_setmetatable(L, -2);
	lua_settable(L, -3);

	lua_pushstring(L, "FNames");
	lua_newuserdata(L, 1);
	luaL_newmetatable(L, "Engine.FNames");
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, FNamesIndex);
	lua_settable(L, -3);
	lua_pushstring(L, "__len");
	lua_pushcfunction(L, FNamesLen);
	lua_settable(L, -3);
	lua_setmetatable(L, -2);
	lua_settable(L, -3);

	lua_pushstring(L, "UObjects");
	lua_newuserdata(L, 1);
	luaL_newmetatable(L, "Engine.UObjects");
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, UObjectsIndex);
	lua_settable(L, -3);
	lua_pushstring(L, "__len");
	lua_pushcfunction(L, UObjectsLen);
	lua_settable(L, -3);
	lua_setmetatable(L, -2);
	lua_settable(L, -3);

	luaL_newmetatable(L, EngineUFunction);
	lua_pushstring(L, "__call");
	lua_pushcfunction(L, UFunctionCall);
	lua_settable(L, -3);
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, UFunctionIndex);
	lua_settable(L, -3);
	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, UFunctionNewIndex);
	lua_settable(L, -3);
	lua_pushstring(L, "__len");
	lua_pushcfunction(L, UFunctionLen);
	lua_settable(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, EngineUObject);
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, UObjectIndex);
	lua_settable(L, -3);
	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, UObjectNewIndex);
	lua_settable(L, -3);
	lua_pushstring(L, "__len");
	lua_pushcfunction(L, UObjectLen);
	lua_settable(L, -3);
	lua_pop(L, 1);

	luaL_newmetatable(L, EngineUProperty);
	lua_pushstring(L, "__index");
	lua_pushcfunction(L, UPropertyIndex);
	lua_settable(L, -3);
	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, UPropertyNewIndex);
	lua_settable(L, -3);
	lua_pop(L, 1);

	return(1);
}


void ShutdownLua(void)
{
	if (L)
	{
		Log("System: Lua Shutdown");
		lua_close(L);
		L = 0;
	}
}


void InitLua(void)
{
	if (!L)
	{
		ResetPropertyCache();
		char szBotPath[MAX_PATH];

		Log("System: Lua Started");
		fputs("Lua Restarted\n", stdout);

		if(hLogFile != NULL && bLogActive) //added
		{
			fclose(hLogFile);
			hLogFile = fopen(szLogFile, "w");
		}

		L = lua_open();
		luaL_openlibs(L);
		lua_openEngine(L);
		lua_gc(L, LUA_GCRESTART, 0);

		lua_pushcfunction(L, Load);
		
		sprintf(szBotPath, "%s%s", Path, DEFAULT_BOTFILE);
		lua_pushstring(L, DEFAULT_BOTFILE);
		lua_pcall(L, 1, 0, 0);
		
		fputs("\n> ", stdout);
	}
}


/******************************************************************************
*
* Lua Callbacks.
*
******************************************************************************/
int KeyEvent(DWORD Key, DWORD Action, float Value)
{
	int Result;

	Result = 0;
	try
	{
		if (L)
		{
			lua_settop(L, 0);
			lua_getglobal(L, "KeyEvent");
			if (!lua_isnil(L, -1))
			{
				lua_pushinteger(L, Key);
				lua_pushinteger(L, Action);
				lua_pushnumber(L, Value);

				if (lua_pcall(L, 3, 1, 0))
				{
					const char * pError = lua_tostring(L, -1);
					if (pError)
						Log("KeyEvent Error: %s", pError);
					else
						Log("KeyEvent Error: [unknown]");
				}

				if (!lua_isboolean(L, -1))
				{
					Log("KeyEvent Error: KeyEvent needs to return a boolean.");
				}
				else
				{
					Result = lua_toboolean(L, -1);
				}
			}
		}
	}
	catch(...)
	{
		Log("KeyEvent Error: GPF");
		ResetPropertyCache();
	}

	return(Result);
}


void PostRender(UObject * pCanvas)
{
	UObjectData * pObjectData;


	try
	{
		if (L)
		{
			lua_settop(L, 0);
			lua_getglobal(L, "PostRender");
			if (!lua_isnil(L, -1))
			{
				if (pCanvas)
				{
					pObjectData = (UObjectData *)lua_newuserdata(L, sizeof(*pObjectData));
					pObjectData->pObject    = pCanvas;
					pObjectData->pClass     = pCanvas->pClass;
					pObjectData->pData      = (char *)pCanvas;
					pObjectData->ActorIndex = (DWORD)-1;

					luaL_getmetatable(L, EngineUObject);
					lua_setmetatable(L, -2);
				}
				else
				{
					lua_pushnil(L);
				}

				if (lua_pcall(L, 1, 0, 0))
				{
					const char * pError = lua_tostring(L, -1);
					if (pError)
						Log("PostRender Error: %s", pError);
					else
						Log("PostRender Error: [unknown]");
				}
			}
		}
	}
	catch(...)
	{
		Log("PostRender Error: GPF");
		ResetPropertyCache();
	}

}


void PreRender(UObject * pCanvas)
{
	UObjectData * pObjectData;


	try
	{
		if (L)
		{
			lua_settop(L, 0);
			lua_getglobal(L, "PreRender");
			if (!lua_isnil(L, -1))
			{
				if (pCanvas)
				{
					pObjectData = (UObjectData *)lua_newuserdata(L, sizeof(*pObjectData));
					pObjectData->pObject    = pCanvas;
					pObjectData->pClass     = pCanvas->pClass;
					pObjectData->pData      = (char *)pCanvas;
					pObjectData->ActorIndex = (DWORD)-1;

					luaL_getmetatable(L, EngineUObject);
					lua_setmetatable(L, -2);
				}
				else
				{
					lua_pushnil(L);
				}

				if (lua_pcall(L, 1, 0, 0))
				{
					const char * pError = lua_tostring(L, -1);
					if (pError)
						Log("PreRender Error: %s", pError);
					else
						Log("PreRender Error: [unknown]");
				}
			}
		}
	}
	catch(...)
	{
		Log("PreRender Error: GPF");
		ResetPropertyCache();
	}
}


void Tick(float DeltaTime)
{
	UObjectData * pObjectData;

	//Reload Lua Script
	EngineInit();
	if (RestartLua)
	{
		RestartLua = 0;
		ShutdownLua();
	}
	InitLua();

	char szWindowText[50];
	HWND fgw;

	if((fgw = GetForegroundWindow()) != NULL)
	{
		GetWindowText(fgw, szWindowText, sizeof(szWindowText));

		if(strstr(szWindowText, "America's Army"))
		{
			if ((GetAsyncKeyState(BASEKEY) & 0x8000) &&
				(GetAsyncKeyState((BYTE)VkKeyScan(RESTART_KEY)) & 0x8000))
			{
				RestartLua = true;
				Sleep(250);
			}

			if ((GetAsyncKeyState(BASEKEY) & 0x8000) &&
				(GetAsyncKeyState((BYTE)VkKeyScan(OPEN_LOG_KEY)) & 0x8000) &&
				bLogActive)
			{
				ShellExecute(0, "open", "notepad.exe", szLogFile, "", 1);
				Sleep(250);
			}

			//Toggle the console
			static int ConsoleOneShot;
			if ((GetAsyncKeyState(BASEKEY) & 0x8000) &&
				(GetAsyncKeyState((BYTE)VkKeyScan(OPEN_LUA_CONSOLE)) & 0x8000))
			{
				if (!ConsoleOneShot)
				{
					ConsoleOneShot = 1;
					if (hConsole)
						CloseConsole(L);
					else
						OpenConsole(L);
				}
			}
			else
			{
				ConsoleOneShot = 0;
			}
		}
	}

	//Process any console data.
	if (hConsole && Input[0])
	{
		try
		{
			if (L)
			{
				lua_settop(L, 0);
				if (luaL_dostring(L, &Input[1]))
				{
					const char * pError = lua_tostring(L, -1);

					fputs("Console Error: ", stdout);
					if (pError)
					{
						fputs(pError, stdout);
						Log("Console Error: %s", pError);
					}
					else
					{
						fputs("[unknown]", stdout);
						Log("Console Error: [unknown]");
					}

					fputs("\n\n", stdout);
				}

				fputs("> ", stdout);

				Input[0] = 0;
				SetEvent(hInputEvent);
			}
		}
		catch(...)
		{
			Log("Console Error: GPF");
			ResetPropertyCache();
		}
	}


	//Call Lua Tick
	try
	{
		if (L)
		{
			lua_settop(L, 0);
			lua_getglobal(L, "Tick");
			if (!lua_isnil(L, -1))
			{
				lua_pushnumber(L, DeltaTime);

				if (pViewport)
				{
					pAActorArray = &pViewport->Actor->XLevel->ActorArray;

					pObjectData = (UObjectData *)lua_newuserdata(L, sizeof(*pObjectData));
					pObjectData->pObject    = pViewport;
					pObjectData->pClass     = pViewport->pClass;
					pObjectData->pData      = (char *)pViewport;
					pObjectData->ActorIndex = (DWORD)-1;

					luaL_getmetatable(L, EngineUObject);
					lua_setmetatable(L, -2);
				}
				else
				{
					lua_pushnil(L);
				}

				if (lua_pcall(L, 2, 0, 0))
				{
					const char * pError = lua_tostring(L, -1);
					if (pError)
						Log("Tick Error: %s", pError);
					else
						Log("Tick Error: [unknown]");
				}
			}
		}
	}
	catch(...)
	{
		Log("Tick Error: GPF");
		ResetPropertyCache();
	}
}



/******************************************************************************
*
* DLL Attach/Detach
*
******************************************************************************/
void ProcessAttach(HINSTANCE hInstance)
{
	char FileName[MAX_PATH];
	char* pChar;

	//get base path
	GetModuleFileName(hInstance, FileName, sizeof(FileName));

	//Strip our file from the path.
	strcpy(Path, FileName);
	pChar = strrchr(Path, '\\');
	if (pChar)
		pChar[1] = 0;

	//Start a log file.
	if(strlen(DEFAULT_LOGFILE) > 0)
	{
		sprintf(szLogFile, "%s%s", Path, DEFAULT_LOGFILE);
		hLogFile = fopen(szLogFile, "w");
		bLogActive = true;
		
		Log("System: LScript Started");
	}

	//Create the event.
	hInputEvent = CreateEvent(0, 0, 0, 0);
}

void ProcessDetach()
{
	CloseConsole(0);
	CloseHandle(hInputEvent);
	ShutdownLua();

	Log("System: LScript Shutdown");
	
	if(hLogFile)
		fclose(hLogFile);
}


/******************************************************************************
*******************************************************************************
*******************************************************************************
* 
* Hook code
*
*******************************************************************************
*******************************************************************************
******************************************************************************/
DWORD * pKeyEventHook;
DWORD   KeyEventOffset;
int  (__stdcall * pReal_KeyEvent)(DWORD, DWORD, float);

DWORD * pPostRenderHook;
DWORD   PostRenderOffset;
void (__stdcall * pReal_PostRender)(UObject *);

DWORD * pPreRenderHook;
DWORD   PreRenderOffset;
void (__stdcall * pReal_PreRender)(UObject *);

DWORD * pTickHook;
DWORD   TickOffset;
void (__stdcall * pReal_Tick)(float);

int __stdcall Hooked_KeyEvent(DWORD Key, DWORD Action, float Value)
{
	void * pThis;
	int    Result;

	__asm mov pThis,ecx;

	//Call Lua KeyEvent
	Result = KeyEvent(Key, Action, Value);

	//Call the original 
	if (!Result)
	{
		__asm mov  eax,Value;
		__asm push eax;
		__asm mov  eax,Action;
		__asm push eax;
		__asm mov  eax,Key;
		__asm push eax;
		__asm mov  ecx,pThis;
		__asm call pReal_KeyEvent;
		__asm mov  Result,eax;
	}

	__asm mov  ecx,pThis;
	return(Result);
}


void __stdcall Hooked_PostRender(UObject * pCanvas)
{
	void * pThis;

	__asm mov pThis,ecx;

	//Call the original post render
	__asm mov  eax,pCanvas;
	__asm push eax;
	__asm mov  ecx,pThis;
	__asm call pReal_PostRender;

	//Call Lua PostRender
	PostRender(pCanvas);

	__asm mov  ecx,pThis;
}

void __stdcall Hooked_PreRender(UObject * pCanvas)
{
	void * pThis;

	__asm mov pThis,ecx;

	//Call Lua PreRender
	PreRender(pCanvas);

	//Call the original Pre render
	__asm mov  eax,pCanvas;
	__asm push eax;
	__asm mov  ecx,pThis;
	__asm call pReal_PreRender;

	__asm mov  ecx,pThis;
}


void __stdcall Hooked_Tick(float DeltaTime)
{
	void * pThis;

	__asm 
	{
		mov pThis,ecx;

	//Call Lua Tick
	Tick(DeltaTime);

	//Call the original tick
	 mov  eax,DeltaTime;
	 push eax;
	 mov  ecx,pThis;
	 call pReal_Tick;
	 mov  ecx,pThis;
	}
}


void HookEngine(void)
{
	DWORD Protect;
	DWORD hEngine = (DWORD)GetModuleHandle("engine.dll");

	pKeyEventHook = (DWORD *)(hEngine + 0xb34000);
	KeyEventOffset = *pKeyEventHook;
	pReal_KeyEvent = (int (__stdcall *)(DWORD Key, DWORD Action, float Value))((DWORD)pKeyEventHook + *pKeyEventHook + 4);
	VirtualProtect(pKeyEventHook, sizeof(DWORD), PAGE_READWRITE, &Protect);
	*pKeyEventHook = (DWORD)Hooked_KeyEvent - ((DWORD)pKeyEventHook + 4);
	VirtualProtect(pKeyEventHook, sizeof(DWORD), Protect, &Protect);

	pPostRenderHook = (DWORD *)(hEngine + 0xb34300);
	PostRenderOffset = *pPostRenderHook;
	pReal_PostRender = (void (__stdcall *)(UObject *))((DWORD)pPostRenderHook + *pPostRenderHook + 4);
	VirtualProtect(pPostRenderHook, sizeof(DWORD), PAGE_READWRITE, &Protect);
	*pPostRenderHook = (DWORD)Hooked_PostRender - ((DWORD)pPostRenderHook + 4);
	VirtualProtect(pPostRenderHook, sizeof(DWORD), Protect, &Protect);

	pPreRenderHook = (DWORD *)(hEngine + 0xb34220);
	PreRenderOffset = *pPreRenderHook;
	pReal_PreRender = (void (__stdcall *)(UObject *))((DWORD)pPreRenderHook + *pPreRenderHook + 4);
	VirtualProtect(pPreRenderHook, sizeof(DWORD), PAGE_READWRITE, &Protect);
	*pPreRenderHook = (DWORD)Hooked_PreRender - ((DWORD)pPreRenderHook + 4);
	VirtualProtect(pPreRenderHook, sizeof(DWORD), Protect, &Protect);

	pTickHook = (DWORD *)(hEngine + 0xb34130);
	TickOffset = *pTickHook;
	pReal_Tick = (void (__stdcall *)(float))((DWORD)pTickHook + *pTickHook + 4);
	VirtualProtect(pTickHook, sizeof(DWORD), PAGE_READWRITE, &Protect);
	*pTickHook = (DWORD)Hooked_Tick - ((DWORD)pTickHook + 4);
	VirtualProtect(pTickHook, sizeof(DWORD), Protect, &Protect);
}

void UnhookEngine(void)
{
	DWORD Protect;

	VirtualProtect(pKeyEventHook, sizeof(DWORD), PAGE_READWRITE, &Protect);
	*pKeyEventHook = KeyEventOffset;
	VirtualProtect(pKeyEventHook, sizeof(DWORD), Protect, &Protect);

	VirtualProtect(pPostRenderHook, sizeof(DWORD), PAGE_READWRITE, &Protect);
	*pPostRenderHook = PostRenderOffset;
	VirtualProtect(pPostRenderHook, sizeof(DWORD), Protect, &Protect);

	VirtualProtect(pPreRenderHook, sizeof(DWORD), PAGE_READWRITE, &Protect);
	*pPreRenderHook = PreRenderOffset;
	VirtualProtect(pPreRenderHook, sizeof(DWORD), Protect, &Protect);

	VirtualProtect(pTickHook, sizeof(DWORD), PAGE_READWRITE, &Protect);
	*pTickHook = TickOffset;
	VirtualProtect(pTickHook, sizeof(DWORD), Protect, &Protect);
}

/******************************************************************************
*
* DLL initialization code.
*
******************************************************************************/
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpvReason*/)
{
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			ProcessAttach(hInstance);
			HookEngine();
		}
		break;
	}
	return TRUE;
}
