#pragma once

#include "LaTex_Table.h"
#include "CSVto_PanelBase.h"

  /********************************/
 /* CSVtoLaTex_Panel Decleration */
/********************************/

class CSVtoLaTex_Panel : public CSVto_PanelBase
{
private:
	wxTimer m_Timer;
	wxArrayString m_AlignmentOptions, m_BorderOptions, m_CaptionLocationOptions;

	wxTextCtrl* m_TableCaptionTextBox, * m_LabelTextBox;
	wxCheckBox* m_HereOrTopCheckBox, * m_MinimalWorkingCheckBox;
	wxBoxSizer* m_OutputSettingsSizer3, * m_LeftSizer, * m_MiddleSizer1, * m_MiddleSizer2, * m_RightSizer;
	wxComboBox* m_TableBorderComboBox, * m_TableAlignmentComboBox, * m_TextAligmentComboBox, * m_CaptionLocationComboBox;
public:
	CSVtoLaTex_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours);

	virtual void PopulateOutputDataTextBox() override;
	virtual void SetupSpecificOutputSectionItems() override;
	virtual void LockOrUnlockItems(bool lock) override;
};