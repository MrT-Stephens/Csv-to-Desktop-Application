#pragma once

#include <wx/wx.h>
#include <wx/notebook.h>

#include "GlobalDefualts.h"
#include "CSVtoSQL_Panel.h"
#include "CSVtoASCII_Panel.h"
#include "CSVtoMarkdown.h"

class MrT_App : public wxApp
{
public:
    virtual bool OnInit() override;
};

class Main_Frame : public wxFrame
{
private:
    wxNotebook* m_Notebook;

    const mrtApp::AppColours m_Colours;

    std::vector<std::pair<CSVto_PanelBase*, std::string>> m_Panels;
public:
    Main_Frame(const wxString& title, const wxPoint& pos, const wxSize& size, long style);
    ~Main_Frame();
};