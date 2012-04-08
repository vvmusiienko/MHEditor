//---------------------------------------------------------------------------


#pragma hdrstop

#include "Other.h"
#include <BorlandBridge.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)

TVectorProperty::TVectorProperty(Controls::TWinControl* AOwner, string name, string obj_id, string setter, float def_x, float def_y, float def_z)
{
        _id = obj_id;
        _setter = setter;

        label = new TLabel(AOwner);
        label->Parent = AOwner;
        label->Caption = AnsiString(name.c_str());

        editX = new TEdit(AOwner);
        editX->Parent = AOwner;
        editX->Text = AnsiString(FloatToStr(def_x));
        editX->BorderStyle = bsNone;
        editX->OnChange = editXChange;
        editX->OnKeyDown = editKeyDown;
        editX->OnKeyPress = editKeyPress;

        editY = new TEdit(AOwner);
        editY->Parent = AOwner;
        editY->Text = AnsiString(FloatToStr(def_y));
        editY->BorderStyle = bsNone;
        editY->OnChange = editXChange;
        editY->OnKeyDown = editKeyDown;

        editZ = new TEdit(AOwner);
        editZ->Parent = AOwner;
        editZ->Text = AnsiString(FloatToStr(def_z));
        editZ->BorderStyle = bsNone;
        editZ->OnChange = editXChange;
        editZ->OnKeyDown = editKeyDown;
}


void TVectorProperty::init(int x, int y)
{
        label->Top = y + 5;
        label->Left = x + 6;
        label->Width = 63;
        label->Height = 14;

        editX->Top = y + 5;
        editX->Left = x + 70;
        editX->Width = 40;
        editX->Height = 14;

        editY->Top = y + 5;
        editY->Left = x + 113;
        editY->Width = 40;
        editY->Height = 14;

        editZ->Top = y + 5;
        editZ->Left = x + 156;
        editZ->Width = 40;
        editZ->Height = 14;
}


void __fastcall TVectorProperty::editXChange(TObject *Sender) {
        if (editX->Text.Length() <= 0) return;
        if (editY->Text.Length() <= 0) return;
        if (editZ->Text.Length() <= 0) return;

        string x = editX->Text.c_str();
        string y = editY->Text.c_str();
        string z = editZ->Text.c_str();

        for (unsigned int i = 0; i < x.length(); i++) {
                if (x.at(i) == ',')
                    x[i] = '.';
        }
        for (unsigned int i = 0; i < y.length(); i++) {
                if (y.at(i) == ',')
                    y[i] = '.';
        }
        for (unsigned int i = 0; i < z.length(); i++) {
                if (z.at(i) == ',')
                    z[i] = '.';
        }

        string s(_id);
        s.append(".");
        s.append(_setter);
        s.append("(");
        s.append(x);
        s.append(",");
        s.append(y);
        s.append(",");
        s.append(z);
        s.append(")");

        game_doscript(s.c_str());
}

void __fastcall TVectorProperty::editKeyDown(TObject *Sender, WORD &Key, TShiftState Shift) {
        if (Key == VK_UP) {
                Key = 0;
                if (Sender == editX)
                        editX->Text = FloatToStr(editX->Text.ToDouble() + 1);
                if (Sender == editY)
                        editY->Text = FloatToStr(StrToFloat(editY->Text) + 1);
                if (Sender == editZ)
                        editZ->Text = FloatToStr(StrToFloat(editZ->Text) + 1);
        }
        if (Key == VK_DOWN) {
                Key = 0;
                if (Sender == editX)
                        editX->Text = FloatToStr(StrToFloat(editX->Text) - 1);
                if (Sender == editY)
                        editY->Text = FloatToStr(StrToFloat(editY->Text) - 1);
                if (Sender == editZ)
                        editZ->Text = FloatToStr(StrToFloat(editZ->Text) - 1);
        }
}


void __fastcall TVectorProperty::editKeyPress(TObject *Sender, char &Key) {
        if ((Key >= 48 && Key <= 57) || Key == ','|| Key == '-' || Key == VK_UP || Key == VK_DOWN || Key == 8 || Key == 46) {
        } else {
                Key = 0;
        }
}


TVectorProperty::~TVectorProperty() {
        delete label;
        delete editX;
        delete editY;
        delete editZ;
}

// +++++++++++++++++++++++++++++++ COLOR


TColorProperty::TColorProperty(Controls::TWinControl* AOwner, string name, string obj_id, string setter, float def_x, float def_y, float def_z, float def_a)
{
        _id = obj_id;
        _setter = setter;

        label = new TLabel(AOwner);
        label->Parent = AOwner;
        label->Caption = AnsiString(name.c_str());

        editX = new TEdit(AOwner);
        editX->Parent = AOwner;
        editX->Text = AnsiString(FloatToStr(def_x));
        editX->BorderStyle = bsNone;
        editX->OnChange = editXChange;
        editX->OnKeyDown = editKeyDown;
        editX->OnKeyPress = editKeyPress;

        editY = new TEdit(AOwner);
        editY->Parent = AOwner;
        editY->Text = AnsiString(FloatToStr(def_y));
        editY->BorderStyle = bsNone;
        editY->OnChange = editXChange;
        editY->OnKeyDown = editKeyDown;

        editZ = new TEdit(AOwner);
        editZ->Parent = AOwner;
        editZ->Text = AnsiString(FloatToStr(def_z));
        editZ->BorderStyle = bsNone;
        editZ->OnChange = editXChange;
        editZ->OnKeyDown = editKeyDown;

        editA = new TEdit(AOwner);
        editA->Parent = AOwner;
        editA->Text = AnsiString(FloatToStr(def_a));
        editA->BorderStyle = bsNone;
        editA->OnChange = editXChange;
        editA->OnKeyDown = editKeyDown;
}


void TColorProperty::init(int x, int y)
{
        label->Top = y + 5;
        label->Left = x + 6;
        label->Width = 53;
        label->Height = 14;

        editX->Top = y + 5;
        editX->Left = x + 70;
        editX->Width = 25;
        editX->Height = 14;

        editY->Top = y + 5;
        editY->Left = x + 98;
        editY->Width = 25;
        editY->Height = 14;

        editZ->Top = y + 5;
        editZ->Left = x + 126;
        editZ->Width = 25;
        editZ->Height = 14;

        editA->Top = y + 5;
        editA->Left = x + 154;
        editA->Width = 25;
        editA->Height = 14;

}


void __fastcall TColorProperty::editXChange(TObject *Sender) {
        if (editX->Text.Length() <= 0) return;
        if (editY->Text.Length() <= 0) return;
        if (editZ->Text.Length() <= 0) return;
        if (editA->Text.Length() <= 0) return;

        string x = editX->Text.c_str();
        string y = editY->Text.c_str();
        string z = editZ->Text.c_str();
        string a = editA->Text.c_str();

        for (unsigned int i = 0; i < x.length(); i++) {
                if (x.at(i) == ',')
                    x[i] = '.';
        }
        for (unsigned int i = 0; i < y.length(); i++) {
                if (y.at(i) == ',')
                    y[i] = '.';
        }
        for (unsigned int i = 0; i < z.length(); i++) {
                if (z.at(i) == ',')
                    z[i] = '.';
        }
        for (unsigned int i = 0; i < a.length(); i++) {
                if (a.at(i) == ',')
                    a[i] = '.';
        }

        string s(_id);
        s.append(".");
        s.append(_setter);
        s.append("(");
        s.append(x);
        s.append(",");
        s.append(y);
        s.append(",");
        s.append(z);
        s.append(",");
        s.append(a);
        s.append(")");

        game_doscript(s.c_str());
}

void __fastcall TColorProperty::editKeyDown(TObject *Sender, WORD &Key, TShiftState Shift) {
        if (Key == VK_UP) {
                Key = 0;
                if (Sender == editX)
                        editX->Text = FloatToStr(editX->Text.ToDouble() + 0.1);
                if (Sender == editY)
                        editY->Text = FloatToStr(StrToFloat(editY->Text) + 0.1);
                if (Sender == editZ)
                        editZ->Text = FloatToStr(StrToFloat(editZ->Text) + 0.1);
                if (Sender == editA)
                        editA->Text = FloatToStr(StrToFloat(editA->Text) + 0.1);
        }
        if (Key == VK_DOWN) {
                Key = 0;
                if (Sender == editX)
                        editX->Text = FloatToStr(StrToFloat(editX->Text) - 0.1);
                if (Sender == editY)
                        editY->Text = FloatToStr(StrToFloat(editY->Text) - 0.1);
                if (Sender == editZ)
                        editZ->Text = FloatToStr(StrToFloat(editZ->Text) - 0.1);
                if (Sender == editA)
                        editA->Text = FloatToStr(StrToFloat(editA->Text) - 0.1);
        }
}


void __fastcall TColorProperty::editKeyPress(TObject *Sender, char &Key) {
        if ((Key >= 48 && Key <= 57) || Key == ',' || Key == VK_UP || Key == VK_DOWN || Key == 8 || Key == 46) {
        } else {
                Key = 0;
        }
}


TColorProperty::~TColorProperty() {
        delete label;
        delete editX;
        delete editY;
        delete editZ;
        delete editA;

}




// ++++++++++++++++++++++++ BOOL ++++++++++++++++

TBoolProperty::TBoolProperty(Controls::TWinControl* AOwner, string name, string obj_id, string setter, bool _def)
{
        _id = obj_id;
        _setter = setter;

        check = new TCheckBox(AOwner);
        check->Parent = AOwner;
        check->Caption = AnsiString(name.c_str());
        check->Alignment = taLeftJustify;
        check->Checked = _def;
        check->OnClick = CheckBoxClick;
}


void TBoolProperty::init(int x, int y)
{
        check->Top = y + 5;
        check->Left = x + 6;
        check->Width = 190;
        check->Height = 16;
}


void __fastcall TBoolProperty::CheckBoxClick(TObject *Sender) {
        string s(_id);
        s.append(".");
        s.append(_setter);
        s.append("(");
        s.append(check->Checked ? "true" : "false");
        s.append(")");

        game_doscript(s.c_str());
}

TBoolProperty::~TBoolProperty() {
        delete check;
}



// +++++++++++++++++++++++++++++  FLOAT  +++++++++++++++++++++++++++++++

TFloatProperty::TFloatProperty(Controls::TWinControl* AOwner, string name, string obj_id, string setter, float def_x)
{
        _id = obj_id;
        _setter = setter;

        label = new TLabel(AOwner);
        label->Parent = AOwner;
        label->Caption = AnsiString(name.c_str());

        editX = new TEdit(AOwner);
        editX->Parent = AOwner;
        editX->Text = AnsiString(FloatToStr(def_x));
        editX->BorderStyle = bsNone;
        editX->OnChange = editXChange;
        editX->OnKeyDown = editKeyDown;
        editX->OnKeyPress = editKeyPress;
}


void TFloatProperty::init(int x, int y)
{
        label->Top = y + 5;
        label->Left = x + 6;
        label->Width = 93;
        label->Height = 14;

        editX->Top = y + 5;
        editX->Left = x + 155;
        editX->Width = 40;
        editX->Height = 15;
}


void __fastcall TFloatProperty::editXChange(TObject *Sender) {
        if (editX->Text.Length() <= 0) return;

        string x = editX->Text.c_str();

        for (unsigned int i = 0; i < x.length(); i++) {
                if (x.at(i) == ',')
                    x[i] = '.';
        }

        string s(_id);
        s.append(".");
        s.append(_setter);
        s.append("(");
        s.append(x);
        s.append(")");

        game_doscript(s.c_str());
}

void __fastcall TFloatProperty::editKeyDown(TObject *Sender, WORD &Key, TShiftState Shift) {
        if (Key == VK_UP) {
                Key = 0;
                if (Sender == editX)
                        editX->Text = FloatToStr(editX->Text.ToDouble() + 0.1);
        }
        if (Key == VK_DOWN) {
                Key = 0;
                if (Sender == editX)
                        editX->Text = FloatToStr(StrToFloat(editX->Text) - 0.1);
        }
}


void __fastcall TFloatProperty::editKeyPress(TObject *Sender, char &Key) {
        if ((Key >= 48 && Key <= 57) || Key == ',' || Key == '-' || Key == VK_UP || Key == VK_DOWN || Key == 8 || Key == 46) {
        } else {
                Key = 0;
        }
}


TFloatProperty::~TFloatProperty() {
        delete label;
        delete editX;
}



// +++++++++++++++++++++++++++++  STRING  +++++++++++++++++++++++++++++++

TStringProperty::TStringProperty(Controls::TWinControl* AOwner, string name, string obj_id, string setter, string def_x)
{
        _id = obj_id;
        _setter = setter;

        label = new TLabel(AOwner);
        label->Parent = AOwner;
        label->Caption = AnsiString(name.c_str());

        editX = new TEdit(AOwner);
        editX->Parent = AOwner;
        editX->Text = AnsiString(def_x.c_str());
        editX->BorderStyle = bsNone;
        editX->OnKeyDown = editKeyDown;
}


void TStringProperty::init(int x, int y)
{
        label->Top = y + 5;
        label->Left = x + 6;
        label->Width = 73;
        label->Height = 14;

        editX->Top = y + 5;
        editX->Left = x + 71;
        editX->Width = 125;
        editX->Height = 15;
}


void __fastcall TStringProperty::editKeyDown(TObject *Sender, WORD &Key, TShiftState Shift) {
        if (Key == 13) {

                if (editX->Text.Length() <= 0) return;

                string x = editX->Text.c_str();

                string s(_id);
                s.append(".");
                s.append(_setter);
                s.append("(\"");
                s.append(x);
                s.append("\")");

                game_doscript(s.c_str());
                if (_setter == "setModel")
                        game_doscript("needRefreshProps = true");
        }
}


TStringProperty::~TStringProperty() {
        delete label;
        delete editX;
}


// +++++++++++++++++++++++++++++  LABEL  +++++++++++++++++++++++++++++++

TLabelProperty::TLabelProperty(Controls::TWinControl* AOwner, string name, string obj_id)
{
        _id = obj_id;
        label = new TLabel(AOwner);
        label->Parent = AOwner;
        label->Caption = AnsiString(name.c_str());
        label->Alignment = taCenter;
        label->Font->Style = label->Font->Style << fsBold;
}


void TLabelProperty::init(int x, int y)
{
        label->Top = y + 5;
        label->Left = x + 6;
        label->Width = 193;
        label->Height = 14;
}


TLabelProperty::~TLabelProperty() {
        delete label;
}



// +++++++++++++++++++++++++++++  COMBO BOX  +++++++++++++++++++++++++++++++


TListProperty::TListProperty(Controls::TWinControl* AOwner, string name, string obj_id, string setter, vector<string>names,  vector<string>values, string _def) {
        _id = obj_id;
        _setter = setter;
        this->names = names;
        this->values = values;

        label = new TLabel(AOwner);
        label->Parent = AOwner;
        label->Caption = AnsiString(name.c_str());

        combo = new TComboBox(AOwner);
        combo->Parent = AOwner;
        combo->Style = csDropDownList;

        int def_index = -1;
        for (unsigned int i = 0; i < names.size(); i++) {
                combo->Items->Add( AnsiString(names.at(i).c_str()) );
                if (values.at(i) == _def)
                        def_index = i;
        }

        combo->ItemIndex = def_index;

        combo->OnChange = onChange;
}


void TListProperty::init(int x, int y) {
        label->Top = y + 5;
        label->Left = x + 6;
        label->Width = 73;
        label->Height = 14;

        combo->Top = y + 1;
        combo->Left = x + 75;
        combo->Width = 120;
}


void __fastcall TListProperty::onChange(TObject *Sender) {
        if (combo->ItemIndex < 0) return;

        string x = values.at(combo->ItemIndex);

        string s(_id);
        s.append(".");
        s.append(_setter);
        s.append("(\"");
        s.append(x);
        s.append("\")");

        game_doscript(s.c_str());
}


TListProperty::~TListProperty() {
        delete label;
        delete combo;
}

