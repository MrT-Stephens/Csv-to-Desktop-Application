#pragma once

#include "YAML_Generator.h"
#include "CSVto_PanelBase.h"

  /*******************************/
 /* CSVtoYAML_Panel Declaration */
/*******************************/

enum Quotes_Types
{
	NoQuotes,
	DoubleQuotes,
	SingleQuotes
};

class CSVtoYAML_Panel : public CSVto_PanelBase
{
private:
	wxArrayString m_QuotesTypes;
	wxBoxSizer* m_OutputSettingsSizer3;
	wxCheckBox* m_ExplictTypeCheckBox;
	wxComboBox* m_QuotesTypeComboBox;
public:
	CSVtoYAML_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours);

	virtual void PopulateOutputDataTextBox() override;
	virtual void SetupSpecificOutputSectionItems() override;
	virtual void LockOrUnlockItems(bool lock) override;
	virtual void OutputFile() override;

	StrType GetQuoteString(int selection);
};