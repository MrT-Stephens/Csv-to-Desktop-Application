#pragma once

#include "PHP_Generator.h"
#include "CSVto_PanelBase.h"

  /******************************/
 /* CSVtoPHP_Panel Declaration */
/******************************/

class CSVtoPHP_Panel : public CSVto_PanelBase
{
public:
	CSVtoPHP_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours);

	virtual void PopulateOutputDataTextBox() override;
	virtual void SetupSpecificOutputSectionItems() override;
	virtual void LockOrUnlockItems(bool lock) override;
	virtual void OutputFile() override;
};