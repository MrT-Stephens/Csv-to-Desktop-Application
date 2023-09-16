#include "CSVtoXLSX_Panel.h"

  /**********************************/
 /* CSVtoXLSX_Panel Implementation */
/**********************************/

CSVtoXLSX_Panel::CSVtoXLSX_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours)
	: CSVto_PanelBase(_parent, _name, _colours)
{
	SetupDataInputSection();
	SetupOutputSettingsSection();
	SetupSpecificOutputSectionItems();
	SetupDataOutputSection();

	SetSizer(m_MainSizer);
}

void CSVtoXLSX_Panel::SetupSpecificOutputSectionItems()
{
	m_OutputSettingsSizer3 = new wxBoxSizer(wxHORIZONTAL);

	m_BoldHeaderCheckBox = new wxCheckBox(this, wxID_ANY, "Bold Header", wxDefaultPosition, wxDefaultSize);
	m_BoldHeaderCheckBox->SetMinSize(FromDIP(wxSize(120, 22)));
	m_BoldHeaderCheckBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_BoldHeaderCheckBox->SetOwnBackgroundColour(m_Colours->BACKGROUND);
	m_BoldHeaderCheckBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_BoldHeaderCheckBox->SetToolTip("Bold header in the workbook");

	m_BoldHeaderCheckBox->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				PopulateData();
			}
		}
	);

	m_TextColours.Add("Black");
	m_TextColours.Add("Blue");
	m_TextColours.Add("Brown");
	m_TextColours.Add("Cyan");
	m_TextColours.Add("Gray");
	m_TextColours.Add("Green");
	m_TextColours.Add("Lime");
	m_TextColours.Add("Magenta");
	m_TextColours.Add("Navy");
	m_TextColours.Add("Orange");
	m_TextColours.Add("Pink");
	m_TextColours.Add("Purple");
	m_TextColours.Add("Red");
	m_TextColours.Add("Silver");
	m_TextColours.Add("White");
	m_TextColours.Add("Yellow");

	m_TextColourComboBox = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, m_TextColours, wxCB_READONLY);
	m_TextColourComboBox->SetSelection(0);
	m_TextColourComboBox->SetMinSize(FromDIP(wxSize(120, 22)));
	m_TextColourComboBox->SetOwnFont(MAIN_FONT_TEXT(11));
	m_TextColourComboBox->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_TextColourComboBox->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_TextColourComboBox->SetToolTip("Select a text colour for the text within the workbook");

	m_TextColourComboBox->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				PopulateData();
			}
		}
	);

	m_SheetNameTextCtrl = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
	m_SheetNameTextCtrl->SetHint("Worksheet Name");
	m_SheetNameTextCtrl->SetMinSize(FromDIP(wxSize(120, 22)));
	m_SheetNameTextCtrl->SetOwnFont(MAIN_FONT_TEXT(11));
	m_SheetNameTextCtrl->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_SheetNameTextCtrl->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_SheetNameTextCtrl->SetToolTip("Set a name for the worksheet");

	m_SheetNameTextCtrl->Bind(wxEVT_TEXT, [this](wxCommandEvent& event)
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

	m_OutputSettingsSizer3->Add(m_BoldHeaderCheckBox, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(10));
	m_OutputSettingsSizer3->Add(m_TextColourComboBox, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(10));
	m_OutputSettingsSizer3->Add(m_SheetNameTextCtrl, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_MainSizer->Add(m_OutputSettingsSizer3, 0, wxEXPAND | wxALL, FromDIP(0));
}

void CSVtoXLSX_Panel::PopulateOutputDataTextBox()
{
#if defined(MRT_DEBUG)
	auto start = std::chrono::high_resolution_clock::now();
#endif

	{
		// XLSXWriter must be created with a file path. So we must:
		// 
		// 1. Create a temporary file path.
		// 2. Create the workbook with the temporary file path.
		// 3. Insert all data into the workbook.
		// 
		// If the user wants to save the file we must:
		// 
		// 1. Close the workbook at the temporary file path.
		// 2. Copy the temporary file to the user's chosen file path.
		// 3. Delete the temporary file.

		// Handle the cases where the workbook has already been created or not created.
		if (m_WorkbookTempDir.empty() || m_Workbook == nullptr)
		{
			m_WorkbookTempDir = std::format("{}\\csvToXlsxTemp.xlsx", std::filesystem::current_path().string());
		}
		else
		{
			workbook_close(m_Workbook);
			std::filesystem::remove(m_WorkbookTempDir);
		}

		// Create the workbook and worksheet.
		m_Workbook = workbook_new(m_WorkbookTempDir.c_str());

		lxw_worksheet* worksheet = workbook_add_worksheet(m_Workbook, ((m_SheetNameTextCtrl->GetValue().ToStdString().empty()) ? NULL : m_SheetNameTextCtrl->GetValue().ToStdString().c_str()));

		// Create the formats for the header and rows.
		lxw_format* headerFormat = workbook_add_format(m_Workbook), * rowFormat = workbook_add_format(m_Workbook);

		if (m_BoldHeaderCheckBox->GetValue())
		{
			format_set_bold(headerFormat);
		}

		format_set_font_color(headerFormat, Get_LXW_Colour(m_TextColourComboBox->GetSelection()));
		format_set_font_color(rowFormat, Get_LXW_Colour(m_TextColourComboBox->GetSelection()));

		// Create the validator for chosing whether to write a number or string.
		mrt::Basic_Str_Validator<StrType> validator(mrt::Basic_Str_Filter_Digits | mrt::Basic_Str_Filter_Empty);

		{	// Write the header names to the worksheet.
			const std::vector<StrType>& headerData = m_CSVData->GetHeaderNames();

			for (size_t col = 0; col < headerData.size(); ++col)
			{
				if (headerData[col].empty())
				{
					worksheet_write_blank(worksheet, 0, col, headerFormat);
				}
				else if (validator.IsValid(headerData[col]))
				{
					worksheet_write_number(worksheet, 0, col, std::stod(headerData[col]), headerFormat);
				}
				else
				{
					worksheet_write_string(worksheet, 0, col, UTF8_Encoder::From_wstring(headerData[col]).c_str(), headerFormat);
				}
			}
		}

		// Write the row data to the worksheet.
		for (size_t row = 0; row < m_CSVData->GetRowCount(); ++row)
		{
			const std::vector<StrType>& rowData = m_CSVData->GetRowData(row);

			for (size_t col = 0; col < rowData.size(); ++col)
			{
				if (rowData[col].empty())
				{
					worksheet_write_blank(worksheet, row + 1, col, rowFormat);
				}
				else if (validator.IsValid(rowData[col]))
				{
					worksheet_write_number(worksheet, row + 1, col, std::stod(rowData[col]), rowFormat);
				}
				else
				{
					worksheet_write_string(worksheet, row + 1, col, UTF8_Encoder::From_wstring(rowData[col]).c_str(), rowFormat);
				}
			}
		}

		m_OutputDataTextBox->SetValue(L"Please download the file to view the generated 'XLSX' workbook \U0001F642");
	}

#if defined(MRT_DEBUG)
	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	MRT_DEBUG_LOG_TIME("Time taken by 'PopulateOutputDataTextBox' in XLSX class", duration.count(), "milliseconds");
#endif

	std::lock_guard<std::mutex> lock(m_OutputDataMutex);

	m_TextBoxThreadRunning = false;

	if (!m_ListViewThreadRunning)
	{
		LockOrUnlockItems(false);
	}
}

void CSVtoXLSX_Panel::OutputFile()
{
	m_WorkbookDir = GetOutputFileDirectory("XLSX (*.xlsx)|*.xlsx");

	if (m_WorkbookDir != StrType())
	{
		lxw_error error = workbook_close(m_Workbook);
		m_Workbook = nullptr;

		std::filesystem::copy_file(m_WorkbookTempDir, m_WorkbookDir, std::filesystem::copy_options::overwrite_existing);
		std::filesystem::remove(m_WorkbookTempDir);

		if (error != lxw_error::LXW_NO_ERROR)
		{
			mrt::MrT_UniDialog errorDialog(this, "Error", std::format("Failed to save file!\nPlease try to re-save the file.\nError: {}.", lxw_strerror(error)),
				m_Colours, wxICON(wxICON_ERROR), mrt::MrT_UniDialogType_OK, FromDIP(wxSize(400, 200)));

			errorDialog.ShowModal();
		}
	}
}

void CSVtoXLSX_Panel::LockOrUnlockItems(bool lock)
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
	m_BoldHeaderCheckBox->Enable(!lock);
	m_SheetNameTextCtrl->Enable(!lock);
}

lxw_color_t CSVtoXLSX_Panel::Get_LXW_Colour(int colourIndex)
{
	switch (colourIndex)
	{
	case 0:
		return LXW_COLOR_BLACK;
	case 1:
		return LXW_COLOR_BLUE;
	case 2:
		return LXW_COLOR_BROWN;
	case 3:
		return LXW_COLOR_CYAN;
	case 4:
		return LXW_COLOR_GRAY;
	case 5:
		return LXW_COLOR_GREEN;
	case 6:
		return LXW_COLOR_LIME;
	case 7:
		return LXW_COLOR_MAGENTA;
	case 8:
		return LXW_COLOR_NAVY;
	case 9:
		return LXW_COLOR_ORANGE;
	case 10:
		return LXW_COLOR_PINK;
	case 11:
		return LXW_COLOR_PURPLE;
	case 12:
		return LXW_COLOR_RED;
	case 13:
		return LXW_COLOR_SILVER;
	case 14:
		return LXW_COLOR_WHITE;
	case 15:
		return LXW_COLOR_YELLOW;
	default:
		return LXW_COLOR_BLACK;
	}
}

CSVtoXLSX_Panel::~CSVtoXLSX_Panel()
{
	if (!m_WorkbookTempDir.empty() && m_Workbook != nullptr)
	{
		workbook_close(m_Workbook);
		std::filesystem::remove(m_WorkbookTempDir);
	}
}