//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ScriptEditor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#include "Unit1.h"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Memo1Exit(TObject *Sender)
{
        Form1->applyScript();
}
//---------------------------------------------------------------------------
