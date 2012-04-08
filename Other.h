//---------------------------------------------------------------------------

#ifndef OtherH
#define OtherH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "CSPIN.h"
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <ValEdit.hpp>


#include <string>
#include <vector>

using namespace std;


struct Event {
        string obj_id;
        string name;
        string params;
        string body;
};

//  Editor object types
#define EO_EVENT        1

class Property {
public:
        virtual ~Property();
};

Property::~Property() {
}

class TVectorProperty : public Property  {
public:
     string _id;
     string _setter;

     TLabel *label;
     TEdit *editX;
     TEdit *editY;
     TEdit *editZ;

     TVectorProperty(Controls::TWinControl* AOwner, string name, string obj_id, string setter, float def_x, float def_y, float def_z);
     void init(int x, int y);
     ~TVectorProperty();


     void __fastcall editXChange(TObject *Sender);
     void __fastcall editKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
     void __fastcall editKeyPress(TObject *Sender, char &Key);
};



class TColorProperty : public Property  {
public:
     string _id;
     string _setter;

     TLabel *label;
     TEdit *editX;
     TEdit *editY;
     TEdit *editZ;
     TEdit *editA;

     TColorProperty(Controls::TWinControl* AOwner, string name, string obj_id, string setter, float def_x, float def_y, float def_z, float a);
     void init(int x, int y);
     ~TColorProperty();


     void __fastcall editXChange(TObject *Sender);
     void __fastcall editKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
     void __fastcall editKeyPress(TObject *Sender, char &Key);
};


class TBoolProperty : public Property  {
public:
     string _id;
     string _setter;

     TLabel *label;
     TCheckBox *check;

     TBoolProperty(Controls::TWinControl* AOwner, string name, string obj_id, string setter, bool _def);
     void init(int x, int y);
     ~TBoolProperty();


     void __fastcall CheckBoxClick(TObject *Sender);
};



class TFloatProperty : public Property  {
public:
     string _id;
     string _setter;

     TLabel *label;
     TEdit *editX;

     TFloatProperty(Controls::TWinControl* AOwner, string name, string obj_id, string setter, float def_x);
     void init(int x, int y);
     ~TFloatProperty();


     void __fastcall editXChange(TObject *Sender);
     void __fastcall editKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
     void __fastcall editKeyPress(TObject *Sender, char &Key);
};


class TStringProperty : public Property  {
public:
     string _id;
     string _setter;

     TLabel *label;
     TEdit *editX;

     TStringProperty(Controls::TWinControl* AOwner, string name, string obj_id, string setter, string def_x);
     void init(int x, int y);
     ~TStringProperty();

     void __fastcall editKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
};



class TLabelProperty : public Property  {
public:
     string _id;
     TLabel *label;

     TLabelProperty(Controls::TWinControl* AOwner, string name, string obj_id);
     void init(int x, int y);
     ~TLabelProperty();
};




class TListProperty : public Property  {
public:
     string _id;
     string _setter;
     vector<string> names;
     vector<string> values;

     TLabel *label;
     TComboBox *combo;

     TListProperty(Controls::TWinControl* AOwner, string name, string obj_id, string setter, vector<string>names,  vector<string>values, string _def);
     void init(int x, int y);
     void __fastcall onChange(TObject *Sender);
     ~TListProperty();
};



#endif
 