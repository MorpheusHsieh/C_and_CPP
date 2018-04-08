object MainForm: TMainForm
  Left = 202
  Top = 147
  Width = 640
  Height = 480
  Caption = 'DBF轉檔程式 v1.0'
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
        Caption = '檔頭資訊'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = '標楷體'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        object Label1: TLabel
          Left = 16
          Top = 24
          Width = 90
          Height = 17
          Caption = '備註欄位：'
        end
        object Label2: TLabel
          Left = 16
          Top = 53
          Width = 126
          Height = 17
          Caption = '最後異動日期：'
        end
        object Label3: TLabel
          Left = 16
          Top = 83
          Width = 180
          Height = 17
          Caption = '最後一筆資料錄號碼：'
        end
        object Label4: TLabel
          Left = 16
          Top = 112
          Width = 126
          Height = 17
          Caption = '資料起始位置：'
        end
        object Label5: TLabel
          Left = 16
          Top = 141
          Width = 108
          Height = 17
          Caption = '資料錄大小：'
        end
        object Edit1: TEdit
          Left = 104
          Top = 20
          Width = 180
          Height = 25
          ImeName = '中文 (繁體) - 新注音'
          TabOrder = 0
        end
        object Edit2: TEdit
          Left = 144
          Top = 49
          Width = 137
          Height = 25
          ImeName = '中文 (繁體) - 新注音'
          TabOrder = 1
        end
        object Edit3: TEdit
          Left = 192
          Top = 79
          Width = 89
          Height = 25
          ImeName = '中文 (繁體) - 新注音'
          TabOrder = 2
        end
        object Edit4: TEdit
          Left = 144
          Top = 108
          Width = 137
          Height = 25
          ImeName = '中文 (繁體) - 新注音'
          TabOrder = 3
        end
        object Edit5: TEdit
          Left = 120
          Top = 137
          Width = 161
          Height = 25
          ImeName = '中文 (繁體) - 新注音'
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
      ImeName = '中文 (繁體) - 新注音'
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
    ImeName = '中文 (繁體) - 新注音'
    ScrollBars = ssBoth
    TabOrder = 3
  end
  object MainMenu1: TMainMenu
    Left = 144
    Top = 224
    object N1: TMenuItem
      Caption = '檔案'
      object Open: TMenuItem
        Caption = '開啟舊檔'
        OnClick = OpenClick
      end
      object SaveAs: TMenuItem
        Caption = '另存新檔'
        Enabled = False
        OnClick = SaveAsClick
      end
      object N5: TMenuItem
        Caption = '-'
      end
      object Exit: TMenuItem
        Caption = '結束'
      end
    end
    object N8: TMenuItem
      Caption = '轉檔'
      object ConvertToText: TMenuItem
        Caption = '文字檔'
        Enabled = False
        OnClick = ConvertToTextClick
      end
    end
    object N9: TMenuItem
      Caption = '說明'
    end
  end
  object OpenDialog1: TOpenDialog
    Filter = 'DBASE (*.dbf)|*.dbf|All Files (*.*)|*.*'
    Left = 176
    Top = 224
  end
  object SaveDialog1: TSaveDialog
    Filter = 'Text 檔案(TXT)|*.txt|All Files (*.*)|*.*'
    Left = 208
    Top = 224
  end
end
