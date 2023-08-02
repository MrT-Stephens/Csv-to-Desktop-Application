#pragma once

#include "CSVto_PanelBase.h"

class CSVtoLatex_Panel : public CSVto_PanelBase
{
private:

public:
	CSVtoLatex_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours);

	virtual void PopulateOutputDataTextBox() override;
	virtual void SetupSpecificOutputSectionItems() override;
	virtual void LockOrUnlockItems(bool lock) override;
};