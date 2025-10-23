object Form1: TForm1
  Left = 219
  Top = 115
  Width = 586
  Height = 500
  Caption = 'CAN '#36890#29992#27979#35797#20363#23376
  Color = clBtnFace
  Font.Charset = GB2312_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = #23435#20307
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 12
  object GroupBox2: TGroupBox
    Left = 8
    Top = 16
    Width = 457
    Height = 137
    Caption = #35774#22791#21442#25968
    TabOrder = 5
    object Label8: TLabel
      Left = 192
      Top = 24
      Width = 48
      Height = 12
      Caption = #32034#24341#21495#65306
    end
    object Label9: TLabel
      Left = 320
      Top = 24
      Width = 66
      Height = 12
      Caption = #31532#20960#36335'CAN'#65306
    end
    object Label14: TLabel
      Left = 8
      Top = 24
      Width = 30
      Height = 12
      Caption = #31867#22411':'
    end
    object GroupBox3: TGroupBox
      Left = 8
      Top = 48
      Width = 441
      Height = 81
      Caption = #21021#22987#21270'CAN'#21442#25968
      TabOrder = 0
      object Label1: TLabel
        Left = 15
        Top = 24
        Width = 60
        Height = 12
        Caption = #39564#25910#30721#65306'0x'
      end
      object Label2: TLabel
        Left = 160
        Top = 24
        Width = 60
        Height = 12
        Caption = #23631#34109#30721#65306'0x'
      end
      object Label10: TLabel
        Left = 8
        Top = 56
        Width = 66
        Height = 12
        Caption = #23450#26102#22120'0'#65306'0x'
      end
      object Label11: TLabel
        Left = 154
        Top = 56
        Width = 66
        Height = 12
        Caption = #23450#26102#22120'1'#65306'0x'
      end
      object Label12: TLabel
        Left = 296
        Top = 24
        Width = 60
        Height = 12
        Caption = #28388#27874#26041#24335#65306
      end
      object Label13: TLabel
        Left = 320
        Top = 56
        Width = 36
        Height = 12
        Caption = #27169#24335#65306
      end
      object Edit2: TEdit
        Left = 80
        Top = 19
        Width = 65
        Height = 20
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        TabOrder = 0
        Text = '00000000'
      end
      object Edit3: TEdit
        Left = 224
        Top = 19
        Width = 65
        Height = 20
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        TabOrder = 1
        Text = 'FFFFFFFF'
      end
      object Edit5: TEdit
        Left = 80
        Top = 52
        Width = 65
        Height = 20
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        TabOrder = 2
        Text = '00'
      end
      object Edit6: TEdit
        Left = 224
        Top = 52
        Width = 65
        Height = 20
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        TabOrder = 3
        Text = '14'
      end
      object ComboBox3: TComboBox
        Left = 360
        Top = 19
        Width = 73
        Height = 20
        Style = csDropDownList
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ItemHeight = 12
        TabOrder = 4
        Items.Strings = (
          #21452#28388#27874
          #21333#28388#27874)
      end
      object ComboBox4: TComboBox
        Left = 360
        Top = 52
        Width = 73
        Height = 20
        Style = csDropDownList
        ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
        ItemHeight = 12
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
      Height = 20
      Style = csDropDownList
      DropDownCount = 15
      ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      ItemHeight = 12
      TabOrder = 1
      Items.Strings = (
        '1'
        '2'
        '3'
        ''
        ''
        ''
        ''
        ''
        ''
        ''
        ''
        ''
        ''
        ''
        '')
    end
  end
  object ComboBox1: TComboBox
    Left = 256
    Top = 35
    Width = 65
    Height = 20
    Style = csDropDownList
    DropDownCount = 10
    ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899
    ItemHeight = 12
    TabOrder = 0
    Items.Strings = (
      '0'
      '1'
      '2'
      '3'
      '4'
      '5'
      '6'
      '7'
      '8'
      '9')
  end
  object ComboBox2: TComboBox
    Left = 392
    Top = 35
    Width = 65
    Height = 20
    Style = csDropDownList
    ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899
    ItemHeight = 12
    TabOrder = 1
    Items.Strings = (
      '0'
      '1'
      '2'
      '3')
  end
  object Button1: TButton
    Left = 480
    Top = 24
    Width = 57
    Height = 25
    Caption = #36830#25509
    TabOrder = 2
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 480
    Top = 112
    Width = 75
    Height = 25
    Caption = #22797#20301'CAN'
    TabOrder = 3
    OnClick = Button2Click
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 168
    Width = 561
    Height = 81
    Caption = ' '#21457#36865#25968#25454#24103' '
    TabOrder = 4
    object Label3: TLabel
      Left = 16
      Top = 24
      Width = 60
      Height = 12
      Caption = #21457#36865#26684#24335#65306
    end
    object Label4: TLabel
      Left = 168
      Top = 24
      Width = 48
      Height = 12
      Caption = #24103#31867#22411#65306
    end
    object Label5: TLabel
      Left = 304
      Top = 24
      Width = 48
      Height = 12
      Caption = #24103#26684#24335#65306
    end
    object Label6: TLabel
      Left = 440
      Top = 24
      Width = 36
      Height = 12
      Caption = #24103'ID'#65306
    end
    object Label7: TLabel
      Left = 16
      Top = 56
      Width = 36
      Height = 12
      Caption = #25968#25454#65306
    end
    object Button5: TButton
      Left = 272
      Top = 49
      Width = 57
      Height = 24
      Caption = #21457#36865
      TabOrder = 0
      OnClick = Button5Click
    end
    object ComboBox5: TComboBox
      Left = 72
      Top = 19
      Width = 89
      Height = 20
      Style = csDropDownList
      ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      ItemHeight = 12
      TabOrder = 1
      Items.Strings = (
        #27491#24120#21457#36865
        #21333#27425#21457#36865
        #33258#21457#33258#25910
        #21333#27425#33258#21457#33258#25910)
    end
    object ComboBox6: TComboBox
      Left = 216
      Top = 19
      Width = 81
      Height = 20
      Style = csDropDownList
      ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      ItemHeight = 12
      TabOrder = 2
      Items.Strings = (
        #26631#20934#24103
        #25193#23637#24103)
    end
    object ComboBox7: TComboBox
      Left = 352
      Top = 19
      Width = 81
      Height = 20
      Style = csDropDownList
      ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      ItemHeight = 12
      TabOrder = 3
      Items.Strings = (
        #25968#25454#24103
        #36828#31243#24103)
    end
    object Edit1: TEdit
      Left = 480
      Top = 19
      Width = 73
      Height = 20
      ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      TabOrder = 4
      Text = '00000080'
    end
    object Edit4: TEdit
      Left = 56
      Top = 51
      Width = 201
      Height = 20
      ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      TabOrder = 5
      Text = '01 02 03 04 05 06 07 08 '
    end
  end
  object Button6: TButton
    Left = 480
    Top = 64
    Width = 75
    Height = 25
    Caption = #21551#21160'CAN'
    TabOrder = 6
    OnClick = Button6Click
  end
  object GroupBox6: TGroupBox
    Left = 8
    Top = 264
    Width = 561
    Height = 201
    Caption = #20449#24687
    TabOrder = 7
    object ListBox1: TListBox
      Left = 8
      Top = 16
      Width = 545
      Height = 177
      ImeName = #20013#25991' ('#31616#20307') - '#25628#29399#25340#38899#36755#20837#27861
      ItemHeight = 12
      TabOrder = 0
    end
  end
end
