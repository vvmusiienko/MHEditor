object Form3: TForm3
  Left = 307
  Top = 170
  Width = 430
  Height = 302
  Caption = 'Script Editor'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  DesignSize = (
    422
    275)
  PixelsPerInch = 96
  TextHeight = 13
  object Memo1: TMemo
    Left = 0
    Top = 0
    Width = 421
    Height = 275
    Anchors = [akLeft, akTop, akRight, akBottom]
    Enabled = False
    Lines.Strings = (
      'No Event Selected')
    ScrollBars = ssBoth
    TabOrder = 0
    OnExit = Memo1Exit
  end
end
