#pragma once

#include "CSVto_PanelBase.h"

  /******************************/
 /* CSVtoCSV_Panel Declaration */
/******************************/

enum Delimiter_Types
{
	CSVData_COMMA = 0,
	CSVData_SEMICOLON = 1,
	CSVData_COLON = 2,
	CSVData_TAB = 3,
	CSVData_PIPE = 4,
	CSVData_SLASH = 5,
	CSVData_HASH = 6
};

class CSVtoCSV_Panel : public CSVto_PanelBase
{
private:
	wxArrayString m_DelimiterChoices;
	wxBoxSizer* m_OutputSettingsSizer3;
	wxComboBox* m_DelimiterComboBox;
	wxButton* m_IncludeHeaderButton, * m_QuotesButton;
public:
	CSVtoCSV_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours);

	virtual void PopulateOutputDataTextBox() override;
	virtual void SetupSpecificOutputSectionItems() override;
	virtual void LockOrUnlockItems(bool lock) override;
	virtual void OutputFile() override;

	ValueType GetDelimiterType(int selection);
};