#include "CSVtoSQL_Panel.h"

  /*********************************/
 /* CSVtoSQL_Panel Implementation */
/*********************************/

CSVtoSQL_Panel::CSVtoSQL_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours) 
	: CSVto_PanelBase(_parent, _name, _colours)
{
	SetupDataInputSection();
	SetupOutputSettingsSection();
	SetupSpecificOutputSectionItems();
	SetupDataOutputSection();

	SetSizer(m_MainSizer);
}

void CSVtoSQL_Panel::SetupSpecificOutputSectionItems()
{
	m_OutputSettingsSizer3 = new wxBoxSizer(wxHORIZONTAL);

	m_GenerateTable = new wxCheckBox(this, wxID_ANY, "Generate create table code", wxDefaultPosition, wxDefaultSize);
	m_GenerateTable->SetMinSize(FromDIP(wxSize(120, 22)));
	m_GenerateTable->SetOwnFont(MAIN_FONT_TEXT(11));
	m_GenerateTable->SetOwnBackgroundColour(m_Colours->BACKGROUND);
	m_GenerateTable->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_GenerateTable->SetToolTip("Generate the create table statement");

	m_GenerateTable->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				PopulateData();
			}
		}
	);

	m_InsertMultiRows = new wxCheckBox(this, wxID_ANY, "Insert multiple rows", wxDefaultPosition, wxDefaultSize);
	m_InsertMultiRows->SetMinSize(FromDIP(wxSize(120, 22)));
	m_InsertMultiRows->SetOwnFont(MAIN_FONT_TEXT(11));
	m_InsertMultiRows->SetOwnBackgroundColour(m_Colours->BACKGROUND);
	m_InsertMultiRows->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_InsertMultiRows->SetToolTip("Insert multiple rows at once");

	m_InsertMultiRows->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				PopulateData();
			}
		}
	);

	m_CheckBoxSizer = new wxBoxSizer(wxVERTICAL);
	m_CheckBoxSizer->Add(m_GenerateTable, 0, wxRIGHT | wxLEFT | wxTOP | wxEXPAND | wxCENTER, FromDIP(10));
	m_CheckBoxSizer->Add(m_InsertMultiRows, 0, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_TableNameInput = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
	m_TableNameInput->SetHint("Table Name");
	m_TableNameInput->SetMinSize(FromDIP(wxSize(120, 22)));
	m_TableNameInput->SetOwnFont(MAIN_FONT_TEXT(11));
	m_TableNameInput->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_TableNameInput->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_TableNameInput->SetToolTip("Set a table name for the data");

	m_TableNameInput->Bind(wxEVT_TEXT, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				m_PopulateDataTimer.Start(2000);
			}
		}
	);

	m_PopulateDataTimer.Bind(wxEVT_TIMER, [this](wxTimerEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				m_PopulateDataTimer.Stop();
				PopulateData();
			}
		}
	);

	m_QuotesChoices.Add("No Quotes");
	m_QuotesChoices.Add("Double Quotes");
	m_QuotesChoices.Add("MySQL Quotes");
	m_QuotesChoices.Add("SQLite / SQL Server quotes");

	m_QuoteTypeSelect = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, m_QuotesChoices, wxCB_READONLY);
	m_QuoteTypeSelect->SetSelection(0);
	m_QuoteTypeSelect->SetMinSize(FromDIP(wxSize(120, 22)));
	m_QuoteTypeSelect->SetOwnFont(MAIN_FONT_TEXT(11));
	m_QuoteTypeSelect->SetOwnBackgroundColour(m_Colours->PRIMARY);
	m_QuoteTypeSelect->SetOwnForegroundColour(m_Colours->FOREGROUND);
	m_QuoteTypeSelect->SetToolTip("Select a quote type for the generated data");

	m_QuoteTypeSelect->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent& event)
		{
			if (m_CSVData != nullptr)
			{
				PopulateData();
			}
		}
	);

	m_QuoteBoxSizer = new wxBoxSizer(wxVERTICAL);
	m_QuoteBoxSizer->Add(m_TableNameInput, 0, wxRIGHT | wxLEFT | wxTOP | wxEXPAND | wxCENTER, FromDIP(10));
	m_QuoteBoxSizer->Add(m_QuoteTypeSelect, 0, wxALL | wxEXPAND | wxCENTER, FromDIP(10));

	m_OutputSettingsSizer3->Add(m_CheckBoxSizer, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(0));
	m_OutputSettingsSizer3->Add(m_QuoteBoxSizer, 1, wxALL | wxEXPAND | wxCENTER, FromDIP(0));

	m_MainSizer->Add(m_OutputSettingsSizer3, 0, wxEXPAND | wxALL, FromDIP(0));
}

void CSVtoSQL_Panel::PopulateOutputDataTextBox()
{
#if defined(MRT_DEBUG)
	auto start = std::chrono::high_resolution_clock::now();
#endif

	std::vector<std::wstring> headerNames{m_CSVData->GetHeaderNames()};		// Get the header names from the CSV data readey for the code generation.

	// Create table code generation.
	if (m_GenerateTable->GetValue())										// If the generate table checkbox is checked.
	{																		// Will generate the create table statement.

		std::wstringstream createTableCodeSS;

		createTableCodeSS << "CREATE TABLE " << GenerateQuoteString(m_TableNameInput->GetValue().ToStdWstring()) << " (\n";

		for (size_t i = 0; i < m_CSVData->GetColumnCount(); ++i)			// Loop through the header names and generate the columns. E.g. column1 VARCHAR(256) NOT NULL,
		{
			createTableCodeSS << "\t" << GenerateQuoteString(headerNames[i]) << " VARCHAR(" << mrt::RoundToNearest10<size_t>(m_CSVData->GetMaxColumnWidth(i)) << ") NOT NULL" << ((i == m_CSVData->GetColumnCount() - 1) ? "\n);\n\n" : ",\n");
		}

		m_OutputDataTextBox->AppendText(std::forward<std::wstring>(createTableCodeSS.str()));
	}

	{
		bool firstLoopCheck1 = true, firstLoopCheck2 = true;				// Used to check if we are on the first loop of the code generation.
		std::wstringstream insertIntoCodeSS;

		// Insert into table code generation.
		for (size_t i0 = 0; i0 < m_CSVData->GetRowCount(); ++i0)
		{
			std::wstringstream rowDataConcatenation;

			rowDataConcatenation << '(';

			if (firstLoopCheck1 || !m_InsertMultiRows->GetValue())			// Generate the first part of the code. E.g. INSERT INTO table_name (column1, column2, column3, ...)
			{																// If the user wants to insert multiple rows at once, then we don't need to generate this part of the code every time.
				firstLoopCheck1 = false;
				insertIntoCodeSS << "INSERT INTO " << GenerateQuoteString(m_TableNameInput->GetValue().ToStdWstring()) << " (";

				for (size_t i = 0; i < m_CSVData->GetColumnCount(); ++i)
				{
					insertIntoCodeSS << GenerateQuoteString(headerNames[i]) << ((i == m_CSVData->GetColumnCount() - 1) ? ")" : ", ");
				}
			}

			std::vector<std::wstring> rowData{m_CSVData->GetRowData(i0)};	// Get the row data ready for concatenation.

			for (size_t i = 0; i < m_CSVData->GetColumnCount(); ++i)		// Generate the second part of the code. E.g. VALUES ('value1', 'value2', 'value3', ...)
			{
				rowDataConcatenation << '\'' << rowData[i] << '\'' << ((i == m_CSVData->GetColumnCount() - 1) ? ")" : ", ");
			}

			if (firstLoopCheck2 || !m_InsertMultiRows->GetValue())			// If the user wants to insert multiple rows at once, then we don't need to generate this part of the code every time.
			{
				firstLoopCheck2 = false;
				insertIntoCodeSS << " VALUES " << rowDataConcatenation.str() << ((m_InsertMultiRows->GetValue()) ? ",\n\t" : ";\n");
			}
			else
			{
				insertIntoCodeSS << rowDataConcatenation.str() << ((i0 == m_CSVData->GetRowCount() - 1) ? ";\n" : ",\n\t");
			}
		}

		m_OutputDataTextBox->AppendText(std::forward<std::wstring>(insertIntoCodeSS.str()));
	}

#if defined(MRT_DEBUG)
	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	MRT_DEBUG_LOG_TIME("Time taken by 'PopulateOutputDataTextBox' in SQL class", duration.count(), "milliseconds");
#endif

	std::lock_guard<std::mutex> lock(m_OutputDataMutex);

	m_TextBoxThreadRunning = false;

	if (!m_ListViewThreadRunning)
	{
		LockOrUnlockItems(false);
	}
}

std::wstring CSVtoSQL_Panel::GenerateQuoteString(const std::wstring& innerString)
{
	std::wstringstream outString;

	switch (m_QuoteTypeSelect->GetSelection())
	{
	case 0:
		outString << innerString;
		break;
	case 1:
		outString << "\"" << innerString << "\"";
		break;
	case 2:
		outString << "`" << innerString << "`";
		break;
	case 3:
		outString << "[" << innerString << "]";
		break;
	}

	return std::forward<std::wstring>(outString.str());
}

void CSVtoSQL_Panel::LockOrUnlockItems(bool lock)
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
	m_GenerateTable->Enable(!lock);
	m_InsertMultiRows->Enable(!lock);
	m_TableNameInput->Enable(!lock);
	m_QuoteTypeSelect->Enable(!lock);
}