object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'BasicDemo'
  ClientHeight = 621
  ClientWidth = 839
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 560
    Top = 32
    Width = 249
    Height = 121
    Caption = 'Initialization'
    TabOrder = 0
  end
  object Display: TPanel
    Left = 32
    Top = 111
    Width = 505
    Height = 482
    Caption = 'Display'
    TabOrder = 1
  end
  object DeviceList: TComboBox
    Left = 32
    Top = 32
    Width = 505
    Height = 21
    ImeName = #20013#25991'('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
    TabOrder = 2
    Text = 'DeviceList'
  end
  object GroupBox2: TGroupBox
    Left = 560
    Top = 193
    Width = 249
    Height = 153
    Caption = 'Image Acquisition'
    TabOrder = 3
    object SoftwareOnce: TButton
      Left = 144
      Top = 104
      Width = 81
      Height = 25
      Caption = 'Trigger Once'
      TabOrder = 0
      OnClick = SoftwareOnceClick
    end
    object ContinuesMode: TRadioButton
      Left = 24
      Top = 24
      Width = 113
      Height = 17
      Caption = 'Continuous'
      TabOrder = 1
      OnClick = ContinuesModeClick
    end
    object TriggerMode: TRadioButton
      Left = 143
      Top = 24
      Width = 113
      Height = 17
      Caption = 'Trigger Mode'
      TabOrder = 2
      OnClick = TriggerModeClick
    end
    object Software: TCheckBox
      Left = 17
      Top = 112
      Width = 121
      Height = 17
      Caption = 'Trigger by Software'
      TabOrder = 3
      OnClick = SoftwareClick
    end
  end
  object GroupBox3: TGroupBox
    Left = 560
    Top = 352
    Width = 249
    Height = 65
    Caption = 'Picture Storage'
    TabOrder = 4
  end
  object GroupBox4: TGroupBox
    Left = 560
    Top = 440
    Width = 249
    Height = 153
    Caption = 'Parameters'
    TabOrder = 5
    object Label1: TLabel
      Left = 24
      Top = 32
      Width = 70
      Height = 13
      Caption = 'Exposure Time'
    end
    object Label2: TLabel
      Left = 24
      Top = 72
      Width = 21
      Height = 13
      Caption = 'Gain'
    end
    object ExpouseEdit: TEdit
      Left = 112
      Top = 32
      Width = 121
      Height = 21
      ImeName = #20013#25991'('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      TabOrder = 0
      Text = 'ExpouseEdit'
    end
    object GainEdit: TEdit
      Left = 112
      Top = 72
      Width = 121
      Height = 21
      ImeName = #20013#25991'('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      TabOrder = 1
      Text = 'GainEdit'
    end
  end
  object SetPara: TButton
    Left = 704
    Top = 552
    Width = 81
    Height = 25
    Caption = 'Set Parameter'
    TabOrder = 6
    OnClick = SetParaClick
  end
  object GetPara: TButton
    Left = 577
    Top = 552
    Width = 82
    Height = 25
    Caption = 'Get Parameter'
    TabOrder = 7
    OnClick = GetParaClick
  end
  object SaveJPG: TButton
    Left = 704
    Top = 373
    Width = 75
    Height = 25
    Caption = 'Save as JPG'
    TabOrder = 8
    OnClick = SaveJPGClick
  end
  object SaveBMP: TButton
    Left = 584
    Top = 373
    Width = 75
    Height = 25
    Caption = 'Save as BMP'
    TabOrder = 9
    OnClick = SaveBMPClick
  end
  object StopGrabbing: TButton
    Left = 697
    Top = 248
    Width = 82
    Height = 25
    Caption = 'Stop'
    TabOrder = 10
    OnClick = StopGrabbingClick
  end
  object StartGrabbing: TButton
    Left = 584
    Top = 248
    Width = 75
    Height = 25
    Caption = 'Start'
    TabOrder = 11
    OnClick = StartGrabbingClick
  end
  object Close: TButton
    Left = 702
    Top = 111
    Width = 75
    Height = 25
    Caption = 'Close Device'
    TabOrder = 12
    OnClick = CloseClick
  end
  object Open: TButton
    Left = 600
    Top = 111
    Width = 75
    Height = 25
    Caption = 'Open Device'
    TabOrder = 13
    OnClick = OpenClick
  end
  object Enum: TButton
    Left = 600
    Top = 57
    Width = 177
    Height = 25
    Caption = 'Search Device'
    TabOrder = 14
    OnClick = EnumClick
  end
end
