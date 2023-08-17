#include "CSVtoCSV_Panel.h"

  /*********************************/
 /* CSVtoCSV_Panel Implementation */
/*********************************/

CSVtoCSV_Panel::CSVtoCSV_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours)
	: CSVto_PanelBase(_parent, _name, _colours)
{
	SetupDataInputSection();
	SetupOutputSettingsSection();
	SetupSpecificOutputSectionItems();
	SetupDataOutputSection();

	SetSizer(m_MainSizer);
}

void CSVtoCSV_Panel::SetupSpecificOutputSectionItems()
{
	m_OutputSettingsSizer3 = new wxBoxSizer(wxHORIZONTAL);

	m_IncludeHeaderButton = new wxButton(this, wxID_ANY, "Exclude Header", wxDefaultPosition, wxDefaultSize);
	m_IncludeHeaderButton->SetMinSize(FromDIP(wxSize(120, 22)));
	m_IncludeHeaderButton->SetOwnFont(MAIN_FONT_TEXT(10));
	m_IncludeHeaderButton->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_IncludeHeaderButton->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_IncludeHeaderButton->SetToolTip("Include or exclude double quotes in data");

	m_IncludeHeaderButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			if (m_IncludeHeaderButton->GetLabel() == "Exclude Header")
			{
				m_IncludeHeaderButton->SetLabel("Include Header");
			}
			else
			{
				m_IncludeHeaderButton->SetLabel("Exclude Header");
			}

			if (m_CSVData != nullptr)
			{
				PopulateData();
			}
		}
	);

	m_QuotesButton = new wxButton(this, wxID_ANY, "Include Quotes", wxDefaultPosition, wxDefaultSize);
	m_QuotesButton->SetMinSize(FromDIP(wxSize(120, 22)));
	m_QuotesButton->SetOwnFont(MAIN_FONT_TEXT(10));
	m_QuotesButton->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_QuotesButton->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_QuotesButton->SetToolTip("Include or exclude double quotes in data");

	m_QuotesButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			if (m_QuotesButton->GetLabel() == "Include Quotes")
			{
				m_QuotesButton->SetLabel("Exclude Quotes");
			}
			else
			{
				m_QuotesButton->SetLabel("Include Quotes");
			}

			if (m_CSVData != nullptr)
			{
				PopulateData();
			}
		}
	);

	m_DelimiterChoices.Add("Comma (,)");
	m_DelimiterChoices.Add("Semicolon (;)");
	m_DelimiterChoices.Add("Colon (:)");
	m_DelimiterChoices.Add("Tab (\\t)");
	m_DelimiterChoices.Add("Pipe (|)");
	m_DelimiterChoices.Add("Slash (/)");
	m_DelimiterChoices.Add("Hash (#)");

	m_DelimiterComboBox = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, m_DelimiterChoices, wxCB_READONLY);
	m_DelimiterComboBox->SetSelection(0);
	m_DelimiterComboBox->SetMinSize(FromDIP(wxSize(120, 22)));
	m_DelimiterComboBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_DelimiterComboBox->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_DelimiterComboBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_DelimiterComboBox->SetToolTip("Select a delimiter type for the generated csv data");

	m_DelimiterComboBox->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				PopulateData();
			}
		}
	);

	m_OutputSettingsSizer3->Add(m_IncludeHeaderButton, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(10));
	m_OutputSettingsSizer3->Add(m_QuotesButton, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(10));
	m_OutputSettingsSizer3->Add(m_DelimiterComboBox, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_MainSizer->Add(m_OutputSettingsSizer3, 0, wxEXPAND | wxALL, FromDIP(0));
}

void CSVtoCSV_Panel::OutputFile()
{
	wxFileDialog saveFileDialog(this, "Download File", (wxStandardPaths::Get().GetDocumentsDir()), "", "Comma-Separated Values (*.csv)|*.csv", wxFD_SAVE);

	if (saveFileDialog.ShowModal() == wxID_OK)
	{
		std::ofstream file(saveFileDialog.GetPath().ToStdString());

		if (!file.is_open())
		{
			mrt::MrT_UniDialog errorDialog(this, "Error", "Failed to save file!\nPlease try to re-save the file.",
				m_Colours, wxICON(wxICON_ERROR), mrt::MrT_UniDialogType_OK, FromDIP(wxSize(400, 200)));

			errorDialog.ShowModal();
		}
		else
		{
			file << std::move(m_OutputDataTextBox->GetValue().ToStdString());
		}
		file.close();
	}
}

void CSVtoCSV_Panel::PopulateOutputDataTextBox()
{
#if defined(MRT_DEBUG)
	auto start = std::chrono::high_resolution_clock::now();
#endif

	{
		mrt::CSVData<std::wstring>::OStrStream ss;

		mrt::CSVData<std::wstring>::SaveCsvToStream(m_CSVData, &ss, GetDelimiterType(m_DelimiterComboBox->GetSelection()), 
			(m_IncludeHeaderButton->GetLabel() == "Exclude Header" ? true : false), (m_QuotesButton->GetLabel() == "Include Quotes" ? false : true));

		m_OutputDataTextBox->SetValue(ss.str());
	}

#if defined(MRT_DEBUG)
	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	MRT_DEBUG_LOG_TIME("Time taken by 'PopulateOutputDataTextBox' in CSV class", duration.count(), "milliseconds");
#endif

	std::lock_guard<std::mutex> lock(m_OutputDataMutex);

	m_TextBoxThreadRunning = false;

	if (!m_ListViewThreadRunning)
	{
		LockOrUnlockItems(false);
	}
}

void CSVtoCSV_Panel::LockOrUnlockItems(bool lock)
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
	m_DelimiterComboBox->Enable(!lock);
	m_IncludeHeaderBtn->Enable(!lock);
	m_QuotesButton->Enable(!lock);
}

wchar_t CSVtoCSV_Panel::GetDelimiterType(int selection)
{
	switch (selection)
	{
	case CSVData_COMMA: 
		return ',';
	case CSVData_SEMICOLON: 
		return ';';
	case CSVData_COLON:
		return ':';
	case CSVData_TAB:
		return '\t';
	case CSVData_PIPE:
		return '|';
	case CSVData_SLASH:
		return '/';
	case CSVData_HASH:
		return '#';
	default:
		return ',';
	}
}