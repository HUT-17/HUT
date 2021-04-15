object Form1: TForm1
  Left = 280
  Top = 191
  Width = 822
  Height = 542
  Caption = 'BasicDemo'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox4: TGroupBox
    Left = 584
    Top = 368
    Width = 201
    Height = 113
    Caption = 'Parameters'
    TabOrder = 19
    object Label1: TLabel
      Left = 8
      Top = 24
      Width = 70
      Height = 13
      Caption = 'Exposure Time'
    end
    object Label2: TLabel
      Left = 16
      Top = 56
      Width = 22
      Height = 13
      Caption = 'Gain'
    end
  end
  object GroupBox1: TGroupBox
    Left = 584
    Top = 24
    Width = 201
    Height = 105
    Caption = 'Initialzation'
    TabOrder = 0
  end
  object GroupBox2: TGroupBox
    Left = 584
    Top = 144
    Width = 201
    Height = 137
    Caption = 'Image Acquisition'
    TabOrder = 1
    object Software: TCheckBox
      Left = 0
      Top = 104
      Width = 113
      Height = 17
      Caption = 'Trigger by Software'
      TabOrder = 0
      OnClick = SoftwareClick
    end
  end
  object GroupBox3: TGroupBox
    Left = 584
    Top = 296
    Width = 201
    Height = 65
    Caption = 'Picture Storage'
    TabOrder = 2
  end
  object SetPara: TButton
    Left = 688
    Top = 448
    Width = 83
    Height = 25
    Caption = 'Set Parameter'
    TabOrder = 3
    OnClick = SetParaClick
  end
  object GetPara: TButton
    Left = 592
    Top = 448
    Width = 83
    Height = 25
    Caption = 'Get Parameter'
    TabOrder = 4
    OnClick = GetParaClick
  end
  object SaveJPG: TButton
    Left = 696
    Top = 320
    Width = 75
    Height = 25
    Caption = 'Save as JPG'
    TabOrder = 5
    OnClick = SaveJPGClick
  end
  object SaveBMP: TButton
    Left = 600
    Top = 320
    Width = 75
    Height = 25
    Caption = 'Save as BMP'
    TabOrder = 6
    OnClick = SaveBMPClick
  end
  object SoftwareOnce: TButton
    Left = 704
    Top = 240
    Width = 75
    Height = 25
    Caption = 'Trigger Once'
    TabOrder = 7
    OnClick = SoftwareOnceClick
  end
  object StopGrabbing: TButton
    Left = 696
    Top = 200
    Width = 75
    Height = 25
    Caption = 'Stop'
    TabOrder = 8
    OnClick = StopGrabbingClick
  end
  object StartGrabbing: TButton
    Left = 600
    Top = 200
    Width = 75
    Height = 25
    Caption = 'Start'
    TabOrder = 9
    OnClick = StartGrabbingClick
  end
  object Close: TButton
    Left = 696
    Top = 88
    Width = 75
    Height = 25
    Caption = 'Close Device'
    TabOrder = 10
    OnClick = CloseClick
  end
  object Open: TButton
    Left = 600
    Top = 88
    Width = 75
    Height = 25
    Caption = 'Open Device'
    TabOrder = 11
    OnClick = OpenClick
  end
  object Enum: TButton
    Left = 600
    Top = 48
    Width = 169
    Height = 25
    Caption = 'Search Device'
    TabOrder = 12
    OnClick = EnumClick
  end
  object GainEdit: TEdit
    Left = 672
    Top = 416
    Width = 97
    Height = 21
    ImeName = #20013#25991'('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
    TabOrder = 13
    Text = 'Edit1'
  end
  object ExpouseEdit: TEdit
    Left = 672
    Top = 384
    Width = 97
    Height = 21
    ImeName = #20013#25991'('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
    TabOrder = 14
    Text = 'Edit1'
  end
  object ContinuesMode: TRadioButton
    Left = 600
    Top = 176
    Width = 81
    Height = 17
    Caption = 'Continuous'
    TabOrder = 15
    OnClick = ContinuesModeClick
  end
  object TriggerMode: TRadioButton
    Left = 696
    Top = 176
    Width = 81
    Height = 17
    Caption = 'Trigger Mode'
    TabOrder = 16
    OnClick = TriggerModeClick
  end
  object DeviceList: TComboBox
    Left = 24
    Top = 24
    Width = 537
    Height = 21
    ImeName = #20013#25991'('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
    ItemHeight = 13
    TabOrder = 17
    Text = 'DeviceList'
  end
  object Display: TPanel
    Left = 24
    Top = 56
    Width = 537
    Height = 425
    Caption = 'Display'
    TabOrder = 18
  end
end
