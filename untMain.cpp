//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "untMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CGAUGES"
#pragma resource "*.dfm"
TfrmMain*   frmMain;

#include <stdio.h>

bool        fBusy   = false;
bool        fCancel = false;
bool        fClose  = false;

//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner):
    TForm(Owner)
{
}
//---------------------------------------------------------------------------
__int64 TfrmMain::GetDriveSize(char Drive)
{
    char*   szUNCPath   = "\\\\.\\@:";
    szUNCPath[4] = Drive;

    FILE*   f   = fopen(szUNCPath, "rb");

    if (f)
    {
        unsigned char r = szUNCPath[4] - 64;
        __int64 ret = DiskSize(r);
        if (ret == -1)
        {
            ret = 0;
        }
        return ret;
    }
    return 0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnSelectClick(TObject* Sender)
{
    if (dlgSave->Execute())
    {
        edtDestination->Text = dlgSave->FileName;
        if (edtDestination->Text.SubString(edtDestination->Text.Length() - 3, 4).LowerCase() != ".iso")
        {
            edtDestination->Text = edtDestination->Text + ".iso";
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::edtDestinationChange(TObject* Sender)
{
    CheckValidStart();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnStartClick(TObject* Sender)
{
    if (fBusy == true)
    {
        fCancel = true;
        btnStart->Enabled = false;
        return;
    }
    fBusy = true;
    btnStart->Caption = "Stop";
    edtDestination->Enabled = false;
    cbxDrives->Enabled = false;
    btnSelect->Enabled = false;
    btnRefresh->Enabled = false;

    m_pSrcFile = NULL;
    m_pDestFile = NULL;

    m_iTBR = 0;
    char*   szUNCPath   = "\\\\.\\@:";
    char    Drive       = cbxDrives->Text.c_str()[1];
    szUNCPath[4] = Drive;
    m_szSrcUNC = szUNCPath;
    m_iSrcSize = DiskSize(szUNCPath[4] - 64);

    m_iBytesRead = 0;

    int     iIcon           = MB_ICONINFORMATION;

    bool    fMayContinue    = false;
    m_pSrcFile = fopen(m_szSrcUNC.c_str(), "rb");
    if (m_pSrcFile)
    {
        m_pDestFile = fopen(edtDestination->Text.c_str(), "wb");
        if (m_pDestFile)
        {
            fMayContinue = true;
        }
        else
        {
            lblStatus->Caption = "Could not write to file.";
            iIcon = MB_ICONERROR;
        }
    }
    else
    {
        lblStatus->Caption = "Could not read from drive.";
        iIcon = MB_ICONERROR;
    }

    if (fMayContinue)
    {
        lblStatus->Caption = "Ripping..";
        bool    fDone   = false;
        while (!fDone)
        {
            Read();
            if (fCancel)
            {
                lblStatus->Caption = "Rip cancelled.";
                OnEnd();
                iIcon = MB_ICONERROR;
                break;
            }
            Write();
            if (feof(m_pSrcFile))
            {
                if (m_iSrcSize == m_iTBR)
                {
                    lblStatus->Caption = "Rip complete.";
                    OnEnd();
                }
                else
                {
                    lblStatus->Caption = "Rip failed.";
                    OnEnd();
                    iIcon = MB_ICONERROR;
                }
                fDone = true;
            }
            Application->ProcessMessages();
        }
    }
    if (m_pSrcFile)
    {
        fclose(m_pSrcFile);
    }
    if (m_pDestFile)
    {
        fclose(m_pDestFile);
    }

    fCancel = false;
    fBusy = false;
    btnStart->Caption = "Start";
    btnStart->Enabled = true;
    edtDestination->Enabled = true;
    cbxDrives->Enabled = true;
    btnSelect->Enabled = true;
    btnRefresh->Enabled = true;
    prg->Progress = 0;
    prg->Update();

    Caption = "DiscToISO";
    Application->Title = Caption;

    Application->MessageBox(lblStatus->Caption.c_str(), Caption.c_str(), MB_OK | iIcon);

    if (fClose)
    {
        Close();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormClose(TObject* Sender, TCloseAction& Action)
{
    if (fBusy)
    {
        fCancel = true;
        fClose = true;
        btnStart->Enabled = false;
        Action = caNone;
    }
    else
    {
        Action = caFree;
    }
}
//---------------------------------------------------------------------------

void TfrmMain::Read()
{
    long    S   = ftell(m_pSrcFile);
    fread(m_pBuffer, 1048576, 1, m_pSrcFile);
    long    E   = ftell(m_pSrcFile);
    m_iBytesRead = E - S;
}

//---------------------------------------------------------------------------

void TfrmMain::Write()
{
    m_iTBR += m_iBytesRead;
    fwrite(m_pBuffer, m_iBytesRead, 1, m_pDestFile);

    int iProgress   = float(m_iTBR) / float(m_iSrcSize) * 100.0;
    prg->Progress = iProgress;
    Caption = AnsiString("DiscToISO (") + AnsiString(iProgress) + "%)";
    Application->Title = Caption;
    prg->Update();
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::cbxDrivesChange(TObject* Sender)
{
    char*   szUNCPath   = "\\\\.\\@:";
    char    Drive       = cbxDrives->Text.c_str()[1];
    szUNCPath[4] = Drive;
    m_szSrcUNC = szUNCPath;
    m_iSrcSize = DiskSize(szUNCPath[4] - 64);
    CheckValidStart();
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnRefreshClick(TObject* Sender)
{
    cbxDrives->Clear();

    DWORD   Drives;
    Drives = GetLogicalDrives();
    AnsiString  DrivesStr;

    int         iItemIndex  = 0;

    char        CDrive      = 'A';
    int         g           = 1;
    for (int a = 0; a < 26; a++)
    {
        if (Drives & g && g != 1)
        {
            char    szDrive[2];
            szDrive[0] = CDrive;
            szDrive[1] = 0;
            AnsiString  Root                = szDrive + AnsiString(":\\");

            char        szVolumeName[128]   =
            {
                0
            };
            DWORD       dwSerialNumber      = 0;
            DWORD       dwFSMaxFilenameLen  = 0;
            DWORD       dwFSFlags           = 0;
            char        szFileSystem[128]   =
            {
                0
            };
            GetVolumeInformation(Root.c_str(), szVolumeName, 127, &dwSerialNumber, &dwFSMaxFilenameLen, &dwFSFlags, szFileSystem, 127);

            m_iSrcSize = GetDriveSize(CDrive);

            if ((GetDriveType(Root.c_str()) == DRIVE_CDROM) && (m_iSrcSize > 0))
            {
                AnsiString  VolumeLabel = szVolumeName;
                double      fSize       = m_iSrcSize;
                AnsiString  Measure     = " B";
                if (fSize > 1024)
                {
                    Measure = "KB";
                    fSize /= 1024.0;
                    if (fSize > 1024)
                    {
                        Measure = "MB";
                        fSize /= 1024.0;
                        if (fSize > 1024)
                        {
                            Measure = "GB";
                            fSize /= 1024.0;
                            if (fSize > 1024)
                            {
                                Measure = "TB";
                                fSize /= 1024.0;
                                if (fSize > 1024)
                                {
                                    Measure = "PB";
                                    fSize /= 1024.0;
                                }
                            }
                        }
                    }
                }
                char    szSize[128];
                sprintf(szSize, "%6.2f", fSize);

                AnsiString  Megs;
                if (m_iSrcSize > 0)
                {
                    Megs = szSize + Measure;
                }
                else
                {
                    Megs = "(No media)";
                }

                if (VolumeLabel == "")
                {
                    VolumeLabel = "(No label)";
                }

                AnsiString  SubDesc;
                if (m_iSrcSize > 0)
                {
                    SubDesc = AnsiString("]  ") + Megs + "       " + VolumeLabel;
                }
                else
                {
                    SubDesc = AnsiString("]  ") + Megs;
                }
                AnsiString  Desc    = "[" + Root + SubDesc;


                cbxDrives->Items->Add(Desc);
            }
        }
        CDrive++;
        g *= 2;
    }
    if (cbxDrives->Items->Count > 0)
    {
        cbxDrives->Enabled = true;
        btnSelect->Enabled = true;
        btnStart->Enabled = true;

        cbxDrives->ItemIndex = 0;
        cbxDrivesChange(this);
    }
    else
    {
        edtDestination->Enabled = false;
        cbxDrives->Enabled = false;
        btnSelect->Enabled = false;
        btnStart->Enabled = false;
    }
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormCreate(TObject* Sender)
{
    btnRefreshClick(this);
}

//---------------------------------------------------------------------------

void TfrmMain::CheckValidStart()
{
    if (m_iSrcSize > 0 && edtDestination->Text != "")
    {
        btnStart->Enabled = true;
    }
    else
    {
        btnStart->Enabled = false;
    }
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::edtDestinationKeyDown(TObject* Sender, WORD& Key, TShiftState Shift)
{
    if (Key == 13)
    {
        btnStartClick(this);
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Label1MouseDown(TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    ShellExecute(NULL, "open", "http://www.antivoid.net", "", "", SW_SHOW);
}
//---------------------------------------------------------------------------


void TfrmMain::OnEnd()
{
    m_iBytesRead = 0;
}
