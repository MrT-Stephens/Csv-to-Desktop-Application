#include "CSVtoLaTex_Panel.h"

CSVtoLaTex_Panel::CSVtoLaTex_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours)
	: CSVto_PanelBase(_parent, _name, _colours)
{
	SetupDataInputSection();
	SetupOutputSettingsSection();
	SetupSpecificOutputSectionItems();
	SetupDataOutputSection();

	SetSizer(m_MainSizer);
}

void CSVtoLaTex_Panel::SetupSpecificOutputSectionItems()
{
	m_OutputSettingsSizer3 = new wxBoxSizer(wxHORIZONTAL);

	m_HereOrTopCheckBox = new wxCheckBox(this, wxID_ANY, "Here or top of page", wxDefaultPosition, wxDefaultSize);
	m_HereOrTopCheckBox->SetMinSize({ 120, 30 });
	m_HereOrTopCheckBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_HereOrTopCheckBox->SetOwnBackgroundColour(m_Colours->BACKGROUND);
	m_HereOrTopCheckBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_HereOrTopCheckBox->SetToolTip("Add the here or top of page statement to the laTex code");

	m_HereOrTopCheckBox->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& event)
		{
			if (!m_OutputDataTextBox->IsEmpty())
			{
				PopulateData();
			}
		}
	);

	m_MinimalWorkingCheckBox = new wxCheckBox(this, wxID_ANY, "Minimal working", wxDefaultPosition, wxDefaultSize);
	m_MinimalWorkingCheckBox->SetMinSize({ 120, 30 });
	m_MinimalWorkingCheckBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_MinimalWorkingCheckBox->SetOwnBackgroundColour(m_Colours->BACKGROUND);
	m_MinimalWorkingCheckBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_MinimalWorkingCheckBox->SetToolTip("Add the minimal working statements to the laTex code");

	m_MinimalWorkingCheckBox->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& event)
		{
			if (!m_OutputDataTextBox->IsEmpty())
			{
				PopulateData();
			}
		}
	);

	m_LeftSizer = new wxBoxSizer(wxVERTICAL);
	m_LeftSizer->Add(m_HereOrTopCheckBox, 0, wxRIGHT | wxLEFT | wxTOP | wxEXPAND | wxCENTER, FromDIP(10));
	m_LeftSizer->Add(m_MinimalWorkingCheckBox, 0, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_LabelTextBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
	m_LabelTextBox->SetValue("label");
	m_LabelTextBox->SetMinSize({ 120, 30 });
	m_LabelTextBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_LabelTextBox->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_LabelTextBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_LabelTextBox->SetToolTip("Set a label for the laTex table. Leave empty for no label");

	m_LabelTextBox->Bind(wxEVT_TEXT, [this](wxCommandEvent& event)
		{
			if (!m_OutputDataTextBox->IsEmpty())
			{
				m_Timer.Start(2000);
			}
		}
	);

	m_Timer.Bind(wxEVT_TIMER, [this](wxTimerEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				m_Timer.Stop();
				PopulateData();
			}
		}
	);

	m_TableCaptionTextBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
	m_TableCaptionTextBox->SetValue("caption");
	m_TableCaptionTextBox->SetMinSize({ 120, 30 });
	m_TableCaptionTextBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_TableCaptionTextBox->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_TableCaptionTextBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_TableCaptionTextBox->SetToolTip("Set a caption for the laTex table. Leave empty for no caption");

	m_TableCaptionTextBox->Bind(wxEVT_TEXT, [this](wxCommandEvent& event)
		{
			if (!m_OutputDataTextBox->IsEmpty())
			{
				m_Timer.Start(2000);
			}
		}
	);

	m_MiddleSizer1 = new wxBoxSizer(wxVERTICAL);
	m_MiddleSizer1->Add(m_LabelTextBox, 0, wxRIGHT | wxLEFT | wxTOP | wxEXPAND | wxCENTER, FromDIP(10));
	m_MiddleSizer1->Add(m_TableCaptionTextBox, 0, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_BorderOptions.Add("All");
	m_BorderOptions.Add("MySQL");
	m_BorderOptions.Add("Excel");
	m_BorderOptions.Add("Horizontal");
	m_BorderOptions.Add("Markdown");
	m_BorderOptions.Add("None");

	m_TableBorderComboBox = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, m_BorderOptions, wxCB_READONLY);
	m_TableBorderComboBox->SetSelection(0);
	m_TableBorderComboBox->SetMinSize({ 120, 30 });
	m_TableBorderComboBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_TableBorderComboBox->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_TableBorderComboBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_TableBorderComboBox->SetToolTip("Select a border type for the laTex table");

	m_TableBorderComboBox->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent& event)
		{
			if (!m_OutputDataTextBox->IsEmpty())
			{
				PopulateData();
			}
		}
	);

	m_CaptionLocationOptions.Add("Top");
	m_CaptionLocationOptions.Add("Bottom");

	m_CaptionLocationComboBox = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, m_CaptionLocationOptions, wxCB_READONLY);
	m_CaptionLocationComboBox->SetSelection(0);
	m_CaptionLocationComboBox->SetMinSize({ 120, 30 });
	m_CaptionLocationComboBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_CaptionLocationComboBox->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_CaptionLocationComboBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_CaptionLocationComboBox->SetToolTip("Select a caption location for the laTex table");

	m_CaptionLocationComboBox->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent& event)
		{
			if (!m_OutputDataTextBox->IsEmpty())
			{
				PopulateData();
			}
		}
	);

	m_MiddleSizer2 = new wxBoxSizer(wxVERTICAL);
	m_MiddleSizer2->Add(m_TableBorderComboBox, 0, wxRIGHT | wxLEFT | wxTOP | wxEXPAND | wxCENTER, FromDIP(10));
	m_MiddleSizer2->Add(m_CaptionLocationComboBox, 0, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_AlignmentOptions.Add("Left");
	m_AlignmentOptions.Add("Center");
	m_AlignmentOptions.Add("Right");

	m_TableAlignmentComboBox = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, m_AlignmentOptions, wxCB_READONLY);
	m_TableAlignmentComboBox->SetSelection(0);
	m_TableAlignmentComboBox->SetMinSize({ 120, 30 });
	m_TableAlignmentComboBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_TableAlignmentComboBox->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_TableAlignmentComboBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_TableAlignmentComboBox->SetToolTip("Select a table alignment for the laTex table");

	m_TableAlignmentComboBox->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent& event)
		{
			if (!m_OutputDataTextBox->IsEmpty())
			{
				PopulateData();
			}
		}
	);

	m_TextAligmentComboBox = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, m_AlignmentOptions, wxCB_READONLY);
	m_TextAligmentComboBox->SetSelection(0);
	m_TextAligmentComboBox->SetMinSize({ 120, 30 });
	m_TextAligmentComboBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_TextAligmentComboBox->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_TextAligmentComboBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_TextAligmentComboBox->SetToolTip("Select a table alignment for the laTex table");

	m_TextAligmentComboBox->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent& event)
		{
			if (!m_OutputDataTextBox->IsEmpty())
			{
				PopulateData();
			}
		}
	);

	m_RightSizer = new wxBoxSizer(wxVERTICAL);
	m_RightSizer->Add(m_TableAlignmentComboBox, 0, wxRIGHT | wxLEFT | wxTOP | wxEXPAND | wxCENTER, FromDIP(10));
	m_RightSizer->Add(m_TextAligmentComboBox, 0, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_OutputSettingsSizer3->Add(m_LeftSizer, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(0));
	m_OutputSettingsSizer3->Add(m_MiddleSizer1, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(0));
	m_OutputSettingsSizer3->Add(m_MiddleSizer2, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(0));
	m_OutputSettingsSizer3->Add(m_RightSizer, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(0));

	m_MainSizer->Add(m_OutputSettingsSizer3, 0, wxEXPAND | wxALL, FromDIP(0));
}

void CSVtoLaTex_Panel::PopulateOutputDataTextBox()
{
#if defined(MRT_DEBUG)
	auto start = std::chrono::high_resolution_clock::now();
#endif

	{
		mrt::LaTex_Table<std::string> table(m_CSVData, m_LabelTextBox->GetValue().ToStdString(), m_TableCaptionTextBox->GetValue().ToStdString(), m_CaptionLocationComboBox->GetSelection());

		mrt::LaTex_Table<std::string>::OStrStream ss;

		table.WriteToStream(&ss, m_TableBorderComboBox->GetSelection(), m_TableAlignmentComboBox->GetSelection(), 
			m_TextAligmentComboBox->GetSelection(), m_HereOrTopCheckBox->GetValue(), m_MinimalWorkingCheckBox->GetValue());

		m_OutputDataTextBox->SetValue(ss.str());
	}

#if defined(MRT_DEBUG)
	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	OutputDebugString(L"[MRT_DEBUG] Time taken by 'PopulateOutputDataTextBox' (LaTex CLASS): " + std::to_string(duration.count()) + " milliseconds.\n");
#endif

	std::lock_guard<std::mutex> lock(m_OutputDataMutex);

	m_TextBoxThreadRunning = false;

	if (!m_ListViewThreadRunning)
	{
		LockOrUnlockItems(false);
	}
}

void CSVtoLaTex_Panel::LockOrUnlockItems(bool lock)
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
	m_HereOrTopCheckBox->Enable(!lock);
	m_MinimalWorkingCheckBox->Enable(!lock);
	m_LabelTextBox->Enable(!lock);
	m_TableCaptionTextBox->Enable(!lock);
	m_TableAlignmentComboBox->Enable(!lock);
	m_TextAligmentComboBox->Enable(!lock);
	m_TableBorderComboBox->Enable(!lock);
	m_CaptionLocationComboBox->Enable(!lock);
}