object Form2: TForm2
  Left = 178
  Top = 236
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Execute Script'
  ClientHeight = 261
  ClientWidth = 384
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object SpeedButton1: TSpeedButton
    Left = 2
    Top = 238
    Width = 375
    Height = 21
    Caption = 'Run'
    OnClick = SpeedButton1Click
  end
  object Memo1: TMemo
    Left = 0
    Top = 8
    Width = 385
    Height = 227
    TabOrder = 0
  end
end
