#pragma once

#include "xlsxwriter.h"
#include "UTF8_Encoder.h"
#include "CSVto_PanelBase.h"

  /*******************************/
 /* CSVtoXLSX_Panel Declaration */
/*******************************/

class CSVtoXLSX_Panel : public CSVto_PanelBase
{
private:
	wxArrayString m_TextColours;
	wxTimer m_DataSpacingTimer;
	wxBoxSizer* m_OutputSettingsSizer3;
	wxTextCtrl* m_SheetNameTextCtrl;
	wxCheckBox* m_BoldHeaderCheckBox;
	wxComboBox* m_TextColourComboBox;

	// Items for the XLSX workbook
	StrType m_WorkbookDir;
	std::string m_WorkbookTempDir;
	lxw_workbook* m_Workbook = nullptr;
public:
	CSVtoXLSX_Panel(wxWindow* _parent, const std::string& _name, const mrtApp::AppColours* _colours);

	virtual void PopulateOutputDataTextBox() override;
	virtual void SetupSpecificOutputSectionItems() override;
	virtual void LockOrUnlockItems(bool lock) override;
	virtual void OutputFile() override;

	lxw_color_t GetLXWcolour(int _colourIndex);

	~CSVtoXLSX_Panel();
};