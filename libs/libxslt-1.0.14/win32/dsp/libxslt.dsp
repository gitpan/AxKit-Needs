# Microsoft Developer Studio Project File - Name="libxslt" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=libxslt - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libxslt.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libxslt.mak" CFG="libxslt - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libxslt - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libxslt - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libxslt - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "libxslt"
# PROP Intermediate_Dir "libxslt"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "libxslt_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /O2 /I "../.." /I "../../libxslt" /I "../../../gnome-xml/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "IN_LIBXSLT" /D "WITH_DEBUGGER" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 libxml2.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /libpath:"../../../gnome-xml/win32/dsp/libxml2"
# Begin Custom Build - Creating static library libxslt_a.lib...
InputPath=.\libxslt\libxslt.dll
SOURCE="$(InputPath)"

"libxslt\libxslt_a.lib" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	lib /nologo /out:libxslt\libxslt_a.lib libxslt\*.obj

# End Custom Build

!ELSEIF  "$(CFG)" == "libxslt - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "libxslt___Win32_Debug"
# PROP BASE Intermediate_Dir "libxslt___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "libxslt"
# PROP Intermediate_Dir "libxslt"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "libxslt_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /Zi /Od /I "../.." /I "../../libxslt" /I "../../../gnome-xml/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "IN_LIBXSLT" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libxml2.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"../../../gnome-xml/win32/dsp/libxml2"
# Begin Custom Build - Creating static library libxslt_a.lib...
InputPath=.\libxslt\libxslt.dll
SOURCE="$(InputPath)"

"libxslt\libxslt_a.lib" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	lib /nologo /out:libxslt\libxslt_a.lib libxslt\*.obj

# End Custom Build

!ENDIF 

# Begin Target

# Name "libxslt - Win32 Release"
# Name "libxslt - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\libxslt\attributes.c
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\documents.c
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\extensions.c
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\extra.c
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\functions.c
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\imports.c
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\keys.c
# End Source File
# Begin Source File

SOURCE=.\libxslt.def
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\namespaces.c
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\numbers.c
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\pattern.c
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\preproc.c
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\templates.c
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\transform.c
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\variables.c
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\xslt.c
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\xsltutils.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\libxslt\attributes.h
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\documents.h
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\extensions.h
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\extra.h
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\functions.h
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\imports.h
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\keys.h
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\libxslt.h
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\namespaces.h
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\numbersInternals.h
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\pattern.h
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\preproc.h
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\templates.h
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\transform.h
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\variables.h
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\win32config.h
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\xslt.h
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\xsltconfig.h
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\xsltInternals.h
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\xsltutils.h
# End Source File
# Begin Source File

SOURCE=..\..\libxslt\xsltwin32config.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
