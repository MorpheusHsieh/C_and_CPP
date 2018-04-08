object MainForm: TMainForm
  Left = 192
  Top = 124
  Width = 696
  Height = 480
  Caption = '查詢DBF檔案格式的欄位位資訊'
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -20
  Font.Name = '標楷體'
  Font.Style = []
  Menu = MainMenu
  OldCreateOrder = False
  PixelsPerInch = 120
  TextHeight = 20
  object StatusBar: TStatusBar
    Left = 0
    Top = 400
    Width = 688
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
    Width = 688
    Height = 2
    Align = alTop
    TabOrder = 1
  end
  object Memo1: TMemo
    Left = 0
    Top = 2
    Width = 688
    Height = 398
    Align = alClient
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -17
    Font.Name = '標楷體'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
  end
  object MainMenu: TMainMenu
    Left = 112
    Top = 56
    object N1: TMenuItem
      Caption = '檔案'
      object N2: TMenuItem
        Caption = '開啟舊檔'
        OnClick = N2Click
      end
      object N4: TMenuItem
        Caption = '-'
      end
      object N3: TMenuItem
        Caption = '另存新檔'
      end
    end
  end
  object OpenDialog: TOpenDialog
    Filter = 'dBase, (*.dbf)|*.dbf|All Files, (*.*)|*.*'
    Left = 144
    Top = 56
  end
end
