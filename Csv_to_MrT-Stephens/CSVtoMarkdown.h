#pragma once

#include "TableGenerators.h"
#include "CSVto_PanelBase.h"

class CSVtoMarkdown_Panel : public CSVto_PanelBase
{
private:
	wxComboBox* m_TextAlignment, * m_TableStyle;
	wxCheckBox* m_BoldFirstRow, m_BoldFirstCol;
	wxBoxSizer* m_LeftSettingsSizer, * m_MiddleSettingSizer, * m_RightSettingsSizer, * m_OutputSettingsSizer3;

	wxArrayString* m_TextAlignmentChoices;
public:
	CSVtoMarkdown_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours);

	virtual void PopulateOutputDataTextBox() override;
	virtual void SetupSpecificOutputSectionItems() override;
	virtual void LockOrUnlockItems(bool lock) override;
};