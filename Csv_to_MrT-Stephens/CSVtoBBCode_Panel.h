#pragma once

#include "CSVto_PanelBase.h"
#include "BBCode_Generator.h"

  /*********************************/
 /* CSVtoBBCode_Panel Declaration */
/*********************************/

class CSVtoBBCode_Panel : public CSVto_PanelBase
{
private:
	wxBoxSizer* m_OutputSettingsSizer3;
	wxCheckBox* m_MinifyBBCodeCheckBox;
public:
	CSVtoBBCode_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours);

	virtual void PopulateOutputDataTextBox() override;
	virtual void SetupSpecificOutputSectionItems() override;
	virtual void LockOrUnlockItems(bool lock) override;
	virtual void OutputFile() override;
};