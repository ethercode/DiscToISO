//---------------------------------------------------------------------------

#ifndef untMainH
#define untMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include "CGAUGES.h"

#include <stdio.h>
//---------------------------------------------------------------------------
class TfrmMain:public TForm
{
__published:    // IDE-managed Components
    TGroupBox*              GroupBox1;
    TComboBox*              cbxDrives;
    TGroupBox*              GroupBox2;
    TEdit*                  edtDestination;
    TButton*                btnSelect;
    TSaveDialog*            dlgSave;
    TGroupBox*              GroupBox3;
    TCGauge*                prg;
    TButton*                btnStart;
    TLabel*                 lblStatus;
    TButton*                btnRefresh;
    TLabel*                 Label1;
    TLabel*                 Label9;
    void __fastcall         btnSelectClick(TObject* Sender);
    void __fastcall         edtDestinationChange(TObject* Sender);
    void __fastcall         btnStartClick(TObject* Sender);
    void __fastcall         FormClose(TObject* Sender, TCloseAction& Action);
    void __fastcall         cbxDrivesChange(TObject* Sender);
    void __fastcall         btnRefreshClick(TObject* Sender);
    void __fastcall         FormCreate(TObject* Sender);
    void __fastcall         edtDestinationKeyDown(TObject* Sender, WORD& Key, TShiftState Shift);
    void __fastcall         Label1MouseDown(TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
private:    // User declarations
public:     // User declarations
    __fastcall              TfrmMain(TComponent* Owner);
    __int64                 GetDriveSize(char szDrive);
    void                    Read();
    void                    Write();
    void                    CheckValidStart();
    void                    OnEnd();

    AnsiString              m_szSrcUNC;
    __int64                 m_iTBR;
    __int64                 m_iSrcSize;
    __int64                 m_iBytesRead;
    FILE*                   m_pSrcFile;
    FILE*                   m_pDestFile;
    BYTE                    m_pBuffer[1048576];
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain*frmMain;
//---------------------------------------------------------------------------
#endif
