object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Delphi BasicDemo'
  ClientHeight = 512
  ClientWidth = 788
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = OnClose
  OnCreate = OnCreat
  PixelsPerInch = 96
  TextHeight = 13
  object INIT_GroupBox: TGroupBox
    Left = 576
    Top = 16
    Width = 185
    Height = 89
    Caption = #21021#22987#21270
    TabOrder = 0
  end
  object ENUM_BUTTON: TButton
    Left = 586
    Top = 40
    Width = 169
    Height = 25
    Caption = #26597#25214#35774#22791
    TabOrder = 1
    OnClick = ENUM_BUTTONClick
  end
  object OPEN_BUTTON: TButton
    Left = 586
    Top = 71
    Width = 75
    Height = 25
    Caption = #25171#24320#35774#22791
    Enabled = False
    TabOrder = 2
    OnClick = OPEN_BUTTONClick
  end
  object CLOSE_BUTTON: TButton
    Left = 680
    Top = 71
    Width = 75
    Height = 25
    Caption = #20851#38381#35774#22791
    Enabled = False
    TabOrder = 3
    OnClick = CLOSE_BUTTONClick
  end
  object DEVICE_COMBO: TComboBox
    Left = 19
    Top = 16
    Width = 534
    Height = 21
    ImeName = #20013#25991'('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
    TabOrder = 4
  end
  object IMAGE_GRAB_GroupBox: TGroupBox
    Left = 576
    Top = 128
    Width = 185
    Height = 121
    Caption = #22270#20687#37319#38598
    TabOrder = 5
    object START_GRABBING_BUTTON: TButton
      Left = 10
      Top = 51
      Width = 75
      Height = 25
      Caption = #24320#22987#37319#38598
      Enabled = False
      TabOrder = 0
      OnClick = START_GRABBING_BUTTONClick
    end
    object STOP_GRABBING_BUTTON: TButton
      Left = 95
      Top = 51
      Width = 75
      Height = 25
      Caption = #20572#27490#37319#38598
      Enabled = False
      TabOrder = 1
      OnClick = STOP_GRABBING_BUTTONClick
    end
    object CONTINUS_MODE_RadioButton: TRadioButton
      Left = 15
      Top = 26
      Width = 70
      Height = 17
      Caption = #36830#32493#27169#24335
      Enabled = False
      TabOrder = 2
      OnClick = OnBnClickedContinusModeRadio
    end
    object TRIGGER_MODE_RadioButton: TRadioButton
      Left = 100
      Top = 26
      Width = 113
      Height = 17
      Caption = #35302#21457#27169#24335
      Enabled = False
      TabOrder = 3
      OnClick = OnBnClickedTriggerModeRadio
    end
    object SOFTWARE_TRIGGER_CheckBox: TCheckBox
      Left = 15
      Top = 91
      Width = 57
      Height = 17
      Caption = #36719#35302#21457
      Enabled = False
      TabOrder = 4
      OnClick = OnBnClickedSoftwareTriggerCheck
    end
    object SOFTWARE_ONCE_BUTTON: TButton
      Left = 96
      Top = 86
      Width = 75
      Height = 25
      Caption = #36719#35302#21457#19968#27425
      Enabled = False
      TabOrder = 5
      OnClick = OnBnClickedSoftwareOnceButton
    end
  end
  object DISPLAY_Panel: TPanel
    Left = 19
    Top = 54
    Width = 534
    Height = 450
    Caption = 'DISPLAY_Panel'
    TabOrder = 6
  end
  object PARAMETER_GroupBox: TGroupBox
    Left = 576
    Top = 336
    Width = 185
    Height = 145
    Caption = #21442#25968
    TabOrder = 7
    object EXPOSURE_StaticText: TStaticText
      Left = 18
      Top = 30
      Width = 28
      Height = 17
      Caption = #26333#20809
      TabOrder = 0
    end
    object GAIN_StaticText: TStaticText
      Left = 18
      Top = 65
      Width = 28
      Height = 17
      Caption = #22686#30410
      TabOrder = 1
    end
    object EXPOSURE_Edit: TEdit
      Left = 52
      Top = 19
      Width = 121
      Height = 21
      Enabled = False
      ImeName = #20013#25991'('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      TabOrder = 2
    end
    object GAIN_Edit: TEdit
      Left = 52
      Top = 60
      Width = 121
      Height = 21
      Enabled = False
      ImeName = #20013#25991'('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      TabOrder = 3
    end
    object GET_PARAMETER_BUTTON: TButton
      Left = 14
      Top = 104
      Width = 75
      Height = 25
      Caption = #21442#25968#33719#21462
      Enabled = False
      TabOrder = 4
      OnClick = GET_PARAMETER_BUTTONClick
    end
    object SET_PARAMETER_BUTTON: TButton
      Left = 95
      Top = 103
      Width = 75
      Height = 25
      Caption = #21442#25968#35774#32622
      Enabled = False
      TabOrder = 5
      OnClick = SET_PARAMETER_BUTTONClick
    end
  end
  object SAVE_IMAGE_GroupBox: TGroupBox
    Left = 576
    Top = 264
    Width = 185
    Height = 66
    Caption = #22270#29255#20445#23384
    TabOrder = 8
    object SAVE_BMP_BUTTON: TButton
      Left = 10
      Top = 24
      Width = 75
      Height = 25
      Caption = #20445#23384'BMP'
      Enabled = False
      TabOrder = 0
      OnClick = SAVE_BMP_BUTTONClick
    end
    object SAVE_JPG_BUTTON: TButton
      Left = 95
      Top = 24
      Width = 75
      Height = 25
      Caption = #20445#23384'JPG'
      Enabled = False
      TabOrder = 1
      OnClick = SAVE_JPG_BUTTONClick
    end
  end
end
