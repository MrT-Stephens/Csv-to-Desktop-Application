#pragma once

#include "CSVto_PanelBase.h"

  /*******************************/
 /* CSVtoXLSX_Panel Declaration */
/*******************************/

class CSVtoXLSX_Panel : public CSVto_PanelBase
{
private:
	wxTimer m_DataSpacingTimer;
	wxBoxSizer* m_OutputSettingsSizer3;
	wxTextCtrl* m_SheetNameTextCtrl;
	wxCheckBox* m_IncludeHeaderCheckBox;
public:
	CSVtoXLSX_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours);

	virtual void PopulateOutputDataTextBox() override;
	virtual void SetupSpecificOutputSectionItems() override;
	virtual void LockOrUnlockItems(bool lock) override;
	virtual void OutputFile() override;
};