object MainForm: TMainForm
  Left = 213
  Top = 110
  Width = 640
  Height = 480
  Caption = 'DBF���ɵ{�� v1.0'
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Courier New'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnActivate = FormActivate
  PixelsPerInch = 120
  TextHeight = 16
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 632
    Height = 2
    Align = alTop
    BevelOuter = bvLowered
    TabOrder = 0
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 2
    Width = 632
    Height = 397
    ActivePage = TabSheet3
    Align = alClient
    TabOrder = 1
    object TabSheet1: TTabSheet
      Caption = 'DBase �ɮ�'
      object Panel2: TPanel
        Left = 0
        Top = 0
        Width = 624
        Height = 175
        Align = alTop
        BevelOuter = bvNone
        Caption = 'Panel2'
        TabOrder = 0
        object GroupBox1: TGroupBox
          Left = 0
          Top = 0
          Width = 289
          Height = 175
          Align = alLeft
          Caption = '���Y��T'
          TabOrder = 0
          object Label1: TLabel
            Left = 16
            Top = 24
            Width = 80
            Height = 16
            Caption = '�Ƶ����G'
          end
          object Label2: TLabel
            Left = 16
            Top = 53
            Width = 112
            Height = 16
            Caption = '�̫Ყ�ʤ���G'
          end
          object Label3: TLabel
            Left = 16
            Top = 83
            Width = 160
            Height = 16
            Caption = '�̫�@����ƿ����X�G'
          end
          object Label4: TLabel
            Left = 16
            Top = 112
            Width = 112
            Height = 16
            Caption = '��ư_�l��m�G'
          end
          object Label5: TLabel
            Left = 16
            Top = 141
            Width = 96
            Height = 16
            Caption = '��ƿ��j�p�G'
          end
          object Edit1: TEdit
            Left = 104
            Top = 20
            Width = 177
            Height = 25
            ImeName = '���� (�c��) - �s�`��'
            TabOrder = 0
          end
          object Edit2: TEdit
            Left = 144
            Top = 49
            Width = 137
            Height = 25
            ImeName = '���� (�c��) - �s�`��'
            TabOrder = 1
          end
          object Edit3: TEdit
            Left = 192
            Top = 79
            Width = 89
            Height = 25
            ImeName = '���� (�c��) - �s�`��'
            TabOrder = 2
          end
          object Edit4: TEdit
            Left = 144
            Top = 108
            Width = 137
            Height = 25
            ImeName = '���� (�c��) - �s�`��'
            TabOrder = 3
          end
          object Edit5: TEdit
            Left = 120
            Top = 137
            Width = 161
            Height = 25
            ImeName = '���� (�c��) - �s�`��'
            TabOrder = 4
          end
        end
        object Memo1: TMemo
          Left = 289
          Top = 0
          Width = 335
          Height = 175
          Align = alClient
          ScrollBars = ssBoth
          TabOrder = 1
        end
      end
      object Memo2: TMemo
        Left = 0
        Top = 175
        Width = 624
        Height = 191
        Align = alClient
        ScrollBars = ssBoth
        TabOrder = 1
      end
    end
    object TabSheet3: TTabSheet
      Caption = 'SQL ���O'
      ImageIndex = 2
      object Panel4: TPanel
        Left = 0
        Top = 0
        Width = 624
        Height = 145
        Align = alTop
        Caption = 'Panel4'
        TabOrder = 0
        object Label6: TLabel
          Left = 8
          Top = 12
          Width = 80
          Height = 16
          Caption = '�ɮצW�١G'
        end
        object Memo3: TMemo
          Left = 1
          Top = 40
          Width = 488
          Height = 104
          ScrollBars = ssVertical
          TabOrder = 0
        end
        object Button1: TButton
          Left = 496
          Top = 8
          Width = 121
          Height = 25
          Caption = '�M�w���W��'
          TabOrder = 1
          OnClick = Button1Click
        end
        object Edit6: TEdit
          Left = 88
          Top = 8
          Width = 401
          Height = 24
          TabOrder = 2
        end
        object Button2: TButton
          Left = 496
          Top = 40
          Width = 121
          Height = 25
          Caption = '����SQL���O'
          TabOrder = 3
          OnClick = Button2Click
        end
      end
      object DBGrid1: TDBGrid
        Left = 0
        Top = 145
        Width = 624
        Height = 221
        Align = alClient
        DataSource = DataSource1
        TabOrder = 1
        TitleFont.Charset = ANSI_CHARSET
        TitleFont.Color = clWindowText
        TitleFont.Height = -13
        TitleFont.Name = 'Courier New'
        TitleFont.Style = []
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'TabSheet2'
      ImageIndex = 2
    end
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 399
    Width = 632
    Height = 25
    Panels = <
      item
        Width = 500
      end
      item
        Width = 50
      end>
    SimplePanel = False
  end
  object MainMenu1: TMainMenu
    Left = 144
    Top = 232
    object N1: TMenuItem
      Caption = '�ɮ�'
      object Open: TMenuItem
        Caption = '�}������'
        OnClick = OpenClick
      end
      object SaveAs: TMenuItem
        Caption = '�t�s�s��'
        Enabled = False
        OnClick = SaveAsClick
      end
      object N5: TMenuItem
        Caption = '-'
      end
      object Exit: TMenuItem
        Caption = '����'
      end
    end
    object N8: TMenuItem
      Caption = '����'
      object ConvertToText: TMenuItem
        Caption = '��r��'
        Enabled = False
        OnClick = ConvertToTextClick
      end
    end
    object N9: TMenuItem
      Caption = '����'
    end
  end
  object OpenDialog1: TOpenDialog
    Filter = 'DBASE (*.dbf)|*.dbf|All Files (*.*)|*.*'
    Left = 180
    Top = 232
  end
  object SaveDialog1: TSaveDialog
    Filter = 'Text �ɮ�(TXT)|*.txt|All Files (*.*)|*.*'
    Left = 216
    Top = 232
  end
  object DataSource1: TDataSource
    DataSet = Table1
    Left = 324
    Top = 232
  end
  object Table1: TTable
    DatabaseName = 'DBF2Access'
    TableName = 'aaa'
    Left = 288
    Top = 232
  end
  object Query1: TQuery
    DatabaseName = 'DBF2Access'
    DataSource = DataSource1
    Left = 364
    Top = 232
  end
  object Database1: TDatabase
    DriverName = 'Microsoft Access Driver (*.mdb)'
    SessionName = 'Default'
    Left = 252
    Top = 229
  end
end
