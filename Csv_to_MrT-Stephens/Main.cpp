#include "Main.h"

bool MrT_App::OnInit()
{
    Main_Frame* mainFrame = new Main_Frame(mrtApp::APP_TITLE, wxDefaultPosition, { 700, 800 }, wxDEFAULT_FRAME_STYLE);

    mainFrame->SetMinSize({ 700, 800 });
    mainFrame->SetIcon(CSV_to_Logo);
    mainFrame->Show(true);

    return true;
}

Main_Frame::Main_Frame(const wxString& title, const wxPoint& pos, const wxSize& size, long style) 
    : wxFrame(nullptr, wxID_ANY, title, pos, size, style)
{
    m_Notebook = new wxNotebook(this, wxNB_TOP | wxNB_FIXEDWIDTH); 	    // Creating the notebook to hold the panels.
    m_Notebook->SetOwnFont(MAIN_FONT_TEXT(11));
    m_Notebook->SetOwnBackgroundColour(m_Colours.BACKGROUND);
    m_Notebook->SetOwnForegroundColour(m_Colours.FOREGROUND);

    m_Panels = {                                                        // Initialising the panels and their names.
        std::make_pair<CSVto_PanelBase*, std::string>(new CSVtoSQL_Panel(m_Notebook, "CSV to SQL ", &m_Colours), "SQL"),
        std::make_pair<CSVto_PanelBase*, std::string>(new CSVtoASCII_Panel(m_Notebook, "CSV to Ascii ", &m_Colours), "Ascii"),
        std::make_pair<CSVto_PanelBase*, std::string>(new CSVtoMarkdown_Panel(m_Notebook, "CSV to Markdown", &m_Colours), "Markdown")
    };

    for (std::pair<CSVto_PanelBase*, std::string>& panel : m_Panels)    // Looping through 'm_Panels' and adding the panels to the notebook.
	{
		m_Notebook->AddPage(panel.first, panel.second);
	}

    Bind(wxEVT_CLOSE_WINDOW, [this](wxCloseEvent& event)		        // Binding the close event to stop exceptions being thrown when the window is closed with threads running.
        {
            if (std::any_of(m_Panels.begin(), m_Panels.end(), [](const std::pair<CSVto_PanelBase*, std::string>& panel) { return panel.first->isThreadsRunning(); }))
            {                                                           // If any of the panels have threads running, show a warning dialog.
                mrt::MrT_UniDialog warningDialog(this, "Warning", "Threads are still running in the background.\nPlease wait for all threads to finish processing before exiting the application.",
                    &m_Colours, wxICON(wxICON_WARNING), mrt::MrT_UniDialogType_OK, { 400, 200 });

                warningDialog.ShowModal();
            }
            else
            {
				Destroy();                                              // If no threads are running, destroy the window.
			}
        }
    );
}

Main_Frame::~Main_Frame()
{
    for (std::pair<CSVto_PanelBase*, std::string>& panel : m_Panels)
	{
		delete panel.first;
	}
}

wxIMPLEMENT_APP(MrT_App);          

// Hovering over buttons, change the colour of the text.
// Flickering on the open file button.