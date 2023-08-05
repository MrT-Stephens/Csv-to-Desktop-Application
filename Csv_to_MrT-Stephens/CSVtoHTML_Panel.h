#pragma once

#include "HTML_Table.h"
#include "CSVto_PanelBase.h"

  /*******************************/
 /* CSVtoHTML_Panel Declaration */
/*******************************/

class CSVtoHTML_Panel : public CSVto_PanelBase
{
private:
	wxBoxSizer* m_OutputSettingsSizer3;
	wxButton* m_TheadTbodyHtmlBtn;
	wxCheckBox* m_MinifyHtmlCheckBox;
public:
	CSVtoHTML_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours);

	virtual void PopulateOutputDataTextBox() override;
	virtual void SetupSpecificOutputSectionItems() override;
	virtual void LockOrUnlockItems(bool lock) override;
};