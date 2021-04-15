program BasicDemo;

uses
  Forms,
  CameraParamsUnit in '..\src\CameraParamsUnit.pas',
  MainUnit in '..\src\MainUnit.pas' {Form1},
  PixelTypeUnit in '..\src\PixelTypeUnit.pas',
  ToolFunctionUnit in '..\src\ToolFunctionUnit.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
