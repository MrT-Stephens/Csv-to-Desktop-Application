#include "CSVtoMarkdown_Panel.h"

  /**************************************/
 /* CSVtoMarkdown_Panel Implementation */
/**************************************/

CSVtoMarkdown_Panel::CSVtoMarkdown_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours)
	: CSVto_PanelBase(_parent, _name, _colours)
{
	SetupDataInputSection();
	SetupOutputSettingsSection();
	SetupSpecificOutputSectionItems();
	SetupDataOutputSection();

	SetSizer(m_MainSizer);
}

void CSVtoMarkdown_Panel::SetupSpecificOutputSectionItems()
{
	m_OutputSettingsSizer3 = new wxBoxSizer(wxHORIZONTAL);

	m_BoldFirstRow = new wxCheckBox(this, wxID_ANY, "Bold First Row");
	m_BoldFirstRow->SetMinSize(FromDIP(wxSize(120, 22)));
	m_BoldFirstRow->SetOwnFont(MAIN_FONT_TEXT(11));
	m_BoldFirstRow->SetOwnBackgroundColour(m_Colours->BACKGROUND);
	m_BoldFirstRow->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_BoldFirstRow->SetToolTip("Bold the first row of the table");

	m_BoldFirstRow->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				PopulateData();
			}
		}
	);

	m_BoldFirstCol = new wxCheckBox(this, wxID_ANY, "Bold First Column");
	m_BoldFirstCol->SetMinSize(FromDIP(wxSize(120, 22)));
	m_BoldFirstCol->SetOwnFont(MAIN_FONT_TEXT(11));
	m_BoldFirstCol->SetOwnBackgroundColour(m_Colours->BACKGROUND);
	m_BoldFirstCol->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_BoldFirstCol->SetToolTip("Bold the first row of the table");

	m_BoldFirstCol->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				PopulateData();
			}
		}
	);

	m_LeftSettingsSizer = new wxBoxSizer(wxVERTICAL);
	m_LeftSettingsSizer->Add(m_BoldFirstRow, 0, wxRIGHT | wxLEFT | wxTOP | wxEXPAND | wxCENTER, FromDIP(10));
	m_LeftSettingsSizer->Add(m_BoldFirstCol, 0, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_TableStyleChoices.Add("Markdown table (Normal)");
	m_TableStyleChoices.Add("Markdown table (Simple)");

	m_TableStyle = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, m_TableStyleChoices, wxCB_READONLY);
	m_TableStyle->SetSelection(0);
	m_TableStyle->SetMinSize(FromDIP(wxSize(120, 22)));
	m_TableStyle->SetOwnFont(MAIN_FONT_TEXT(11));
	m_TableStyle->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_TableStyle->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_TableStyle->SetToolTip("Select a markdown table style");

	m_TableStyle->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				PopulateData();
			}
		}
	);

	m_TextAlignmentChoices.Add("Left");
	m_TextAlignmentChoices.Add("Centre");
	m_TextAlignmentChoices.Add("Right");

	m_TextAlignment = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, m_TextAlignmentChoices, wxCB_READONLY);
	m_TextAlignment->SetSelection(0);
	m_TextAlignment->SetMinSize(FromDIP(wxSize(120, 22)));
	m_TextAlignment->SetOwnFont(MAIN_FONT_TEXT(11));
	m_TextAlignment->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_TextAlignment->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_TextAlignment->SetToolTip("Select a text alignment");

	m_TextAlignment->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				PopulateData();
			}
		}
	);

	m_RightSettingsSizer = new wxBoxSizer(wxVERTICAL);
	m_RightSettingsSizer->Add(m_TableStyle, 0, wxRIGHT | wxLEFT | wxTOP | wxEXPAND | wxCENTER, FromDIP(10));
	m_RightSettingsSizer->Add(m_TextAlignment, 0, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_OutputSettingsSizer3->Add(m_LeftSettingsSizer, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(0));
	m_OutputSettingsSizer3->Add(m_RightSettingsSizer, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(0));

	m_MainSizer->Add(m_OutputSettingsSizer3, 0, wxEXPAND | wxALL, FromDIP(0));
}

void CSVtoMarkdown_Panel::PopulateOutputDataTextBox()
{
#if defined(MRT_DEBUG)
	auto start = std::chrono::high_resolution_clock::now();
#endif

	{
		mrt::MarkdownTableGenerator<StrType> markdownTableGenerator(m_CSVData);

		mrt::MarkdownTableGenerator<StrType>::OStrStream ss;

		markdownTableGenerator.GenerateMarkdownTable(&ss, m_TableStyle->GetSelection(), m_TextAlignment->GetSelection(), m_BoldFirstRow->GetValue(), m_BoldFirstCol->GetValue());

		m_OutputDataTextBox->SetValue(ss.str());
	}

#if defined(MRT_DEBUG)
	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	MRT_DEBUG_LOG_TIME("Time taken by 'PopulateOutputDataTextBox' in Markdown class", duration.count(), "milliseconds");
#endif

	std::lock_guard<std::mutex> lock(m_OutputDataMutex);

	m_TextBoxThreadRunning = false;

	if (!m_ListViewThreadRunning)
	{
		LockOrUnlockItems(false);
	}
}

void CSVtoMarkdown_Panel::LockOrUnlockItems(bool lock)
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
	m_BoldFirstCol->Enable(!lock);
	m_BoldFirstRow->Enable(!lock);
	m_TableStyle->Enable(!lock);
	m_TextAlignment->Enable(!lock);
}