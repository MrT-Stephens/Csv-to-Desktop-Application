#include "CSVtoYAML_Panel.h"

  /**********************************/
 /* CSVtoYAML_Panel Implementation */
/**********************************/

CSVtoYAML_Panel::CSVtoYAML_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours)
	: CSVto_PanelBase(_parent, _name, _colours)
{
	SetupDataInputSection();
	SetupOutputSettingsSection();
	SetupSpecificOutputSectionItems();
	SetupDataOutputSection();

	SetSizer(m_MainSizer);
}

void CSVtoYAML_Panel::SetupSpecificOutputSectionItems()
{
	m_OutputSettingsSizer3 = new wxBoxSizer(wxHORIZONTAL);

	m_ExplictTypeCheckBox = new wxCheckBox(this, wxID_ANY, "Explict Type Statements", wxDefaultPosition, wxDefaultSize);
	m_ExplictTypeCheckBox->SetMinSize(FromDIP(wxSize(120, 22)));
	m_ExplictTypeCheckBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_ExplictTypeCheckBox->SetOwnBackgroundColour(m_Colours->BACKGROUND);
	m_ExplictTypeCheckBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_ExplictTypeCheckBox->SetToolTip("Adds an explict type statement before the data");

	m_ExplictTypeCheckBox->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				PopulateData();
			}
		}
	);

	m_QuotesTypes.Add("No Quotes");
	m_QuotesTypes.Add("Double Quotes");
	m_QuotesTypes.Add("Single Quotes");

	m_QuotesTypeComboBox = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, m_QuotesTypes, wxCB_READONLY);
	m_QuotesTypeComboBox->SetSelection(0);
	m_QuotesTypeComboBox->SetMinSize(FromDIP(wxSize(120, 22)));
	m_QuotesTypeComboBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_QuotesTypeComboBox->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_QuotesTypeComboBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_QuotesTypeComboBox->SetToolTip("Select quote type for the strings in the generated data");

	m_QuotesTypeComboBox->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				PopulateData();
			}
		}
	);

	m_OutputSettingsSizer3->Add(m_ExplictTypeCheckBox, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(10));
	m_OutputSettingsSizer3->Add(m_QuotesTypeComboBox, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_MainSizer->Add(m_OutputSettingsSizer3, 0, wxEXPAND | wxALL, FromDIP(0));
}

void CSVtoYAML_Panel::OutputFile()
{
	StrType fileDir = GetOutputFileDirectory("YAML (*.yaml)|*.yaml");

	if (fileDir != StrType())
	{
		OFStream file(fileDir);

		if (!file.is_open())
		{
			mrt::MrT_UniDialog errorDialog(this, "Error", "Failed to save file!\nPlease try to re-save the file.",
				m_Colours, wxICON(wxICON_ERROR), mrt::MrT_UniDialogType_OK, FromDIP(wxSize(400, 200)));

			errorDialog.ShowModal();
		}
		else
		{
			file << m_OutputDataTextBox->GetValue().ToStdWstring();
		}
		file.close();
	}
}

void CSVtoYAML_Panel::PopulateOutputDataTextBox()
{
#if defined(MRT_DEBUG)
	auto start = std::chrono::high_resolution_clock::now();
#endif

	{
		mrt::YAML_Generator<StrType> yamlGenerator(m_CSVData);

		mrt::YAML_Generator<StrType>::OStrStream os;

		yamlGenerator.WriteToStream(&os, m_ExplictTypeCheckBox->GetValue(), GetQuoteString(m_QuotesTypeComboBox->GetSelection()));

		m_OutputDataTextBox->SetValue(os.str());
	}

#if defined(MRT_DEBUG)
	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	MRT_DEBUG_LOG_TIME("Time taken by 'PopulateOutputDataTextBox' in YAML class", duration.count(), "milliseconds");
#endif

	std::lock_guard<std::mutex> lock(m_OutputDataMutex);

	m_TextBoxThreadRunning = false;

	if (!m_ListViewThreadRunning)
	{
		LockOrUnlockItems(false);
	}
}

void CSVtoYAML_Panel::LockOrUnlockItems(bool lock)
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
	m_ExplictTypeCheckBox->Enable(!lock);
	m_QuotesTypeComboBox->Enable(!lock);
}

CSVtoYAML_Panel::StrType CSVtoYAML_Panel::GetQuoteString(int selection)
{
	OStrStream os;

	switch (selection)
	{
		case NoQuotes:
			break;
		case DoubleQuotes:
			os << "\"";
			break;
		case SingleQuotes:
			os << "'";
			break;
		default:
			break;
	}

	return os.str();
}