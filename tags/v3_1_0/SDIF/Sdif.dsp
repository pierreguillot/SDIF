# Microsoft Developer Studio Project File - Name="Sdif" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Sdif - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Sdif.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Sdif.mak" CFG="Sdif - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Sdif - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Sdif - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe

!IF  "$(CFG)" == "Sdif - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "NDEBUG" /D "_WINDOWS" /D "WIN32" /D "_AFXDLL" /YX /FD /D _SDIF_VERSION="\"Win32 1.00.alpha\"" /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Sdif - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W4 /GX /Z7 /Od /D "_WINDOWS" /D "_DEBUG" /D "_SdifMemoryReport" /D "WIN32" /D "_AFXDLL" /FR /YX /FD /D _SDIF_VERSION="\"Win32 1.00.alpha\"" /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "Sdif - Win32 Release"
# Name "Sdif - Win32 Debug"
# Begin Group "doc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\doc\index.html
# End Source File
# Begin Source File

SOURCE=.\doc\indexManuel.html
# End Source File
# Begin Source File

SOURCE=.\doc\indexNorm.html
# End Source File
# Begin Source File

SOURCE=.\doc\indexProg.html
# End Source File
# Begin Source File

SOURCE=.\doc\Patch.html
# End Source File
# Begin Source File

SOURCE=.\doc\SdifManuelBar.html
# End Source File
# Begin Source File

SOURCE=.\doc\SdifManuelDoc.html
# End Source File
# Begin Source File

SOURCE=.\doc\SdifNormBar.html
# End Source File
# Begin Source File

SOURCE=.\doc\SdifNormDoc.html
# End Source File
# Begin Source File

SOURCE=.\doc\SdifNormDocFr.html
# End Source File
# Begin Source File

SOURCE=.\doc\SdifProgBar.html
# End Source File
# Begin Source File

SOURCE=.\doc\SdifProgDoc.html
# End Source File
# End Group
# Begin Group "predefined types"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\sdif\SdifTypes.STYP
# End Source File
# End Group
# Begin Group "Unix Makefiles"

# PROP Default_Filter ""
# Begin Group "sdif"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\sdif\Makefile
# End Source File
# Begin Source File

SOURCE=.\sdif\Makefile.headers
# End Source File
# Begin Source File

SOURCE=.\sdif\Makefile.src
# End Source File
# End Group
# Begin Source File

SOURCE=.\Makefile
# End Source File
# Begin Source File

SOURCE=.\Makefile.postamble
# End Source File
# Begin Source File

SOURCE=.\Makefile.preamble
# End Source File
# Begin Source File

SOURCE=.\Readme
# End Source File
# End Group
# Begin Source File

SOURCE=.\sdif\sdif.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifConvToText.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifConvToText.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifErrMess.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifErrMess.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifError.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifError.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifFGet.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifFGet.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifFile.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifFile.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifFileStruct.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifFPrint.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifFPrint.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifFPut.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifFPut.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifFrame.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifFrame.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifFrameType.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifFrameType.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifFRead.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifFRead.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifFScan.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifFScan.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifFWrite.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifFWrite.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifGlobals.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifGlobals.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifHard_OS.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifHard_OS.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifHash.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifHash.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifList.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifList.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifMatrix.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifMatrix.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifMatrixType.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifMatrixType.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifMemory.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifMemory.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifNameValue.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifNameValue.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifPreTypes.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifPreTypes.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifPrint.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifPrint.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifRWLowLevel.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifRWLowLevel.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifStreamID.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifStreamID.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifTest.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifTest.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifTextConv.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifTextConv.h
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifTimePosition.c
# End Source File
# Begin Source File

SOURCE=.\sdif\SdifTimePosition.h
# End Source File
# End Target
# End Project