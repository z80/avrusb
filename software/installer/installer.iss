; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
AppName=brushless_motor
AppVerName=BrushlessMotor-1.0.0
AppPublisher=Transmagnetics
AppPublisherURL=http://transmag.cn/
AppSupportURL=http://transmag.cn/
AppUpdatesURL=http://transmag.cn/
DefaultDirName=c:\BrushlessMotor
DefaultGroupName=BrushlessMotor
AllowNoIcons=yes
OutputBaseFilename=BrushlessMotor-1.0.0
SetupIconFile=icon.ico
Compression=lzma
SolidCompression=yes
SourceDir=.
ArchitecturesInstallIn64BitMode=x64

;[Languages]
;Name: "english"; MessagesFile: "compiler:Default.isl"; InfoAfterFile: "infoafter.txt"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked;

[Files]
Source: "..\app\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\BrushlessMotor"; Filename: "{app}\ctrl.exe"; WorkingDir: "{app}"
Name: "{group}\{cm:ProgramOnTheWeb,brushless_motor}"; Filename: "http://transmag.cn/"
Name: "{group}\{cm:UninstallProgram,brushless_motor}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\BrushlessMotor"; Filename: "{app}\ctrl.exe"; WorkingDir: "{app}"; Tasks: desktopicon

[Run]
;Filename: "{app}\winusb_driver\installer_x86.exe"; Description: "x86 Device driver install."; Check: not Is64BitInstallMode
;Filename: "{app}\winusb_driver\installer_x64.exe"; Description: "x64 Device driver install."; Check: Is64BitInstallMode

Filename: "{app}\vcredist_x86.exe"; Description: "x86 runtime envirinment install. It's necessary to install it at least once."; Check: not Is64BitInstallMode
Filename: "{app}\vcredist_x86.exe"; Description: "x86 runtime envirinment install. It's necessary to install it at least once."; Check: Is64BitInstallMode



