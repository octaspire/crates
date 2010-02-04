;NSIS script for crates win32 installer
;based on NSIS examples: "Welcome/Finish Page Example" and "Header Bitmap Example"
;and also "Reusable installer script"
;This script assumes that it is run from the etc-directory of the
;source distribution and that the necessary DLLs for SDL and SDL_mixer
;are present in the top level source directory (parent of etc).

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;General

  !define company  "Octaspire"
  !define prodname "Crates"
  !define regkey "Software\${company}\${prodname}"
  !define uninstkey "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}"
  !define startmenu "$SMPROGRAMS\${company} ${prodname}"

  ;Name and file
  Name "${prodname}"
  Caption "${prodname}"
  OutFile "crates-0.3.0-setup.exe"

  CRCCheck on
  SetCompressor /SOLID lzma

  ;Default installation folder
  InstallDir "$PROGRAMFILES\${company}\${prodname}"

  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "${regkey}" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel user ;admin

;--------------------------------
;Interface Settings
  !define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\orange-install.ico"
  !define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\orange-uninstall.ico"

  !define MUI_HEADERIMAGE
  !define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\orange.bmp" ; optional
  !define MUI_HEADERIMAGE_UNBITMAP "${NSISDIR}\Contrib\Graphics\Header\orange-uninstall.bmp"
  !define MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange.bmp"
  !define MUI_UNWELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange-uninstall.bmp"
  !define MUI_ABORTWARNING


;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "..\COPYING"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "Crates game files" SecDummy
  SectionIn RO

  SetOutPath "$INSTDIR"

  ;ADD YOUR OWN FILES HERE...
  File ..\crates.exe
  File ..\*.dll

  CreateDirectory "$INSTDIR\resources\"
  CreateDirectory "$INSTDIR\resources\entities"
  CreateDirectory "$INSTDIR\resources\halloffame"
  CreateDirectory "$INSTDIR\resources\images"
  CreateDirectory "$INSTDIR\resources\meshes"
  CreateDirectory "$INSTDIR\resources\missions"
  CreateDirectory "$INSTDIR\resources\musics"
  CreateDirectory "$INSTDIR\resources\sounds"
  CreateDirectory "$INSTDIR\resources\states"
  CreateDirectory "$INSTDIR\resources\textures"

  SetOutPath $INSTDIR\resources
  File "..\resources\config.lua"

  SetOutPath $INSTDIR\resources\entities
  File "..\resources\entities\*.lua"

  SetOutPath $INSTDIR\resources\halloffame
  File "..\resources\halloffame\default"

  SetOutPath $INSTDIR\resources\images
  File "..\resources\images\*.ico"

  SetOutPath $INSTDIR\resources\meshes
  File "..\resources\meshes\*.lua"

  SetOutPath $INSTDIR\resources\missions
  File "..\resources\missions\default.lua"

  SetOutPath $INSTDIR\resources\sounds
  File "..\resources\sounds\*.wav"

  SetOutPath $INSTDIR\resources\states
  File "..\resources\states\*.lua"

  SetOutPath $INSTDIR\resources\textures
  File "..\resources\textures\*.png"


  ;Store installation folder
  WriteRegStr HKCU "${regkey}" "Install_Dir" $INSTDIR

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

Section "Start menu shortcuts" SecStartMenuShortCuts
  SectionIn RO

  CreateDirectory "${startmenu}"

  SetOutPath $INSTDIR

  CreateShortCut "${startmenu}\${prodname}.lnk" "$INSTDIR\crates.exe" "" "$INSTDIR\resources\images\crateslogo16x16x32.ico"
  CreateShortCut "${startmenu}\Resources.lnk "$INSTDIR\resources"
  CreateShortCut "${startmenu}\Uninstall ${prodname}.lnk" "$INSTDIR\uninstall.exe"

SectionEnd

Section "Desktop icon" SecDesktopIcon

  CreateShortCut "$DESKTOP\${company} ${prodname}.lnk" "$INSTDIR\crates.exe" "" "$INSTDIR\resources\images\crateslogo48x48x32.ico"

SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecDummy              ${LANG_ENGLISH} "Files needed for playing Crates puzzle game"
  LangString DESC_SecStartMenuShortCuts ${LANG_ENGLISH} "Start menu shortcuts for Crates"
  LangString DESC_SecDesktopIcon        ${LANG_ENGLISH} "Desktop shortcut for Crates"

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDummy} $(DESC_SecDummy)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecStartMenuShortCuts} $(DESC_SecStartMenuShortCuts)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDesktopIcon} $(DESC_SecDesktopIcon)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...
  Delete $INSTDIR\crates.exe"
  Delete $INSTDIR\*.dll"

  CreateDirectory "$INSTDIR\resources\"
  CreateDirectory "$INSTDIR\resources\entities"
  CreateDirectory "$INSTDIR\resources\halloffame"
  CreateDirectory "$INSTDIR\resources\images"
  CreateDirectory "$INSTDIR\resources\meshes"
  CreateDirectory "$INSTDIR\resources\missions"
  CreateDirectory "$INSTDIR\resources\musics"
  CreateDirectory "$INSTDIR\resources\sounds"
  CreateDirectory "$INSTDIR\resources\states"
  CreateDirectory "$INSTDIR\resources\textures"

  Delete "$INSTDIR\resources\*.*"
  Delete "$INSTDIR\resources\entities\*.*"
  Delete "$INSTDIR\resources\halloffame\*"
  Delete "$INSTDIR\resources\images\*.*"
  Delete "$INSTDIR\resources\meshes\*.*"
  Delete "$INSTDIR\resources\missions\*.*"
  Delete "$INSTDIR\resources\sounds\*.*"
  Delete "$INSTDIR\resources\states\*.*"
  Delete "$INSTDIR\resources\textures\*.*"

  RMDir "$INSTDIR\resources\entities"
  RMDir "$INSTDIR\resources\halloffame"
  RMDir "$INSTDIR\resources\images"
  RMDir "$INSTDIR\resources\meshes"
  RMDir "$INSTDIR\resources\missions"
  RMDir "$INSTDIR\resources\sounds"
  RMDir "$INSTDIR\resources\states"
  RMDir "$INSTDIR\resources\textures"

  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR"

  DeleteRegKey /ifempty HKCU "${regkey}"

  Delete "${startmenu}\*.*"
  Delete "${startmenu}"
  RMDIR "${startmenu}"
  Delete "$DESKTOP\${company} ${prodname}.lnk"

SectionEnd
