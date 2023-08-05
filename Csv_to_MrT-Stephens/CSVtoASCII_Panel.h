#pragma once

// MrT Includes
#include "CSVto_PanelBase.h"
#include "Ascii_Table_Generators.h"

  /********************************/
 /* CSVtoASCII_Panel Declaration */
/********************************/

class CSVtoASCII_Panel : public CSVto_PanelBase
{
private:
	// CsvToAscii visual elements
	wxCheckBox* m_ForceSeparators;
	wxComboBox* m_AsciiTableStyle, * m_CommentType, * m_TextAlignment;
	wxBoxSizer* m_LeftSettingsSizer, * m_RightSettingsSizer, * m_OutputSettingsSizer3;

	// Choices for the combo boxes
	wxArrayString m_AsciiTableStyleChoices, m_CommentTypeChoices, m_TextAlignmentChoices;
public:
	CSVtoASCII_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours);

	virtual void PopulateOutputDataTextBox() override;
	virtual void SetupSpecificOutputSectionItems() override;
	virtual void LockOrUnlockItems(bool lock) override;

	std::string GetCommentCharacters();
};