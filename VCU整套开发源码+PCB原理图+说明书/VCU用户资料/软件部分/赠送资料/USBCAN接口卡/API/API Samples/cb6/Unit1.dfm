object Form1: TForm1
  Left = 284
  Top = 192
  Width = 573
  Height = 510
  Caption = 'CAN'#36890#29992#27979#35797#20363#23376
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox2: TGroupBox
    Left = 8
    Top = 8
    Width = 449
    Height = 153
    Caption = #35774#22791#21442#25968
    TabOrder = 5
    object Label8: TLabel
      Left = 184
      Top = 24
      Width = 48
      Height = 13
      Caption = #32034#24341#21495#65306
    end
    object Label9: TLabel
      Left = 304
      Top = 24
      Width = 70
      Height = 13
      Caption = #31532#20960#36335'CAN'#65306
    end
    object Label14: TLabel
      Left = 8
      Top = 24
      Width = 27
      Height = 13
      Caption = #31867#22411':'
    end
    object GroupBox3: TGroupBox
      Left = 8
      Top = 48
      Width = 433
      Height = 97
      Caption = #21021#22987#21270'CAN'#21442#25968
      TabOrder = 0
      object Label1: TLabel
        Left = 8
        Top = 24
        Width = 73
        Height = 13
        Caption = #39564#25910#30721':0x'
      end
      object Label2: TLabel
        Left = 8
        Top = 64
        Width = 81
        Height = 13
        Caption = #23631#34109#30721':0x'
      end
      object Label10: TLabel
        Left = 160
        Top = 24
        Width = 73
        Height = 13
        Caption = #23450#26102#22120'0:0x'
      end
      object Label11: TLabel
        Left = 162
        Top = 64
        Width = 79
        Height = 13
        Caption = #23450#26102#22120'1:0x'
      end
      object Label12: TLabel
        Left = 296
        Top = 24
        Width = 60
        Height = 13
        Caption = #28388#27874#26041#24335#65306
      end
      object Label13: TLabel
        Left = 320
        Top = 64
        Width = 36
        Height = 13
        Caption = #27169#24335#65306
      end
      object Edit2: TEdit
        Left = 88
        Top = 20
        Width = 65
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        TabOrder = 0
        Text = '00000000'
      end
      object Edit3: TEdit
        Left = 88
        Top = 60
        Width = 65
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        TabOrder = 1
        Text = 'FFFFFFFF'
      end
      object Edit5: TEdit
        Left = 248
        Top = 20
        Width = 41
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        TabOrder = 2
        Text = '00'
      end
      object Edit6: TEdit
        Left = 248
        Top = 60
        Width = 41
        Height = 21
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        TabOrder = 3
        Text = '14'
      end
      object ComboBox3: TComboBox
        Left = 352
        Top = 20
        Width = 73
        Height = 21
        Style = csDropDownList
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ItemHeight = 13
        TabOrder = 4
        Items.Strings = (
          #21452#28388#27874
          #21333#28388#27874)
      end
      object ComboBox4: TComboBox
        Left = 352
        Top = 60
        Width = 73
        Height = 21
        Style = csDropDownList
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ItemHeight = 13
        TabOrder = 5
        Items.Strings = (
          #27491#24120#27169#24335
          #21482#21548#27169#24335)
      end
    end
    object ComboBox_devtype: TComboBox
      Left = 48
      Top = 16
      Width = 129
      Height = 21
      Style = csDropDownList
      DropDownCount = 15
      ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      ItemHeight = 13
      TabOrder = 1
      Items.Strings = (
        'VCI_PCI5121'
        'VCI_PCI9810'
        'VCI_USBCAN1'
        'VCI_USBCAN2'
        'VCI_USBCAN2A'
        'VCI_PCI9820'
        'VCI_PCI5110'
        'VCI_ISA9620'
        'VCI_ISA5420'
        'VCI_PC104CAN'
        'VCI_DNP9810'
        'VCI_PCI9840'
        'VCI_PC104CAN2'
        'VCI_PCI9820I'
        'VCI_PEC9920')
    end
  end
  object ComboBox1: TComboBox
    Left = 248
    Top = 27
    Width = 59
    Height = 21
    Style = csDropDownList
    ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
    ItemHeight = 13
    TabOrder = 0
    Items.Strings = (
      '0'
      '1'
      '2'
      '3'
      '4'
      '5'
      '6'
      '7')
  end
  object ComboBox2: TComboBox
    Left = 392
    Top = 27
    Width = 59
    Height = 21
    Style = csDropDownList
    ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
    ItemHeight = 13
    TabOrder = 1
    Items.Strings = (
      '0'
      '1'
      '2'
      '3')
  end
  object Button1: TButton
    Left = 475
    Top = 24
    Width = 75
    Height = 25
    Caption = #36830#25509
    TabOrder = 2
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 472
    Top = 104
    Width = 75
    Height = 25
    Caption = #22797#20301'CAN'
    TabOrder = 3
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 472
    Top = 72
    Width = 73
    Height = 25
    Caption = #21551#21160'CAN'
    TabOrder = 4
    OnClick = Button3Click
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 178
    Width = 545
    Height = 89
    Caption = ' '#21457#36865#25968#25454#24103' '
    TabOrder = 6
    object Label3: TLabel
      Left = 16
      Top = 24
      Width = 60
      Height = 13
      Caption = #21457#36865#26684#24335#65306
    end
    object Label4: TLabel
      Left = 168
      Top = 24
      Width = 48
      Height = 13
      Caption = #24103#31867#22411#65306
    end
    object Label5: TLabel
      Left = 296
      Top = 24
      Width = 48
      Height = 13
      Caption = #24103#26684#24335#65306
    end
    object Label6: TLabel
      Left = 424
      Top = 24
      Width = 35
      Height = 13
      Caption = #24103'ID'#65306
    end
    object Label7: TLabel
      Left = 16
      Top = 56
      Width = 36
      Height = 13
      Caption = #25968#25454#65306
    end
    object ComboBox5: TComboBox
      Left = 80
      Top = 20
      Width = 81
      Height = 21
      Style = csDropDownList
      ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      ItemHeight = 13
      TabOrder = 0
      Items.Strings = (
        #27491#24120#21457#36865
        #21333#27425#21457#36865
        #33258#21457#33258#25910
        #21333#27425#33258#21457#33258#25910)
    end
    object ComboBox6: TComboBox
      Left = 216
      Top = 20
      Width = 73
      Height = 21
      Style = csDropDownList
      ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      ItemHeight = 13
      TabOrder = 1
      Items.Strings = (
        #26631#20934#24103
        #25193#23637#24103)
    end
    object ComboBox7: TComboBox
      Left = 344
      Top = 20
      Width = 73
      Height = 21
      Style = csDropDownList
      ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      ItemHeight = 13
      TabOrder = 2
      Items.Strings = (
        #25968#25454#24103
        #36828#31243#24103)
    end
    object Edit1: TEdit
      Left = 464
      Top = 20
      Width = 73
      Height = 21
      ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      TabOrder = 3
      Text = '00000080'
    end
    object Edit4: TEdit
      Left = 56
      Top = 53
      Width = 193
      Height = 21
      ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      TabOrder = 4
      Text = '01 02 03 04 05 06 07 08 '
    end
    object Button4: TButton
      Left = 272
      Top = 51
      Width = 75
      Height = 25
      Caption = #21457#36865
      TabOrder = 5
      OnClick = Button4Click
    end
  end
  object GroupBox6: TGroupBox
    Left = 8
    Top = 280
    Width = 545
    Height = 193
    Caption = #20449#24687
    TabOrder = 7
    object ListBox1: TListBox
      Left = 8
      Top = 16
      Width = 529
      Height = 169
      ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      ItemHeight = 13
      TabOrder = 0
    end
  end
end
