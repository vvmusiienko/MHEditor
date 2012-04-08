//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
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
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <ImgList.hpp>

#include <gl/gl.h>
#include <gl/glu.h>

#include <math.h>
#include <vector>

#include <vector>

extern "C" {
# include "lua\lua.h"
# include "lua\lauxlib.h"
# include "lua\lualib.h"
}

#include "Other.h"

using namespace std;

#define OP_VECTOR       0
#define OP_INTEGER      1
#define OP_FLOAT        2
#define OP_STRING       3
#define OP_COLOR        4
#define OP_BOOL         5
#define OP_EVENT        6
#define OP_LIST         7
#define OP_LABEL        8




//---------------------------------------------------------------------------
class NodeStack {
private:
        vector<TTreeNode*> items;
public:
        void pushNode(TTreeNode* node);
        TTreeNode* popNode();
        NodeStack();
        ~NodeStack();
};


class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TPanel *OpenGLPanel;
        TMainMenu *MainMenu1;
        TMenuItem *File1;
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TTabSheet *TabSheet2;
        TStatusBar *StatusBar1;
        TScrollBox *ScrollBox1;
        TListBox *ListBox2;
        TLabel *Label1;
        TMenuItem *LoadMap1;
        TMenuItem *SaveMap1;
        TMenuItem *SaveMapWithoutEditorMetadata1;
        TMenuItem *Edit2;
        TMenuItem *Copy1;
        TMenuItem *Paste1;
        TMenuItem *Templates1;
        TMenuItem *LoadTemplate1;
        TMenuItem *SaveTemplate1;
        TOpenDialog *openMapDialog;
        TSaveDialog *saveMapDialog;
        TTreeView *TreeView1;
        TImageList *ObjectImages;
        TMenuItem *NewMap1;
        TPanel *Panel1;
        TSpeedButton *SpeedButton1;
        TSpeedButton *SpeedButton2;
        TSpeedButton *SpeedButton6;
        TSpeedButton *SpeedButton7;
        TSpeedButton *SpeedButton8;
        TBevel *Bevel2;
        TBevel *Bevel1;
        TSpeedButton *SpeedButton4;
        TBevel *Bevel3;
        TSpeedButton *SpeedButton3;
        TEdit *Edit1;
        TComboBox *ComboBox1;
        TBevel *Bevel4;
        TBevel *Bevel5;
        TSpeedButton *SpeedButton9;
        TCheckBox *inpToEngine;
        TCheckBox *physDraw;
        TSpeedButton *SpeedButton5;
        TPageControl *PageControl2;
        TTabSheet *TabSheet3;
        TTabSheet *TabSheet4;
        TTreeView *TreeView2;
        TSpeedButton *SpeedButton11;
        TSpeedButton *SpeedButton12;
        TSpeedButton *SpeedButton13;
        TSpeedButton *SpeedButton14;
        TSpeedButton *SpeedButton15;
        TSpeedButton *SpeedButton16;
        TSpeedButton *SpeedButton17;
        TListBox *ListBox1;
        TLabel *Label2;
        TLabel *Label3;
        TListBox *ListBox3;
        TSpeedButton *SpeedButton10;
        TSpeedButton *SpeedButton18;
        TSpeedButton *SpeedButton19;
        TSpeedButton *SpeedButton20;
        TEdit *Edit3;
        TSpeedButton *SpeedButton21;
        TSpeedButton *SpeedButton22;
        TMenuItem *N1;
        TMenuItem *SaveObject1;
        TMenuItem *LoadObject1;
        TOpenDialog *OpenObject;
        TSaveDialog *SaveObject;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall OpenGLPanelResize(TObject *Sender);
        void __fastcall OpenGLPanelMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall OpenGLPanelMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall OpenGLPanelMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall FormKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall FormKeyPress(TObject *Sender, char &Key);
        void __fastcall inpToEngineClick(TObject *Sender);
        void __fastcall SpeedButton1Click(TObject *Sender);
        void __fastcall SpeedButton2Click(TObject *Sender);
        void __fastcall SpeedButton3Click(TObject *Sender);
        void __fastcall SpeedButton4Click(TObject *Sender);
        void __fastcall ListBox2Click(TObject *Sender);
        void __fastcall SaveMap1Click(TObject *Sender);
        void __fastcall LoadMap1Click(TObject *Sender);
        void __fastcall SpeedButton5Click(TObject *Sender);
        void __fastcall SpeedButton6Click(TObject *Sender);
        void __fastcall SpeedButton8Click(TObject *Sender);
        void __fastcall SpeedButton7Click(TObject *Sender);
        void __fastcall SpeedButton9Click(TObject *Sender);
        void __fastcall TreeView1Click(TObject *Sender);
        void __fastcall NewMap1Click(TObject *Sender);
        void __fastcall TreeView1DragOver(TObject *Sender, TObject *Source,
          int X, int Y, TDragState State, bool &Accept);
        void __fastcall TreeView1DragDrop(TObject *Sender, TObject *Source,
          int X, int Y);
        void __fastcall physDrawClick(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall TreeView2Click(TObject *Sender);
        void __fastcall SpeedButton11Click(TObject *Sender);
        void __fastcall SpeedButton12Click(TObject *Sender);
        void __fastcall SpeedButton13Click(TObject *Sender);
        void __fastcall SpeedButton14Click(TObject *Sender);
        void __fastcall SpeedButton15Click(TObject *Sender);
        void __fastcall SpeedButton16Click(TObject *Sender);
        void __fastcall SpeedButton17Click(TObject *Sender);
        void __fastcall ListBox1Click(TObject *Sender);
        void __fastcall ListBox3Click(TObject *Sender);
        void __fastcall SpeedButton20Click(TObject *Sender);
        void __fastcall SpeedButton10Click(TObject *Sender);
        void __fastcall SpeedButton18Click(TObject *Sender);
        void __fastcall SpeedButton19Click(TObject *Sender);
        void __fastcall SpeedButton21Click(TObject *Sender);
        void __fastcall SpeedButton22Click(TObject *Sender);
        void __fastcall SaveObject1Click(TObject *Sender);
        void __fastcall LoadObject1Click(TObject *Sender);
private:	// User declarations
    HDC hdc;
    HGLRC hrc;
    int PixelFormat;
    bool inputToEngine;
    vector<Property*> properties;
    vector<Event*> events;
    string path;
    bool play;
    string lastSavePath;
    string currentObjectID;

public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
    void __fastcall IdleLoop(TObject*, bool&); 
    void __fastcall RenderGLScene(); 
    void __fastcall SetPixelFormatDescriptor();
    void saveMap(string path);
    bool loadMap(string path);
    bool loadObject(string id, string path);
    bool editorMask[1000][1000];
    void applyScript();

    static int l_setCurrentObjectID(lua_State* luaVM);
    static int l_addProperty(lua_State* luaVM);
    static int l_clearProperties(lua_State* luaVM);
    static int l_afterMapSaved(lua_State* luaVM);
    static int l_showMessage(lua_State* luaVM);
    static int l_setMapMaskSizeAndOffset(lua_State* luaVM);
    static int l_setMapMask(lua_State* luaVM);
    // Tree View
    static int l_addNode(lua_State* luaVM);
    static int l_pushNode(lua_State* luaVM);
    static int l_popNode(lua_State* luaVM);
    static int l_clearTree(lua_State* luaVM);
    static int l_restoreLastSelection(lua_State* luaVM);
    static int l_selectTreeNode(lua_State* luaVM);
    static int l_setAccess(lua_State* luaVM); // set flag to true if dragdrop allowed for specified objects

    TTreeNode* currentNode;
    TTreeNode* parentNode;
    NodeStack nodeStack;

    int lastSelectedNodeIndex;
    int lastSelectionCount;
    bool dropAllowed;

    // Collision Shape Tree View
    static int l_cs_addNode(lua_State* luaVM);
    static int l_cs_pushNode(lua_State* luaVM);
    static int l_cs_popNode(lua_State* luaVM);
    static int l_cs_clearTree(lua_State* luaVM);
    TTreeNode* cs_currentNode;
    TTreeNode* cs_parentNode;
    NodeStack* cs_nodeStack;

    void addCS(int type);

    // Constraints
    static int l_addConstraint(lua_State* luaVM);
    static int l_clearConstraints(lua_State* luaVM);
    void addCN(int type);

};


//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
 