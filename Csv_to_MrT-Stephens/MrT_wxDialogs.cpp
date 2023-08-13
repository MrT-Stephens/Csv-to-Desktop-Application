#include "MrT_wxDialogs.h"

  /*********************************/
 /* MrT_InfoDialog Implementation */
/*********************************/

mrt::MrT_InfoDialog::MrT_InfoDialog(wxWindow* parent, const std::string& title, const std::string& message, const mrtApp::AppColours* colours, const wxSize& size)
	: wxDialog(parent, wxID_ANY, "", wxDefaultPosition, size, (wxDEFAULT_DIALOG_STYLE & ~(wxSYSTEM_MENU)) | wxSTAY_ON_TOP)
{
	SetIcon(wxICON(wxICON_INFORMATION));
	SetOwnBackgroundColour(colours->BACKGROUND);

	m_MainSizer = new wxBoxSizer(wxVERTICAL);

	m_Logo = new wxStaticBitmap(this, wxID_ANY, wxBitmap(CSV_to_Logo).ConvertToImage().Rescale(60, 60, wxIMAGE_QUALITY_HIGH));

	m_MainSizer->Add(m_Logo, 0, wxALL | wxALIGN_CENTER, FromDIP(10));

	m_AppName = new wxStaticText(this, wxID_ANY, mrtApp::APP_TITLE, wxDefaultPosition, wxDefaultSize);
	m_AppName->SetOwnForegroundColour(colours->SECONDARY);
	m_AppName->SetOwnFont(MAIN_FONT_TEXT(16));

	m_MainSizer->Add(m_AppName, 0, wxALL | wxALIGN_CENTER, FromDIP(10));

	m_LogoTileLine = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	m_MainSizer->Add(m_LogoTileLine, 0, wxALL | wxEXPAND, FromDIP(5));

	m_Title = new wxStaticText(this, wxID_ANY, title, wxDefaultPosition, wxDefaultSize);
	m_Title->SetOwnForegroundColour(colours->SECONDARY);
	m_Title->SetOwnFont(MAIN_FONT_TEXT(16));

	m_MainSizer->Add(m_Title, 0, wxALL | wxALIGN_CENTER, FromDIP(10));

	m_Message = new wxTextCtrl(this, wxID_ANY, message, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxNO_BORDER | wxTE_BESTWRAP | wxTE_NO_VSCROLL);
	m_Message->SetOwnBackgroundColour(colours->BACKGROUND);
	m_Message->SetOwnForegroundColour(colours->FOREGROUND);
	m_Message->SetOwnFont(MAIN_FONT_TEXT(10));

	m_MainSizer->Add(m_Message, 1, wxALL | wxEXPAND, FromDIP(10));

	m_ButtonSizer = new wxBoxSizer(wxHORIZONTAL);

	m_GitHubButton = new wxButton(this, wxID_ANY, "GitHub", wxDefaultPosition, wxDefaultSize);
	m_GitHubButton->SetOwnForegroundColour(colours->SECONDARY);
	m_GitHubButton->SetOwnBackgroundColour(colours->PRIMARY);
	m_GitHubButton->SetOwnFont(MAIN_FONT_TEXT(10));
	m_GitHubButton->SetMinSize(FromDIP(wxSize(120, 30)));

	m_GitHubButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			wxLaunchDefaultBrowser(mrtApp::GITHUB_URL);
		}
	);

	m_ButtonSizer->Add(m_GitHubButton, 1, wxALL | wxEXPAND, FromDIP(10));

	m_OkButton = new wxButton(this, wxID_CLOSE, "Close", wxDefaultPosition, wxDefaultSize);
	m_OkButton->SetOwnForegroundColour(colours->SECONDARY);
	m_OkButton->SetOwnBackgroundColour(colours->PRIMARY);
	m_OkButton->SetOwnFont(MAIN_FONT_TEXT(10));
	m_OkButton->SetMinSize(FromDIP(wxSize(120, 30)));

	m_OkButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			EndModal(wxID_CLOSE);
		}
	);

	m_ButtonSizer->Add(m_OkButton, 1, wxALL | wxEXPAND, FromDIP(10));

	m_MainSizer->Add(m_ButtonSizer, 0, wxALL | wxEXPAND, FromDIP(0));

	SetSizer(m_MainSizer);

	CentreOnParent();
}

  /********************************/
 /* MrT_UniDialog Implementation */
/********************************/

mrt::MrT_UniDialog::MrT_UniDialog(wxWindow* parent, const std::string& title, const std::string& message, const mrtApp::AppColours* colours, const wxIcon& icon, long dialogType, const wxSize& size) 
	: wxDialog(parent, wxID_ANY, "", wxDefaultPosition, size, (wxDEFAULT_DIALOG_STYLE & ~(wxSYSTEM_MENU)) | wxSTAY_ON_TOP)
{
	SetIcon(icon);
	SetOwnBackgroundColour(colours->BACKGROUND);

	m_MainSizer = new wxBoxSizer(wxVERTICAL);

	if (!title.empty())
	{
		m_Title = new wxStaticText(this, wxID_ANY, title, wxDefaultPosition, wxDefaultSize);
		m_Title->SetOwnForegroundColour(colours->SECONDARY);
		m_Title->SetOwnFont(MAIN_FONT_TEXT(16));

		m_MainSizer->Add(m_Title, 0, wxALL | wxALIGN_CENTER, FromDIP(10));
	}

	if (!message.empty())
	{
		m_Message = new wxTextCtrl(this, wxID_ANY, message, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxNO_BORDER | wxTE_BESTWRAP | wxTE_NO_VSCROLL);
		m_Message->SetOwnBackgroundColour(colours->BACKGROUND);
		m_Message->SetOwnForegroundColour(colours->FOREGROUND);
		m_Message->SetOwnFont(MAIN_FONT_TEXT(10));

		m_MainSizer->Add(m_Message, 1, wxALL | wxEXPAND, FromDIP(10));
	}

	m_ButtonSizer = new wxBoxSizer(wxHORIZONTAL);

	if ((MrT_UniDialogType_OK & dialogType) != 0)
	{
		m_OkButton = new wxButton(this, wxID_OK, "OK", wxDefaultPosition, wxDefaultSize);
		m_OkButton->SetOwnForegroundColour(colours->SECONDARY);
		m_OkButton->SetOwnBackgroundColour(colours->PRIMARY);
		m_OkButton->SetOwnFont(MAIN_FONT_TEXT(10));
		m_OkButton->SetMinSize(FromDIP(wxSize(120, 30)));

		m_OkButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
			{
				EndModal(wxID_OK);
			}
		);

		m_ButtonSizer->Add(m_OkButton, 1, wxALL | wxEXPAND, FromDIP(10));
	}
	if ((MrT_UniDialogType_YES & dialogType) != 0)
	{
		m_YesButton = new wxButton(this, wxID_YES, "Yes", wxDefaultPosition, wxDefaultSize);
		m_YesButton->SetOwnForegroundColour(colours->SECONDARY);
		m_YesButton->SetOwnBackgroundColour(colours->PRIMARY);
		m_YesButton->SetOwnFont(MAIN_FONT_TEXT(10));
		m_YesButton->SetMinSize(FromDIP(wxSize(120, 30)));

		m_YesButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
			{
				EndModal(wxID_YES);
			}
		);

		m_ButtonSizer->Add(m_YesButton, 1, wxALL | wxEXPAND, FromDIP(10));
	}
	if ((MrT_UniDialogType_NO & dialogType) != 0)
	{
		m_NoButton = new wxButton(this, wxID_NO, "No", wxDefaultPosition, wxDefaultSize);
		m_NoButton->SetOwnForegroundColour(colours->SECONDARY);
		m_NoButton->SetOwnBackgroundColour(colours->PRIMARY);
		m_NoButton->SetOwnFont(MAIN_FONT_TEXT(10));
		m_NoButton->SetMinSize(FromDIP(wxSize(120, 30)));

		m_NoButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
			{
				EndModal(wxID_NO);
			}
		);

		m_ButtonSizer->Add(m_NoButton, 1, wxALL | wxEXPAND, FromDIP(10));
	}
	if ((MrT_UniDialogType_CANCEL & dialogType) != 0)
	{
		m_CancelButton = new wxButton(this, wxID_CANCEL, "Cancel", wxDefaultPosition, wxDefaultSize);
		m_CancelButton->SetOwnForegroundColour(colours->SECONDARY);
		m_CancelButton->SetOwnBackgroundColour(colours->PRIMARY);
		m_CancelButton->SetOwnFont(MAIN_FONT_TEXT(10));
		m_CancelButton->SetMinSize(FromDIP(wxSize(120, 30)));

		m_CancelButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
			{
				EndModal(wxID_CANCEL);
			}
		);

		m_ButtonSizer->Add(m_CancelButton, 1, wxALL | wxEXPAND, FromDIP(10));
	}

	m_MainSizer->Add(m_ButtonSizer, 0, wxALL | wxEXPAND, FromDIP(0));

	SetSizer(m_MainSizer);

	CentreOnParent();
}

  /*****************************************/
 /* MrT_CSVDataEdit_Dialog Implementation */
/*****************************************/

mrt::MrT_CSVDataEdit_Dialog::MrT_CSVDataEdit_Dialog(wxWindow* parent, mrt::CSVData<std::string>* const csvData, const mrtApp::AppColours* colours, const wxPoint& pos, const wxSize& size)
	: wxDialog(parent, wxID_ANY, "Edit Table Data", pos, size, (wxDEFAULT_DIALOG_STYLE & ~(wxSYSTEM_MENU)) | wxSTAY_ON_TOP)
{
	SetOwnBackgroundColour(colours->BACKGROUND);

	m_MainSizer = new wxBoxSizer(wxVERTICAL);

	m_MainPanel = new wxScrolled<wxPanel>(this, wxID_ANY);

	m_PanelSizer = new wxBoxSizer(wxVERTICAL);

	m_MainPanel->SetScrollRate(0, 20);
	m_MainPanel->SetOwnBackgroundColour(colours->BACKGROUND);

	m_RowNumberStaticText = new wxStaticText(m_MainPanel, wxID_ANY, "Row Number:");
	m_RowNumberStaticText->SetOwnForegroundColour(colours->SECONDARY);
	m_RowNumberStaticText->SetOwnFont(MAIN_FONT_TEXT(13));	

	m_RowNumberTextCtrl = new wxTextCtrl(m_MainPanel, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize);
	m_RowNumberTextCtrl->SetMinSize(FromDIP(wxSize(120, 30)));
	m_RowNumberTextCtrl->SetOwnFont(MAIN_FONT_TEXT(11));
	m_RowNumberTextCtrl->SetOwnBackgroundColour(colours->PRIMARY);
	m_RowNumberTextCtrl->SetOwnForegroundColour(colours->FOREGROUND);
	m_RowNumberTextCtrl->SetToolTip("Pick a row number that you would like to edit. Zero for header.");

	m_RowNumberTextCtrl->Bind(wxEVT_TEXT, [this](wxCommandEvent& event)
		{
			m_Timer.Start(2000);
		}
	);

	m_Timer.Bind(wxEVT_TIMER, [this, &csvData](wxTimerEvent& event)
		{
			m_Timer.Stop();

			unsigned int row;
			m_RowNumberTextCtrl->GetValue().ToUInt(&row);
			const std::vector<std::string>& data = (row == 0) ? csvData->GetHeaderNames() : csvData->GetRowData(row - 1);

			for (size_t i = 0; i < csvData->GetColumnCount(); ++i)
			{
				m_DataEditCtrls[i]->SetValue(data[i]);
			}
		}
	);

	m_PanelSizer->Add(m_RowNumberStaticText, 0, wxLEFT | wxRIGHT | wxTOP | wxALIGN_LEFT, FromDIP(10));
	m_PanelSizer->Add(m_RowNumberTextCtrl, 0, wxALL | wxEXPAND, FromDIP(10));

	m_RowNumberStaticLine = new wxStaticLine(m_MainPanel, wxID_ANY);

	m_PanelSizer->Add(m_RowNumberStaticLine, 0, wxALL | wxEXPAND, FromDIP(5));

	m_DataEditCtrls.resize(csvData->GetColumnCount());

	{
		unsigned int row;
		m_RowNumberTextCtrl->GetValue().ToUInt(&row);
		const std::vector<std::string>& data = (row == 0) ? csvData->GetHeaderNames() : csvData->GetRowData(row - 1);

		for (size_t i = 0; i < csvData->GetColumnCount(); ++i)
		{
			m_DataEditCtrls[i] = new wxTextCtrl(m_MainPanel, wxID_ANY, data[i], wxDefaultPosition, wxDefaultSize);
			m_DataEditCtrls[i]->SetMinSize(FromDIP(wxSize(120, 30)));
			m_DataEditCtrls[i]->SetOwnFont(MAIN_FONT_TEXT(11));
			m_DataEditCtrls[i]->SetOwnBackgroundColour(colours->PRIMARY);
			m_DataEditCtrls[i]->SetOwnForegroundColour(colours->FOREGROUND);


			m_PanelSizer->Add(m_DataEditCtrls[i], 0, wxALL | wxEXPAND, FromDIP(10));
		}
	}

	m_MainPanel->SetSizer(m_PanelSizer);

	m_MainSizer->Add(m_MainPanel, 1, wxALL | wxEXPAND, FromDIP(0));

	m_ApplyButton = new wxButton(this, wxID_APPLY, "Apply", wxDefaultPosition, wxDefaultSize);
	m_ApplyButton->SetMinSize(FromDIP(wxSize(120, 30)));
	m_ApplyButton->SetOwnFont(MAIN_FONT_TEXT(11));
	m_ApplyButton->SetOwnBackgroundColour(colours->PRIMARY);
	m_ApplyButton->SetOwnForegroundColour(colours->FOREGROUND);

	m_ApplyButton->Bind(wxEVT_BUTTON, [this, &csvData](wxCommandEvent& event)
		{
			unsigned int row;
			m_RowNumberTextCtrl->GetValue().ToUInt(&row);
			std::vector<std::string>& data = (row == 0) ? csvData->GetHeaderNames() : csvData->GetRowData(row - 1);

			for (size_t i = 0; i < csvData->GetColumnCount(); ++i)
			{
				data[i] = m_DataEditCtrls[i]->GetValue();
			}

			EndModal(wxID_APPLY);
		}
	);

	m_CancelButton = new wxButton(this, wxID_CANCEL, "Cancel", wxDefaultPosition, wxDefaultSize);
	m_CancelButton->SetMinSize(FromDIP(wxSize(120, 30)));
	m_CancelButton->SetOwnFont(MAIN_FONT_TEXT(11));
	m_CancelButton->SetOwnBackgroundColour(colours->PRIMARY);
	m_CancelButton->SetOwnForegroundColour(colours->FOREGROUND);

	m_CancelButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			EndModal(wxID_CANCEL);
		}
	);

	m_ButtonSizer = new wxBoxSizer(wxHORIZONTAL);

	m_ButtonSizer->Add(m_ApplyButton, 1, wxALL | wxEXPAND, FromDIP(10));
	m_ButtonSizer->Add(m_CancelButton, 1, wxALL | wxEXPAND, FromDIP(10));

	m_MainSizer->Add(m_ButtonSizer, 0, wxALL | wxEXPAND, FromDIP(0));

	SetSizer(m_MainSizer);

	CentreOnParent();
}