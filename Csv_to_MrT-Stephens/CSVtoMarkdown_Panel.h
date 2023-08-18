#pragma once

#include "CSVto_PanelBase.h"
#include "Ascii_Table_Generators.h"

  /***********************************/
 /* CSVtoMarkdown_Panel Declaration */
/***********************************/

class CSVtoMarkdown_Panel : public CSVto_PanelBase
{
private:
	wxComboBox* m_TextAlignment, * m_TableStyle;
	wxCheckBox* m_BoldFirstRow, * m_BoldFirstCol;
	wxBoxSizer* m_LeftSettingsSizer, * m_RightSettingsSizer, * m_OutputSettingsSizer3;

	wxArrayString m_TextAlignmentChoices, m_TableStyleChoices;
public:
	CSVtoMarkdown_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours);

	virtual void PopulateOutputDataTextBox() override;
	virtual void SetupSpecificOutputSectionItems() override;
	virtual void LockOrUnlockItems(bool lock) override;
	virtual void OutputFile() override;
};