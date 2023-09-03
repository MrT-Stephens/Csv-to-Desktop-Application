#include "CSVtoMultiline_Panel.h"

  /***************************************/
 /* CSVtoMultiline_Panel Implementation */
/***************************************/

CSVtoMultiline_Panel::CSVtoMultiline_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours)
	: CSVto_PanelBase(_parent, _name, _colours)
{
	SetupDataInputSection();
	SetupOutputSettingsSection();
	SetupSpecificOutputSectionItems();
	SetupDataOutputSection();

	SetSizer(m_MainSizer);
}

void CSVtoMultiline_Panel::SetupSpecificOutputSectionItems()
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

	m_DataSpacingTextBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
	m_DataSpacingTextBox->SetHint("Record Separator");
	m_DataSpacingTextBox->SetMinSize(FromDIP(wxSize(120, 22)));
	m_DataSpacingTextBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_DataSpacingTextBox->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_DataSpacingTextBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_DataSpacingTextBox->SetToolTip("Set a separator for the outputted data");

	m_DataSpacingTextBox->Bind(wxEVT_TEXT, [this](wxCommandEvent& event)
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
	m_OutputSettingsSizer3->Add(m_DataSpacingTextBox, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_MainSizer->Add(m_OutputSettingsSizer3, 0, wxEXPAND | wxALL, FromDIP(0));
}

void CSVtoMultiline_Panel::PopulateOutputDataTextBox()
{
#if defined(MRT_DEBUG)
	auto start = std::chrono::high_resolution_clock::now();
#endif

	{
		OStrStream os;

		if (!m_IncludeHeaderCheckBox->GetValue())
		{
			const std::vector<StrType>& header = m_CSVData->GetHeaderNames();

			for (size_t i = 0; i < header.size(); ++i)
			{
				os << header[i] << "\n";
			}

			if (!m_DataSpacingTextBox->GetValue().ToStdString().empty())
			{
				os << m_DataSpacingTextBox->GetValue().ToStdWstring() << "\n";
			}
		}

		for (size_t i0 = 0; i0 < m_CSVData->GetRowCount(); ++i0)
		{
			const std::vector<StrType>& row = m_CSVData->GetRowData(i0);

			for (size_t i1 = 0; i1 < row.size(); ++i1)
			{
				os << row[i1] << "\n";
			}

			if (!m_DataSpacingTextBox->GetValue().ToStdString().empty())
			{
				os << m_DataSpacingTextBox->GetValue().ToStdWstring() << "\n";
			}
		}

		m_OutputDataTextBox->SetValue(os.str());
	}

#if defined(MRT_DEBUG)
	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	MRT_DEBUG_LOG_TIME("Time taken by 'PopulateOutputDataTextBox' in Multi-line class", duration.count(), "milliseconds");
#endif

	std::lock_guard<std::mutex> lock(m_OutputDataMutex);

	m_TextBoxThreadRunning = false;

	if (!m_ListViewThreadRunning)
	{
		LockOrUnlockItems(false);
	}
}

void CSVtoMultiline_Panel::LockOrUnlockItems(bool lock)
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
	m_DataSpacingTextBox->Enable(!lock);
}