#include "CSVto_PanelBase.h"

CSVto_PanelBase::CSVto_PanelBase(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours) 
	: wxPanel(_parent), m_PanelName(_name), m_Colours(_colours)
{
	m_MainSizer = new wxBoxSizer(wxVERTICAL);
}

CSVto_PanelBase::~CSVto_PanelBase()
{
	delete m_CSVData;
}

void CSVto_PanelBase::SetupDataInputSection()
{
	m_DataInBtnSizer = new wxBoxSizer(wxHORIZONTAL);

	m_LogoButton = new wxBitmapButton(this, wxID_ANY, wxBitmap(CSV_to_Logo).ConvertToImage().Rescale(50, 50, wxIMAGE_QUALITY_HIGH), wxDefaultPosition, wxDefaultSize);
	m_LogoButton->SetMinSize({ 60, 60 });
	m_LogoButton->SetBackgroundColour(m_Colours->BACKGROUND);

	m_LogoButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& _evt)
		{
			mrt::MrT_InfoDialog infoMessageBox(this, "Infomation", 
				"Hello,\n\nThis software is continuously being developed for better performance, less bugs, more features, and better user experience."
				"\n\nCsv to... is a free to use software and if you like it enough consider donating."
				"\n\nIf you have got issues with the software or you just want to share you feedback, click the email button or contact me through GitHub.",
				m_Colours, {400, 500});

			infoMessageBox.ShowModal();
		}
	);

	// Set the text for the logo
	auto sql_Text = new wxStaticText(this, wxID_ANY, m_PanelName, wxDefaultPosition, wxDefaultSize);
	sql_Text->SetOwnForegroundColour(m_Colours->SECONDARY);
	sql_Text->SetOwnFont(MAIN_FONT_TEXT(13));

	auto name_Text = new wxStaticText(this, wxID_ANY, "By: MrT Stephens ", wxDefaultPosition, wxDefaultSize);
	name_Text->SetOwnForegroundColour(m_Colours->SECONDARY);
	name_Text->SetOwnFont(MAIN_FONT_TEXT(13));

	// Add the logo and text to the horizontal sizer
	m_DataInBtnSizer->Add(m_LogoButton, 0, wxALL | wxCENTER, FromDIP(5));
	m_DataInTextSizer = new wxBoxSizer(wxVERTICAL);
	m_DataInTextSizer->Add(sql_Text, 0, wxALL | wxLEFT, FromDIP(2));
	m_DataInTextSizer->Add(name_Text, 0, wxALL | wxLEFT, FromDIP(2));
	m_DataInBtnSizer->Add(m_DataInTextSizer, 0, wxALL | wxCENTER, FromDIP(0));

	// Create the open file button and format it
	m_LoadData = new wxButton(this, wxID_ANY, "Open File", wxDefaultPosition, wxDefaultSize);
	m_LoadData->SetMinSize({ 120, 25 });
	m_LoadData->SetOwnFont(MAIN_FONT_TEXT(10));
	m_LoadData->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_LoadData->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_LoadData->SetToolTip("Open a CSV file");

	// Bind the open file button to the event
	m_LoadData->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			wxFileDialog openFileDialog(this, "Open File", (wxStandardPaths::Get().GetDocumentsDir()), "", "CSV files(*.csv) | *.csv", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

			if (openFileDialog.ShowModal() == wxID_OK)
			{
				m_FileDir = openFileDialog.GetPath();

				delete (m_CSVData);
				m_CSVData = new mrt::CSVData(m_FileDir, true);

				if (m_CSVData->GetError() == mrt::CSVData_Error::NONE)
				{
					PopulateData();
				}
				else if (m_CSVData->GetError() == mrt::CSVData_Error::CANNOT_OPEN_FILE)
				{
					mrt::MrT_UniDialog errorDialog(this, "Error", "Failed to open file.\nPlease try to re-open the file.",
						m_Colours, wxICON(wxICON_ERROR), mrt::MrT_UniDialogType_OK, { 400, 200 });

					errorDialog.ShowModal();
				}
				else
				{
					mrt::MrT_UniDialog errorDialog(this, "Error", "Failed to input data from file.\nPlease try to re-open the file.",
						m_Colours, wxICON(wxICON_ERROR), mrt::MrT_UniDialogType_OK, { 400, 200 });

					errorDialog.ShowModal();
				}
			}
		}
	);

	// Create the example file button and format it
	m_ExampleData = new wxButton(this, wxID_ANY, "Example File", wxDefaultPosition, wxDefaultSize);
	m_ExampleData->SetMinSize({ 120, 25 });
	m_ExampleData->SetOwnFont(MAIN_FONT_TEXT(10));
	m_ExampleData->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_ExampleData->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_ExampleData->SetToolTip("Open the example CSV file");

	// Bind the example file button to the event
	m_ExampleData->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			delete (this->m_CSVData);
			m_CSVData = new mrt::CSVData({
				{ "John", "James", "39", "M", "P001" },
				{ "James", "Fisher", "25", "M", "P002" },
				{ "Matt", "Lewis", "18", "M", "P003" }, 
				{ "Sarah", "Vills", "22", "F", "P004" }}, 
				{"firstname", "lastname", "age", "gender", "personId" });

			PopulateData();
		}
	);

	// Add the buttons to the horizontal sizer
	m_DataInBtnSizer->AddStretchSpacer(1);

	m_DataInBtnSizer->Add(m_LoadData, 0, wxALL | wxEXPAND | wxCENTER, FromDIP(5));
	m_DataInBtnSizer->Add(m_ExampleData, 0, wxALL | wxEXPAND | wxCENTER, FromDIP(5));

	m_MainSizer->Add(m_DataInBtnSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, FromDIP(5));

	m_DataListViewSizer = new wxBoxSizer(wxHORIZONTAL);

	// Create the list view and format it
	m_DataInputListView = new wxListView(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	m_DataInputListView->SetOwnFont(MAIN_FONT_TEXT(10));
	m_DataInputListView->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_DataInputListView->SetOwnForegroundColour(m_Colours->FOREGROUND);

	// Bind the list view to the column click event
	m_DataInputListView->Bind(wxEVT_LIST_COL_CLICK, [this](wxListEvent& event)
		{
			wchar_t upArrow = '\u2191';
			wchar_t downArrow = '\u2193';

			int column = event.GetColumn() - 1;

			if (!m_ListViewThreadRunning && !m_TextBoxThreadRunning && column > -1)
			{
				if (m_CurrentSortColumn == 0)
				{
					m_CurrentSortColumn = column;
					m_CurrentSortOrder = 1;
				}
				else if (m_CurrentSortColumn == column)
				{
					m_CurrentSortOrder = ((m_CurrentSortOrder == 2) ? 1 : 2);
				}
				else
				{
					m_CurrentSortColumn = column;
					m_CurrentSortOrder = 1;
				}

				m_CSVData->SortByColumn(column, ((m_CurrentSortOrder < 2) ? true : false));
				PopulateData();
			}
		}
	);

	// Add the list view to the horizontal sizer
	m_DataListViewSizer->Add(m_DataInputListView, 1, wxALL | wxEXPAND, FromDIP(10));

	m_MainSizer->Add(m_DataListViewSizer, 1, wxEXPAND | wxALL, FromDIP(0));
}

void CSVto_PanelBase::SetupOutputSettingsSection()
{
	m_OutputSettingsSizer1 = new wxBoxSizer(wxHORIZONTAL);
	m_OutputSettingsSizer2 = new wxBoxSizer(wxHORIZONTAL);

	auto editor_Text = new wxStaticText(this, wxID_ANY, "Output Settings: ", wxDefaultPosition, wxDefaultSize);
	editor_Text->SetOwnForegroundColour(m_Colours->SECONDARY);
	editor_Text->SetOwnFont(MAIN_FONT_TEXT(13));

	m_MainSizer->Add(editor_Text, 0, wxLEFT | wxRIGHT | wxTOP, FromDIP(10));

	m_ClearBtn = new wxButton(this, wxID_ANY, "Clear Data", wxDefaultPosition, wxDefaultSize);
	m_ClearBtn->SetMinSize({ 120, 30 });
	m_ClearBtn->SetOwnFont(MAIN_FONT_TEXT(10));
	m_ClearBtn->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_ClearBtn->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_ClearBtn->SetToolTip("Clear the data from the application");

	m_ClearBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			m_OutputDataTextBox->Clear();
			m_DataInputListView->ClearAll();
			delete (m_CSVData);
			m_CSVData = nullptr;
		}
	);

	m_OutputSettingsSizer1->Add(m_ClearBtn, 1, wxRIGHT | wxLEFT | wxTOP | wxEXPAND | wxCENTER, FromDIP(10));

	m_LowercaseBtn = new wxButton(this, wxID_ANY, "Lowercase", wxDefaultPosition, wxDefaultSize);
	m_LowercaseBtn->SetMinSize({ 120, 30 });
	m_LowercaseBtn->SetOwnFont(MAIN_FONT_TEXT(10));
	m_LowercaseBtn->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_LowercaseBtn->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_LowercaseBtn->SetToolTip("Convert all data to lowercase");

	m_LowercaseBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			if (!m_OutputDataTextBox->IsEmpty())
			{
				m_CSVData->LowerUpperData();
				PopulateData();
			}
		}
	);

	m_OutputSettingsSizer1->Add(m_LowercaseBtn, 1, wxRIGHT | wxLEFT | wxTOP | wxEXPAND | wxCENTER, FromDIP(10));

	m_UppercaseBtn = new wxButton(this, wxID_ANY, "Uppercase", wxDefaultPosition, wxDefaultSize);
	m_UppercaseBtn->SetMinSize({ 120, 30 });
	m_UppercaseBtn->SetOwnFont(MAIN_FONT_TEXT(10));
	m_UppercaseBtn->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_UppercaseBtn->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_UppercaseBtn->SetToolTip("Convert all data to uppercase");

	m_UppercaseBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			if (!m_OutputDataTextBox->IsEmpty())
			{
				m_CSVData->LowerUpperData(false);
				PopulateData();
			}
		}
	);

	m_OutputSettingsSizer1->Add(m_UppercaseBtn, 1, wxRIGHT | wxLEFT | wxTOP | wxEXPAND | wxCENTER, FromDIP(10));

	m_CapitalizeBtn = new wxButton(this, wxID_ANY, "Capitalize", wxDefaultPosition, wxDefaultSize);
	m_CapitalizeBtn->SetMinSize({ 120, 30 });
	m_CapitalizeBtn->SetOwnFont(MAIN_FONT_TEXT(10));
	m_CapitalizeBtn->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_CapitalizeBtn->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_CapitalizeBtn->SetToolTip("Capitalize the data");

	m_CapitalizeBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			if (!m_OutputDataTextBox->IsEmpty())
			{
				m_CSVData->CapitalizeData();
				PopulateData();
			}
		}
	);

	m_OutputSettingsSizer2->Add(m_CapitalizeBtn, 1, wxRIGHT | wxLEFT | wxEXPAND | wxCENTER, FromDIP(10));

	m_TransposeBtn = new wxButton(this, wxID_ANY, "Transpose", wxDefaultPosition, wxDefaultSize);
	m_TransposeBtn->SetMinSize({ 120, 30 });
	m_TransposeBtn->SetOwnFont(MAIN_FONT_TEXT(10));
	m_TransposeBtn->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_TransposeBtn->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_TransposeBtn->SetToolTip("Transpose the data");

	m_TransposeBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			if (!m_OutputDataTextBox->IsEmpty())
			{
				m_CSVData->TransposeData();
				PopulateData();
			}
		}
	);

	m_OutputSettingsSizer2->Add(m_TransposeBtn, 1, wxRIGHT | wxLEFT | wxEXPAND | wxCENTER, FromDIP(10));

	m_DeleteBlanksBtn = new wxButton(this, wxID_ANY, "Delete Spaces", wxDefaultPosition, wxDefaultSize);
	m_DeleteBlanksBtn->SetMinSize({ 120, 30 });
	m_DeleteBlanksBtn->SetOwnFont(MAIN_FONT_TEXT(10));
	m_DeleteBlanksBtn->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_DeleteBlanksBtn->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_DeleteBlanksBtn->SetToolTip("Delete all spaces from data");

	m_DeleteBlanksBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			if (!m_OutputDataTextBox->IsEmpty())
			{
				m_CSVData->RemoveWhiteSpace();
				PopulateData();
			}
		}
	);

	m_OutputSettingsSizer2->Add(m_DeleteBlanksBtn, 1, wxRIGHT | wxLEFT | wxEXPAND | wxCENTER, FromDIP(10));

	m_MainSizer->Add(m_OutputSettingsSizer1, 0, wxEXPAND | wxALL, FromDIP(0));
	m_MainSizer->Add(m_OutputSettingsSizer2, 0, wxEXPAND | wxALL, FromDIP(0));
}

void CSVto_PanelBase::SetupDataOutputSection()
{
	m_DataOutputSizer = new wxBoxSizer(wxHORIZONTAL);

	auto output_Text = new wxStaticText(this, wxID_ANY, "Data Output: ", wxDefaultPosition, wxDefaultSize);
	output_Text->SetOwnForegroundColour(m_Colours->SECONDARY);
	output_Text->SetOwnFont(MAIN_FONT_TEXT(13));

	m_MainSizer->Add(output_Text, 0, wxLEFT | wxRIGHT | wxTOP, FromDIP(10));

	m_OutputDataTextBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_DONTWRAP);
	m_OutputDataTextBox->SetOwnFont(MAIN_FONT_TEXT(10));
	m_OutputDataTextBox->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_OutputDataTextBox->SetOwnForegroundColour(m_Colours->FOREGROUND);

	m_DataOutputSizer->Add(m_OutputDataTextBox, 1, wxEXPAND | wxALL, FromDIP(10));

	m_MainSizer->Add(m_DataOutputSizer, 1, wxEXPAND | wxALL, FromDIP(0));

	m_DataOutputBtnSizer = new wxBoxSizer(wxHORIZONTAL);

	m_CopyDataBtn = new wxButton(this, wxID_ANY, "Copy", wxDefaultPosition, wxDefaultSize);
	m_CopyDataBtn->SetMinSize({ 120, 30 });
	m_CopyDataBtn->SetOwnFont(MAIN_FONT_TEXT(10));
	m_CopyDataBtn->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_CopyDataBtn->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_CopyDataBtn->SetToolTip("Copy the generated data to the clipboard");

	m_CopyDataBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			if (!m_OutputDataTextBox->IsEmpty())
			{
				wxClipboard::Get()->SetData(new wxTextDataObject(std::move(m_OutputDataTextBox->GetValue())));
			}
		}
	);

	m_DataOutputBtnSizer->Add(m_CopyDataBtn, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_DownloadBtn = new wxButton(this, wxID_ANY, "Download", wxDefaultPosition, wxDefaultSize);
	m_DownloadBtn->SetMinSize({ 120, 30 });
	m_DownloadBtn->SetOwnFont(MAIN_FONT_TEXT(10));
	m_DownloadBtn->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_DownloadBtn->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_DownloadBtn->SetToolTip("Download the generated data to a text file");

	m_DownloadBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			if (!m_OutputDataTextBox->IsEmpty())
			{
				wxFileDialog saveFileDialog(this, "Download File", (wxStandardPaths::Get().GetDocumentsDir()), "", "Text files (*.txt)|*.txt", wxFD_SAVE);

				if (saveFileDialog.ShowModal() == wxID_OK)
				{
					std::ofstream file(saveFileDialog.GetPath().ToStdString());

					if (!file.is_open())
					{
						mrt::MrT_UniDialog errorDialog(this, "Error", "Failed to save file!\nPlease try to re-save the file.",
							m_Colours, wxICON(wxICON_ERROR), mrt::MrT_UniDialogType_OK, { 400, 200 });

						errorDialog.ShowModal();
					}
					else
					{
						file << std::move(m_OutputDataTextBox->GetValue().ToStdString());
					}
					file.close();
				}
			}
		}
	);

	m_DataOutputBtnSizer->Add(m_DownloadBtn, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_MainSizer->Add(m_DataOutputBtnSizer, 0, wxEXPAND | wxALL, FromDIP(0));
}

void CSVto_PanelBase::PopulateData()
{
	m_OutputDataTextBox->Clear();
	m_DataInputListView->ClearAll();

	LockOrUnlockItems(true);

	m_ListViewThreadRunning = true;
	m_TextBoxThreadRunning = true;
	
	std::thread listViewWorker(&CSVto_PanelBase::PopulateDataListView, this);
	std::thread outputDataWorker(&CSVto_PanelBase::PopulateOutputDataTextBox, this);

	listViewWorker.detach();
	outputDataWorker.detach();
}

void CSVto_PanelBase::PopulateDataListView()
{
	unsigned int average_width = m_DataInputListView->GetSize().GetWidth() / (m_CSVData->GetColumnCount() + 1);

	m_DataInputListView->InsertColumn(0, "Row Number", wxLIST_ALIGN_SNAP_TO_GRID, average_width);

	for (size_t i = 0; (i < m_CSVData->GetColumnCount()); ++i)
	{
		m_DataInputListView->InsertColumn(i + 1, m_CSVData->GetHeaderNames()[i], wxLIST_ALIGN_SNAP_TO_GRID, average_width);
	}

	for (size_t i0 = 0; (i0 < m_CSVData->GetRowCount()); ++i0)
	{
		m_DataInputListView->InsertItem(i0, std::to_string(i0));

		const std::vector<std::string> data = m_CSVData->GetRowData(i0);

		for (size_t i1 = 0; i1 < m_CSVData->GetColumnCount(); ++i1)
		{
			m_DataInputListView->SetItem(i0, i1 + 1, data[i1]);
		}
	}

	std::lock_guard<std::mutex> lock(m_OutputDataMutex);

	m_ListViewThreadRunning = false;

	if (!m_TextBoxThreadRunning)
	{
		LockOrUnlockItems(false);
	}
}

void CSVto_PanelBase::PopulateOutputDataTextBox()
{
	std::lock_guard<std::mutex> lock(m_OutputDataMutex);

	m_TextBoxThreadRunning = false;

	if (!m_ListViewThreadRunning)
	{
		LockOrUnlockItems(false);
	}
}

void CSVto_PanelBase::LockOrUnlockItems(bool lock)
{
	m_LoadData->Enable(!lock);
	m_ExampleData->Enable(!lock);
	m_LowercaseBtn->Enable(!lock);
	m_UppercaseBtn->Enable(!lock);
	m_CapitalizeBtn->Enable(!lock);
	m_ClearBtn->Enable(!lock);
	m_TransposeBtn->Enable(!lock);
	m_DeleteBlanksBtn->Enable(!lock);
}

bool CSVto_PanelBase::isThreadsRunning()
{
	std::lock_guard<std::mutex> lock(m_OutputDataMutex);

	return (m_ListViewThreadRunning || m_TextBoxThreadRunning) ? true : false;
}