#include "CSVtoHTML_Panel.h"

CSVtoHTML_Panel::CSVtoHTML_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours)
	: CSVto_PanelBase(_parent, _name, _colours)
{
	SetupDataInputSection();
	SetupOutputSettingsSection();
	SetupSpecificOutputSectionItems();
	SetupDataOutputSection();

	SetSizer(m_MainSizer);
}

void CSVtoHTML_Panel::PopulateOutputDataTextBox()
{

}

void CSVtoHTML_Panel::SetupSpecificOutputSectionItems()
{

}

void CSVtoHTML_Panel::LockOrUnlockItems(bool lock)
{

}