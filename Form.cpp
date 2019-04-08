//---------------------------------------------------------------------------

#include <vcl.h>
#include <sstream>
#pragma hdrstop

#include <vector>

#include <cstring>
#include "Form.h"
#include "sqlite3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma resource "*.dfm"
TForm1 *Form1;

std::string db = "History";
const char * dbName = db.c_str();

sqlite3* Database;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
   VirtualStringTree->NodeDataSize = sizeof(MyNodeStruct);
}
//---------------------------------------------------------------------------

// Текст ячейки
void __fastcall TForm1::VirtualStringTreeGetText(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
		  TVSTTextType TextType, UnicodeString &CellText)
{
	if (!Node) return;
	MyNodeStruct *nodeData = (MyNodeStruct *)Sender -> GetNodeData(Node);
	switch (Column)
	{
		case 0:
			CellText = nodeData -> Id;
			break;
		case 1:
			CellText = nodeData -> Guid;
			break;
		case 2:
			CellText = nodeData -> CurrentPath;
			break;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TableButtonClick(TObject *Sender)
{
	VirtualStringTree -> Clear();
	sqlite3_open(dbName, &Database);

	if (Database == NULL)
	{
		Label -> Caption = "Failed to open Database";
		return;
	}

	sqlite3_stmt *pStatement;

	Label -> Caption = "Performing query...";
	int result = sqlite3_prepare_v2(Database, "select * from downloads", -1, &pStatement, NULL);

	if (result != SQLITE_OK)
	{
		const char *errmsg = sqlite3_errmsg(Database);
		Label -> Caption = "Query execution error";
	}
	else
	{
		Label -> Caption = "Table formation...";
        std::string addedRowsNumber = FillTable(pStatement);
		std::string msg = "Formation complete. Added " + addedRowsNumber +  " rows.";
		Label -> Caption = msg.c_str();
	}

	sqlite3_finalize(pStatement);
	sqlite3_close(Database);
}
//---------------------------------------------------------------------------
std::string __fastcall TForm1::FillTable(sqlite3_stmt *pStatement)
{
	int columnsNumber = sqlite3_column_count(pStatement);
	long numberOfRows = 0;
	while (sqlite3_step(pStatement) != SQLITE_DONE) {
		int i;
		PVirtualNode entryNode = VirtualStringTree -> AddChild(VirtualStringTree -> RootNode);
		MyNodeStruct *nodeData = (MyNodeStruct*)VirtualStringTree -> GetNodeData(entryNode);
		for (i = 0; i < columnsNumber; i++)
		{
			FillColumns(nodeData, pStatement);
		}
		numberOfRows++;
	}

	std::string addedRowsNumber = ConvertNumberToString32(numberOfRows);

    return addedRowsNumber;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::FillColumns(MyNodeStruct *nodeData, sqlite3_stmt *pStatement)
{
	nodeData -> Id = sqlite3_column_int(pStatement, 0);
	nodeData -> Guid = (wchar_t*)sqlite3_column_text16(pStatement, 1);
	nodeData -> CurrentPath  = (wchar_t*)sqlite3_column_text16(pStatement, 2);
}

//---------------------------------------------------------------------------

void __fastcall TForm1::VirtualStringTreeFreeNode(TBaseVirtualTree *Sender, PVirtualNode Node)
{
	MyNodeStruct *nodeData = (MyNodeStruct *)Sender -> GetNodeData(Node);
	nodeData->~tagNodeStruct();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CloseButtonClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::DeleteButtonClick(TObject *Sender)
{
	int selectedNodeIndex = NULL;
	PVirtualNode selectedNode = VirtualStringTree -> FocusedNode;
	if(selectedNode == NULL)
	{
        Label -> Caption = "No records selected.";
		return;
	}
	selectedNodeIndex = selectedNode -> Index;

	int count = VirtualStringTree->SelectedCount;
	std::vector<PVirtualNode> selectedNodes;
	std::vector<int> ids;
	int sum = 0;

	for (int i = 0; i < count; i++) {
		if (i==0)
		{
			selectedNodes.push_back(VirtualStringTree -> GetFirstSelected());
		}
		else
		{
			selectedNodes.push_back(VirtualStringTree -> GetNextSelected(selectedNodes[i-1]));
		}

		MyNodeStruct *nodeData = (MyNodeStruct *)VirtualStringTree -> GetNodeData(selectedNodes[i]);
		ids.push_back(nodeData->Id);
	}

    DeleteSelectedRecords(selectedNodes, ids, count);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DeleteSelectedRecords(std::vector<PVirtualNode> selectedNodes, std::vector<int> ids, int count)
{
	if (count == 0)
	{
		Label -> Caption = "No records selected.";
		return;
	}

	PVirtualNode selectedNext = VirtualStringTree -> GetNext(selectedNodes[count-1]);
	VirtualStringTree -> AddToSelection(selectedNext);
	sqlite3_open(dbName ,&Database);

	for (int i = 0; i < count; i++) {
		DeleteRecords(i, selectedNodes[i], ids[i]);
	}

	sqlite3_close(Database);

	std::string deletedRowsNumber = ConvertNumberToString16(count);
	std::string msg = deletedRowsNumber + " records was deleted.";
	Label -> Caption = msg.c_str();
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TForm1::DeleteRecords(int index, PVirtualNode selectedNode, int recordId)
{
	std::string id = ConvertNumberToString16(recordId);
	std::string literalQuery = "delete from downloads where id=" + id;

	char query[literalQuery.size()+1];
	strcpy(query, literalQuery.c_str());

	sqlite3_stmt *delStmt;

	int result = sqlite3_prepare_v2(Database, query, -1, &delStmt, NULL);

	result = sqlite3_step(delStmt);
	if(result != SQLITE_DONE)
	{
		const char * errorMsg = sqlite3_errmsg(Database);
		Label -> Caption = errorMsg;
	}
	else
	{
		VirtualStringTree -> DeleteNode(selectedNode);
	}
	sqlite3_finalize(delStmt);
}

//---------------------------------------------------------------------------
void __fastcall TForm1::DeleteAllRecordsButtonClick(TObject *Sender)
{
	VirtualStringTree->BeginUpdate();
	sqlite3_open(dbName ,&Database);

	char *errorMsg;
	int result = sqlite3_exec(Database, "delete from downloads", NULL, NULL, &errorMsg);
	if(result != SQLITE_OK)
	{
		Label -> Caption = errorMsg;
		sqlite3_close(Database);
		return;
	}

	VirtualStringTree->Clear();
	VirtualStringTree->EndUpdate();

	Label -> Caption = "The table has been cleared";
	sqlite3_close(Database);
}
//---------------------------------------------------------------------------
std::string __fastcall TForm1::ConvertNumberToString16(int number)
{
	std::string num;
	std::stringstream strstream;
	strstream << number;
	strstream >> num;

	return num;
}
//---------------------------------------------------------------------------
std::string __fastcall TForm1::ConvertNumberToString32(long number)
{
    std::string num;
	std::stringstream strstream;
	strstream << number;
	strstream >> num;

	return num;
}
//---------------------------------------------------------------------------
