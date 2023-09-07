#include "CSVtoXLSX_Panel.h"

  /**********************************/
 /* CSVtoXLSX_Panel Implementation */
/**********************************/

CSVtoXLSX_Panel::CSVtoXLSX_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours)
	: CSVto_PanelBase(_parent, _name, _colours)
{
	SetupDataInputSection();
	SetupOutputSettingsSection();
	SetupSpecificOutputSectionItems();
	SetupDataOutputSection();

	SetSizer(m_MainSizer);
}

void CSVtoXLSX_Panel::SetupSpecificOutputSectionItems()
{
	m_OutputSettingsSizer3 = new wxBoxSizer(wxHORIZONTAL);

	m_IncludeHeaderCheckBox = new wxCheckBox(this, wxID_ANY, "Exclude Header", wxDefaultPosition, wxDefaultSize);
	m_IncludeHeaderCheckBox->SetMinSize(FromDIP(wxSize(120, 22)));
	m_IncludeHeaderCheckBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_IncludeHeaderCheckBox->SetOwnBackgroundColour(m_Colours->BACKGROUND);
	m_IncludeHeaderCheckBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_IncludeHeaderCheckBox->SetToolTip("Exclude header in the generated text");

	m_IncludeHeaderCheckBox->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				PopulateData();
			}
		}
	);

	m_SheetNameTextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
	m_SheetNameTextCtrl->SetHint("Worksheet Name");
	m_SheetNameTextCtrl->SetMinSize(FromDIP(wxSize(120, 22)));
	m_SheetNameTextCtrl->SetOwnFont(MAIN_FONT_TEXT(11));
	m_SheetNameTextCtrl->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_SheetNameTextCtrl->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_SheetNameTextCtrl->SetToolTip("Set a name for the worksheet");

	m_SheetNameTextCtrl->Bind(wxEVT_TEXT, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				m_DataSpacingTimer.Start(1200);
			}
		}
	);

	m_DataSpacingTimer.Bind(wxEVT_TIMER, [this](wxTimerEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				m_DataSpacingTimer.Stop();
				PopulateData();
			}
		}
	);

	m_OutputSettingsSizer3->Add(m_IncludeHeaderCheckBox, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(10));
	m_OutputSettingsSizer3->Add(m_SheetNameTextCtrl, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_MainSizer->Add(m_OutputSettingsSizer3, 0, wxEXPAND | wxALL, FromDIP(0));
}

void CSVtoXLSX_Panel::PopulateOutputDataTextBox()
{
#if defined(MRT_DEBUG)
	auto start = std::chrono::high_resolution_clock::now();
#endif

	{
		
	}

#if defined(MRT_DEBUG)
	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	MRT_DEBUG_LOG_TIME("Time taken by 'PopulateOutputDataTextBox' in XLSX class", duration.count(), "milliseconds");
#endif

	std::lock_guard<std::mutex> lock(m_OutputDataMutex);

	m_TextBoxThreadRunning = false;

	if (!m_ListViewThreadRunning)
	{
		LockOrUnlockItems(false);
	}
}

void CSVtoXLSX_Panel::OutputFile()
{
	wxFileDialog saveFileDialog(this, "Download File", (wxStandardPaths::Get().GetDocumentsDir()), "", "XLSX (*.xlsx)|*.xlsx", wxFD_SAVE);

	if (saveFileDialog.ShowModal() == wxID_OK)
	{
		OFStream file(saveFileDialog.GetPath().ToStdWstring());

		if (!file.is_open())
		{
			mrt::MrT_UniDialog errorDialog(this, "Error", "Failed to save file!\nPlease try to re-save the file.",
				m_Colours, wxICON(wxICON_ERROR), mrt::MrT_UniDialogType_OK, FromDIP(wxSize(400, 200)));

			errorDialog.ShowModal();
		}
		else
		{

		}
		file.close();
	}
}

void CSVtoXLSX_Panel::LockOrUnlockItems(bool lock)
{
	m_EditDataBtn->Enable(!lock);
	m_UndoBtn->Enable(!lock);
	m_RedoBtn->Enable(!lock);
	m_LoadData->Enable(!lock);
	m_ExampleData->Enable(!lock);
	m_LowercaseBtn->Enable(!lock);
	m_UppercaseBtn->Enable(!lock);
	m_CapitalizeBtn->Enable(!lock);
	m_ClearBtn->Enable(!lock);
	m_TransposeBtn->Enable(!lock);
	m_DeleteBlanksBtn->Enable(!lock);
	m_IncludeHeaderCheckBox->Enable(!lock);
	m_SheetNameTextCtrl->Enable(!lock);
}