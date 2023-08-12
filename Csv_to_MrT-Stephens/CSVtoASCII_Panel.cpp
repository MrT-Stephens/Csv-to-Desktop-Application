#include "CSVtoASCII_Panel.h"

  /********************************/
 /* CSVtoASCII_Panel Declaration */
/********************************/

CSVtoASCII_Panel::CSVtoASCII_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours) 
	: CSVto_PanelBase(_parent, _name, _colours)
{
	SetupDataInputSection();
	SetupOutputSettingsSection();
	SetupSpecificOutputSectionItems();
	SetupDataOutputSection();

	SetSizer(m_MainSizer);
}

void CSVtoASCII_Panel::SetupSpecificOutputSectionItems()
{
	m_OutputSettingsSizer3 = new wxBoxSizer(wxHORIZONTAL);

	m_ForceSeparators = new wxCheckBox(this, wxID_ANY, "Force Separation");
	m_ForceSeparators->SetMinSize(FromDIP(wxSize(120, 30)));
	m_ForceSeparators->SetOwnFont(MAIN_FONT_TEXT(11));
	m_ForceSeparators->SetOwnBackgroundColour(m_Colours->BACKGROUND);
	m_ForceSeparators->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_ForceSeparators->SetToolTip("Force separation in rows");

	m_ForceSeparators->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				PopulateData();
			}
		}
	);

	m_AsciiTableStyleChoices.Add("Ascii table (MySql)");
	m_AsciiTableStyleChoices.Add("Ascii table (Dots)");
	m_AsciiTableStyleChoices.Add("Ascii table (Dots curved)");
	m_AsciiTableStyleChoices.Add("Ascii table (Compact)");
	m_AsciiTableStyleChoices.Add("Ascii table (Header Separated)");
	m_AsciiTableStyleChoices.Add("Ascii table (Simple)");
	m_AsciiTableStyleChoices.Add("Ascii table (Wavy)");
	m_AsciiTableStyleChoices.Add("Ascii table (MySql extended)");

	m_AsciiTableStyle = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, m_AsciiTableStyleChoices, wxCB_READONLY);
	m_AsciiTableStyle->SetSelection(0);
	m_AsciiTableStyle->SetMinSize(FromDIP(wxSize(120, 30)));
	m_AsciiTableStyle->SetOwnFont(MAIN_FONT_TEXT(11));
	m_AsciiTableStyle->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_AsciiTableStyle->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_AsciiTableStyle->SetToolTip("Select a table style");

	m_AsciiTableStyle->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				PopulateData();
			}
		}
	);

	m_LeftSettingsSizer = new wxBoxSizer(wxVERTICAL);
	m_LeftSettingsSizer->Add(m_ForceSeparators, 0, wxRIGHT | wxLEFT | wxTOP | wxEXPAND | wxCENTER, FromDIP(10));
	m_LeftSettingsSizer->Add(m_AsciiTableStyle, 0, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_CommentTypeChoices.Add("No comment style");
	m_CommentTypeChoices.Add("( // )");
	m_CommentTypeChoices.Add("( -- )");
	m_CommentTypeChoices.Add("( # )");
	m_CommentTypeChoices.Add("( % )");
	m_CommentTypeChoices.Add("( ; )");
	m_CommentTypeChoices.Add("( * )");


	m_CommentType = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, m_CommentTypeChoices, wxCB_READONLY);
	m_CommentType->SetSelection(0);
	m_CommentType->SetMinSize(FromDIP(wxSize(120, 30)));
	m_CommentType->SetOwnFont(MAIN_FONT_TEXT(11));
	m_CommentType->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_CommentType->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_CommentType->SetToolTip("Select a language comment type");

	m_CommentType->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent& event)
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
	m_TextAlignment->SetMinSize(FromDIP(wxSize(120, 30)));
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
	m_RightSettingsSizer->Add(m_CommentType, 0, wxRIGHT | wxLEFT | wxTOP | wxEXPAND | wxCENTER, FromDIP(10));
	m_RightSettingsSizer->Add(m_TextAlignment, 0, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_OutputSettingsSizer3->Add(m_LeftSettingsSizer, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(0));
	m_OutputSettingsSizer3->Add(m_RightSettingsSizer, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(0));

	m_MainSizer->Add(m_OutputSettingsSizer3, 0, wxEXPAND | wxALL, FromDIP(0));
}

void CSVtoASCII_Panel::PopulateOutputDataTextBox()
{
#if defined(MRT_DEBUG)
	auto start = std::chrono::high_resolution_clock::now();
#endif

	{
		mrt::AsciiTableGenerator<std::string> asciiTableGenerator(m_CSVData);

		mrt::AsciiTableGenerator<std::string>::OStrStream ss;

		asciiTableGenerator.GenerateAsciiTable(&ss, m_AsciiTableStyle->GetSelection(), GetCommentCharacters(), m_ForceSeparators->GetValue(), m_TextAlignment->GetSelection());

		m_OutputDataTextBox->SetValue(ss.str());
	}

#if defined(MRT_DEBUG)
	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	MRT_DEBUG_LOG_TIME("Time taken by 'PopulateOutputDataTextBox' in ASCII class", duration.count(), "milliseconds");
#endif
	std::lock_guard<std::mutex> lock(m_OutputDataMutex);

	m_TextBoxThreadRunning = false;

	if (!m_ListViewThreadRunning)
	{
		LockOrUnlockItems(false);
	}
}

void CSVtoASCII_Panel::LockOrUnlockItems(bool lock)
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
	m_ForceSeparators->Enable(!lock);
	m_AsciiTableStyle->Enable(!lock);
	m_CommentType->Enable(!lock);
	m_TextAlignment->Enable(!lock);
}

std::string CSVtoASCII_Panel::GetCommentCharacters()
{
	switch (m_CommentType->GetSelection())
	{
	case 0:
		return "";
	case 1:
		return "//  ";
	case 2:
		return "--  ";
	case 3:
		return "#  ";
	case 4:
		return "%  ";
	case 5:
		return ";  ";
	case 6:
		return "*  ";
	}
	return "";
}