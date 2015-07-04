#include ReadReg(HKEY_LOCAL_MACHINE,'Software\Sherlock Software\InnoTools\Downloader','ScriptPath','')

#define MyAppId "BAM-utils"
#define srcdir abs_top_srcdir_win
#define builddir abs_top_builddir_win
#define MY_HOME "C:\msys\1.0\home\schoon\"

#ifdef BAM_UTILS64
  #define MyAppName "BAM-utils 64-bit"
  #define GTK_INSTALLER_EXE "gtk3-runtime-3.14.13-2015-07-03-ts-win64.exe"
  #define MY_MINGW "C:\TDM-GCC-64\"
#else
  #define MyAppName "BAM-utils 32-bit"
  #define GTK_INSTALLER_EXE "gtk2-runtime-2.24.8-2011-12-03-ash.exe"
  #define MY_MINGW "C:\MinGW32\"
#endif

#define MyAppPublisher "Tom Schoonjans"
#define MyAppURL "http://github.com/tschoonj/bam-utils"
#define XRAYLIB_VERSION '3.1.0'
#define XRAYLIB_VERSION_MIN '3.1.0'

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

Source: "{#MY_HOME}\share\plplot5.11.0\*.*" ; DestDir: "{app}\Share\plplot" ; Components: core

Source: "{#MY_HOME}\github\bam-utils\build\bam-utils-{#MyAppVersion}.tar.gz" ; DestDir: "{app}\Sources" ; Components: source

[Icons]
Name: "{group}\{cm:LaunchProgram,chi2dat}"; Filename: "{app}\Bin\chi2dat.exe"
Name: "{group}\{cm:LaunchProgram,asc2athena}"; Filename: "{app}\Bin\asc2athena.exe"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"

[Run]
Filename: "{tmp}\{#GTK_INSTALLER_EXE}" ; Parameters: "/sideeffects=no /dllpath=root /translations=no /S /D={app}\GTK3" ; StatusMsg: "Installing GTK3 runtime libraries..." ; Components: core
Filename: "{tmp}\xraylib-{#XRAYLIB_VERSION}.exe" ; Parameters: "/VERYSILENT /SP- /SUPPRESSMSGBOXES" ; Flags: skipifdoesntexist ; StatusMsg: "Installing xraylib..."

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



[Code]

//taken from http://blog.lextudio.com/2007/08/inno-setup-script-sample-for-version-comparison-2/

function GetNumber(var temp: String): Integer;
var
  part: String;
  pos1: Integer;
begin
  if Length(temp) = 0 then
  begin
    Result := -1;
    Exit;
  end;
    pos1 := Pos('.', temp);
    if (pos1 = 0) then
    begin
      Result := StrToInt(temp);
    temp := '';
    end
    else
    begin
    part := Copy(temp, 1, pos1 - 1);
      temp := Copy(temp, pos1 + 1, Length(temp));
      Result := StrToInt(part);
    end;
end;
 
function CompareInner(var temp1, temp2: String): Integer;
var
  num1, num2: Integer;
begin
    num1 := GetNumber(temp1);
  num2 := GetNumber(temp2);
  if (num1 = -1) or (num2 = -1) then
  begin
    Result := 0;
    Exit;
  end;
      if (num1 > num2) then
      begin
        Result := 1;
      end
      else if (num1 < num2) then
      begin
        Result := -1;
      end
      else
      begin
        Result := CompareInner(temp1, temp2);
      end;
end;
 
function CompareVersion(str1, str2: String): Integer;
var
  temp1, temp2: String;
begin
    temp1 := str1;
    temp2 := str2;
    Result := CompareInner(temp1, temp2);
end;
procedure InitializeWizard();
var subkeyName: String;
var value: String;
  
begin
  ITD_Init;

  //do we need to install or update xraylib?
  Log('Checking for xraylib');
  value := '';
#ifdef BAM_UTILS64
  subkeyName := ExpandConstant('Software\Microsoft\Windows\CurrentVersion\Uninstall\xraylib_64_is1');
  RegQueryStringValue(HKLM, subkeyName, 'DisplayVersion', value);

#else
  subkeyName := ExpandConstant('Software\Microsoft\Windows\CurrentVersion\Uninstall\xraylib_is1');
  if not RegQueryStringValue(HKLM, subkeyName, 'DisplayVersion', value) then
    begin
    subkeyName := ExpandConstant('Software\Microsoft\Windows\CurrentVersion\Uninstall\xraylib');
    RegQueryStringValue(HKLM, subkeyName, 'DisplayVersion', value);
  end;
#endif

  if (((value <> '') and (CompareVersion(value, '{#XRAYLIB_VERSION_MIN}') < 0)) or (value = '')) then
  begin
     //xraylib was not found or too old
#ifdef BAM_UTILS64
     ITD_AddFile(ExpandConstant('http://lvserver.ugent.be/xraylib/xraylib-{#XRAYLIB_VERSION}-win64.exe'), ExpandConstant('{tmp}\xraylib-{#XRAYLIB_VERSION}.exe'));
     ITD_AddMirror(ExpandConstant('http://xraylib.s3.amazonaws.com/xraylib-{#XRAYLIB_VERSION}-win64.exe'), ExpandConstant('{tmp}\xraylib-{#XRAYLIB_VERSION}.exe'));
     ITD_AddMirror(ExpandConstant('http://10.0.2.2/~schoon/xraylib-{#XRAYLIB_VERSION}-win64.exe'), ExpandConstant('{tmp}\xraylib-{#XRAYLIB_VERSION}.exe'));
#else
     ITD_AddFile(ExpandConstant('http://lvserver.ugent.be/xraylib/xraylib-{#XRAYLIB_VERSION}-win32.exe'), ExpandConstant('{tmp}\xraylib-{#XRAYLIB_VERSION}.exe'));
     ITD_AddMirror(ExpandConstant('http://xraylib.s3.amazonaws.com/xraylib-{#XRAYLIB_VERSION}-win32.exe'), ExpandConstant('{tmp}\xraylib-{#XRAYLIB_VERSION}.exe'));
     ITD_AddMirror(ExpandConstant('http://10.0.2.2/~schoon/xraylib-{#XRAYLIB_VERSION}-win32.exe'), ExpandConstant('{tmp}\xraylib-{#XRAYLIB_VERSION}.exe'));
#endif
     ITD_DownloadAfter(wpReady);
  end;

end;
