#pragma once

#include "CSVto_PanelBase.h"

class CSVtoSQL_Panel : public CSVto_PanelBase
{
private:
	// CsvToSql misculaneous items
	wxTimer m_PopulateDataTimer;
	wxArrayString m_QuotesChoices;

	// CsvToSql visual elements
	wxTextCtrl* m_TableNameInput;
	wxComboBox* m_QuoteTypeSelect;
	wxCheckBox* m_GenerateTable, * m_InsertMultiRows;
	wxBoxSizer* m_CheckBoxSizer, * m_QuoteBoxSizer, * m_OutputSettingsSizer3;
public:
	CSVtoSQL_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours);

	virtual void PopulateOutputDataTextBox() override;
	virtual void SetupSpecificOutputSectionItems() override;
	virtual void LockOrUnlockItems(bool lock) override;

	std::string GenerateQuoteString(const std::string& innerString);
};