# Microsoft Developer Studio Project File - Name="test5" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=test5 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "test5.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "test5.mak" CFG="test5 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "test5 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "test5 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/boost/libs/numeric/ublas/test5", YDCAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "test5 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
dCPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\..\.." /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "USE_MSVC" /YX /FD /Zm500 /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "test5 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
dCPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\..\.." /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "USE_MSVC" /YX /FD /GZ /Zm500 /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "test5 - Win32 Release"
# Name "test5 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\test5.cpp
# End Source File
# Begin Source File

SOURCE=.\test51.cpp
# End Source File
# Begin Source File

SOURCE=.\test52.cpp
# End Source File
# Begin Source File

SOURCE=.\test53.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\boost\numeric\ublas\banded.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\boost\numeric\ublas\blas.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\boost\numeric\ublas\concepts.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\boost\numeric\ublas\config.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\boost\numeric\ublas\duff.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\boost\numeric\ublas\exception.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\boost\numeric\ublas\functional.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\boost\numeric\ublas\hermitean.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\boost\numeric\ublas\io.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\boost\numeric\ublas\iterator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\boost\numeric\ublas\math.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\boost\numeric\ublas\matrix.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\boost\numeric\ublas\matrix_et.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\boost\numeric\ublas\matrix_pr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\boost\numeric\ublas\matrix_sp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\boost\numeric\ublas\storage.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\boost\numeric\ublas\storage_sp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\boost\numeric\ublas\symmetric.h
# End Source File
# Begin Source File

SOURCE=.\test5.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\boost\numeric\ublas\timer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\boost\numeric\ublas\traits.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\boost\numeric\ublas\triangular.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\boost\numeric\ublas\vector.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\boost\numeric\ublas\vector_et.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\boost\numeric\ublas\vector_pr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\boost\numeric\ublas\vector_sp.h
# End Source File
# End Group
# End Target
# End Project
