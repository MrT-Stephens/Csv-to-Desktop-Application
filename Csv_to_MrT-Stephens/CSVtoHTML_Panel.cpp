#include "CSVtoHTML_Panel.h"

  /**********************************/
 /* CSVtoHTML_Panel Implementation */
/**********************************/

CSVtoHTML_Panel::CSVtoHTML_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours)
	: CSVto_PanelBase(_parent, _name, _colours)
{
	SetupDataInputSection();
	SetupOutputSettingsSection();
	SetupSpecificOutputSectionItems();
	SetupDataOutputSection();

	SetSizer(m_MainSizer);
}

void CSVtoHTML_Panel::SetupSpecificOutputSectionItems()
{
	m_OutputSettingsSizer3 = new wxBoxSizer(wxHORIZONTAL);

	m_MinifyHtmlCheckBox = new wxCheckBox(this, wxID_ANY, "Minify the generated HTML", wxDefaultPosition, wxDefaultSize);
	m_MinifyHtmlCheckBox->SetMinSize(FromDIP(wxSize(120, 30)));
	m_MinifyHtmlCheckBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_MinifyHtmlCheckBox->SetOwnBackgroundColour(m_Colours->BACKGROUND);
	m_MinifyHtmlCheckBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_MinifyHtmlCheckBox->SetToolTip("Writes all the generated HTML tags onto one line");

	m_MinifyHtmlCheckBox->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				PopulateData();
			}
		}
	);

	m_TheadTbodyHtmlBtn = new wxButton(this, wxID_ANY, "Include thread and tbody", wxDefaultPosition, wxDefaultSize);
	m_TheadTbodyHtmlBtn->SetMinSize(FromDIP(wxSize(120, 30)));
	m_TheadTbodyHtmlBtn->SetOwnFont(MAIN_FONT_TEXT(10));
	m_TheadTbodyHtmlBtn->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_TheadTbodyHtmlBtn->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_TheadTbodyHtmlBtn->SetToolTip("Include or exclude thread and tbody HTML tags");

	m_TheadTbodyHtmlBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			if (m_TheadTbodyHtmlBtn->GetLabel() == "Include thread and tbody")
			{
				m_TheadTbodyHtmlBtn->SetLabel("Exclude thread and tbody");
			}
			else
			{
				m_TheadTbodyHtmlBtn->SetLabel("Include thread and tbody");
			}

			if (m_CSVData != nullptr)
			{
				PopulateData();
			}
		}
	);

	m_OutputSettingsSizer3->Add(m_MinifyHtmlCheckBox, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(10));
	m_OutputSettingsSizer3->Add(m_TheadTbodyHtmlBtn, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_MainSizer->Add(m_OutputSettingsSizer3, 0, wxEXPAND | wxALL, FromDIP(0));
}

void CSVtoHTML_Panel::PopulateOutputDataTextBox()
{
#if defined(MRT_DEBUG)
	auto start = std::chrono::high_resolution_clock::now();
#endif

	{
		mrt::HTML_Table<std::string> htmlTags(m_CSVData);

		mrt::HTML_Table<std::string>::OStrStream ss;

		htmlTags.WriteToStream(&ss, (m_TheadTbodyHtmlBtn->GetLabel() == "Include thread and tbody" ? false : true), !m_MinifyHtmlCheckBox->GetValue());

		m_OutputDataTextBox->SetValue(ss.str());
	}

#if defined(MRT_DEBUG)
	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	MRT_DEBUG_LOG_TIME("Time taken by 'PopulateOutputDataTextBox' in HTML class", duration.count(), "milliseconds");
#endif

	std::lock_guard<std::mutex> lock(m_OutputDataMutex);

	m_TextBoxThreadRunning = false;

	if (!m_ListViewThreadRunning)
	{
		LockOrUnlockItems(false);
	}
}

void CSVtoHTML_Panel::LockOrUnlockItems(bool lock)
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
	m_MinifyHtmlCheckBox->Enable(!lock);
	m_TheadTbodyHtmlBtn->Enable(!lock);
}