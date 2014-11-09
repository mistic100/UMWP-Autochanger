#define AppId "{401CCECB-FFCC-4AF3-846D-A535F59D0834}"
#define AppName "UMWP Autochanger"
#define AppVersion "1.8"
#define AppPublisher "StrangePlanet"
#define PublisherURL "http://www.strangeplanet.fr"
#define ProjectURL "http://www.strangeplanet.fr/work/umwp-autochanger"
#define AppExeName "UMWP Autochanger.exe"
#define SettingsFile "settings.xml"
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

; Windows 7
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
Name: "english"; MessagesFile: "compiler:Default.isl,lang\en_GB\custom.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl,lang\fr_FR\custom.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "autostart"; Description: "{cm:OptionAutostart,{#AppName}}"; GroupDescription: "{cm:Options}"

[Files]
Source: "{#DataRoot}\*"; DestDir: "{app}"; Flags: recursesubdirs createallsubdirs

[Icons]
Name: "{group}\{#AppName}"; Filename: "{app}\{#AppExeName}"; IconFilename: "{app}\icon.ico"
Name: "{group}\{cm:ProgramOnTheWeb,{#AppName}}"; Filename: "{#ProjectURL}"
Name: "{group}\{cm:UninstallProgram,{#AppName}}"; Filename: "{uninstallexe}"; IconFilename: "{app}\icon-uninst.ico"

Name: "{userdesktop}\{#AppName}"; Filename: "{app}\{#AppExeName}"; Tasks: desktopicon

Name: "{userstartup}\{#AppName}"; Filename: "{app}\{#AppExeName}"; Tasks: autostart

[Run]
Filename: "{app}\{#AppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(AppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[InstallDelete]
Type: filesandordirs; Name: "{app}\cache";
Type: files; Name: "{app}\debug.log";

[UninstallDelete]
Type: filesandordirs; Name: "{app}\cache";
Type: files; Name: "{app}\debug.log";

[Code]
// ask for delete config file during uninstall
procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
  case CurUninstallStep of
    usUninstall:
      begin
        if MsgBox(ExpandConstant('{cm:DeleteSettings}'), mbConfirmation, MB_YESNO or MB_DEFBUTTON2) = IDYES then
          begin
             DelTree(ExpandConstant('{app}\{#SettingsFile}'), False, True, False);
          end
      end;
  end;
end; 

// find current version before installation
function InitializeSetup: Boolean;
var Version: String;
begin
  if RegValueExists(HKEY_LOCAL_MACHINE,'Software\Microsoft\Windows\CurrentVersion\Uninstall\{#AppId}_is1', 'DisplayVersion') then
    begin
      RegQueryStringValue(HKEY_LOCAL_MACHINE,'Software\Microsoft\Windows\CurrentVersion\Uninstall\{#AppId}_is1', 'DisplayVersion', Version);
      if Version > '{#AppVersion}' then
        begin
          MsgBox(ExpandConstant('{cm:NewerVersionExists,{#AppName},{#AppVersion}} '+Version), mbInformation, MB_OK);
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

// mark installer for deletion if argument "-delete-installer" is present 
procedure DeinitializeSetup();
var i: integer;
begin
  for i:= 0 to ParamCount do
  begin
    if ParamStr(i) = '-delete-installer' then
    begin
        RestartReplace(ExpandConstant('{srcexe}'), '');
    end;
  end; 
end;

// link in installer footer
procedure URLLabelOnClick(Sender: TObject);
var
  ErrorCode: Integer;
begin
  ShellExec('open', '{#ProjectUrl}', '', '', SW_SHOWNORMAL, ewNoWait, ErrorCode);
end;

procedure InitializeWizard;
var
  URLLabel: TNewStaticText;
begin
  URLLabel := TNewStaticText.Create(WizardForm);
  URLLabel.Caption := ExpandConstant('{cm:Website}');
  URLLabel.Cursor := crHand;
  URLLabel.OnClick := @URLLabelOnClick;
  URLLabel.Parent := WizardForm;
  URLLabel.Font.Style := URLLabel.Font.Style + [fsUnderline];
  URLLabel.Font.Color := clBlue;
  URLLabel.Top := WizardForm.ClientHeight - URLLabel.Height - 15;
  URLLabel.Left := ScaleX(20);
end;
