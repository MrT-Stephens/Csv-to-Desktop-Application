﻿#include "CSVto_PanelBase.h"

  /**********************************/
 /* CSVto_PanelBase Implementation */
/**********************************/

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
	m_LogoButton->SetMinSize(FromDIP(wxSize(60, 60)));
	m_LogoButton->SetBackgroundColour(m_Colours->PRIMARY);

	m_LogoButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& _evt)
		{
			mrt::MrT_InfoDialog infoMessageBox(this, "Infomation", 
				"Hello,\n\nThis software is continuously being developed for better performance, less bugs, more features, and better user experience."
				"\n\nIf you have got issues with the software or you just want to share you feedback, please contact me through GitHub.",
				m_Colours, FromDIP(wxSize(400, 425)));

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
	m_LoadData->SetMinSize(wxSize(FromDIP(120), m_LogoButton->GetMinSize().y));
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

				m_CurrentSortColumn = -1;
				delete (m_CSVData);
				m_CSVData = new mrt::CSVData<StrType>(m_FileDir);

				if (m_CSVData->GetError() == mrt::CSVData_Error::NONE)
				{
					if (m_CSVData->GetColumnCount() > 2500 || m_CSVData->GetRowCount() > 2500)
					{
						mrt::MrT_UniDialog yesNoDialog(this, "Warning", std::format("The file that has been inputted has {} columns and {} rows. This may take a while to process in certain cases. Would you like to continue?",
							m_CSVData->GetColumnCount(), m_CSVData->GetRowCount()), m_Colours, wxICON(wxICON_WARNING), mrt::MrT_UniDialogType_YES_NO, FromDIP(wxSize(400, 215)));

						if (yesNoDialog.ShowModal() == wxID_NO)
						{
							delete (m_CSVData);
							m_CSVData = nullptr;
							return;
						}
					}

					PopulateData();

#if defined(MRT_DEBUG)
					MRT_DEBUG_LOG_MSG(std::format(L"File inputted successfully ({})", m_FileDir));
#endif
				}
				else if (m_CSVData->GetError() == mrt::CSVData_Error::CANNOT_OPEN_FILE)
				{
					mrt::MrT_UniDialog errorDialog(this, "Error", "Failed to open file.\nPlease try to re-open the file.",
						m_Colours, wxICON(wxICON_ERROR), mrt::MrT_UniDialogType_OK, FromDIP(wxSize(400, 200)));

					errorDialog.ShowModal();

#if defined(MRT_DEBUG)
					MRT_DEBUG_LOG_MSG(std::format(L"Failed to open file ({})", m_FileDir));
#endif
					delete (m_CSVData);
					m_CSVData = nullptr;
				}
				else if (m_CSVData->GetError() == mrt::CSVData_Error::FILE_IS_EMPTY)
				{
					mrt::MrT_UniDialog errorDialog(this, "Error", "File is empty.\nPlease try a different file.",
						m_Colours, wxICON(wxICON_ERROR), mrt::MrT_UniDialogType_OK, FromDIP(wxSize(400, 200)));

					errorDialog.ShowModal();

#if defined(MRT_DEBUG)
					MRT_DEBUG_LOG_MSG(std::format(L"Failed to input data. File is empty ({})", m_FileDir));
#endif
					delete (m_CSVData);
					m_CSVData = nullptr;
				}
				else
				{
					mrt::MrT_UniDialog errorDialog(this, "Error", "Failed to input data from file.\nPlease try to re-open the file.",
						m_Colours, wxICON(wxICON_ERROR), mrt::MrT_UniDialogType_OK, FromDIP(wxSize(400, 200)));

					errorDialog.ShowModal();

#if defined(MRT_DEBUG)
					MRT_DEBUG_LOG_MSG(std::format(L"Failed to input data from file ({})", m_FileDir));
#endif
					delete (m_CSVData);
					m_CSVData = nullptr;
				}
			}
		}
	);

	// Create the example file button and format it
	m_ExampleData = new wxButton(this, wxID_ANY, "Preview Data", wxDefaultPosition, wxDefaultSize);
	m_ExampleData->SetMinSize(wxSize(FromDIP(120), m_LogoButton->GetMinSize().y));
	m_ExampleData->SetOwnFont(MAIN_FONT_TEXT(10));
	m_ExampleData->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_ExampleData->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_ExampleData->SetToolTip("Open the example CSV file");

	// Bind the example file button to the event
	m_ExampleData->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			m_CurrentSortColumn = -1;
			delete (this->m_CSVData);
			m_CSVData = new mrt::CSVData<StrType>({
				{ L"Matthew", L"Mann", L"21", L"M", L"P001" },
				{ L"James", L"Williams", L"27", L"M", L"P002" },
				{ L"Matt", L"Lewis", L"18", L"M", L"P003" },
				{ L"Sarah", L"Vills", L"22", L"F", L"P004" } },
				{ L"firstName", L"lastName", L"age", L"gender", L"personId" });

#if defined(MRT_DEBUG)
			MRT_DEBUG_LOG_MSG("Preview file inputted successfully");
#endif

			PopulateData();
		}
	);

	// Add the buttons to the horizontal sizer
	m_DataInBtnSizer->AddStretchSpacer(2);

	m_DataInBtnSizer->Add(m_LoadData, 0, wxALL | wxCENTER, FromDIP(5));
	m_DataInBtnSizer->Add(m_ExampleData, 0, wxALL | wxCENTER, FromDIP(5));

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
			int column = event.GetColumn() - 1;

			if (!m_ListViewThreadRunning && !m_TextBoxThreadRunning && column > -1)		// Checks to see if the column clicked is a valid sort column
			{
				if (m_CurrentSortColumn == column)
				{
					m_CurrentSortOrder = ((m_CurrentSortOrder == ORDER_DESCENDING) ? ORDER_ASCENDING : ORDER_DESCENDING);
				}
				else
				{
					m_CurrentSortColumn = column;
					m_CurrentSortOrder = ORDER_ASCENDING;
				}

				m_CSVData->SortByColumn(m_CSVData, m_CurrentSortColumn, ((m_CurrentSortOrder == ORDER_ASCENDING) ? true : false));
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
	m_OutputHeadingSizer = new wxBoxSizer(wxHORIZONTAL);
	m_OutputSettingsSizer1 = new wxBoxSizer(wxHORIZONTAL);
	m_OutputSettingsSizer2 = new wxBoxSizer(wxHORIZONTAL);

	auto editor_Text = new wxStaticText(this, wxID_ANY, "Output Editor: ", wxDefaultPosition, wxDefaultSize);
	editor_Text->SetOwnForegroundColour(m_Colours->SECONDARY);
	editor_Text->SetOwnFont(MAIN_FONT_TEXT(13));

	m_OutputHeadingSizer->Add(editor_Text, 0, wxLEFT | wxRIGHT | wxTOP | wxCENTER, FromDIP(10));

	m_EditDataBtn = new wxButton(this, wxID_ANY, "Edit Row", wxDefaultPosition, wxDefaultSize);
	m_EditDataBtn->SetMinSize(wxSize(FromDIP(120), wxDefaultSize.GetY()));
	m_EditDataBtn->SetOwnFont(MAIN_FONT_TEXT(10));
	m_EditDataBtn->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_EditDataBtn->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_EditDataBtn->SetToolTip("Edit a list view row. Zero for the header.");

	m_EditDataBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				mrt::MrT_CSVDataEdit_Dialog editDialog(this, m_CSVData, m_Colours, wxDefaultPosition, FromDIP(wxSize(450, 400)));

				if (editDialog.ShowModal() == wxID_APPLY)
				{
					m_CSVData->CheckMaxColumnWidths(m_CSVData);
					m_CurrentSortColumn = -1;

					PopulateData();
				}
			}
		}
	);

	m_BasicAdvViewBtn = new wxButton(this, wxID_ANY, "Advanced View", wxDefaultPosition, wxDefaultSize);
	m_BasicAdvViewBtn->SetMinSize(wxSize(FromDIP(120), wxDefaultSize.GetY()));
	m_BasicAdvViewBtn->SetOwnFont(MAIN_FONT_TEXT(10));
	m_BasicAdvViewBtn->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_BasicAdvViewBtn->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_BasicAdvViewBtn->SetToolTip("Switch to basic or advanced view");

	m_BasicAdvViewBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			if (m_BasicAdvViewBtn->GetLabel() == "Advanced View")
			{
				m_BasicAdvViewBtn->SetLabel("Basic View");

				HideOrShowBasicAdvItems(true);
			}
			else
			{
				m_BasicAdvViewBtn->SetLabel("Advanced View");

				HideOrShowBasicAdvItems(false);
			}
		}
	);

	m_OutputHeadingSizer->AddStretchSpacer(1);

	m_OutputHeadingSizer->Add(m_EditDataBtn, 0, wxLEFT | wxTOP | wxCENTER, FromDIP(10));
	m_OutputHeadingSizer->Add(m_BasicAdvViewBtn, 0, wxLEFT | wxRIGHT | wxTOP | wxCENTER, FromDIP(10));

	m_MainSizer->Add(m_OutputHeadingSizer, 0, wxEXPAND | wxALL, FromDIP(0));

	m_CapitalizeBtn = new wxButton(this, wxID_ANY, "Capitalize", wxDefaultPosition, wxDefaultSize);
	m_CapitalizeBtn->SetMinSize(wxSize(FromDIP(120), wxDefaultSize.GetY()));
	m_CapitalizeBtn->SetOwnFont(MAIN_FONT_TEXT(10));
	m_CapitalizeBtn->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_CapitalizeBtn->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_CapitalizeBtn->SetToolTip("Capitalize the data");
	m_CapitalizeBtn->Hide();

	m_CapitalizeBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				m_CSVData->CreateUndo();
				m_CSVData->CapitalizeData(m_CSVData, (m_IncludeHeaderBtn->GetLabel() == "Exclude Header" ? true : false));
				m_CurrentSortColumn = -1;
				PopulateData();
			}
		}
	);

	m_OutputSettingsSizer1->Add(m_CapitalizeBtn, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_LowercaseBtn = new wxButton(this, wxID_ANY, "Lowercase", wxDefaultPosition, wxDefaultSize);
	m_LowercaseBtn->SetMinSize(wxSize(FromDIP(120), wxDefaultSize.GetY()));
	m_LowercaseBtn->SetOwnFont(MAIN_FONT_TEXT(10));
	m_LowercaseBtn->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_LowercaseBtn->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_LowercaseBtn->SetToolTip("Convert all data to lowercase");
	m_LowercaseBtn->Hide();

	m_LowercaseBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				m_CSVData->CreateUndo();
				m_CSVData->LowerUpperData(m_CSVData, (m_IncludeHeaderBtn->GetLabel() == "Exclude Header" ? true : false), true);
				m_CurrentSortColumn = -1;
				PopulateData();
			}
		}
	);

	m_OutputSettingsSizer1->Add(m_LowercaseBtn, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_UppercaseBtn = new wxButton(this, wxID_ANY, "Uppercase", wxDefaultPosition, wxDefaultSize);
	m_UppercaseBtn->SetMinSize(wxSize(FromDIP(120), wxDefaultSize.GetY()));
	m_UppercaseBtn->SetOwnFont(MAIN_FONT_TEXT(10));
	m_UppercaseBtn->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_UppercaseBtn->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_UppercaseBtn->SetToolTip("Convert all data to uppercase");
	m_UppercaseBtn->Hide();

	m_UppercaseBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				m_CSVData->CreateUndo();
				m_CSVData->LowerUpperData(m_CSVData, (m_IncludeHeaderBtn->GetLabel() == "Exclude Header" ? true : false), false);
				m_CurrentSortColumn = -1;
				PopulateData();
			}
		}
	);

	m_OutputSettingsSizer1->Add(m_UppercaseBtn, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_UndoBtn = new wxButton(this, wxID_ANY, "Undo", wxDefaultPosition, wxDefaultSize);
	m_UndoBtn->SetMinSize(wxSize(FromDIP(120), wxDefaultSize.GetY()));
	m_UndoBtn->SetOwnFont(MAIN_FONT_TEXT(10));
	m_UndoBtn->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_UndoBtn->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_UndoBtn->SetToolTip("Undo the last change");
	m_UndoBtn->Hide();

	m_UndoBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				if (m_CSVData->Undo() == mrt::CSVData_UndoRedoState::CAN_UNDO)
				{
					m_CurrentSortColumn = -1;
					PopulateData();
				}
			}
		}
	);

	m_OutputSettingsSizer1->Add(m_UndoBtn, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_IncludeHeaderBtn = new wxButton(this, wxID_ANY, "Include Header", wxDefaultPosition, wxDefaultSize);
	m_IncludeHeaderBtn->SetMinSize(wxSize(FromDIP(120), wxDefaultSize.GetY()));
	m_IncludeHeaderBtn->SetOwnFont(MAIN_FONT_TEXT(10));
	m_IncludeHeaderBtn->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_IncludeHeaderBtn->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_IncludeHeaderBtn->SetToolTip("Include the header in the editor changes");
	m_IncludeHeaderBtn->Hide();

	m_IncludeHeaderBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			if (m_IncludeHeaderBtn->GetLabel() == "Include Header")
			{
				m_IncludeHeaderBtn->SetLabel("Exclude Header");
			}
			else
			{
				m_IncludeHeaderBtn->SetLabel("Include Header");
			}
		}
	);

	m_OutputSettingsSizer2->Add(m_IncludeHeaderBtn, 1, wxRIGHT | wxLEFT | wxEXPAND | wxCENTER, FromDIP(10));

	m_TransposeBtn = new wxButton(this, wxID_ANY, "Transpose", wxDefaultPosition, wxDefaultSize);
	m_TransposeBtn->SetMinSize(wxSize(FromDIP(120), wxDefaultSize.GetY()));
	m_TransposeBtn->SetOwnFont(MAIN_FONT_TEXT(10));
	m_TransposeBtn->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_TransposeBtn->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_TransposeBtn->SetToolTip("Transpose the data");
	m_TransposeBtn->Hide();

	m_TransposeBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				m_CSVData->CreateUndo();
				m_CSVData->TransposeData(m_CSVData);
				m_CurrentSortColumn = -1;
				PopulateData();
			}
		}
	);

	m_OutputSettingsSizer2->Add(m_TransposeBtn, 1, wxRIGHT | wxLEFT | wxEXPAND | wxCENTER, FromDIP(10));

	m_DeleteBlanksBtn = new wxButton(this, wxID_ANY, "Delete Spaces", wxDefaultPosition, wxDefaultSize);
	m_DeleteBlanksBtn->SetMinSize(wxSize(FromDIP(120), wxDefaultSize.GetY()));
	m_DeleteBlanksBtn->SetOwnFont(MAIN_FONT_TEXT(10));
	m_DeleteBlanksBtn->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_DeleteBlanksBtn->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_DeleteBlanksBtn->SetToolTip("Delete all spaces from data");
	m_DeleteBlanksBtn->Hide();

	m_DeleteBlanksBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				m_CSVData->CreateUndo();
				m_CSVData->RemoveWhiteSpace(m_CSVData, (m_IncludeHeaderBtn->GetLabel() == "Exclude Header" ? true : false));
				m_CurrentSortColumn = -1;
				PopulateData();
			}
		}
	);

	m_OutputSettingsSizer2->Add(m_DeleteBlanksBtn, 1, wxRIGHT | wxLEFT | wxEXPAND | wxCENTER, FromDIP(10));

	m_RedoBtn = new wxButton(this, wxID_ANY, "Redo", wxDefaultPosition, wxDefaultSize);
	m_RedoBtn->SetMinSize(wxSize(FromDIP(120), wxDefaultSize.GetY()));
	m_RedoBtn->SetOwnFont(MAIN_FONT_TEXT(10));
	m_RedoBtn->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_RedoBtn->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_RedoBtn->SetToolTip("Redo the last change");
	m_RedoBtn->Hide();

	m_RedoBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				if (m_CSVData->Redo() == mrt::CSVData_UndoRedoState::CAN_REDO)
				{
					m_CurrentSortColumn = -1;
					PopulateData();
				}
			}
		}
	);

	m_OutputSettingsSizer2->Add(m_RedoBtn, 1, wxRIGHT | wxLEFT | wxEXPAND | wxCENTER, FromDIP(10));

	m_MainSizer->Add(m_OutputSettingsSizer1, 0, wxEXPAND | wxALL, FromDIP(0));
	m_MainSizer->Add(m_OutputSettingsSizer2, 0, wxEXPAND | wxALL, FromDIP(0));
}

void CSVto_PanelBase::SetupDataOutputSection()
{
	m_DataOutputSizer = new wxBoxSizer(wxHORIZONTAL);

	auto output_Text = new wxStaticText(this, wxID_ANY, "Preview Output: ", wxDefaultPosition, wxDefaultSize);
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

	m_ClearBtn = new wxButton(this, wxID_ANY, "Clear Data", wxDefaultPosition, wxDefaultSize);
	m_ClearBtn->SetMinSize(wxSize(FromDIP(120), wxDefaultSize.GetY()));
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

	m_DataOutputBtnSizer->Add(m_ClearBtn, 1, wxLEFT | wxBOTTOM | wxRIGHT | wxEXPAND | wxCENTER, FromDIP(10));

	m_CopyDataBtn = new wxButton(this, wxID_ANY, "Copy", wxDefaultPosition, wxDefaultSize);
	m_CopyDataBtn->SetMinSize(wxSize(FromDIP(120), wxDefaultSize.GetY()));
	m_CopyDataBtn->SetOwnFont(MAIN_FONT_TEXT(10));
	m_CopyDataBtn->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_CopyDataBtn->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_CopyDataBtn->SetToolTip("Copy the generated data to the clipboard");

	m_CopyDataBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				wxClipboard::Get()->SetData(new wxTextDataObject(m_OutputDataTextBox->GetValue()));
			}
		}
	);

	m_DataOutputBtnSizer->Add(m_CopyDataBtn, 1, wxLEFT | wxBOTTOM | wxRIGHT | wxEXPAND | wxCENTER, FromDIP(10));

	m_DownloadBtn = new wxButton(this, wxID_ANY, "Download", wxDefaultPosition, wxDefaultSize);
	m_DownloadBtn->SetMinSize(wxSize(FromDIP(120), wxDefaultSize.GetY()));
	m_DownloadBtn->SetOwnFont(MAIN_FONT_TEXT(10));
	m_DownloadBtn->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_DownloadBtn->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_DownloadBtn->SetToolTip("Download the generated data to a text file");

	m_DownloadBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				OutputFile();
			}
		}
	);

	m_DataOutputBtnSizer->Add(m_DownloadBtn, 1, wxLEFT | wxBOTTOM | wxRIGHT | wxEXPAND | wxCENTER, FromDIP(10));

	m_MainSizer->Add(m_DataOutputBtnSizer, 0, wxEXPAND | wxALL, FromDIP(0));
}

void CSVto_PanelBase::OutputFile()
{
	StrType fileDir = GetOutputFileDirectory("Text files (*.txt)|*.txt");

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

	{
		StrType headerName;									

		for (size_t i = 0; (i < m_CSVData->GetColumnCount()); ++i)
		{															// Adds an arrow to the header name if the column is the current sort column
			headerName = (m_CurrentSortColumn == i) ?
				std::format(L"{} {}", (m_CurrentSortOrder == ORDER_ASCENDING ? L"\U0001F881" : L"\U0001F883"), m_CSVData->GetHeaderNames()[i]) : m_CSVData->GetHeaderNames()[i];

			m_DataInputListView->InsertColumn(i + 1, headerName, wxLIST_ALIGN_SNAP_TO_GRID, average_width);
		}
	}

	for (size_t i0 = 0; (i0 < m_CSVData->GetRowCount()); ++i0)
	{
		m_DataInputListView->InsertItem(i0, std::to_string(i0 + 1));

		const std::vector<StrType> data = m_CSVData->GetRowData(i0);

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
	m_UndoBtn->Enable(!lock);
	m_RedoBtn->Enable(!lock);
	m_EditDataBtn->Enable(!lock);
}

void CSVto_PanelBase::HideOrShowBasicAdvItems(bool show)
{
	if (show)
	{
		m_CapitalizeBtn->Show();
		m_LowercaseBtn->Show();
		m_UppercaseBtn->Show();
		m_UndoBtn->Show();
		m_RedoBtn->Show();
		m_DeleteBlanksBtn->Show();
		m_TransposeBtn->Show();
		m_IncludeHeaderBtn->Show();
	}
	else
	{
		m_CapitalizeBtn->Hide();
		m_LowercaseBtn->Hide();
		m_UppercaseBtn->Hide();
		m_UndoBtn->Hide();
		m_RedoBtn->Hide();
		m_DeleteBlanksBtn->Hide();
		m_TransposeBtn->Hide();
		m_IncludeHeaderBtn->Hide();
	}

	Layout();
	Refresh();
}

bool CSVto_PanelBase::isThreadsRunning()
{
	std::lock_guard<std::mutex> lock(m_OutputDataMutex);

	return (m_ListViewThreadRunning || m_TextBoxThreadRunning) ? true : false;
}

CSVto_PanelBase::StrType CSVto_PanelBase::GetOutputFileDirectory(const std::string& wildcard)
{
	wxFileDialog saveFileDialog(this, "Download File", (wxStandardPaths::Get().GetDocumentsDir()), "", wildcard, wxFD_SAVE);

	if (saveFileDialog.ShowModal() == wxID_OK)
	{
		StrType path = saveFileDialog.GetPath().ToStdWstring();

		if (std::filesystem::exists(path))
		{
			mrt::MrT_UniDialog yesNoDialog(this, "Warning", "The file that you are trying to save to already exists. Would you like to overwrite it?",
				m_Colours, wxICON(wxICON_WARNING), mrt::MrT_UniDialogType_YES_NO, FromDIP(wxSize(400, 215)));

			if (yesNoDialog.ShowModal() == wxID_NO)
			{
				return StrType();
			}
		}

		return path;
	}

	return StrType();
}