#include "CSVtoBBCode_Panel.h"

  /************************************/
 /* CSVtoBBCode_Panel Implementation */
/************************************/

CSVtoBBCode_Panel::CSVtoBBCode_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours)
	: CSVto_PanelBase(_parent, _name, _colours)
{
	SetupDataInputSection();
	SetupOutputSettingsSection();
	SetupSpecificOutputSectionItems();
	SetupDataOutputSection();

	SetSizer(m_MainSizer);
}

void CSVtoBBCode_Panel::SetupSpecificOutputSectionItems()
{
	m_OutputSettingsSizer3 = new wxBoxSizer(wxHORIZONTAL);

	m_MinifyBBCodeCheckBox = new wxCheckBox(this, wxID_ANY, "Minify the generated HTML", wxDefaultPosition, wxDefaultSize);
	m_MinifyBBCodeCheckBox->SetMinSize(FromDIP(wxSize(120, 22)));
	m_MinifyBBCodeCheckBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_MinifyBBCodeCheckBox->SetOwnBackgroundColour(m_Colours->BACKGROUND);
	m_MinifyBBCodeCheckBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_MinifyBBCodeCheckBox->SetToolTip("Writes all the generated BBCode tags onto one line");

	m_MinifyBBCodeCheckBox->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				PopulateData();
			}
		}
	);

	m_OutputSettingsSizer3->Add(m_MinifyBBCodeCheckBox, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_MainSizer->Add(m_OutputSettingsSizer3, 0, wxEXPAND | wxALL, FromDIP(0));
}

void CSVtoBBCode_Panel::PopulateOutputDataTextBox()
{
#if defined(MRT_DEBUG)
	auto start = std::chrono::high_resolution_clock::now();
#endif

	{
		mrt::BBCode_Generator<StrType> generator(m_CSVData);

		mrt::BBCode_Generator<StrType>::OStrStream os;

		generator.WriteToStream(&os, !m_MinifyBBCodeCheckBox->GetValue());

		m_OutputDataTextBox->SetValue(os.str());
	}

#if defined(MRT_DEBUG)
	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	MRT_DEBUG_LOG_TIME("Time taken by 'PopulateOutputDataTextBox' in BBCode class", duration.count(), "milliseconds");
#endif

	std::lock_guard<std::mutex> lock(m_OutputDataMutex);

	m_TextBoxThreadRunning = false;

	if (!m_ListViewThreadRunning)
	{
		LockOrUnlockItems(false);
	}
}

void CSVtoBBCode_Panel::OutputFile()
{
	wxFileDialog saveFileDialog(this, "Download File", (wxStandardPaths::Get().GetDocumentsDir()), "", "BBCode (*.bbcode)|*.bbcode", wxFD_SAVE);

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
			file << m_OutputDataTextBox->GetValue().ToStdWstring();
		}
		file.close();
	}
}

void CSVtoBBCode_Panel::LockOrUnlockItems(bool lock)
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
	m_MinifyBBCodeCheckBox->Enable(!lock);
}