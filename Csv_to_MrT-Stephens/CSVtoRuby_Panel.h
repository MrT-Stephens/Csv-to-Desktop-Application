#pragma once

#include "Ruby_Generator.h"
#include "CSVto_PanelBase.h"

  /*******************************/
 /* CSVtoRuby_Panel Declaration */
/*******************************/

class CSVtoRuby_Panel : public CSVto_PanelBase
{
public:
	CSVtoRuby_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours);

	virtual void PopulateOutputDataTextBox() override;
	virtual void SetupSpecificOutputSectionItems() override;
	virtual void LockOrUnlockItems(bool lock) override;
	virtual void OutputFile() override;
};