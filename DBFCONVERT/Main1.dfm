object MainForm: TMainForm
  Left = 202
  Top = 147
  Width = 640
  Height = 480
  Caption = 'DBF���ɵ{�� v1.0'
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -17
  Font.Name = 'Courier New'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  PixelsPerInch = 120
  TextHeight = 20
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
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 632
    Height = 2
    Align = alTop
    BevelOuter = bvLowered
    TabOrder = 1
  end
  object Panel2: TPanel
    Left = 0
    Top = 2
    Width = 632
    Height = 175
    Align = alTop
    TabOrder = 2
    object Panel3: TPanel
      Left = 1
      Top = 1
      Width = 296
      Height = 173
      Align = alLeft
      BevelOuter = bvNone
      Caption = 'Panel3'
      TabOrder = 0
      object GroupBox1: TGroupBox
        Left = 0
        Top = 1
        Width = 297
        Height = 168
        Caption = '���Y��T'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = '�з���'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        object Label1: TLabel
          Left = 16
          Top = 24
          Width = 90
          Height = 17
          Caption = '�Ƶ����G'
        end
        object Label2: TLabel
          Left = 16
          Top = 53
          Width = 126
          Height = 17
          Caption = '�̫Ყ�ʤ���G'
        end
        object Label3: TLabel
          Left = 16
          Top = 83
          Width = 180
          Height = 17
          Caption = '�̫�@����ƿ����X�G'
        end
        object Label4: TLabel
          Left = 16
          Top = 112
          Width = 126
          Height = 17
          Caption = '��ư_�l��m�G'
        end
        object Label5: TLabel
          Left = 16
          Top = 141
          Width = 108
          Height = 17
          Caption = '��ƿ��j�p�G'
        end
        object Edit1: TEdit
          Left = 104
          Top = 20
          Width = 180
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
    end
    object Memo1: TMemo
      Left = 297
      Top = 1
      Width = 334
      Height = 173
      Align = alClient
      ImeName = '���� (�c��) - �s�`��'
      ScrollBars = ssBoth
      TabOrder = 1
    end
  end
  object Memo2: TMemo
    Left = 0
    Top = 177
    Width = 632
    Height = 222
    Align = alClient
    ImeName = '���� (�c��) - �s�`��'
    ScrollBars = ssBoth
    TabOrder = 3
  end
  object MainMenu1: TMainMenu
    Left = 144
    Top = 224
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
    Left = 176
    Top = 224
  end
  object SaveDialog1: TSaveDialog
    Filter = 'Text �ɮ�(TXT)|*.txt|All Files (*.*)|*.*'
    Left = 208
    Top = 224
  end
end
