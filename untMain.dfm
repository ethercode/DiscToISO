object frmMain: TfrmMain
  Left = 412
  Top = 440
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'DiscToISO v1.0'
  ClientHeight = 206
  ClientWidth = 378
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Verdana'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnCreate = FormCreate
  DesignSize = (
    378
    206)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 230
    Top = 185
    Width = 140
    Height = 13
    Cursor = crHandPoint
    Alignment = taRightJustify
    Anchors = [akRight, akBottom]
    AutoSize = False
    Caption = 'http://www.antivoid.net'
    Font.Charset = ANSI_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'Verdana'
    Font.Style = [fsUnderline]
    ParentFont = False
    OnMouseDown = Label1MouseDown
  end
  object Label9: TLabel
    Left = 208
    Top = 185
    Width = 24
    Height = 13
    Anchors = [akLeft, akBottom]
    Caption = 'Visit'
  end
  object GroupBox1: TGroupBox
    Left = 7
    Top = 4
    Width = 365
    Height = 49
    Caption = 'Source Drive'
    TabOrder = 0
    DesignSize = (
      365
      49)
    object cbxDrives: TComboBox
      Left = 8
      Top = 17
      Width = 260
      Height = 20
      Style = csDropDownList
      Anchors = [akLeft, akTop, akRight]
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Lucida Console'
      Font.Style = []
      ItemHeight = 12
      ParentFont = False
      TabOrder = 0
      OnChange = cbxDrivesChange
    end
    object btnRefresh: TButton
      Left = 280
      Top = 16
      Width = 75
      Height = 23
      Caption = 'Refresh'
      TabOrder = 1
      OnClick = btnRefreshClick
    end
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 56
    Width = 364
    Height = 49
    Caption = 'Destination'
    TabOrder = 1
    DesignSize = (
      364
      49)
    object edtDestination: TEdit
      Left = 8
      Top = 17
      Width = 260
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
      OnChange = edtDestinationChange
      OnKeyDown = edtDestinationKeyDown
    end
    object btnSelect: TButton
      Left = 279
      Top = 16
      Width = 75
      Height = 23
      Anchors = [akTop, akRight]
      Caption = 'Select ...'
      TabOrder = 1
      OnClick = btnSelectClick
    end
  end
  object GroupBox3: TGroupBox
    Left = 8
    Top = 109
    Width = 364
    Height = 73
    Caption = 'Rip'
    TabOrder = 2
    DesignSize = (
      364
      73)
    object prg: TCGauge
      Left = 8
      Top = 48
      Width = 348
      Height = 17
      Anchors = [akLeft, akTop, akRight]
    end
    object lblStatus: TLabel
      Left = 96
      Top = 24
      Width = 201
      Height = 13
      AutoSize = False
      Caption = 'Ready to begin.'
    end
    object btnStart: TButton
      Left = 8
      Top = 18
      Width = 75
      Height = 23
      Caption = 'Start'
      Enabled = False
      TabOrder = 0
      OnClick = btnStartClick
    end
  end
  object dlgSave: TSaveDialog
    Filter = 'Images (*.iso)|*.iso'
    Options = [ofOverwritePrompt, ofExtensionDifferent, ofPathMustExist, ofEnableSizing]
    Left = 249
    Top = 116
  end
end
