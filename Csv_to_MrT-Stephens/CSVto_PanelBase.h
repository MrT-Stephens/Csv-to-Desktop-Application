#pragma once

#include <wx/wx.h>
#include <wx/clipbrd.h>
#include <wx/listctrl.h>
#include <wx/stdpaths.h> 
#include <wx/filename.h>

#include <mutex>
#include <thread>
#include <format>
#include <iomanip>

#include "CSVData.h"
#include "MrT_wxDialogs.h"
#include "GlobalDefualts.h"

enum ListVeiwSortOrder
{
	ORDER_NONE = 0,
	ORDER_ASCENDING = 1,
	ORDER_DESCENDING = 2
};

class CSVto_PanelBase : public wxPanel
{
protected:
	// Main Items
	std::string m_FileDir;
	std::string m_PanelName;
	mrt::CSVData* m_CSVData = nullptr;

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
	wxBoxSizer* m_OutputSettingsSizer1, * m_OutputSettingsSizer2;
	wxButton* m_LowercaseBtn, * m_UppercaseBtn, * m_CapitalizeBtn, * m_ClearBtn, * m_TransposeBtn, * m_DeleteBlanksBtn;

	// Thread Items
	std::mutex m_OutputDataMutex;
	bool m_ListViewThreadRunning = false, m_TextBoxThreadRunning = false;

	// Data Output Section
	wxBoxSizer* m_DataOutputSizer, * m_DataOutputBtnSizer;
	wxTextCtrl* m_OutputDataTextBox;
	wxButton* m_DownloadBtn, * m_CopyDataBtn;
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

	void PopulateData();
	bool isThreadsRunning();
};