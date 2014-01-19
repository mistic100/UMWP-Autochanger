#define AppId "{401CCECB-FFCC-4AF3-846D-A535F59D0834}"
#define AppName "UMWP Autochanger"
#define AppVersion "1.4"
#define AppPublisher "StrangePlanet"
#define PublisherURL "http://www.strangeplanet.fr"
#define ProjectURL "http://www.strangeplanet.fr/work/umwp-autochanger"
#define AppExeName "UMWP Autochanger.exe"
#define DataRoot "data"

[Setup]
; app properties
AppId={{#AppId}
AppName={#AppName}
AppVersion={#AppVersion}
AppVerName={#AppName} {#AppVersion}
AppPublisher={#AppPublisher}
AppPublisherURL={#PublisherURL}
AppSupportURL={#ProjectURL}
AppUpdatesURL={#ProjectURL}

MinVersion=6.1

; directories
DefaultDirName={pf}\{#AppName}
DefaultGroupName={#AppName}
DisableDirPage=auto
DisableProgramGroupPage=auto

; readme
LicenseFile={#DataRoot}\gpl-3.0.txt
InfoAfterFile={#DataRoot}\readme.txt

; output
OutputDir=./
OutputBaseFilename=UMWP_Autochanger_{#AppVersion}_Setup
SetupIconFile={#DataRoot}\icon.ico
Compression=lzma2
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"

[CustomMessages]
english.DeleteSettings=Delete settings file?
french.DeleteSettings=Supprimer le fichier de configuration ?

english.NewerVersionExists=A newer version of {#AppName} is already installed.%n%nInstaller version: {#AppVersion}%nCurrent version: 
french.NewerVersionExists=Une version plus récente de {#AppName} existe déjà.%n%nVersion de l'installeur : {#AppVersion}%nVersion actuelle : 

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "{#DataRoot}\*"; DestDir: "{app}"; Flags: recursesubdirs createallsubdirs

[Icons]
Name: "{group}\{#AppName}"; Filename: "{app}\{#AppExeName}"; IconFilename: "{app}\icon.ico"
Name: "{group}\{cm:ProgramOnTheWeb,{#AppName}}"; Filename: "{#ProjectURL}"
Name: "{group}\{cm:UninstallProgram,{#AppName}}"; Filename: "{uninstallexe}"; IconFilename: "{app}\icon-uninst.ico"
Name: "{commondesktop}\{#AppName}"; Filename: "{app}\{#AppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#AppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(AppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: filesandordirs; Name: "{app}\cache";

[Code]
procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
  case CurUninstallStep of
    usUninstall:
      begin
        if MsgBox(ExpandConstant('{cm:DeleteSettings}'), mbConfirmation, MB_YESNO or MB_DEFBUTTON2) = IDYES then
          begin
             DelTree(ExpandConstant('{app}\settings.xml'), False, True, False);
          end
      end;
  end;
end; 

function InitializeSetup: Boolean;
var
  Version: String;
begin
  if RegValueExists(HKEY_LOCAL_MACHINE,'Software\Microsoft\Windows\CurrentVersion\Uninstall\{#AppId}_is1', 'DisplayVersion') then
    begin
      RegQueryStringValue(HKEY_LOCAL_MACHINE,'Software\Microsoft\Windows\CurrentVersion\Uninstall\{#AppId}_is1', 'DisplayVersion', Version);
      if Version > '{#AppVersion}' then
        begin
          MsgBox(ExpandConstant('{cm:NewerVersionExists} '+Version), mbInformation, MB_OK);
          Result := False;
        end
      else
        begin
          Result := True;
        end
    end
  else
    begin
      Result := True;
    end
end;