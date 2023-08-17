#include "CSVtoJSON_Panel.h"

  /**********************************/
 /* CSVtoJSON_Panel Implementation */
/**********************************/

CSVtoJSON_Panel::CSVtoJSON_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours)
	: CSVto_PanelBase(_parent, _name, _colours)
{
	SetupDataInputSection();
	SetupOutputSettingsSection();
	SetupSpecificOutputSectionItems();
	SetupDataOutputSection();

	SetSizer(m_MainSizer);
}

void CSVtoJSON_Panel::SetupSpecificOutputSectionItems()
{
	m_OutputSettingsSizer3 = new wxBoxSizer(wxHORIZONTAL);

	m_MinifyJSONCheckBox = new wxCheckBox(this, wxID_ANY, "Minify JSON", wxDefaultPosition, wxDefaultSize);
	m_MinifyJSONCheckBox->SetMinSize(FromDIP(wxSize(120, 22)));
	m_MinifyJSONCheckBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_MinifyJSONCheckBox->SetOwnBackgroundColour(m_Colours->BACKGROUND);
	m_MinifyJSONCheckBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_MinifyJSONCheckBox->SetToolTip("Writes all the JSON statements onto one line. Note: not applicable to JSONLines.");

	m_MinifyJSONCheckBox->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				PopulateData();
			}
		}
	);

	m_ParsingJSONCheckBox = new wxCheckBox(this, wxID_ANY, "Parsing JSON", wxDefaultPosition, wxDefaultSize);
	m_ParsingJSONCheckBox->SetMinSize(FromDIP(wxSize(120, 22)));
	m_ParsingJSONCheckBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_ParsingJSONCheckBox->SetOwnBackgroundColour(m_Colours->BACKGROUND);
	m_ParsingJSONCheckBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_ParsingJSONCheckBox->SetToolTip("Turns the JSON into a parsing style. Note: may loose some digits or number.");

	m_ParsingJSONCheckBox->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				PopulateData();
			}
		}
	);

	m_LeftSizer = new wxBoxSizer(wxVERTICAL);
	m_LeftSizer->Add(m_MinifyJSONCheckBox, 0, wxRIGHT | wxLEFT | wxTOP | wxEXPAND | wxCENTER, FromDIP(10));
	m_LeftSizer->Add(m_ParsingJSONCheckBox, 0, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_WrapperByDataBtn = new wxButton(this, wxID_ANY, "Include wrapper by 'data'", wxDefaultPosition, wxDefaultSize);
	m_WrapperByDataBtn->SetMinSize(FromDIP(wxSize(120, 22)));
	m_WrapperByDataBtn->SetOwnFont(MAIN_FONT_TEXT(10));
	m_WrapperByDataBtn->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_WrapperByDataBtn->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_WrapperByDataBtn->SetToolTip("Include or exclude wrapper by 'data'. Note: not applicable to JSONLines.");

	m_WrapperByDataBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			if (m_WrapperByDataBtn->GetLabel() == "Include wrapper by 'data'")
			{
				m_WrapperByDataBtn->SetLabel("Exclude wrapper by 'data'");
			}
			else
			{
				m_WrapperByDataBtn->SetLabel("Include wrapper by 'data'");
			}

			if (m_CSVData != nullptr)
			{
				PopulateData();
			}
		}
	);

	m_JSONStylesChoices.Add("JSON (Array of objects)");
	m_JSONStylesChoices.Add("JSON (2D arrays)");
	m_JSONStylesChoices.Add("JSON (Column arrays)");
	m_JSONStylesChoices.Add("JSON (Keyed arrays)");
	m_JSONStylesChoices.Add("JSONLines (Objects)");
	m_JSONStylesChoices.Add("JSONLines (Arrays)");

	m_JSONStylesComboBox = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, m_JSONStylesChoices, wxCB_READONLY);
	m_JSONStylesComboBox->SetSelection(0);
	m_JSONStylesComboBox->SetMinSize(FromDIP(wxSize(120, 22)));
	m_JSONStylesComboBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_JSONStylesComboBox->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_JSONStylesComboBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_JSONStylesComboBox->SetToolTip("Select a JSON or JSONLines statements style.");

	m_JSONStylesComboBox->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				PopulateData();
			}
		}
	);

	m_RightSizer = new wxBoxSizer(wxVERTICAL);
	m_RightSizer->Add(m_WrapperByDataBtn, 0, wxRIGHT | wxLEFT | wxTOP | wxEXPAND | wxCENTER, FromDIP(10));
	m_RightSizer->Add(m_JSONStylesComboBox, 0, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_OutputSettingsSizer3->Add(m_LeftSizer, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(0));
	m_OutputSettingsSizer3->Add(m_RightSizer, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(0));

	m_MainSizer->Add(m_OutputSettingsSizer3, 0, wxEXPAND | wxALL, FromDIP(0));
}

void CSVtoJSON_Panel::PopulateOutputDataTextBox()
{
#if defined(MRT_DEBUG)
	auto start = std::chrono::high_resolution_clock::now();
#endif

	{
		mrt::JSON_Generator<std::wstring> jsonGenerator(m_CSVData);

		mrt::JSON_Generator<std::wstring>::OStrStream ss;

		jsonGenerator.GenerateJSON(&ss, m_JSONStylesComboBox->GetSelection(), m_ParsingJSONCheckBox->GetValue(), 
			((m_WrapperByDataBtn->GetLabel() == "Include wrapper by 'data'") ? false : true), !m_MinifyJSONCheckBox->GetValue());

		m_OutputDataTextBox->SetValue(ss.str());
	}

#if defined(MRT_DEBUG)
	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	MRT_DEBUG_LOG_TIME("Time taken by 'PopulateOutputDataTextBox' in JSON class", duration.count(), "milliseconds");
#endif

	std::lock_guard<std::mutex> lock(m_OutputDataMutex);

	m_TextBoxThreadRunning = false;

	if (!m_ListViewThreadRunning)
	{
		LockOrUnlockItems(false);
	}
}

void CSVtoJSON_Panel::OutputFile()
{
	wxFileDialog saveFileDialog(this, "Download File", (wxStandardPaths::Get().GetDocumentsDir()), "", "JavaScript Object Notation (*.json)|*.json", wxFD_SAVE);

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

void CSVtoJSON_Panel::LockOrUnlockItems(bool lock)
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
	m_MinifyJSONCheckBox->Enable(!lock);
	m_ParsingJSONCheckBox->Enable(!lock);
	m_WrapperByDataBtn->Enable(!lock);
	m_JSONStylesComboBox->Enable(!lock);
}