; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define BoostVer "cvs"
; #define BoostVer "1.33"
#define BoostName "Boost C++ Libraries"
#define BoostPublisher "Boost.org"
#define BoostURL "http://www.boost.org"
#define BoostExeName "bjam.exe"
#define BoostUrlName "boost.url"

[Setup]
AppName={#BoostName}
AppVerName={#BoostName} {#BoostVer}
AppPublisher={#BoostPublisher}
AppPublisherURL={#BoostURL}
AppSupportURL={#BoostURL}
AppUpdatesURL={#BoostURL}
DefaultDirName={sd}\Boost
DefaultGroupName={#BoostName}
DisableProgramGroupPage=true
OutputBaseFilename=boost-{#BoostVer}-setup
Compression=lzma
SolidCompression=true
MinVersion=0,4.0.1381
ShowLanguageDialog=no
WizardImageFile=compiler:wizmodernimage-is.bmp
WizardSmallImageFile=compiler:wizmodernsmallimage-is.bmp
DirExistsWarning=no
AppMutex=BoostORG
PrivilegesRequired=admin

[Files]
Source: ..\share\docbook-xml-4.2\*; DestDir: {app}\share\docbook-xml-4.2; Flags: ignoreversion recursesubdirs createallsubdirs; Components: Tools/BoostBook
Source: ..\share\docbook-xsl-1.68.1\*; DestDir: {app}\share\docbook-xsl-1.68.1; Flags: ignoreversion recursesubdirs createallsubdirs; Components: Tools/BoostBook
Source: dist\bin\bjam.exe; DestDir: {app}\bin; Flags: ignoreversion; Components: Tools/BoostJam
Source: dist\bin\zlib1.dll; DestDir: {app}\bin; Flags: ignoreversion; Components: Tools/BoostBook
Source: dist\bin\libexslt.dll; DestDir: {app}\bin; Flags: ignoreversion; Components: Tools/BoostBook
Source: dist\bin\libxml2.dll; DestDir: {app}\bin; Flags: ignoreversion; Components: Tools/BoostBook
Source: dist\bin\libxslt.dll; DestDir: {app}\bin; Flags: ignoreversion; Components: Tools/BoostBook
Source: dist\bin\xsltproc.exe; DestDir: {app}\bin; Flags: ignoreversion; Components: Tools/BoostBook
Source: dist\bin\iconv.dll; DestDir: {app}\bin; Flags: ignoreversion; Components: Tools/BoostBook
Source: dist\bin\wave.exe; DestDir: {app}\bin; Flags: ignoreversion; Components: Tools/Wave
Source: dist\bin\compiler_status.exe; DestDir: {app}\bin; Flags: ignoreversion; Components: " Tools/Regression"
Source: dist\bin\process_jam_log.exe; DestDir: {app}\bin; Flags: ignoreversion; Components: " Tools/Regression"
Source: dist\bin\quickbook.exe; DestDir: {app}\bin; Flags: ignoreversion; Components: Tools/BoostBook
Source: dist\bin\bcp.exe; DestDir: {app}\bin; Flags: ignoreversion; Components: Tools/BoostCP
Source: ..\share\boostbook\*; DestDir: {app}\share\boostbook; Flags: ignoreversion recursesubdirs createallsubdirs; Components: Tools/BoostBook

[INI]
Filename: {app}\{#BoostUrlName}; Section: InternetShortcut; Key: URL; String: {#BoostURL}

[Icons]
Name: {group}\{cm:ProgramOnTheWeb,{#BoostName}}; Filename: {app}\{#BoostUrlName}
Name: {group}\{cm:UninstallProgram,{#BoostName}}; Filename: {uninstallexe}

[UninstallDelete]
Type: files; Name: {app}\{#BoostUrlName}

[Components]
Name: Tools; Description: Tools; Types: full custom; Flags: checkablealone
Name: Tools/BoostJam; Description: Boost.Jam (BJam) build tool.; Flags: checkablealone; Types: full custom compact
Name: Tools/BoostBook; Description: Boost.Book and QuickBook tools.; Flags: checkablealone; Types: full custom
Name: Tools/BoostCP; Description: Boost.Copy subset extraction tool.; Flags: checkablealone; Types: full custom
Name: Tools/Wave; Description: Boost.Wave C99/C++ preprocessor.; Flags: checkablealone; Types: full custom
Name: Tools/Regression; Description: Boost regression testing tools.; Flags: checkablealone; Types: full custom

[Tasks]
Name: ModifyENV; Description: Modify environment variables.; Flags: checkablealone

[Registry]
Root: HKLM; Subkey: SYSTEM\CurrentControlSet\Control\Session Manager\Environment; ValueType: expandsz; ValueName: Path; ValueData: "{olddata};{app}\bin;{app}\lib"; Tasks: " ModifyENV"
Root: HKLM; Subkey: SOFTWARE\{#BoostPublisher}\{#BoostVer}; ValueType: string; ValueName: InstallRoot; ValueData: {app}; Flags: uninsdeletevalue; Components: 
