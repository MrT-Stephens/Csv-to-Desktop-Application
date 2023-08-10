#pragma once

#include "JSON_Generator.h"
#include "CSVto_PanelBase.h"

  /*******************************/
 /* CSVtoJSON_Panel Declaration */
/*******************************/

class CSVtoJSON_Panel : public CSVto_PanelBase
{
private:
	wxArrayString m_JSONStylesChoices;
	wxBoxSizer* m_OutputSettingsSizer3, * m_LeftSizer, * m_RightSizer;
	wxComboBox* m_JSONStylesComboBox;
	wxButton* m_WrapperByDataBtn;
	wxCheckBox* m_MinifyJSONCheckBox, * m_ParsingJSONCheckBox;
public:
	CSVtoJSON_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours);

	virtual void PopulateOutputDataTextBox() override;
	virtual void SetupSpecificOutputSectionItems() override;
	virtual void LockOrUnlockItems(bool lock) override;
	virtual void OutputFile() override;
};