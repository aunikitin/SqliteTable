//---------------------------------------------------------------------------

#ifndef FormH
#define FormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <ustring.h>
#include "sqlite3.h"
#include "VirtualTrees.hpp"
//---------------------------------------------------------------------------
typedef struct tagNodeStruct {
	int Id;
	UnicodeString Guid;
	UnicodeString CurrentPath;
}MyNodeStruct;
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TVirtualStringTree *VirtualStringTree;
	TButton *TableButton;
	TLabel *Label;
	TButton *CloseButton;
	TButton *DeleteButton;
	TButton *DeleteAllRecordsButton;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall VirtualStringTreeGetText(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
		  TVSTTextType TextType, UnicodeString &CellText);
	void __fastcall TableButtonClick(TObject *Sender);
	void __fastcall VirtualStringTreeFreeNode(TBaseVirtualTree *Sender, PVirtualNode Node);
	void __fastcall CloseButtonClick(TObject *Sender);
	void __fastcall DeleteButtonClick(TObject *Sender);
	void __fastcall DeleteAllRecordsButtonClick(TObject *Sender);
private:	// User declarations
	void __fastcall FillColumns(MyNodeStruct *nodeData, sqlite3_stmt *pStatement);
	std::string __fastcall FillTable(sqlite3_stmt *pStatement);
	std::string __fastcall ConvertNumberToString16(int number);
	std::string __fastcall ConvertNumberToString32(long number);
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
	void __fastcall DeleteRecords(int index, PVirtualNode selectedNode, int recordId);
	void __fastcall DeleteSelectedRecords(std::vector<PVirtualNode> selectedNodes, std::vector<int> ids, int count);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
