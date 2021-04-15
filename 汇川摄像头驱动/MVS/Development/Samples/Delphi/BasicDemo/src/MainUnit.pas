unit MainUnit;

interface

uses
  TypInfo, Windows, Math, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls, CameraParamsUnit, ToolFunctionUnit;

type
  TForm1 = class(TForm)
    ENUM_BUTTON: TButton;
    OPEN_BUTTON: TButton;
    CLOSE_BUTTON: TButton;
    DEVICE_COMBO: TComboBox;
    INIT_GroupBox: TGroupBox;
    IMAGE_GRAB_GroupBox: TGroupBox;
    START_GRABBING_BUTTON: TButton;
    STOP_GRABBING_BUTTON: TButton;
    DISPLAY_Panel: TPanel;
    PARAMETER_GroupBox: TGroupBox;
    EXPOSURE_StaticText: TStaticText;
    GAIN_StaticText: TStaticText;
    EXPOSURE_Edit: TEdit;
    GAIN_Edit: TEdit;
    GET_PARAMETER_BUTTON: TButton;
    SET_PARAMETER_BUTTON: TButton;
    CONTINUS_MODE_RadioButton: TRadioButton;
    TRIGGER_MODE_RadioButton: TRadioButton;
    SOFTWARE_TRIGGER_CheckBox: TCheckBox;
    SOFTWARE_ONCE_BUTTON: TButton;
    SAVE_IMAGE_GroupBox: TGroupBox;
    SAVE_BMP_BUTTON: TButton;
    SAVE_JPG_BUTTON: TButton;
    procedure ENUM_BUTTONClick(Sender: TObject);
    procedure OPEN_BUTTONClick(Sender: TObject);
    procedure CLOSE_BUTTONClick(Sender: TObject);
    procedure START_GRABBING_BUTTONClick(Sender: TObject);
    procedure STOP_GRABBING_BUTTONClick(Sender: TObject);
    procedure GET_PARAMETER_BUTTONClick(Sender: TObject);
    procedure SET_PARAMETER_BUTTONClick(Sender: TObject);
    procedure OnClose(Sender: TObject; var Action: TCloseAction);
    procedure OnCreat(Sender: TObject);
    procedure OnBnClickedContinusModeRadio(Sender: TObject);
    procedure OnBnClickedTriggerModeRadio(Sender: TObject);
    procedure OnBnClickedSoftwareTriggerCheck(Sender: TObject);
    procedure OnBnClickedSoftwareOnceButton(Sender: TObject);
    procedure SAVE_BMP_BUTTONClick(Sender: TObject);
    procedure SAVE_JPG_BUTTONClick(Sender: TObject);

  private
    { Private declarations }
    function UpdateVars(bUpdateDir: Bool): Integer;  // ch:更新关联的变量和控件 | en:Update associated variables and controls
    function EnableControls(bIsCameraReady: Bool): Integer;
    //ch:设置、获取参数操作 | en:Set and get parameters operation
    function GetTriggerMode(): Integer;  // ch:设置触发模式 | en:Set Trigger Mode
    function SetTriggerMode(): Integer;
    function GetExposureTime(): Integer;   // ch:设置曝光时间 | en:Set Exposure Time
    function SetExposureTime(): Integer;
    function GetGain(): Integer;    // ch:设置增益 | en:Set Gain
    function SetGain(): Integer;
    function GetTriggerSource(): Integer;   // ch:设置触发源 | en:Set Trigger Source
    function SetTriggerSource(): Integer;
    function SaveImage(): Integer;                     // ch:保存图片 | en:Save Image

  public
    { Public declarations }
  end;


var
  Form1: TForm1;


implementation

{$R *.dfm}

var
  m_nRet : Integer;     // ch:错误码 | en:Error code
  m_stDevList: MV_CC_DEVICE_INFO_LIST;    // ch:设备信息列表结构体变量，用来存储设备列表 | //en:Device information list structure variable used to store device list
  m_pstDevList: PMV_CC_DEVICE_INFO_LIST;    // ch:设备信息列表结构体变量，用来存储设备列表 | //en:Device information list structure variable used to store device list
  m_hDevHandle: PPointer = Nil;     // ch:打开的设备句柄 | en:Current device Handle

  m_bOpenDevice: Bool = False;     // ch:是否打开设备 | en:Whether to open device
  m_bStartGrabbing: Bool = False;    // ch:是否开始抓图 | en:Whether to start grabbing
  m_bSoftWareTriggerCheck: Bool = False;    // ch:软触发是否勾选 | en:Whether SoftwareTrigger was checked
  m_nTriggerMode: Integer = 0;     // ch:触发模式 | en:Trigger Mode

  m_nSaveImageType: MV_SAVE_IAMGE_TYPE; //c h:保存图像格式 | en:Save Image Type
  m_pBufForSaveImage: PAnsiChar = Nil;   // ch:用于保存图像的缓存 | en:Buffer to save image
  m_nBufSizeForSaveImage: Cardinal = 0;
  m_pBufForDriver: PAnsiChar = Nil; // ch:用于从驱动获取图像的缓存 | en:Buffer to get image from driver
  m_nBufSizeForDriver: Cardinal = 0;

  m_hwndDisplay: HWND;     // ch:显示句柄 | en:Display Handle

// ch:模拟MFC的DoDataExchange，更新关联的控件和变量
// en:Update associated variables and controls
function TForm1.UpdateVars(bUpdateDir: Bool): Integer;
begin
    if bUpdateDir then
    begin
      // 从控件更新变量
      if SOFTWARE_TRIGGER_CheckBox.Checked then
      begin
          m_bSoftWareTriggerCheck := true;
      end
      else
      begin
        m_bSoftWareTriggerCheck := false;
      end;
    end
    else
    begin
      // 从变量更新控件
      if m_bSoftWareTriggerCheck then
      begin
          SOFTWARE_TRIGGER_CheckBox.Checked := true;
      end
      else
      begin
        SOFTWARE_TRIGGER_CheckBox.Checked := false;
      end;
    end;
end;

// ch:按钮使能 | en:Enable control
function TForm1.EnableControls( bIsCameraReady: Bool ): Integer;
begin
  if m_bOpenDevice then
  begin
    OPEN_BUTTON.Enabled := False;
    SOFTWARE_TRIGGER_CheckBox.Enabled := True;
    EXPOSURE_Edit.Enabled := True;
    GAIN_Edit.Enabled := True;
    //FRAME_RATE_Edit.Enabled := True;
    GET_PARAMETER_BUTTON.Enabled := True;
    SET_PARAMETER_BUTTON.Enabled := True;
    CONTINUS_MODE_RadioButton.Enabled := True;
    TRIGGER_MODE_RadioButton.Enabled := True;
    // ch: 关闭设备 | en:CLOSE_BUTTON
    if bIsCameraReady then
    begin
      CLOSE_BUTTON.Enabled := True;
    end
    else
    begin
      CLOSE_BUTTON.Enabled := False;
    end;
  end
  else
  begin
    CLOSE_BUTTON.Enabled := False;
    SOFTWARE_TRIGGER_CheckBox.Enabled := False;
    EXPOSURE_Edit.Enabled := False;
    GAIN_Edit.Enabled := False;
    GET_PARAMETER_BUTTON.Enabled := False;
    SET_PARAMETER_BUTTON.Enabled := False;
    CONTINUS_MODE_RadioButton.Enabled := False;
    TRIGGER_MODE_RadioButton.Enabled := False;
    // ch: 打开设备 | en:OPEN_BUTTON
    if bIsCameraReady then
    begin
      OPEN_BUTTON.Enabled := True;
    end
    else
    begin
      OPEN_BUTTON.Enabled := False;
    end;
  end;

  if m_bStartGrabbing then
  begin
    STOP_GRABBING_BUTTON.Enabled := True;
    SAVE_BMP_BUTTON.Enabled := True;
    SAVE_JPG_BUTTON.Enabled := True;
    // ch: 开始取流按钮 | en:START_GRABBING_BUTTON
    if bIsCameraReady then
    begin
      START_GRABBING_BUTTON.Enabled := False;
    end
    else if m_bOpenDevice then
     begin
       START_GRABBING_BUTTON.Enabled := True;
      end
      else
      begin
        START_GRABBING_BUTTON.Enabled := False;
     end;
    // ch: 软触发按钮 | en:SOFTWARE_ONCE_BUTTON
    if m_bSoftWareTriggerCheck then
    begin
      SOFTWARE_ONCE_BUTTON.Enabled := True;
    end
    else
    begin
      SOFTWARE_ONCE_BUTTON.Enabled := False;
    end;
  end
  else
  begin
    STOP_GRABBING_BUTTON.Enabled := False;
    SAVE_BMP_BUTTON.Enabled := False;
    SAVE_JPG_BUTTON.Enabled := False;
    SOFTWARE_ONCE_BUTTON.Enabled := False;
    if m_bOpenDevice then
    begin
      START_GRABBING_BUTTON.Enabled := True;
    end
    else
    begin
      START_GRABBING_BUTTON.Enabled := False;
    end;
  end;

  Result := MV_OK;
end;


// ch:按下查找设备按钮:枚举 | en:Click Find Device button:Enumeration
procedure TForm1.ENUM_BUTTONClick(Sender: TObject);
var
  pDeviceInfo : ^MV_CC_DEVICE_INFO;
  strInfoToShow : string;
  nLoopID : Integer;

begin
  // ch:清除设备列表框中的信息 | en:Clear Device List Information
  DEVICE_COMBO.Clear();
  ZeroMemory(@m_stDevList, sizeof(MV_CC_DEVICE_INFO_LIST));

  // ch:枚举子网内所有设备 | en:Enumerate all devices within subnet
  m_nRet := MV_CC_EnumDevices(MV_GIGE_DEVICE or MV_USB_DEVICE, m_stDevList);
  if m_nRet<>MV_OK then
  begin
    ShowMessage( 'Enum devices Failed.' + IntToHex(m_nRet,8) );
    exit
  end;

  // ch:将值加入到信息列表框中并显示出来 | en:Add value to the information list box and display
  for nLoopID:=0 to m_stDevList.nDeviceNum-1 do
  begin
    pDeviceInfo := @m_stDevList.pDeviceInfo[nLoopID].nMajorVer;
    if pDeviceInfo=nil then
    begin
      continue;
    end;
    if pDeviceInfo.nTLayerType = MV_GIGE_DEVICE then
    begin
      GigeDeviceInfoToShow(pDeviceInfo^, strInfoToShow);
    end
    else if pDeviceInfo.nTLayerType = MV_USB_DEVICE then
      begin
        USB3DeviceInfoToShow(pDeviceInfo^, strInfoToShow);
      end
      else
      begin
        ShowMessage( 'Unknown device enumerated.' );
      end;
    strInfoToShow := '[' + IntToStr(nLoopID) + '] ' + strInfoToShow;
    DEVICE_COMBO.Items.Add(strInfoToShow);
  end;

  if (m_stDevList.nDeviceNum=0) then
  begin
    ShowMessage( 'No device' );
  end;
  DEVICE_COMBO.ItemIndex := 0;  //default selection
  EnableControls(True);
end;


// ch:按下打开设备按钮：打开设备 | en:Click Open button: Open Device
procedure TForm1.OPEN_BUTTONClick(Sender: TObject);
var
  handle: Pointer;
  nIndex: Word;
  nPacketSize: Integer;

begin
  UpdateVars(TRUE);
  nIndex := DEVICE_COMBO.ItemIndex;

  if m_bOpenDevice then
  begin
    m_nRet := MV_E_CALLORDER;
    ShowMessage( 'Execution order error.' + IntToHex(m_nRet,8) );
  end;

  // ch:由设备信息创建设备实例 | en:Device instance created by device information
  m_hDevHandle := @handle;
  if m_stDevList.pDeviceInfo[nIndex]=Nil then
  begin
    ShowMessage( 'Device does not exist.' );
    exit
  end;

  m_nRet := MV_CC_CreateHandle(m_hDevHandle, (m_stDevList.pDeviceInfo[nIndex])^);
  if m_nRet<>MV_OK then
  begin
    ShowMessage( 'Create handle Failed.' + IntToHex(m_nRet,8) );
    exit
  end;

  // ch: 打开设备 | en:Open Device
  m_nRet := MV_CC_OpenDevice(m_hDevHandle^);
  if m_nRet<>MV_OK then
  begin
    ShowMessage( 'Open Fail.' + IntToHex(m_nRet,8) );
    exit
  end
  else
  begin
    m_bOpenDevice := True;
    GET_PARAMETER_BUTTONClick(Sender);
    EnableControls(TRUE);
  end;

  // ch:探测网络最佳包大小(只对GigE相机有效) | en:Detection network optimal package size(It only works for the GigE camera)
  if m_stDevList.pDeviceInfo[nIndex].nTLayerType = MV_GIGE_DEVICE then
  begin
    nPacketSize := MV_CC_GetOptimalPacketSize(m_hDevHandle^);
    if nPacketSize > 0 then
    begin
      m_nRet := SetIntValue(m_hDevHandle, 'GevSCPSPacketSize', nPacketSize);
      if m_nRet<>MV_OK then
      begin
        ShowMessage( 'Warning: Set Packet Size fail!.' + IntToHex(m_nRet,8) );
      end
    end
    else
    begin
      ShowMessage( 'Warning: Get Packet Size fail!' + IntToStr(nPacketSize) );
    end;
  end;

end;


// ch:按下关闭设备按钮：关闭设备 | en:Click Close button: Close Device
procedure TForm1.CLOSE_BUTTONClick(Sender: TObject);
begin
  if Nil = m_hDevHandle then
  begin
    m_nRet := MV_E_PARAMETER;
  end;

  m_nRet := MV_CC_CloseDevice(m_hDevHandle^);

  m_nRet := MV_CC_DestroyHandle(m_hDevHandle^);
  m_hDevHandle := Nil;

  m_bOpenDevice := False;
  m_bStartGrabbing := False;

  StrDispose(m_pBufForDriver);
  m_pBufForDriver := Nil;
  m_nBufSizeForDriver := 0;

  StrDispose(m_pBufForSaveImage);
  m_pBufForSaveImage := Nil;
  m_nBufSizeForSaveImage := 0;

  EnableControls(TRUE);
end;


// ch:获取触发模式 | en:Get Trigger Mode
function TForm1.GetTriggerMode(): Integer;
Var
  nEnumValue: Cardinal;
begin
  Result := GetEnumValue(m_hDevHandle, 'TriggerMode', @nEnumValue);
  if Result<>MV_OK then
  begin
    exit
  end;
  m_nTriggerMode := nEnumValue;
end;


// ch:设置触发模式 | en:Set Trigger Mode
function TForm1.SetTriggerMode(): Integer;
begin
  Result := SetEnumValue(m_hDevHandle, 'TriggerMode', m_nTriggerMode);
end;


// ch:获取曝光时间 | en:Get Exposure Time
function TForm1.GetExposureTime(): Integer;
Var
  fFloatValue: Single;
begin
  Result := GetFloatValue(m_hDevHandle, 'ExposureTime', @fFloatValue);
  if Result<>MV_OK then
  begin
    exit
  end;
  EXPOSURE_Edit.Text := FloatToStr(fFloatValue);
end;


// ch:设置曝光时间 | en:Set Exposure Time
function TForm1.SetExposureTime(): Integer;
begin
  SetEnumValue(m_hDevHandle, 'ExposureMode', TypInfo.GetEnumValue(TypeInfo(MV_CAM_EXPOSURE_MODE),'MV_EXPOSURE_MODE_TIMED'));
  SetEnumValue(m_hDevHandle, 'ExposureAuto', TypInfo.GetEnumValue(TypeInfo(MV_CAM_EXPOSURE_AUTO_MODE),'MV_EXPOSURE_AUTO_MODE_OFF'));
  Result := SetFloatValue(m_hDevHandle, 'ExposureTime', StrToFloat(EXPOSURE_Edit.Text));
end;


// ch:获取增益 | en:Get Gain
function TForm1.GetGain(): Integer;
Var
  fFloatValue: Single;
begin
  Result := GetFloatValue(m_hDevHandle, 'Gain', @fFloatValue);
  if Result<>MV_OK then
  begin
    exit
  end;
  GAIN_Edit.Text := FormatFloat('0.000',fFloatValue);
end;


// ch:设置增益 | en:Set Gain
function TForm1.SetGain(): Integer;
begin
  SetEnumValue(m_hDevHandle, 'GainAuto', 0);
  Result := SetFloatValue(m_hDevHandle, 'Gain', StrToFloat(GAIN_Edit.Text));
end;

// ch:获取触发源 | en:Get Trigger Source
function TForm1.GetTriggerSource(): Integer;
Var
  nEnumValue: Cardinal;
begin
  Result := GetEnumValue(m_hDevHandle, 'TriggerSource', @nEnumValue);
  if Result<>MV_OK then
  begin
    exit
  end;

  if TypInfo.GetEnumValue(TypeInfo(MV_CAM_TRIGGER_SOURCE),'MV_TRIGGER_SOURCE_SOFTWARE') = nEnumValue then   // Todo:如何用常量代替
  begin
    SOFTWARE_TRIGGER_CheckBox.Checked := True;
  end
  else
  begin
    SOFTWARE_TRIGGER_CheckBox.Checked := False;
  end;
end;


// ch:设置触发源 | en:Set Trigger Source
function TForm1.SetTriggerSource(): Integer;
begin
  if m_bSoftWareTriggerCheck then
  begin
    Result := SetEnumValue(m_hDevHandle, 'TriggerSource', TypInfo.GetEnumValue(TypeInfo(MV_CAM_TRIGGER_SOURCE),'MV_TRIGGER_SOURCE_SOFTWARE'));
    if Result<>MV_OK then
    begin
      exit
    end;
      SOFTWARE_ONCE_BUTTON.Enabled := True;
  end
  else
  begin
    Result := SetEnumValue(m_hDevHandle, 'TriggerSource', TypInfo.GetEnumValue(TypeInfo(MV_CAM_TRIGGER_SOURCE),'MV_TRIGGER_SOURCE_LINE0'));
    if Result<>MV_OK then
    begin
      exit
    end;
    SOFTWARE_ONCE_BUTTON.Enabled := False;
  end;
end;


// ch:按下连续模式按钮 | en:Click Continues button
procedure TForm1.OnBnClickedContinusModeRadio(Sender: TObject);
begin
    CONTINUS_MODE_RadioButton.Checked := True;
    TRIGGER_MODE_RadioButton.Checked := False;
    m_nTriggerMode := TypInfo.GetEnumValue(TypeInfo(MV_CAM_TRIGGER_MODE),'MV_TRIGGER_MODE_OFF');
    m_nRet := SetTriggerMode();
    if m_nRet<>MV_OK then
    begin
     ShowMessage( 'Set TriggerMode Fail.' + IntToHex(m_nRet,8) );
     exit
    end;
    SOFTWARE_ONCE_BUTTON.Enabled := False;
end;


// ch:按下触发模式按钮 | en:Click Trigger Mode button
procedure TForm1.OnBnClickedTriggerModeRadio(Sender: TObject);
begin
    CONTINUS_MODE_RadioButton.Checked := False;
    TRIGGER_MODE_RadioButton.Checked := True;
    m_nTriggerMode := TypInfo.GetEnumValue(TypeInfo(MV_CAM_TRIGGER_MODE),'MV_TRIGGER_MODE_ON');
    m_nRet := SetTriggerMode();
    if m_nRet<>MV_OK then
    begin
     ShowMessage( 'Set TriggerMode Fail.' + IntToHex(m_nRet,8) );
     exit
    end;
    if m_bStartGrabbing and SOFTWARE_TRIGGER_CheckBox.Checked then
    begin
       SOFTWARE_ONCE_BUTTON.Enabled := True;
    end;

end;


// ch:按下开始采集按钮 | en:Click Start button
procedure TForm1.START_GRABBING_BUTTONClick(Sender: TObject);
begin
  if (m_bOpenDevice = False) or (m_bStartGrabbing = True) then
  begin
    exit
  end;

  m_nRet := MV_CC_StartGrabbing(m_hDevHandle^);
  if m_nRet<>MV_OK then
  begin
    ShowMessage( 'Start grabing Fail.' + IntToHex(m_nRet,8) );
  end
  else
  begin
    m_nRet := MV_CC_Display(m_hDevHandle^, m_hwndDisplay);
    if m_nRet<>MV_OK then
      begin
        ShowMessage( 'Display Fail.' + IntToHex(m_nRet,8) );
      end
    else
      begin
        m_bStartGrabbing := True;
        EnableControls(TRUE);
      end;
  end;
end;


// ch:按下结束采集按钮 | en:Click Stop button
procedure TForm1.STOP_GRABBING_BUTTONClick(Sender: TObject);
begin
  if (m_bOpenDevice = False) or (m_bStartGrabbing = False) then
  begin
    exit
  end;

  m_nRet := MV_CC_StopGrabbing(m_hDevHandle^);
  if m_nRet<>MV_OK then
  begin
    exit
  end
  else
  begin
    m_bStartGrabbing := False;
    EnableControls(TRUE);
  end;
end;


// ch:按下获取参数按钮 | en:Click Get Parameter button
procedure TForm1.GET_PARAMETER_BUTTONClick(Sender: TObject);
begin
  // ch:获取触发模式 | en:Get Trigger Mode
  m_nRet := GetTriggerMode();
  if m_nRet<>MV_OK then
  begin
    ShowMessage( 'Get TriggerMode Fail.' + IntToHex(m_nRet,8) );
  end
  else
  begin
    if m_nTriggerMode = TypInfo.GetEnumValue(TypeInfo(MV_CAM_TRIGGER_MODE),'MV_TRIGGER_MODE_ON') then
    begin
      OnBnClickedTriggerModeRadio(Sender);
    end
    else if m_nTriggerMode = TypInfo.GetEnumValue(TypeInfo(MV_CAM_TRIGGER_MODE),'MV_TRIGGER_MODE_OFF') then
      begin
        OnBnClickedContinusModeRadio(Sender);
      end
      else
      begin
        ShowMessage( 'Unsupport TriggerMode.' );
      end;
  end;

  // ch:获取曝光时间 | en:Get Exposure Time
  m_nRet := GetExposureTime();
  if m_nRet<>MV_OK then
  begin
    ShowMessage( 'Get ExposureTime Fail.' + IntToHex(m_nRet,8) );
  end;

  // ch:获取增益 | en:Get Gain
  m_nRet := GetGain();
  if m_nRet<>MV_OK then
  begin
    ShowMessage( 'Get Gain Fail.' + IntToHex(m_nRet,8) );
  end;

  // ch:获取触发源 | en:Get Trigger Source
  m_nRet := GetTriggerSource();
  if m_nRet<>MV_OK then
  begin
    ShowMessage( 'Get Trigger Source Fail.' + IntToHex(m_nRet,8) );
  end;

end;


// ch:按下设置参数按钮 | en:Click Set Parameter button
procedure TForm1.SET_PARAMETER_BUTTONClick(Sender: TObject);
Var
  bIsSetSucceed: Bool;

begin
  bIsSetSucceed := True;

  // ch:设置曝光时间 | en:Set Exposure Time
  m_nRet := SetExposureTime();
  if m_nRet<>MV_OK then
  begin
    bIsSetSucceed := False;
    ShowMessage( 'Set Exposure Time Fail.' + IntToHex(m_nRet,8) );
  end;

  // ch:设置增益 | en:Set Gain
  m_nRet := SetGain();
  if m_nRet<>MV_OK then
  begin
    bIsSetSucceed := False;
    ShowMessage( 'Set Gain Fail.' + IntToHex(m_nRet,8) );
  end;

  if bIsSetSucceed then
  begin
    ShowMessage( 'Set Parameter Succeed' );
  end;
end;


// ch:按下软触发按钮 | en:Click Software button
procedure TForm1.OnBnClickedSoftwareTriggerCheck(Sender: TObject);
begin
  if SOFTWARE_TRIGGER_CheckBox.Checked  then
  begin
    m_bSoftWareTriggerCheck := true;
  end
  else
  begin
    m_bSoftWareTriggerCheck := false;
  end;

  m_nRet := SetTriggerSource();
  if m_nRet<>MV_OK then
  begin
    ShowMessage( 'Set Trigger Source Fail.' + IntToHex(m_nRet,8) );
  end;
end;


// ch:按下软触发一次按钮 | en:Click Execute button
procedure TForm1.OnBnClickedSoftwareOnceButton(Sender: TObject);
begin
  if m_bStartGrabbing then
  begin
    m_nRet := SetCommandValue(m_hDevHandle, 'TriggerSoftware');
  end;
end;


// ch:保存图片 | en:Save Image
function TForm1.SaveImage(): Integer;
Var
  nRecvBufSize: Cardinal;
  stImageInfo: MV_FRAME_OUT_INFO_EX;
  stParam: MV_SAVE_IMAGE_PARAM_EX;
  chImageName: String;  //ch: 图像文件名 | en:ImageName
  hFile: THandle; // ch:文件句柄 | en: handle of the imageFile
  dwWriteLen: Cardinal; //ch:实际写入图像文件的长度 | en:Length that was written in the imageFile

begin
  if not m_bStartGrabbing then
  begin
    Result := MV_E_CALLORDER;
    exit
  end;

  // ch:文件格式判断 | en: SaveImageType whether supported
  if not ((MV_Image_Bmp=m_nSaveImageType) or (MV_Image_Jpeg=m_nSaveImageType)) then
  begin
    Result := MV_E_SUPPORT;
    exit
  end;

  nRecvBufSize := 0;
  if Nil = m_pBufForDriver then
  begin
     // ch:从相机中获取一帧图像大小 | en:Get size of one frame from camera
     Result := GetIntValue(m_hDevHandle, 'PayloadSize', @nRecvBufSize);
     if Result<>MV_OK then
     begin
       ShowMessage( 'failed in get PayloadSize.' + IntToHex(Result,8) );
       exit
     end;
     m_nBufSizeForDriver := nRecvBufSize;
     m_pBufForDriver := AnsiStrAlloc(m_nBufSizeForDriver);
     if (Nil=m_pBufForDriver) or (not(StrBufSize(m_pBufForDriver)>0)) then
     begin
       ShowMessage( 'malloc m_pBufForDriver failed, run out of memory.' + IntToStr(m_nBufSizeForDriver) );
       exit
     end;
   end;

   // ch:开始取图 | en:Start get one frame
   ZeroMemory(@stImageInfo, sizeof(MV_FRAME_OUT_INFO_EX));
   Result := MV_CC_GetOneFrameTimeout(m_hDevHandle^, m_pBufForDriver, m_nBufSizeForDriver, @stImageInfo, 1000);
   if Result=MV_OK then
   begin
     if Nil = m_pBufForSaveImage then
     begin
       // ch:BMP图片大小：width * height * 3 + 2048(预留BMP头大小)
       // en:BMP image size: width * height * 3 + 2048 (Reserved BMP header size)
       m_nBufSizeForSaveImage := stImageInfo.nWidth * stImageInfo.nHeight * 3 + 2048;
       m_pBufForSaveImage := AnsiStrAlloc(m_nBufSizeForSaveImage);
       if (Nil=m_pBufForSaveImage) or (not(StrBufSize(m_pBufForSaveImage)>0)) then
       begin
         ShowMessage( 'malloc m_pBufForSaveImage failed, run out of memory.' + IntToStr(m_nBufSizeForSaveImage) );
         exit
       end;
     end;

     // ch:设置对应的相机参数 | en:Set camera parameter
     ZeroMemory(@stParam, sizeof(MV_SAVE_IMAGE_PARAM_EX));
     stParam.enImageType := m_nSaveImageType; // ch:需要保存的图像类型 | en:Image format to save
     stParam.enPixelType := stImageInfo.enPixelType;  // ch:相机对应的像素格式 | en:Camera pixel type
     stParam.nWidth := stImageInfo.nWidth;         // ch:相机对应的宽 | en:Width
     stParam.nHeight := stImageInfo.nHeight;          // ch:相机对应的高 | en:Height
     stParam.nDataLen := stImageInfo.nFrameLen;
     stParam.pData := m_pBufForDriver;
     stParam.pImageBuffer := m_pBufForSaveImage;
     stParam.nBufferSize := m_nBufSizeForSaveImage;  // ch:存储节点的大小 | en:Buffer node size
     stParam.nJpgQuality := 80;       // ch:jpg编码，仅在保存Jpg图像时有效。保存BMP时SDK内忽略该参数
                                                // en:jpg encoding, only valid when saving as Jpg. SDK ignore this parameter when saving as BMP
     Result := MV_CC_SaveImageEx2(m_hDevHandle^, @stParam);
     if Result<>MV_OK then exit;

     // 将文件存入本地 | en:Save imageFile
     if MV_Image_Bmp=stParam.enImageType then
     begin
       chImageName := FormatCName('%s%d%s%d%s%03d%s', 'Image_w', stImageInfo.nWidth, '_h', stImageInfo.nHeight, '_fn', stImageInfo.nFrameNum, '.bmp');
     end
     else if True then
          begin
            chImageName := FormatCName('%s%d%s%d%s%03d%s', 'Image_w', stImageInfo.nWidth, '_h', stImageInfo.nHeight, '_fn', stImageInfo.nFrameNum, '.jpg');
          end
          else
          begin
            Result := MV_E_SUPPORT;
          end;
     hFile:= CreateFileA(PAnsiChar(AnsiString(chImageName)), GENERIC_READ or GENERIC_WRITE,
                         FILE_SHARE_READ or FILE_SHARE_WRITE, nil, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
     if hFile = 0 then exit;
     WriteFile(hFile, m_pBufForSaveImage^, stParam.nImageLen, dwWriteLen, nil);
     CloseHandle(hFile);

  end;

  Result := MV_OK;
end;


// ch:按下保存bmp图片按钮 | en:Click Save BMP button
procedure TForm1.SAVE_BMP_BUTTONClick(Sender: TObject);
begin
  m_nSaveImageType := MV_Image_Bmp;
  m_nRet := SaveImage();
  if m_nRet<>MV_OK then
  begin
    ShowMessage( 'Save bmp fail.' + IntToHex(m_nRet,8) );
    exit
  end;
  ShowMessage( 'Save bmp succeed.' );
end;


// ch:按下保存jpg图片按钮 | en:Click Save JPG button
procedure TForm1.SAVE_JPG_BUTTONClick(Sender: TObject);
begin
  m_nSaveImageType := MV_Image_Jpeg;
  m_nRet := SaveImage();
  if m_nRet<>MV_OK then
  begin
    ShowMessage( 'Save jpg fail.' + IntToHex(m_nRet,8) );
    exit
  end;
  ShowMessage( 'Save jpg succeed.' );
end;


// ch:右上角退出 | en:Exit from upper right corner
procedure TForm1.OnClose(Sender: TObject; var Action: TCloseAction);
begin
  CLOSE_BUTTONClick(Sender);
end;


// ch:最开始时的窗口初始化 | en:Initial window initialization
procedure TForm1.OnCreat(Sender: TObject);
begin
  m_nTriggerMode := TypInfo.GetEnumValue(TypeInfo(MV_CAM_TRIGGER_MODE),'MV_TRIGGER_MODE_OFF');     // ch:触发模式 | en:Trigger Mode
  m_nSaveImageType := MV_Image_Undefined;
  m_hwndDisplay := DISPLAY_Panel.Handle;

  EnableControls(FALSE);
end;


end.
