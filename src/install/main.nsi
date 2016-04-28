!define $PROGRAM_NAME "WarGames"
!define $PROGRAM_VERSION "0.0.0.1"
!define $INSTALLER_VERSION "0.0.0.1"
!define $COMPANY_NAME "walkingTarget"

Name "$PROGRAM_NAME"
OutFile "$PROGRAM_NAME.$INSTALLER_VERSION.exe"
BrandingText "This is branding text."

/********** Main installation script *********/
Section
    SetOutPath $INSTDIR
    File "wargames.exe"
    File "libgcc_s_dw2-1.dll"
    File "libstdc++-6.dll"
    File "libwinpthread-1.dll"
    File "Qt5Core.dll"
    File "Qt5Gui.dll"
    File "Qt5Network.dll"
    File "Qt5Widgets.dll"
    SetOutPath $INSTDIR\platforms
    File "qwindows.dll"

    ; Create shortcuts
    SetOutPath $INSTDIR; Sets the working directory for shortcuts
    CreateShortCut "$DESKTOP\${PROGRAM_NAME}.lnk" "$INSTDIR\wargames.exe"
    CreateDirectory "$SMPROGRAMS\${PROGRAM_NAME}"
    CreateShortCut "$SMPROGRAMS\${PROGRAM_NAME}\${PROGRAM_NAME}.lnk" "$INSTDIR\wargames.exe"
    CreateShortCut "$SMPROGRAMS\${PROGRAM_NAME}\Uninstall.lnk" "$INSTDIR\uninstall.exe"
    
    ; Write registry keys for Add/Remove Programs
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "InstallLocation" "$INSTDIR"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "DisplayName" "${PROGRAM_NAME}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "DisplayIcon" "$INSTDIR\wargames.exe,0"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "DisplayVersion" "${PROGRAM_VERSION}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "Publisher" "${COMPANY_NAME}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "HelpLink" "http://wargames.walkingtarget.com"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "URLUpdateInfo" "http://wargames.walkingtarget.com"
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "NoModify" "1"
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}" "NoRepair" "1"
                     
    WriteUninstaller "$INSTDIR\uninstall.exe";
SectionEnd

Section "Uninstall"
    
    ; Remove shortcuts
    Delete "$DESKTOP\${PROGRAM_NAME}.lnk"
    RMDir "$SMPROGRAMS\${PROGRAM_NAME}"
    
    ; Remove registry keys for Add/Remove Programs
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PROGRAM_NAME}"
    
    Delete "$INSTDIR\uninstall.exe"
SectionEnd
