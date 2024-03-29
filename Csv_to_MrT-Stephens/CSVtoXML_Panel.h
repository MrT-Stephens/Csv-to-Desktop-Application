#pragma once

#include "XML.h"
#include "CSVto_PanelBase.h"

  /******************************/
 /* CSVtoXML_Panel Declaration */
/******************************/

class CSVtoXML_Panel : public CSVto_PanelBase
{
private:
	wxTimer m_TextBoxTimer;
	wxBoxSizer* m_RightSizer, * m_MiddleSizer, * m_LeftSizer, * m_OutputSettingsSizer3;
	wxTextCtrl* m_RootNameTextBox, * m_ElementNameTextBox, * m_NameSpaceTextBox, * m_XmlnsTextBox;
	wxCheckBox* m_ExcludePrologCheckBox, * m_MinifyXmlCheckBox;
public:
	CSVtoXML_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours);

	virtual void PopulateOutputDataTextBox() override;
	virtual void SetupSpecificOutputSectionItems() override;
	virtual void LockOrUnlockItems(bool lock) override;
	virtual void OutputFile() override;

	StrType GetNamespaceStr(const StrType& nameSpace, const StrType& data);
};