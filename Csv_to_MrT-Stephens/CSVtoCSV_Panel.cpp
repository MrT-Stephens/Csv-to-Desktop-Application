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

	m_IncludeHeaderCheckBox = new wxCheckBox(this, wxID_ANY, "Exclude header", wxDefaultPosition, wxDefaultSize);
	m_IncludeHeaderCheckBox->SetMinSize(FromDIP(wxSize(120, 30)));
	m_IncludeHeaderCheckBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_IncludeHeaderCheckBox->SetOwnBackgroundColour(m_Colours->BACKGROUND);
	m_IncludeHeaderCheckBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_IncludeHeaderCheckBox->SetToolTip("Excludes the header in generated csv data");

	m_IncludeHeaderCheckBox->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& event)
		{
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
	m_DelimiterComboBox->SetMinSize(FromDIP(wxSize(120, 30)));
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

	m_OutputSettingsSizer3->Add(m_IncludeHeaderCheckBox, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(10));
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
		mrt::CSVData<std::string>::OStrStream ss;

		mrt::CSVData<std::string>::SaveCsvToStream(m_CSVData, &ss, GetDelimiterType(m_DelimiterComboBox->GetSelection()), !m_IncludeHeaderCheckBox->GetValue());

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
	m_IncludeHeaderCheckBox->Enable(!lock);
}

char CSVtoCSV_Panel::GetDelimiterType(int selection)
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