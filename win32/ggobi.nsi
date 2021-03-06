# set PATH for all users - requires admin but I think we do anyway
!define ALL_USERS 1
!include "AddToPath.NSH"

!include "MUI.nsh"

Name GGobi
VIProductVersion ${VERSION}
OutFile ggobi-${VERSION}.exe
InstallDir $PROGRAMFILES\ggobi
InstallDirRegKey HKLM "Software\ggobi" "InstallationDirectory"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "../CPLicense.txt"
#!insertmacro MUI_PAGE_COMPONENTS components
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

Section "GGobi"

    Delete "$INSTDIR\*.dll"

    SetOutPath $INSTDIR
    File ..\src\.libs\ggobi.exe
    File /r /x .svn /x big* ..\data
    #File /r /x .svn ..\doc
    File ..\share\ggobi.ico
    File ..\ggobirc
    File ..\src\.libs\libggobi-2.dll
    # Could install all dependency dlls like this
    #File ..\..\..\..\local\bin\*.dll

    SetOutPath $INSTDIR\share
    File ..\share\colorschemes.xml

    # Install the built plugins
    #SetOutPath $INSTDIR\plugins\GraphLayout
    #File ..\plugins\GraphLayout\plugin.la
    #File ..\plugins\GraphLayout\.libs\plugin-0.dll
    #SetOutPath $INSTDIR\plugins\GraphAction
    #File ..\plugins\GraphAction\plugin.la
    #File ..\plugins\GraphAction\.libs\plugin-0.dll
    #SetOutPath $INSTDIR\plugins\DataViewer
    #File ..\plugins\DataViewer\plugin.la
    #File ..\plugins\DataViewer\.libs\plugin-0.dll
    #SetOutPath $INSTDIR\plugins\ggvis
    #File ..\plugins\ggvis\plugin.la
    #File ..\plugins\ggvis\.libs\plugin-0.dll
    #SetOutPath $INSTDIR\plugins\VarCloud
    #File ..\plugins\VarCloud\plugin.la
    #File ..\plugins\VarCloud\.libs\plugin-0.dll
    #SetOutPath $INSTDIR\plugins\DescribeDisplay
    #File ..\plugins\DescribeDisplay\plugin.la
    #File ..\plugins\DescribeDisplay\.libs\plugin-0.dll

    # Include the header files and libs for those building against GGobi (rggobi)
    SetOutPath $INSTDIR\dev\include\ggobi
    File ..\src\*.h
    File ..\src\GGStructSizes.c
    SetOutPath $INSTDIR\dev\lib
    File ..\src\.libs\libggobi.dll.a
    File ..\src\.libs\libggobi.la

    # Shortcut (install for all users if user is an admin)
    userInfo::getAccountType
    pop $0
    StrCmp $0 "Admin" 0 +2
    SetShellVarContext all
    SetOutPath $INSTDIR\data
    CreateShortCut "$DESKTOP\GGobi.lnk" "$INSTDIR\ggobi.exe" "" "$INSTDIR\ggobi.ico"

    Push $INSTDIR
    Call AddToPath

    WriteRegStr HKLM SOFTWARE\ggobi "InstallationDirectory" "$INSTDIR"

    # Set up for uninstallation
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ggobi" "DisplayName" "GGobi Interactive Graphics Platform"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ggobi" "UninstallString" '"$INSTDIR\uninstall.exe"'
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ggobi" "NoModify" 1
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ggobi" "NoRepair" 1
    WriteUninstaller $INSTDIR\uninstall.exe

SectionEnd

Section "Uninstall"
    # Clean up registry
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ggobi"
    DeleteRegKey HKLM SOFTWARE\ggobi
    # Remove from PATH
    Push $INSTDIR
    Call un.RemoveFromPath
    # Get rid of shortcut
    userInfo::getAccountType
    pop $0
    StrCmp $0 "Admin" 0 +2
    SetShellVarContext all
    Delete "$DESKTOP\ggobi.lnk"
    # Finally, get rid of our files
    RMDir /r "$INSTDIR"
SectionEnd
