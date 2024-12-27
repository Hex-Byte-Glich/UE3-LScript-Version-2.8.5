# Microsoft Developer Studio Project File - Name="LScript" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=LScript - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LScript.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LScript.mak" CFG="LScript - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LScript - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "LScript - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LScript - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O2 /Ob1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "LScript_EXPORTS" /D "_MBCS" /GF /Gy /YX /Fp".\Release/LScript.pch" /Fo".\Release/" /Fd".\Release/" /c /GX 
# ADD CPP /nologo /MT /W3 /O2 /Ob1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "LScript_EXPORTS" /D "_MBCS" /GF /Gy /YX /Fp".\Release/LScript.pch" /Fo".\Release/" /Fd".\Release/" /c /GX 
# ADD BASE MTL /nologo /D"NDEBUG" /mktyplib203 /tlb".\Release\LScript.tlb" /win32 
# ADD MTL /nologo /D"NDEBUG" /mktyplib203 /tlb".\Release\LScript.tlb" /win32 
# ADD BASE RSC /l 1033 /d "NDEBUG" 
# ADD RSC /l 1033 /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /out:".\Release\LScript.dll" /incremental:no /pdb:".\Release\LScript.pdb" /pdbtype:sept /subsystem:windows /implib:".\Release/LScript.lib" /machine:ix86 
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /out:".\Release\LScript.dll" /incremental:no /pdb:".\Release\LScript.pdb" /pdbtype:sept /subsystem:windows /implib:".\Release/LScript.lib" /machine:ix86 

!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /ZI /W3 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "LScript_EXPORTS" /D "_MBCS" /YX /Fp".\Debug/LScript.pch" /Fo".\Debug/" /Fd".\Debug/" /GZ /c /GX 
# ADD CPP /nologo /MTd /ZI /W3 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "LScript_EXPORTS" /D "_MBCS" /YX /Fp".\Debug/LScript.pch" /Fo".\Debug/" /Fd".\Debug/" /GZ /c /GX 
# ADD BASE MTL /nologo /D"_DEBUG" /mktyplib203 /tlb".\Debug\LScript.tlb" /win32 
# ADD MTL /nologo /D"_DEBUG" /mktyplib203 /tlb".\Debug\LScript.tlb" /win32 
# ADD BASE RSC /l 1033 /d "_DEBUG" 
# ADD RSC /l 1033 /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /out:".\Debug\LScript.dll" /incremental:no /debug /pdb:".\Debug\LScript.pdb" /pdbtype:sept /subsystem:windows /implib:".\Debug/LScript.lib" /machine:ix86 
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /out:".\Debug\LScript.dll" /incremental:no /debug /pdb:".\Debug\LScript.pdb" /pdbtype:sept /subsystem:windows /implib:".\Debug/LScript.lib" /machine:ix86 

!ENDIF

# Begin Target

# Name "LScript - Win32 Release"
# Name "LScript - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=lapi.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=lauxlib.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=lbaselib.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=lcode.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=ldblib.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=ldebug.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=ldo.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=ldump.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=lfunc.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=lgc.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=linit.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=liolib.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=llex.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=lmathlib.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=lmem.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=loadlib.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=lobject.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=lopcodes.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=loslib.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=lparser.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=LScript.cpp

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=lstate.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=lstring.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=lstrlib.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=ltable.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=ltablib.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=ltm.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=lundump.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=lvm.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=lzio.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=print.c

!IF  "$(CFG)" == "LScript - Win32 Release"

# ADD CPP /nologo /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GX 
!ELSEIF  "$(CFG)" == "LScript - Win32 Debug"

# ADD CPP /nologo /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LScript_EXPORTS" /GZ /GX 
!ENDIF

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=lapi.h
# End Source File
# Begin Source File

SOURCE=lauxlib.h
# End Source File
# Begin Source File

SOURCE=lcode.h
# End Source File
# Begin Source File

SOURCE=ldebug.h
# End Source File
# Begin Source File

SOURCE=ldo.h
# End Source File
# Begin Source File

SOURCE=lfunc.h
# End Source File
# Begin Source File

SOURCE=lgc.h
# End Source File
# Begin Source File

SOURCE=llex.h
# End Source File
# Begin Source File

SOURCE=llimits.h
# End Source File
# Begin Source File

SOURCE=lmem.h
# End Source File
# Begin Source File

SOURCE=lobject.h
# End Source File
# Begin Source File

SOURCE=lopcodes.h
# End Source File
# Begin Source File

SOURCE=lparser.h
# End Source File
# Begin Source File

SOURCE=lstate.h
# End Source File
# Begin Source File

SOURCE=lstring.h
# End Source File
# Begin Source File

SOURCE=ltable.h
# End Source File
# Begin Source File

SOURCE=ltm.h
# End Source File
# Begin Source File

SOURCE=lua.h
# End Source File
# Begin Source File

SOURCE=luaconf.h
# End Source File
# Begin Source File

SOURCE=lualib.h
# End Source File
# Begin Source File

SOURCE=lundump.h
# End Source File
# Begin Source File

SOURCE=lvm.h
# End Source File
# Begin Source File

SOURCE=lzio.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

