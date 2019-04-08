object Form1: TForm1
  Left = 0
  Top = 0
  ActiveControl = VirtualStringTree
  Caption = #1058#1072#1073#1083#1080#1094#1072' "downloads"'
  ClientHeight = 657
  ClientWidth = 694
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Label: TLabel
    Left = 8
    Top = 636
    Width = 149
    Height = 13
    Caption = #1053#1072#1078#1084#1080#1090#1077' '#1082#1085#1086#1087#1082#1091' "'#1047#1072#1087#1086#1083#1085#1080#1090#1100'"'
    Color = clBtnHighlight
    ParentColor = False
  end
  object VirtualStringTree: TVirtualStringTree
    Left = 8
    Top = 8
    Width = 678
    Height = 561
    Header.AutoSizeIndex = 0
    Header.Font.Charset = DEFAULT_CHARSET
    Header.Font.Color = clWindowText
    Header.Font.Height = -11
    Header.Font.Name = 'Tahoma'
    Header.Font.Style = []
    Header.Options = [hoColumnResize, hoDrag, hoShowSortGlyphs, hoVisible]
    TabOrder = 0
    TreeOptions.PaintOptions = [toShowButtons, toShowDropmark, toShowHorzGridLines, toShowRoot, toShowTreeLines, toShowVertGridLines, toThemeAware, toUseBlendedImages]
    TreeOptions.SelectionOptions = [toFullRowSelect, toMultiSelect]
    OnFreeNode = VirtualStringTreeFreeNode
    OnGetText = VirtualStringTreeGetText
    Columns = <
      item
        Position = 0
        WideText = 'ID'
      end
      item
        Position = 1
        Width = 200
        WideText = 'Guid'
      end
      item
        Position = 2
        Width = 200
        WideText = 'Current path'
      end>
  end
  object TableButton: TButton
    Left = 8
    Top = 587
    Width = 113
    Height = 38
    Caption = #1047#1072#1087#1086#1083#1085#1080#1090#1100
    TabOrder = 1
    OnClick = TableButtonClick
  end
  object CloseButton: TButton
    Left = 596
    Top = 587
    Width = 90
    Height = 38
    Caption = #1047#1072#1082#1088#1099#1090#1100
    TabOrder = 2
    OnClick = CloseButtonClick
  end
  object DeleteButton: TButton
    Left = 144
    Top = 587
    Width = 121
    Height = 38
    Caption = #1059#1076#1072#1083#1080#1090#1100
    TabOrder = 3
    OnClick = DeleteButtonClick
  end
  object DeleteAllRecordsButton: TButton
    Left = 288
    Top = 587
    Width = 129
    Height = 38
    Caption = #1054#1095#1080#1089#1090#1080#1090#1100' '#1090#1072#1073#1083#1080#1094#1091
    TabOrder = 4
    OnClick = DeleteAllRecordsButtonClick
  end
end
