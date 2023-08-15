#include "CSVtoPHP_Panel.h"

  /*********************************/
 /* CSVtoPHP_Panel Implementation */
/*********************************/

CSVtoPHP_Panel::CSVtoPHP_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours)
	: CSVto_PanelBase(_parent, _name, _colours)
{
	SetupDataInputSection();
	SetupOutputSettingsSection();
	SetupSpecificOutputSectionItems();
	SetupDataOutputSection();

	SetSizer(m_MainSizer);
}

void CSVtoPHP_Panel::SetupSpecificOutputSectionItems() { }

void CSVtoPHP_Panel::OutputFile()
{
	wxFileDialog saveFileDialog(this, "Download File", (wxStandardPaths::Get().GetDocumentsDir()), "", "PHP (*.php)|*.php", wxFD_SAVE);

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

void CSVtoPHP_Panel::PopulateOutputDataTextBox()
{
#if defined(MRT_DEBUG)
	auto start = std::chrono::high_resolution_clock::now();
#endif

	{
		mrt::PHP_Generator<std::string> phpGenerator(m_CSVData);

		mrt::PHP_Generator<std::string>::OStrStream ss;

		phpGenerator.WriteToStream(&ss);

		m_OutputDataTextBox->SetValue(ss.str());
	}

#if defined(MRT_DEBUG)
	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	MRT_DEBUG_LOG_TIME("Time taken by 'PopulateOutputDataTextBox' in PHP class", duration.count(), "milliseconds");
#endif

	std::lock_guard<std::mutex> lock(m_OutputDataMutex);

	m_TextBoxThreadRunning = false;

	if (!m_ListViewThreadRunning)
	{
		LockOrUnlockItems(false);
	}
}

void CSVtoPHP_Panel::LockOrUnlockItems(bool lock)
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
}