#include "Main.h"

#if defined(MRT_DEBUG)
#include <Windows.h>   // For debugging
#endif

// Global variables for the application
const std::string mrtApp::APP_NAME = "Csv to...";
const std::string mrtApp::APP_VERSION = "v0.4.0";
const std::string mrtApp::APP_TITLE = mrtApp::APP_NAME + " " + mrtApp::APP_VERSION;
const std::string mrtApp::GITHUB_URL = "https://github.com/MrT-Stephens";

  /**************************/
 /* MrT_App Implementation */
/**************************/

bool MrT_App::OnInit()
{
#if defined(MRT_DEBUG)
    AllocConsole();													    // Allocating a console for debugging purposes.

    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);

    try
    {
        std::locale::global(std::locale("en_GB.UTF-8"));
    }
    catch (const std::runtime_error& e)
    {
        MRT_DEBUG_LOG_MSG(std::format("Failed to set the locale: ", e.what()));
    }

    MRT_DEBUG_LOG_MSG(std::format("Debugging enabled on application ({})", mrtApp::APP_TITLE));
    MRT_DEBUG_LOG_MSG(std::format("Locale set to: {}", std::locale().name()));
    MRT_DEBUG_LOG_MSG(std::format("XLSXWriter version: {}", lxw_version()));
#else 
    std::locale::global(std::locale("en_GB.UTF-8"));
#endif

    Main_Frame* mainFrame = new Main_Frame(mrtApp::APP_TITLE, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE);

    mainFrame->SetMinSize(mainFrame->FromDIP(wxSize(700, 700)));
    mainFrame->SetIcon(CSV_to_Logo);
    mainFrame->Show(true);
    
    return true;
}

  /*****************************/
 /* Main_Frame Implementation */
/*****************************/

Main_Frame::Main_Frame(const wxString& title, const wxPoint& pos, const wxSize& size, long style) 
    : wxFrame(nullptr, wxID_ANY, title, pos, size, style)
{
    m_Notebook = new wxNotebook(this, wxNB_TOP | wxNB_FIXEDWIDTH); 	    // Creating the notebook to hold the panels.
    m_Notebook->SetOwnFont(MAIN_FONT_TEXT(11));
    m_Notebook->SetOwnBackgroundColour(m_Colours.BACKGROUND);
    m_Notebook->SetOwnForegroundColour(m_Colours.FOREGROUND);

    m_Panels = {                                                        // Initialising the panels and their names.
        std::make_pair<CSVto_PanelBase*, std::string>(new CSVtoCSV_Panel(m_Notebook, "Csv to CSV", &m_Colours), "CSV"),
        std::make_pair<CSVto_PanelBase*, std::string>(new CSVtoSQL_Panel(m_Notebook, "Csv to SQL", &m_Colours), "SQL"),
        std::make_pair<CSVto_PanelBase*, std::string>(new CSVtoASCII_Panel(m_Notebook, "Csv to Ascii", &m_Colours), "Ascii"),
        std::make_pair<CSVto_PanelBase*, std::string>(new CSVtoMarkdown_Panel(m_Notebook, "Csv to Markdown", &m_Colours), "Markdown"),
        std::make_pair<CSVto_PanelBase*, std::string>(new CSVtoXML_Panel(m_Notebook, "Csv to XML", &m_Colours), "XML"),
        std::make_pair<CSVto_PanelBase*, std::string>(new CSVtoHTML_Panel(m_Notebook, "Csv to HTML", &m_Colours), "HTML"),
        std::make_pair<CSVto_PanelBase*, std::string>(new CSVtoLaTex_Panel(m_Notebook, "Csv to LaTex", &m_Colours), "LaTex"),
        std::make_pair<CSVto_PanelBase*, std::string>(new CSVtoJSON_Panel(m_Notebook, "Csv to JSON", &m_Colours), "JSON"),
        std::make_pair<CSVto_PanelBase*, std::string>(new CSVtoPHP_Panel(m_Notebook, "Csv to PHP", &m_Colours), "PHP"),
        std::make_pair<CSVto_PanelBase*, std::string>(new CSVtoRuby_Panel(m_Notebook, "Csv to Ruby", &m_Colours), "Ruby"),
        std::make_pair<CSVto_PanelBase*, std::string>(new CSVtoMultiline_Panel(m_Notebook, "Csv to Multi-line", &m_Colours), "Multi-line"),
        std::make_pair<CSVto_PanelBase*, std::string>(new CSVtoYAML_Panel(m_Notebook, "Csv to YAML", &m_Colours), "YAML"),
        std::make_pair<CSVto_PanelBase*, std::string>(new CSVtoBBCode_Panel(m_Notebook, "Csv to BBCode", &m_Colours), "BBCode"),
        std::make_pair<CSVto_PanelBase*, std::string>(new CSVtoXLSX_Panel(m_Notebook, "Csv to XLSX", &m_Colours), "XLSX")
    };

#if defined(MRT_DEBUG)
    MRT_DEBUG_LOG_MSG(std::format("Created {} panels and added to wxNoteBook", m_Panels.size()));
#endif

    for (std::pair<CSVto_PanelBase*, std::string>& panel : m_Panels)    // Looping through 'm_Panels' and adding the panels to the notebook.
	{
		m_Notebook->AddPage(panel.first, panel.second);
	}

    Bind(wxEVT_CLOSE_WINDOW, &Main_Frame::OnClose, this);		        // Binding the close event to stop exceptions being thrown when the window is closed with threads running.
}

void Main_Frame::OnClose(wxCloseEvent& event)
{
    if (std::any_of(m_Panels.begin(), m_Panels.end(), [](const std::pair<CSVto_PanelBase*, std::string>& panel) { return panel.first->isThreadsRunning(); }))
    {                                                                   // If any of the panels have threads running, show a warning dialog.
        mrt::MrT_UniDialog warningDialog(this, "Warning", "Threads are still running in the background.\nPlease wait for all threads to finish processing before exiting the application.",
            &m_Colours, wxICON(wxICON_WARNING), mrt::MrT_UniDialogType_OK, FromDIP(wxSize(400, 200)));

        warningDialog.ShowModal();

#if defined(MRT_DEBUG)
		MRT_DEBUG_LOG_MSG("Warning message sent, threads are still running, unable to close app");
#endif
    }
    else
    {
        Destroy();                                                      // If no threads are running, destroy the window.
    }
}

Main_Frame::~Main_Frame()
{
    for (std::pair<CSVto_PanelBase*, std::string>& panel : m_Panels)
	{
        delete (panel.first);
	}
}

wxIMPLEMENT_APP(MrT_App);   									        // Implementing the application.