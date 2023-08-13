#pragma once

// wxWidgets includes
#include <wx/wx.h>
#include <wx/clipbrd.h>
#include <wx/listctrl.h>
#include <wx/stdpaths.h> 
#include <wx/filename.h>

// std includes
#include <mutex>
#include <thread>
#include <format>
#include <iomanip>

// MrT includes
#include "CSVData.h"
#include "MrT_wxDialogs.h"
#include "UtilityItems.h"

enum ListVeiwSortOrder
{
	ORDER_NONE = 0,
	ORDER_ASCENDING = 1,
	ORDER_DESCENDING = 2
};

  /*******************************/
 /* CSVto_PanelBase Declaration */
/*******************************/

class CSVto_PanelBase : public wxPanel
{
protected: 
	// Main Items
	std::string m_FileDir;
	std::string m_PanelName;
	mrt::CSVData<std::string>* m_CSVData = nullptr;

	wxBoxSizer* m_MainSizer;
	const mrtApp::AppColours* m_Colours;

	// Data Input Section
	int m_CurrentSortColumn = -1;
	ListVeiwSortOrder m_CurrentSortOrder = ORDER_NONE;
	wxBoxSizer* m_DataInBtnSizer, * m_DataInTextSizer, * m_DataListViewSizer;
	wxBitmapButton* m_LogoButton;
	wxButton* m_LoadData, * m_ExampleData;
	wxListView* m_DataInputListView;

	// Output Settings Section
	wxBoxSizer* m_OutputSettingsSizer1, * m_OutputSettingsSizer2, * m_OutputHeadingSizer;
	wxButton* m_LowercaseBtn, * m_UppercaseBtn, * m_CapitalizeBtn, * m_TransposeBtn, 
		* m_DeleteBlanksBtn, * m_UndoBtn, * m_RedoBtn, * m_BasicAdvViewBtn, * m_IncludeHeaderBtn, * m_EditDataBtn;

	// Thread Items
	std::mutex m_OutputDataMutex;
	bool m_ListViewThreadRunning = false, m_TextBoxThreadRunning = false;

	// Data Output Section
	wxBoxSizer* m_DataOutputSizer, * m_DataOutputBtnSizer;
	wxTextCtrl* m_OutputDataTextBox;
	wxButton* m_DownloadBtn, * m_CopyDataBtn, * m_ClearBtn;
public:
	CSVto_PanelBase(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours);

	virtual ~CSVto_PanelBase();

	virtual void SetupDataInputSection();
	virtual void SetupOutputSettingsSection();
	virtual void SetupDataOutputSection();
	virtual void SetupSpecificOutputSectionItems() = 0;
	virtual void PopulateDataListView();
	virtual void PopulateOutputDataTextBox();
	virtual void LockOrUnlockItems(bool lock);
	virtual void HideOrShowBasicAdvItems(bool show);
	virtual void OutputFile();

	void PopulateData();
	bool isThreadsRunning();
};