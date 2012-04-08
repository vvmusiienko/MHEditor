//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include <vcl\condefs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <float.h>
#include <BorlandBridge.h>
#include <fcntl.h>

#include <io.h>

#include <iostream>

#include <fstream>
#include "ScriptEditor.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"

// Collision objects (only for editor)
#define GO_COMPOUND_CS			96
#define GO_BOX_CS			97
#define GO_SPHERE_CS			98
#define GO_CYLINDER_CS			99
#define GO_UNDEFINED_CS			100
#define GO_CONE_CS			101
#define GO_CAPSULE_CS			102
#define GO_MESH_CS			103

// Constraints
#define GO_P2P_CONSTRAINT		128
#define GO_HINGE_CONSTRAINT		129
#define GO_SLIDER_CONSTRAINT	130


enum MouseButton {
	LEFT,
	MIDDLE,
	RIGHT
};

TForm1 *Form1;
__int64 perfFreq, perfLast, perfCurrent, perfDiff;
bool perfFreqDetermined = false;


string currentObjID;
Event* currentEvent = NULL;
string lastSelectedEvent = "NoNe";
bool redirMoveKeys = false;

void NodeStack::pushNode(TTreeNode* node) {
        items.push_back(node);
}

TTreeNode* NodeStack::popNode() {
        items.erase(items.begin() + items.size()-1);
        if (items.size() <= 0)
                return NULL;
        return items.at(items.size()-1);
}

NodeStack::NodeStack() {
}
NodeStack::~NodeStack() {
}

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
    Application->OnIdle = IdleLoop; 
    _control87(MCW_EM, MCW_EM);
    inputToEngine = false;
    if (!QueryPerformanceFrequency((LARGE_INTEGER*)&perfFreq) )
    {
	MessageBox(0, "Error while QueryPerformanceFrequency","Error!", MB_OK | MB_ICONERROR);
    }
        path = string(Application->ExeName.c_str());
	for (unsigned int i = 1; i < path.size(); i++) {
		if (path[path.size()-i] == '\\') {
			path = path.substr(0, path.size()-i+1);
			break;
		}
	}

    play = false;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::IdleLoop(TObject*, bool& done) 
{ 
     done = false; 
     RenderGLScene(); 
     SwapBuffers(hdc); 
}


void __fastcall TForm1::RenderGLScene() 
{ 
	QueryPerformanceCounter((LARGE_INTEGER*)&perfCurrent);

	static float c = 1000000/45;

        	game_iterate( perfDiff / c );

	QueryPerformanceCounter((LARGE_INTEGER*)&perfLast);
	perfDiff = ((perfLast - perfCurrent) * 1000000) / perfFreq;
}

void __fastcall TForm1::FormCreate(TObject *Sender)
{
        hdc = GetDC(OpenGLPanel->Handle);
        SetPixelFormatDescriptor();
        hrc = wglCreateContext(hdc);
        wglMakeCurrent(hdc, hrc);
        lastSelectionCount = 0;

        cs_nodeStack = new NodeStack();
        cs_currentNode = NULL;
        cs_parentNode = NULL;
        currentNode = NULL;
        parentNode = NULL;
        dropAllowed = false;

        adapter_set_fields(hdc, OpenGLPanel->Handle);
        game_init();
        game_regfunc("showMessage", l_showMessage);
        game_regfunc("editor_setMapMaskSizeAndOffset", l_setMapMaskSizeAndOffset);
        game_regfunc("editor_setMapMask", l_setMapMask);
        game_regfunc("editor_addProperty", l_addProperty);
        game_regfunc("editor_clearProperties", l_clearProperties);
        game_regfunc("editor_afterMapSaved", l_afterMapSaved);
        game_regfunc("editor_addNode", l_addNode);
        game_regfunc("editor_pushNode", l_pushNode);
        game_regfunc("editor_popNode", l_popNode);
        game_regfunc("editor_clearTree", l_clearTree);
        game_regfunc("editor_setCurrentObjectID", l_setCurrentObjectID);
        // Edit collision shape
        game_regfunc("editor_cs_addNode", l_cs_addNode);
        game_regfunc("editor_cs_pushNode", l_cs_pushNode);
        game_regfunc("editor_cs_popNode", l_cs_popNode);
        game_regfunc("editor_cs_clearTree", l_cs_clearTree);
        // constraints
        game_regfunc("editor_addConstraint", l_addConstraint);
        game_regfunc("editor_clearConstraints", l_clearConstraints);

        game_regfunc("editor_restoreLastSelection", l_restoreLastSelection);
        game_regfunc("editor_selectTreeNode", l_selectTreeNode);
        game_regfunc("editor_setAccess", l_setAccess);


        game_doscript("dofile(\"./scripts/editor.lua\")");
        game_doscript("refreshList()");

        physDraw->Checked = true;

        if (game_config_exist("last_map")) {
                string lastMap = game_config_get_str("last_map");
                if (lastMap != "") {
                        if(!game_load(lastMap.c_str())) {
                                events.clear();
                                lastSavePath = "";
                                game_doscript("new_game()");
                                game_doscript("refreshList()");
                                return;
                        }
                        if (!loadMap(lastMap.c_str())) {
                                events.clear();
                                lastSavePath = "";
                                game_doscript("new_game()");
                                game_doscript("refreshList()");
                                return;
                        }
                        lastSavePath = lastMap.c_str();
                        game_doscript("refreshList()");
                }
        }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::SetPixelFormatDescriptor() 
{ 
    PIXELFORMATDESCRIPTOR pfd = { 
        sizeof(PIXELFORMATDESCRIPTOR), 
        1, 
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, 
        PFD_TYPE_RGBA, 
        24, 
        0,0,0,0,0,0, 
        0,0, 
        0,0,0,0,0, 
        32, 
        0, 
        0, 
        PFD_MAIN_PLANE, 
        0, 
        0,0,0 
        }; 
    PixelFormat = ChoosePixelFormat(hdc, &pfd); 
    SetPixelFormat(hdc, PixelFormat, &pfd); 
}


//--------------------------------------------------------------------------- 
void __fastcall TForm1::FormDestroy(TObject *Sender)
{
        game_finalize();        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OpenGLPanelResize(TObject *Sender)
{             
        game_set_screen_size(OpenGLPanel->ClientWidth, OpenGLPanel->ClientHeight);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OpenGLPanelMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
        if (Button == mbLeft)
           game_mousedown(0);
        if (Button == mbMiddle) {
           redirMoveKeys = true;
           game_mousedown(1);
        }
        if (Button == mbRight)
           game_mousedown(2);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OpenGLPanelMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
        if (Button == mbLeft)
           game_mouseup(0);
        if (Button == mbMiddle) {
           redirMoveKeys = false;
           game_mouseup(1);
        }
        if (Button == mbRight)
           game_mouseup(2);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OpenGLPanelMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
        game_moved(X, Y);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
       if (Key == 16) {
                game_doscript("lockAxle = true;");
       }

        if (inputToEngine || redirMoveKeys) {
                game_keydown(Key);
                Key = 0;
        }

        if (Key == 17) {
                game_doscript("appendSelect = true");
                game_doscript("ctrlIsDown = true");
        }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
       if (Key == 16) {
                game_doscript("lockAxle = false; lockX = 1; lockY = 1; lockedAxleSetted = false");
       }

        if (inputToEngine || redirMoveKeys) {
                game_keyup(Key);
                Key = 0;
        }

        if (Key == 17) {
                game_doscript("appendSelect = false");
                game_doscript("ctrlIsDown = false");
        }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormKeyPress(TObject *Sender, char &Key)
{
        if (inputToEngine || redirMoveKeys) {
                game_keypressed(Key);
                Key = 0;
        }


}
//---------------------------------------------------------------------------


int TForm1::l_setCurrentObjectID(lua_State* luaVM) {
        Form1->currentObjectID = lua_tostring(luaVM, 1);
        return 0;
}


int TForm1::l_addProperty(lua_State* luaVM) {
        switch(lua_tointeger(luaVM, 1)) {
                case OP_VECTOR: {
                        TVectorProperty* p = new TVectorProperty(Form1->ScrollBox1, lua_tostring(luaVM, 2), lua_tostring(luaVM, 3), lua_tostring(luaVM, 4), lua_tonumber(luaVM, 5), lua_tonumber(luaVM, 6), lua_tonumber(luaVM, 7));
                        p->init(0, 20 * Form1->properties.size());
                        Form1->properties.push_back(p);
                break;}
                case OP_COLOR: {
                        TColorProperty* p = new TColorProperty(Form1->ScrollBox1, lua_tostring(luaVM, 2), lua_tostring(luaVM, 3), lua_tostring(luaVM, 4), lua_tonumber(luaVM, 5), lua_tonumber(luaVM, 6), lua_tonumber(luaVM, 7), lua_tonumber(luaVM, 8));
                        p->init(0, 20 * Form1->properties.size());
                        Form1->properties.push_back(p);
                break;}
                case OP_BOOL: {
                        TBoolProperty* p = new TBoolProperty(Form1->ScrollBox1, lua_tostring(luaVM, 2), lua_tostring(luaVM, 3), lua_tostring(luaVM, 4), lua_toboolean(luaVM, 5));
                        p->init(0, 20 * Form1->properties.size());
                        Form1->properties.push_back(p);
                break;}
                case OP_FLOAT: {
                        TFloatProperty* p = new TFloatProperty(Form1->ScrollBox1, lua_tostring(luaVM, 2), lua_tostring(luaVM, 3), lua_tostring(luaVM, 4), lua_tonumber(luaVM, 5));
                        p->init(0, 20 * Form1->properties.size());
                        Form1->properties.push_back(p);
                break;}
                case OP_STRING: {
                        TStringProperty* p = new TStringProperty(Form1->ScrollBox1, lua_tostring(luaVM, 2), lua_tostring(luaVM, 3), lua_tostring(luaVM, 4), lua_tostring(luaVM, 5));
                        p->init(0, 20 * Form1->properties.size());
                        Form1->properties.push_back(p);
                break;}
                case OP_LABEL: {
                        TLabelProperty* p = new TLabelProperty(Form1->ScrollBox1, lua_tostring(luaVM, 2), lua_tostring(luaVM, 3));
                        p->init(0, 20 * Form1->properties.size());
                        Form1->properties.push_back(p);
                break;}
                case OP_LIST: {
                        int count = lua_tointeger(luaVM, 5);
                        vector<string> names;
                        vector<string> values;
                        for (int i = 0; i < count; i++) {
                                lua_pushinteger(luaVM, i+1); // now the string "bar" is just above the table on the stack
                                lua_gettable(luaVM, 6); // finds the table foo in stack position -2, pops "bar" off the stack, pushes on foo["bar"]
                                names.push_back(lua_tostring(luaVM, -1));
                                lua_pop(luaVM,1);
                                lua_pushinteger(luaVM, i+1); // now the string "bar" is just above the table on the stack
                                lua_gettable(luaVM, 7); // finds the table foo in stack position -2, pops "bar" off the stack, pushes on foo["bar"]
                                values.push_back(lua_tostring(luaVM, -1));
                                lua_pop(luaVM,1);
                        }

                        TListProperty* p = new TListProperty(Form1->ScrollBox1, lua_tostring(luaVM, 2), lua_tostring(luaVM, 3), lua_tostring(luaVM, 4), names, values, lua_tostring(luaVM, 8));
                        p->init(0, 20 * Form1->properties.size());
                        Form1->properties.push_back(p);
                break;}
                case OP_EVENT: {
                        Form1->ListBox2->Items->Add(lua_tostring(luaVM, 2));
                        bool needNew = true;
                        for (unsigned int i = 0; i < Form1->events.size(); i++) {
                                if (Form1->events.at(i)->obj_id.compare(lua_tostring(luaVM, 3)) == 0 &&
                                Form1->events.at(i)->name.compare(lua_tostring(luaVM, 2)) == 0 ) {

                                        needNew = false;
                                        break;
                                }

                        }
                        if (needNew) {
                                Event* e = new Event;
                                e->obj_id = lua_tostring(luaVM, 3);
                                e->name = lua_tostring(luaVM, 2);
                                e->params = lua_tostring(luaVM, 4);
                                e->body = "";
                                Form1->events.push_back(e);
                        }
                        currentObjID = lua_tostring(luaVM, 3);

                        if (lastSelectedEvent.compare(lua_tostring(luaVM, 2)) == 0) {
                                Form1->ListBox2->ItemIndex = Form1->ListBox2->Items->Count-1;
                                Form1->ListBox2->OnClick(Form1->ListBox2);
                        }
                break;}
        }
        return 0;
}

int TForm1::l_clearProperties(lua_State* luaVM) {
        Form3->Memo1->OnExit(Form3->Memo1); // because clearProperties calls before  Form1->Memo1->OnExit
        for (unsigned int i = 0; i < Form1->properties.size(); i++)
                delete Form1->properties.at(i);
        Form1->properties.clear();
        //Form1->events.clear();
        Form1->ListBox2->Items->Clear();
        currentEvent = NULL;
        Form3->Memo1->Text = "No Event Selected";
        Form3->Memo1->Enabled = false;
        Form1->Label1->Caption = "";
        return 0; 
}


void TForm1::applyScript()  {
        if (!currentEvent) return;
        currentEvent->body = Form3->Memo1->Text.c_str();

        string s(currentEvent->obj_id);
        s.append(".");
        s.append(currentEvent->name);
        s.append(" = function");
        s.append(currentEvent->params);
        s.append(" " + currentEvent->body);
        s.append(string(" ") + "end");

        game_doscript(s.c_str());
}


int typeToImageIndex(int type) {
     // 0 this is map;
     int imageIndex = type + 1;
     if (type == 255)
        return 8;
     return imageIndex;
}

AnsiString typeToString(int type) {
        return "";
}


void selectTreeNode(int absoluteIndex) {
        TTreeNode* n = NULL;
        for (int i = 0; i < Form1->TreeView1->Items->Count; i++) {
                if (Form1->TreeView1->Items->Item[i]->AbsoluteIndex == absoluteIndex)
                {
                        n = Form1->TreeView1->Items->Item[i];
                        break;
                }
        }
        if (n)
                Form1->TreeView1->Selected = n;
}


int TForm1::l_selectTreeNode(lua_State* luaVM) {
        selectTreeNode(lua_tointeger(luaVM, 1));
        return 0;
}


int TForm1::l_addNode(lua_State* luaVM) {
        Form1->currentNode = Form1->TreeView1->Items->AddChild(Form1->parentNode, AnsiString(lua_tostring(luaVM, 1)));
        Form1->currentNode->ImageIndex = typeToImageIndex(lua_tointeger(luaVM, 2));
        Form1->currentNode->SelectedIndex = typeToImageIndex(lua_tointeger(luaVM, 2));// = typeToImageIndex(lua_tointeger(luaVM, 2));
        //Form1->currentNode->
        // Adding TreeNode index for selecting node while mouse click on object
        game_doscript((AnsiString(lua_tostring(luaVM, 1)) + ".treeNodeIndex = " + IntToStr(Form1->currentNode->AbsoluteIndex)).c_str());
        return 0;
}


int TForm1::l_pushNode(lua_State* luaVM) {
        Form1->nodeStack.pushNode(Form1->currentNode);
        Form1->parentNode = Form1->currentNode;
        //game_doscript("printf(\"push \")");
        return 0;
}


int TForm1::l_popNode(lua_State* luaVM) {
        Form1->parentNode = Form1->nodeStack.popNode();
        //game_doscript("printf(\"pop \")");
        return 0;
}


int TForm1::l_clearTree(lua_State* luaVM) {
        Form1->parentNode = NULL;
        Form1->currentNode = NULL;
        Form1->TreeView1->Items->Clear();
        return 0;
}


int TForm1::l_restoreLastSelection(lua_State* luaVM) {
        selectTreeNode(Form1->lastSelectedNodeIndex);
        return 0;
}

int TForm1::l_afterMapSaved(lua_State* luaVM) {
        return 0;
}


void __fastcall TForm1::inpToEngineClick(TObject *Sender)
{   
        inputToEngine = inpToEngine->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton1Click(TObject *Sender)
{
        game_doscript("moveMode = true");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton2Click(TObject *Sender)
{
        game_doscript("moveMode = false");
}
//---------------------------------------------------------------------------


void __fastcall TForm1::SpeedButton3Click(TObject *Sender)
{
        if (Edit1->Text.Length() == 0) {
                ShowMessage("Enter Name!");
                return;
        }
        if (game_idused(Edit1->Text.c_str())) {
                ShowMessage("ID already used!");
                return;
        }
        int type_id = ComboBox1->ItemIndex;

        if (type_id == 6)
                type_id = 255;
        if (type_id == 5)
                type_id = 6;

        game_doscript(("editor.createObject(" + IntToStr(type_id) + ",\"" + Edit1->Text + "\")").c_str());
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton4Click(TObject *Sender)
{
        game_doscript("deleteSelectedObjects()");
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ListBox2Click(TObject *Sender)
{
        if (ListBox2->ItemIndex <= -1) return;

        Event* e = NULL;
        for (unsigned int i = 0; i < events.size(); i++) {
                if (events.at(i)->obj_id.compare(currentObjID) == 0 && events.at(i)->name.compare(ListBox2->Items->Strings[ListBox2->ItemIndex].c_str()) == 0) {
                        e =  Form1->events.at(i);
                        break;
                }
        }
        if (!e) { ShowMessage("Event not found in events vector"); return;}
        Form3->Memo1->Enabled = true;
        currentEvent = e;
        Form3->Memo1->Text = e->body.c_str();
        Label1->Caption = (e->name + e->params).c_str();
        lastSelectedEvent = e->name;
        Form3->Show();
}
//---------------------------------------------------------------------------



void __fastcall TForm1::SaveMap1Click(TObject *Sender)
{
        if (play) {
                ShowMessage("First stop game!");
                return;
        }
        if(saveMapDialog->Execute()) {
                saveMap(saveMapDialog->FileName.c_str());
                lastSavePath = saveMapDialog->FileName.c_str();
        }
}
//---------------------------------------------------------------------------


void TForm1::saveMap(string path) {
        string s = path;
        for (unsigned int i = 0; i < s.length(); i++) {
                if (s[i] == '\\') {
                        s.insert(i, "\\");
                        i++;
                }
        }

        game_doscript("game.onObjectSelected(nil, false)"); // for return all selected colors to normal
        // generating script
        string script = "";
        string itersPart = "";
        /*vector<string> ids;
        //  get all ID's
        for (unsigned int i = 0; i < events.size(); i++) {
                Event* e = events.at(i);
                if (e->body.length() <= 0) continue;
                bool included = false;
                for (unsigned int j = 0; j < ids.size(); j++) {
                        if (ids.at(j) == e->obj_id)
                                included = true;
                }
                if (!included && e->obj_id != "map")
                        ids.push_back(e->obj_id);
        }

        for (unsigned int j = 0; j < ids.size(); j++) {
                //script.append(ids.at(j)+" = {}");
        }*/

        script.append("\n\n");

        for (unsigned int i = 0; i < events.size(); i++) {
                Event* e = events.at(i);
                if (e->body.length() <= 0 || (!game_idused(e->obj_id.c_str()) && e->obj_id != "map" ) ) continue;

                script.append(e->obj_id+"."+e->name+" = function"+e->params+"\n"+e->body+"\nend\n\n\n");

                if (e->name == "onIterate" && e->obj_id != "map")
                        itersPart.append("game.registerIterator(" + e->obj_id+", "+e->obj_id+"."+e->name+")\n");


        }

        script.append("\n");
        script.append(itersPart);

        FILE* ff = fopen("fuck.txt", "w");
        fwrite(script.c_str(), script.length(), 1, ff);
        fclose(ff);

        if (!game_save(s.c_str(), script.c_str())) {
                ShowMessage("Can't save map!");
                return;
        }

	FILE* f = fopen(path.c_str(), "a+b");
        if (!f) {
                ShowMessage("Can't save map!");
                return;
        }

	// Write count of objects
	int count = 0;
	for (unsigned int i = 0; i < events.size(); i++) {
                if (events.at(i)->body.length() > 0  && !(!game_idused(events.at(i)->obj_id.c_str()) && events.at(i)->obj_id != "map" ) ) count++;
        }
	fwrite(&count, sizeof count, 1, f);
	for (unsigned int i = 0; i < events.size(); i++) {
                Event* e = events.at(i);
                if (e->body.length() <= 0  || (!game_idused(e->obj_id.c_str()) && e->obj_id != "map" ) ) continue;

                int type = EO_EVENT;
                int sizeOfString = 0;
	        fwrite(&type, sizeof type, 1, f);

		sizeOfString = e->obj_id.size(); // Write string without null char
		fwrite(&sizeOfString, sizeof sizeOfString, 1, f); // size
		fwrite(e->obj_id.c_str(), sizeOfString, 1, f); // body

		sizeOfString = e->name.size(); // Write string without null char
		fwrite(&sizeOfString, sizeof sizeOfString, 1, f); // size
		fwrite(e->name.c_str(), sizeOfString, 1, f); // body

		sizeOfString = e->params.size(); // Write string without null char
		fwrite(&sizeOfString, sizeof sizeOfString, 1, f); // size
		fwrite(e->params.c_str(), sizeOfString, 1, f); // body

		sizeOfString = e->body.size(); // Write string without null char
		fwrite(&sizeOfString, sizeof sizeOfString, 1, f); // size
		fwrite(e->body.c_str(), sizeOfString, 1, f); // body
	}
	fclose(f);
}


bool TForm1::loadMap(string path) {
        events.clear();
	FILE* f = fopen(path.c_str(), "rb");

        if (! f) {
                return false;
        }

	// Read offset
        fseek(f, sizeof(int), SEEK_CUR);
	int offset;
	fread(&offset, sizeof offset, 1, f);
        // go to editor section
        fseek(f, offset, SEEK_SET);
        // load...
	int count;
	fread(&count, sizeof count, 1, f);
	for (int i = 0; i < count; i++) {
                int type;
        	fread(&type, sizeof type, 1, f); // read Type
		switch (type) {
			case EO_EVENT: {
                                Event* e = new Event;
                                int sizeOfString; // Write string without null char
                                char* buffer;

			        fread(&sizeOfString, sizeof sizeOfString, 1, f); // size
			        buffer = new char[sizeOfString + 1];
			        fread(buffer, sizeOfString, 1, f); // body
			        buffer[sizeOfString] = 0;
			        e->obj_id = string(buffer);

			        fread(&sizeOfString, sizeof sizeOfString, 1, f); // size
			        buffer = new char[sizeOfString + 1];
			        fread(buffer, sizeOfString, 1, f); // body
			        buffer[sizeOfString] = 0;
			        e->name = string(buffer);

			        fread(&sizeOfString, sizeof sizeOfString, 1, f); // size
			        buffer = new char[sizeOfString + 1];
			        fread(buffer, sizeOfString, 1, f); // body
			        buffer[sizeOfString] = 0;
			        e->params = string(buffer);

			        fread(&sizeOfString, sizeof sizeOfString, 1, f); // size
			        buffer = new char[sizeOfString + 1];
			        fread(buffer, sizeOfString, 1, f); // body
			        buffer[sizeOfString] = 0;
			        e->body = string(buffer);

                                events.push_back(e);
			break;}
                }
        }
        fclose(f);
        return true;
}

void __fastcall TForm1::LoadMap1Click(TObject *Sender)
{
        if (play) {
                ShowMessage("First stop game!");
                return;        
        }
        if(openMapDialog->Execute()) {
                game_load(openMapDialog->FileName.c_str());
                if (!loadMap(openMapDialog->FileName.c_str())) {
                        ShowMessage("Can't open map!");
                        events.clear();
                        lastSavePath = "";
                        game_doscript("new_game()");
                        game_doscript("refreshList()");
                        return;
                }
                lastSavePath = openMapDialog->FileName.c_str();
                game_doscript("refreshList()");
                game_config_save_str("last_map", openMapDialog->FileName.c_str());
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton5Click(TObject *Sender)
{
        //Form3->Memo1->OnExit(Form3->Memo1);
        Form3->Show();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::SpeedButton6Click(TObject *Sender)
{
        if (play) return;
        saveMap(path + "map.tmp");
        game_doscript("s_editorMode(false)");
        game_load((path + "map.tmp").c_str());
        loadMap((path + "map.tmp").c_str());
        game_doscript("editor_cs_clearTree()");
        game_doscript("editor_clearConstraints()");
        inpToEngine->Checked = true;
        physDraw->Checked = false;
        play = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton8Click(TObject *Sender)
{
        if (!play) return;
        game_doscript("s_editorMode(true)");
        game_load((path + "map.tmp").c_str());
        loadMap((path + "map.tmp").c_str());
        game_doscript("selectedObjs = {}");
        game_doscript("editor_cs_clearTree()");
        game_doscript("editor_clearConstraints()");
        inpToEngine->Checked = false;
        physDraw->Checked = true;
        play = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton7Click(TObject *Sender)
{
        if (!play) return;
        game_doscript("s_editorMode(not g_editorMode())");
}
//---------------------------------------------------------------------------



void __fastcall TForm1::SpeedButton9Click(TObject *Sender)
{
        if (play) {
                ShowMessage("First stop game!");
                return;
        }
        if (lastSavePath == "") {
                ShowMessage("First save map for \"quick Saving\"");
                return;
        }
        saveMap(lastSavePath.c_str());
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


int TForm1::l_showMessage(lua_State* luaVM) {
        ShowMessage(AnsiString(lua_tostring(luaVM, 1)));
        return 0;
}


int TForm1::l_setMapMaskSizeAndOffset(lua_State* luaVM) {
        return 0;
}


int TForm1::l_setMapMask(lua_State* luaVM) {
        Form1->editorMask[lua_tointeger(luaVM, 1)][lua_tointeger(luaVM, 2)] = (bool)lua_tointeger(luaVM, 3);
        return 0;
}

void __fastcall TForm1::TreeView1Click(TObject *Sender)
{
        if (TreeView1->SelectionCount == lastSelectionCount) {
                return;
        }

        if (TreeView1->Selected) {
                string s;
                game_doscript("needSelectTree = false");
                s.append("game.onObjectSelected(");
                s.append(TreeView1->Selected->Text.c_str());
                s.append(TreeView1->SelectionCount == 1 ? ", false)" : ", true)");
                game_doscript(s.c_str());
                if (TreeView1->SelectionCount == 1) {
                        game_doscript("refreshCollisionShape()");
                        game_doscript("refreshConstraints()");
                lastSelectionCount = 0;
                } else {
                lastSelectionCount = TreeView1->SelectionCount;
                }
                lastSelectedNodeIndex = TreeView1->Selected->AbsoluteIndex;
                //ShowMessage(TreeView1->Selected->AbsoluteIndex);
        }
}
//---------------------------------------------------------------------------



void __fastcall TForm1::NewMap1Click(TObject *Sender)
{
        events.clear();
        lastSavePath = "";
        game_doscript("new_game()");
        game_doscript("refreshList()");
}
//---------------------------------------------------------------------------


int TForm1::l_setAccess(lua_State* luaVM) { // set flag to true if dragdrop allowed for specified objects
        Form1->dropAllowed = lua_toboolean(luaVM, 1);
        //game_doscript(AnsiString(("printf(")+ IntToStr(Form1->dropAllowed) +AnsiString(")  asd")).c_str());
        return 0;
}

void __fastcall TForm1::TreeView1DragOver(TObject *Sender, TObject *Source,
      int X, int Y, TDragState State, bool &Accept)
{
        TTreeNode* to = TreeView1->GetNodeAt(X, Y);
        if (TreeView1->Selected && to) {
                string s;
                s.append("checkDropPossibility(");
                s.append(TreeView1->Selected->Text.c_str());
                s.append(", ");
                s.append(to->Text.c_str());
                s.append(")");
                game_doscript(s.c_str());
                Accept = dropAllowed;
                return;
        }

        Accept = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TreeView1DragDrop(TObject *Sender, TObject *Source,
      int X, int Y)
{
        TTreeNode* to = TreeView1->GetNodeAt(X, Y);
        if (TreeView1->Selected && to) {
                string s;
                s.append("moveToParent(");
                s.append(TreeView1->Selected->Text.c_str());
                s.append(", ");
                s.append(to->Text.c_str());
                s.append(")");
                game_doscript(s.c_str());
                lastSelectedNodeIndex = to->AbsoluteIndex;
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::physDrawClick(TObject *Sender)
{
        if (physDraw->Checked)
                game_doscript("s_debugPhysDraw(true)");
        else
                game_doscript("s_debugPhysDraw(false)");

}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormResize(TObject *Sender)
{
        game_set_screen_size(OpenGLPanel->Width, OpenGLPanel->Height);
}
//---------------------------------------------------------------------------


AnsiString typeToName(int type) {
        switch (type) {
                case GO_COMPOUND_CS:
                return "Compound";
                case GO_BOX_CS:
                return "Box";
                case GO_SPHERE_CS:
                return "Sphere";
                case GO_CYLINDER_CS:
                return "Cylinder";
                case GO_UNDEFINED_CS:
                return "Undefined";
                case GO_CAPSULE_CS:
                return "Capsule";
                case GO_CONE_CS:
                return "Cone";
                case GO_MESH_CS:
                return "Mesh";
        }
        ShowMessage(IntToStr(type));
        return "wtf?";
}


int TForm1::l_cs_addNode(lua_State* luaVM) {
        Form1->cs_currentNode = Form1->TreeView2->Items->AddChild(Form1->cs_parentNode, typeToName(lua_tointeger(luaVM, 2)));
        Form1->cs_currentNode->Data = (void *)lua_tostring(luaVM, 1);
        // Adding TreeNode index for selecting node while mouse click on object
        game_doscript((AnsiString(lua_tostring(luaVM, 1)) + ".treeNodeIndex = " + IntToStr(Form1->cs_currentNode->AbsoluteIndex)).c_str());
        return 0;
}


int TForm1::l_cs_pushNode(lua_State* luaVM) {
        Form1->cs_nodeStack->pushNode(Form1->cs_currentNode);
        Form1->cs_parentNode = Form1->cs_currentNode;
        //game_doscript("printf(\"push \")");
        return 0;
}


int TForm1::l_cs_popNode(lua_State* luaVM) {
        Form1->cs_parentNode = Form1->cs_nodeStack->popNode();
        //game_doscript("printf(\"pop \")");
        return 0;
}


int TForm1::l_cs_clearTree(lua_State* luaVM) {
        Form1->cs_parentNode = NULL;
        Form1->cs_currentNode = NULL;
        Form1->TreeView2->Items->Clear();
        return 0;
}
void __fastcall TForm1::TreeView2Click(TObject *Sender)
{
        if (TreeView2->Selected) {
                game_doscript("needSelectTree = false");
                string s;
                s.append("game.onObjectSelected(");
                s.append(((char*)TreeView2->Selected->Data));
                s.append(", false)");
                game_doscript(s.c_str());
                //lastSelectedNodeIndex = TreeView1->Selected->AbsoluteIndex;
                //ShowMessage(TreeView1->Selected->AbsoluteIndex);
        }
}
//---------------------------------------------------------------------------


void TForm1::addCS(int type) {
        if (TreeView2->Selected) {
                string s;
                s.append("addCollisionShape(");
                s.append(((char*)TreeView2->Selected->Data));
                s.append(", ");
                s.append(IntToStr(type).c_str());
                s.append(")");
                game_doscript(s.c_str());
        }
}
void __fastcall TForm1::SpeedButton11Click(TObject *Sender)
{
        addCS(GO_COMPOUND_CS);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton12Click(TObject *Sender)
{
        addCS(GO_BOX_CS);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton13Click(TObject *Sender)
{
        addCS(GO_SPHERE_CS);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton14Click(TObject *Sender)
{
        addCS(GO_CYLINDER_CS);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton15Click(TObject *Sender)
{
        addCS(GO_CAPSULE_CS);        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton16Click(TObject *Sender)
{
        addCS(GO_CONE_CS);        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton17Click(TObject *Sender)
{
        if (TreeView2->Selected) {
                string s;
                s.append("deleteCollisionShape(");
                s.append(((char*)TreeView2->Selected->Data));
                s.append(")");
                game_doscript(s.c_str());
        }
}
//---------------------------------------------------------------------------

AnsiString constraintTypeToStr(int type) {
        switch (type) {
                case GO_P2P_CONSTRAINT:
                return "P2P";
                case GO_HINGE_CONSTRAINT:
                return "Hinge";
                case GO_SLIDER_CONSTRAINT:
                return "Slider";

        }
        return "Undefined";
}

// 1 - name, 2 - type, 3 - as second object
int TForm1::l_addConstraint(lua_State* luaVM) {
        if (lua_tointeger(luaVM, 3) == 0) {
                Form1->ListBox1->Items->Add(AnsiString(lua_tostring(luaVM, 1)) + " ("+constraintTypeToStr(lua_tointeger(luaVM, 2))+")" );
        } else {
                Form1->ListBox3->Items->Add(AnsiString(lua_tostring(luaVM, 1)) + " ("+constraintTypeToStr(lua_tointeger(luaVM, 2))+")" );
        }
        return 0;
}


int TForm1::l_clearConstraints(lua_State* luaVM) {
        Form1->ListBox1->Items->Clear();
        Form1->ListBox3->Items->Clear();
        return 0;
}

void __fastcall TForm1::ListBox1Click(TObject *Sender)
{
        if (ListBox1->ItemIndex >= 0) {
                game_doscript("needSelectTree = false");
                string s;
                s.append("selectConstraint(");
                s.append(IntToStr(ListBox1->ItemIndex+1).c_str());
                s.append(", 0)");
                game_doscript(s.c_str());
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListBox3Click(TObject *Sender)
{
        if (ListBox3->ItemIndex >= 0) {
                game_doscript("needSelectTree = false");
                string s;
                s.append("selectConstraint(");
                s.append(IntToStr(ListBox3->ItemIndex+1).c_str());
                s.append(", 1)");
                game_doscript(s.c_str());
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton20Click(TObject *Sender)
{
        if (ListBox1->ItemIndex >= 0) {
                string s;
                s.append("deleteConstraint(");
                s.append(IntToStr(ListBox1->ItemIndex+1).c_str());
                s.append(")");
                game_doscript(s.c_str());
        }
}
//---------------------------------------------------------------------------

void TForm1::addCN(int type) {
        string s;
        s.append("addConstraint(");
        s.append(IntToStr(type).c_str());
        s.append(", \"");
        s.append(Edit3->Text.c_str());
        s.append("\", ");
        s.append(Edit3->Text != "" ? Edit3->Text.c_str() : "puckaGellsd787__dd");
        s.append(")");
        game_doscript(s.c_str());
}

void __fastcall TForm1::SpeedButton10Click(TObject *Sender)
{
        addCN(GO_P2P_CONSTRAINT);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton18Click(TObject *Sender)
{
        addCN(GO_HINGE_CONSTRAINT);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton19Click(TObject *Sender)
{
        addCN(GO_SLIDER_CONSTRAINT);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::SpeedButton21Click(TObject *Sender)
{
        Form2->Show();        
}
//---------------------------------------------------------------------------


void __fastcall TForm1::SpeedButton22Click(TObject *Sender)
{
        addCS(GO_MESH_CS);        
}
//---------------------------------------------------------------------------

bool isHaveRoot(string root, string path) {
        if (path.find(root) == 0)
                return true;
        return false;
}

void __fastcall TForm1::SaveObject1Click(TObject *Sender)
{
        if (play) {
                ShowMessage("First stop game!");
                return;        
        }
        if(SaveObject->Execute() && currentObjID != "") {

        string s = SaveObject->FileName.c_str();
        for (unsigned int i = 0; i < s.length(); i++) {
                if (s[i] == '\\') {
                        s.insert(i, "\\");
                        i++;
                }
        }

        game_doscript("game.onObjectSelected(nil, false)"); // for return all selected colors to normal
        // generating script
        string script = "";
        string itersPart = "";
        script.append("\n\n");

        string objID = currentObjID;

        for (unsigned int i = 0; i < events.size(); i++) {
                Event* e = events.at(i);
                if (e->body.length() <= 0 || (!game_idused(e->obj_id.c_str()) && e->obj_id != "map" ) || !isHaveRoot(objID, events.at(i)->obj_id) ) continue;

                script.append(e->obj_id+"."+e->name+" = function"+e->params+"\n"+e->body+"\nend\n\n\n");

                if (e->name == "onIterate" && e->obj_id != "map" && isHaveRoot(objID, events.at(i)->obj_id))
                        itersPart.append("game.registerIterator(" + e->obj_id+", "+e->obj_id+"."+e->name+")\n");
        }

        script.append("\n");
        script.append(itersPart);

        FILE* ff = fopen("fuck2.txt", "w");
        fwrite(script.c_str(), script.length(), 1, ff);
        fclose(ff);

        game_save_object(objID.c_str(), s.c_str(), script.c_str());

	FILE* f = fopen(s.c_str(), "a+b");
        if (!f) {
                ShowMessage("Can't save map!");
                return;
        }

	// Write count of objects
	int count = 0;
	for (unsigned int i = 0; i < events.size(); i++) {
                if (events.at(i)->body.length() > 0   && !(!game_idused(events.at(i)->obj_id.c_str()) && events.at(i)->obj_id != "map" ) && isHaveRoot(objID, events.at(i)->obj_id)) count++;
        }
	fwrite(&count, sizeof count, 1, f);
	for (unsigned int i = 0; i < events.size(); i++) {
                Event* e = events.at(i);
                if (e->body.length() <= 0  || (!game_idused(e->obj_id.c_str()) && e->obj_id != "map" ) || !isHaveRoot(objID, events.at(i)->obj_id) ) continue;

                int type = EO_EVENT;
                int sizeOfString = 0;
	        fwrite(&type, sizeof type, 1, f);

		sizeOfString = e->obj_id.size(); // Write string without null char
		fwrite(&sizeOfString, sizeof sizeOfString, 1, f); // size
		fwrite(e->obj_id.c_str(), sizeOfString, 1, f); // body

		sizeOfString = e->name.size(); // Write string without null char
		fwrite(&sizeOfString, sizeof sizeOfString, 1, f); // size
		fwrite(e->name.c_str(), sizeOfString, 1, f); // body

		sizeOfString = e->params.size(); // Write string without null char
		fwrite(&sizeOfString, sizeof sizeOfString, 1, f); // size
		fwrite(e->params.c_str(), sizeOfString, 1, f); // body

		sizeOfString = e->body.size(); // Write string without null char
		fwrite(&sizeOfString, sizeof sizeOfString, 1, f); // size
		fwrite(e->body.c_str(), sizeOfString, 1, f); // body
	}
	fclose(f);
        }

}
//---------------------------------------------------------------------------

bool TForm1::loadObject(string id, string path) {
	FILE* f = fopen(path.c_str(), "rb");
        if (! f) {
                return false;
        }
	// Read offset
        fseek(f, sizeof(int), SEEK_CUR);
	int offset;
	fread(&offset, sizeof offset, 1, f);
        // go to editor section
        fseek(f, offset, SEEK_SET);
        // load...
	int count;
	fread(&count, sizeof count, 1, f);
	for (int i = 0; i < count; i++) {
                int type;
        	fread(&type, sizeof type, 1, f); // read Type
		switch (type) {
			case EO_EVENT: {
                                Event* e = new Event;
                                int sizeOfString; // Write string without null char
                                char* buffer;

			        fread(&sizeOfString, sizeof sizeOfString, 1, f); // size
			        buffer = new char[sizeOfString + 1];
			        fread(buffer, sizeOfString, 1, f); // body
			        buffer[sizeOfString] = 0;
			        e->obj_id = string(buffer);

			        fread(&sizeOfString, sizeof sizeOfString, 1, f); // size
			        buffer = new char[sizeOfString + 1];
			        fread(buffer, sizeOfString, 1, f); // body
			        buffer[sizeOfString] = 0;
			        e->name = string(buffer);

			        fread(&sizeOfString, sizeof sizeOfString, 1, f); // size
			        buffer = new char[sizeOfString + 1];
			        fread(buffer, sizeOfString, 1, f); // body
			        buffer[sizeOfString] = 0;
			        e->params = string(buffer);

			        fread(&sizeOfString, sizeof sizeOfString, 1, f); // size
			        buffer = new char[sizeOfString + 1];
			        fread(buffer, sizeOfString, 1, f); // body
			        buffer[sizeOfString] = 0;
			        e->body = string(buffer);

                                // change root object name
                                string new_id;
                                int first_point_pos = e->obj_id.find(".");
                                if (first_point_pos == -1) {
                                        new_id = id;
                                } else {
                                        new_id = id + e->obj_id.substr(first_point_pos, e->obj_id.size() - first_point_pos);
                                }
                                e->obj_id = new_id;

                                events.push_back(e);
			break;}
                }
        }
        fclose(f);
        return true;
}

void __fastcall TForm1::LoadObject1Click(TObject *Sender)
{
        if (play) {
                ShowMessage("First stop game!");
                return;        
        }
        if(OpenObject->Execute()) {

                string objectID;
                objectID = InputBox("Name", "Enter ID for root object", "").c_str();

                if (objectID != "") {
                        game_load_object(OpenObject->FileName.c_str(), objectID.c_str());

                        if (!loadObject(objectID , OpenObject->FileName.c_str())) {
                                ShowMessage("Can't open object!");
                                return;
                        }
                        game_doscript("refreshList()");

                }

        }

}
//---------------------------------------------------------------------------

