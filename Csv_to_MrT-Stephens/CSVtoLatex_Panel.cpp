#include "CSVtoLatex_Panel.h"

CSVtoLatex_Panel::CSVtoLatex_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours)
	: CSVto_PanelBase(_parent, _name, _colours)
{
	SetupDataInputSection();
	SetupOutputSettingsSection();
	SetupSpecificOutputSectionItems();
	SetupDataOutputSection();

	SetSizer(m_MainSizer);
}

void CSVtoLatex_Panel::PopulateOutputDataTextBox()
{

}

void CSVtoLatex_Panel::SetupSpecificOutputSectionItems()
{

}

void CSVtoLatex_Panel::LockOrUnlockItems(bool lock)
{

}