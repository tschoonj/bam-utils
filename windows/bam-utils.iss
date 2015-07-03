#define MyAppId "BAM-utils"
#define srcdir abs_top_srcdir_win
#define builddir abs_top_builddir_win
#define MY_HOME "C:\msys\1.0\home\schoon\"

#ifdef BAM_UTILS64
  #define MyAppName "BAM-utils 64-bit"
  #define GTK_INSTALLER_EXE "gtk3-runtime-3.14.13-2015-06-23-ts-win64.exe"
  #define MY_MINGW "C:\TDM-GCC-64\"
#else
  #define MyAppName "BAM-utils 32-bit"
  #define GTK_INSTALLER_EXE "gtk2-runtime-2.24.8-2011-12-03-ash.exe"
  #define MY_MINGW "C:\MinGW32\"
#endif

#define MyAppPublisher "Tom Schoonjans"
#define MyAppURL "http://github.com/tschoonj/bam-utils"

[Setup]
AppName={#MyAppName}
AppId={#MyAppId}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
LicenseFile={#srcdir}\License.rtf
OutputDir={#builddir}\windows
#ifdef BAM_UTILS64
OutputBaseFilename={#MyAppId}-{#MyAppVersion}-win64
ArchitecturesInstallIn64BitMode=x64
ArchitecturesAllowed=x64
#else
OutputBaseFilename={#MyAppId}-{#MyAppVersion}-win32
#endif
Compression=lzma
ChangesEnvironment=yes
SetupLogging=yes
ChangesAssociations=yes
#ifdef BAM_UTILS64
MinVersion=6.0
#else
MinVersion=5.1
#endif
VersionInfoVersion={#MyAppVersion}

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Types]
Name: "minimal" ; Description: "Minimal installation"
Name: "full" ; Description: "Full installation"
Name: "custom" ; Description: "Custom installation" ; Flags: iscustom

[Components]
Name: "core" ; Description: "Core components" ; Flags : fixed ; Types: full minimal custom
Name: "source" ; Description: "Source code" ; Types: full

[Files]
Source: "{#srcdir}\windows\{#GTK_INSTALLER_EXE}" ; Flags: deleteafterinstall ; DestDir: "{tmp}" ; Components: core 
Source: "{#MY_HOME}\bin\libgsl-0.dll" ; DestDir: "{app}\Lib" ; Components: core
Source: "{#MY_HOME}\bin\libgslcblas-0.dll" ; DestDir: "{app}\Lib" ; Components: core
Source: "{#MY_HOME}\bin\libplplotcxx.dll" ; DestDir: "{app}\Lib" ; Components: core
Source: "{#MY_HOME}\bin\libplplot.dll" ; DestDir: "{app}\Lib" ; Components: core
Source: "{#MY_HOME}\bin\libcsirocsa.dll" ; DestDir: "{app}\Lib" ; Components: core
Source: "{#MY_HOME}\bin\libqsastime.dll" ; DestDir: "{app}\Lib" ; Components: core

Source: "{#MY_HOME}\bin\chi2dat.exe" ; DestDir: "{app}\Bin" ; Components: core
Source: "{#MY_HOME}\bin\asc2athena.exe" ; DestDir: "{app}\Bin" ; Components: core

Source: "{#MY_HOME}\github\bam-utils\build\bam-utils-{#MyAppVersion}.tar.gz" ; DestDir: "{app}\Sources" ; Components: source

[Icons]
Name: "{group}\{cm:LaunchProgram,chi2dat}"; Filename: "{app}\Bin\chi2dat.exe"
Name: "{group}\{cm:LaunchProgram,asc2athena}"; Filename: "{app}\Bin\asc2athena.exe"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"

[Run]
Filename: "{tmp}\{#GTK_INSTALLER_EXE}" ; Parameters: "/sideeffects=no /dllpath=root /translations=no /S /D={app}\GTK3" ; StatusMsg: "Installing GTK3 runtime libraries..." ; Components: core

[UninstallRun]
Filename: "{app}\GTK3\gtk3_runtime_uninst.exe" ; Parameters: "/remove_config=yes /sideeffects=no /dllpath=root /translations=no /compatdlls=no /S" 

[UninstallDelete]
Type: filesandordirs ; Name: "{app}\GTK3"
Type: dirifempty ; Name: "{app}"

[Registry]
Root: HKLM; Subkey: "Software\BAM-utils" ; ValueType: string ; ValueName: "" ; ValueData: "{app}" ; Flags: uninsdeletekey 
Root: HKLM; Subkey: "Software\BAM-utils" ; ValueType: string ; ValueName: "InstallationDirectory" ; ValueData: "{app}"
Root: HKLM; Subkey: "Software\BAM-utils" ; ValueType: string ; ValueName: "Vendor" ; ValueData: "Tom Schoonjans"

Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\App Paths\chi2dat.exe" ; ValueType: string ; ValueName: "" ; ValueData: "{app}\Bin\chi2dat.exe" ; Flags: uninsdeletekey
Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\App Paths\chi2dat.exe" ; ValueType: string ; ValueName: "Path" ; ValueData: "{app}\Bin;{app}\Lib;{app}\GTK3"

Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\App Paths\asc2athena.exe" ; ValueType: string ; ValueName: "" ; ValueData: "{app}\Bin\asc2athena.exe" ; Flags: uninsdeletekey
Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\App Paths\asc2athena.exe" ; ValueType: string ; ValueName: "Path" ; ValueData: "{app}\Bin;{app}\Lib;{app}\GTK3"
