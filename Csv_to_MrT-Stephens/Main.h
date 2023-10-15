#pragma once

#include <wx/wx.h>
#include <wx/notebook.h>

#include "UtilityItems.h"
#include "CSVtoCSV_Panel.h"
#include "CSVtoSQL_Panel.h"
#include "CSVtoASCII_Panel.h"
#include "CSVtoMarkdown_Panel.h"
#include "CSVtoXML_Panel.h"
#include "CSVtoHTML_Panel.h"
#include "CSVtoLatex_Panel.h"
#include "CSVtoJSON_Panel.h"
#include "CSVtoPHP_Panel.h"
#include "CSVtoRuby_Panel.h"
#include "CSVtoMultiline_Panel.h"
#include "CSVtoYAML_Panel.h"
#include "CSVtoBBCode_Panel.h"
#include "CSVtoXLSX_Panel.h"

  /***********************/
 /* MrT_App Declaration */
/***********************/

class MrT_App : public wxApp
{
public:
    virtual bool OnInit() override;
};

  /**************************/
 /* Main_Frame Declaration */
/**************************/

class Main_Frame : public wxFrame
{
private:
    wxNotebook* m_Notebook;
    std::vector<std::pair<CSVto_PanelBase*, std::string>> m_Panels;

    mrtApp::AppColours m_Colours = wxSystemSettings::GetAppearance().IsDark() ? mrtApp::DARKMODE_DEFAULT_COLOUR : mrtApp::LIGHTMODE_DEFAULT_COLOUR;
public:
    // Constructor & Destructor
    Main_Frame(const wxString& title, const wxPoint& pos, const wxSize& size, long style);
    ~Main_Frame();

    // Member Functions
    void OnClose(wxCloseEvent& event);
};