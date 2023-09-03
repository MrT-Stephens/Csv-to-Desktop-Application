#pragma once

#include "CSVto_PanelBase.h"

  /************************************/
 /* CSVtoMultiline_Panel Declaration */
/************************************/

class CSVtoMultiline_Panel : public CSVto_PanelBase
{
private:
	wxTimer m_DataSpacingTimer;
	wxBoxSizer* m_OutputSettingsSizer3;
	wxTextCtrl* m_DataSpacingTextBox;
	wxCheckBox* m_IncludeHeaderCheckBox;
public:
	CSVtoMultiline_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours);

	virtual void PopulateOutputDataTextBox() override;
	virtual void SetupSpecificOutputSectionItems() override;
	virtual void LockOrUnlockItems(bool lock) override;
};