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
	m_GitHubButton->SetMinSize({ 120, 30 });

	m_GitHubButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			wxLaunchDefaultBrowser(mrtApp::GITHUB_URL);
		}
	);

	m_ButtonSizer->Add(m_GitHubButton, 1, wxALL | wxEXPAND, FromDIP(10));

	m_DonateButton = new wxButton(this, wxID_ANY, "Donate", wxDefaultPosition, wxDefaultSize);
	m_DonateButton->SetOwnForegroundColour(colours->SECONDARY);
	m_DonateButton->SetOwnBackgroundColour(colours->PRIMARY);
	m_DonateButton->SetOwnFont(MAIN_FONT_TEXT(10));
	m_DonateButton->SetMinSize({ 120, 30 });

	m_DonateButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			wxLaunchDefaultBrowser(mrtApp::DONATE_URL);
		}
	);

	m_ButtonSizer->Add(m_DonateButton, 1, wxALL | wxEXPAND, FromDIP(10));

	m_OkButton = new wxButton(this, wxID_CLOSE, "Close", wxDefaultPosition, wxDefaultSize);
	m_OkButton->SetOwnForegroundColour(colours->SECONDARY);
	m_OkButton->SetOwnBackgroundColour(colours->PRIMARY);
	m_OkButton->SetOwnFont(MAIN_FONT_TEXT(10));
	m_OkButton->SetMinSize({ 120, 30 });

	m_OkButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			EndModal(wxID_CLOSE);
		}
	);

	m_ButtonSizer->Add(m_OkButton, 1, wxALL | wxEXPAND, FromDIP(10));

	m_MainSizer->Add(m_ButtonSizer, 0, wxALL | wxEXPAND, FromDIP(0));

	SetSizer(m_MainSizer);
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
		m_OkButton->SetMinSize({ 120, 30 });

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
		m_YesButton->SetMinSize({ 120, 30 });

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
		m_NoButton->SetMinSize({ 120, 30 });

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
		m_CancelButton->SetMinSize({ 120, 30 });

		m_CancelButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
			{
				EndModal(wxID_CANCEL);
			}
		);

		m_ButtonSizer->Add(m_CancelButton, 1, wxALL | wxEXPAND, FromDIP(10));
	}

	m_MainSizer->Add(m_ButtonSizer, 0, wxALL | wxEXPAND, FromDIP(0));

	SetSizer(m_MainSizer);
}

  /*************************************/
 /* MrT_DataEditDialog Implementation */
/*************************************/

mrt::MrT_DataEditDialog::MrT_DataEditDialog(wxWindow* parent, const std::string& title, const mrtApp::AppColours* colours,
	const std::vector<std::string>& dataEditTitles, std::vector<std::string>& editableData, const wxSize& size)
	: wxDialog(parent, wxID_ANY, "", wxDefaultPosition, size, (wxDEFAULT_DIALOG_STYLE & ~(wxSYSTEM_MENU)) | wxSTAY_ON_TOP)
{
	SetIcon(wxICON(wxICON_INFORMATION));
	SetOwnBackgroundColour(colours->BACKGROUND);

	m_Panel = new wxScrolled<wxPanel>(this, wxID_ANY);
	m_Panel->SetOwnBackgroundColour(colours->BACKGROUND);
	m_Panel->SetScrollRate(0, FromDIP(10));

	m_MainSizer = new wxBoxSizer(wxVERTICAL);

	m_Title = new wxStaticText(m_Panel, wxID_ANY, title, wxDefaultPosition, wxDefaultSize);
	m_Title->SetOwnForegroundColour(colours->SECONDARY);
	m_Title->SetOwnFont(MAIN_FONT_TEXT(16));

	m_MainSizer->Add(m_Title, 0, wxALL | wxALIGN_CENTER, FromDIP(10));

	m_DataEditTitles.resize(dataEditTitles.size());
	m_DataEditBoxes.resize(dataEditTitles.size());

	for (size_t i = 0; i < dataEditTitles.size(); ++i)
	{
		m_DataEditTitles[i] = new wxStaticText(m_Panel, wxID_ANY, std::format("Edit ({}):", dataEditTitles[i]), wxDefaultPosition, wxDefaultSize);
		m_DataEditTitles[i]->SetOwnForegroundColour(colours->SECONDARY);
		m_DataEditTitles[i]->SetOwnFont(MAIN_FONT_TEXT(10));

		m_DataEditBoxes[i] = new wxTextCtrl(m_Panel, wxID_ANY, editableData[i], wxDefaultPosition, wxDefaultSize, wxTE_NO_VSCROLL);
		m_DataEditBoxes[i]->SetOwnBackgroundColour(colours->PRIMARY);
		m_DataEditBoxes[i]->SetOwnForegroundColour(colours->FOREGROUND);
		m_DataEditBoxes[i]->SetOwnFont(MAIN_FONT_TEXT(10));

		m_MainSizer->Add(m_DataEditTitles[i], 0, wxLEFT | wxTOP | wxRIGHT | wxEXPAND, FromDIP(10));
		m_MainSizer->Add(m_DataEditBoxes[i], 0, wxALL | wxEXPAND, FromDIP(10));
	}

	m_ButtonSizer = new wxBoxSizer(wxHORIZONTAL);

	m_ApplyButton = new wxButton(m_Panel, wxID_APPLY, "Apply", wxDefaultPosition, wxDefaultSize);
	m_ApplyButton->SetOwnForegroundColour(colours->SECONDARY);
	m_ApplyButton->SetOwnBackgroundColour(colours->PRIMARY);
	m_ApplyButton->SetOwnFont(MAIN_FONT_TEXT(10));
	m_ApplyButton->SetMinSize({ 120, 30 });

	m_ApplyButton->Bind(wxEVT_BUTTON, [this, &editableData](wxCommandEvent& event)
		{
			for (size_t i = 0; i < editableData.size(); ++i)
			{
				editableData[i] = m_DataEditBoxes[i]->GetValue().ToStdString();
			}

			EndModal(wxID_APPLY);
		}
	);

	m_ButtonSizer->Add(m_ApplyButton, 1, wxALL | wxEXPAND, FromDIP(10));

	m_CancelButton = new wxButton(m_Panel, wxID_CANCEL, "Cancel", wxDefaultPosition, wxDefaultSize);
	m_CancelButton->SetOwnForegroundColour(colours->SECONDARY);
	m_CancelButton->SetOwnBackgroundColour(colours->PRIMARY);
	m_CancelButton->SetOwnFont(MAIN_FONT_TEXT(10));
	m_CancelButton->SetMinSize({ 120, 30 });

	m_CancelButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
		{
			EndModal(wxID_CANCEL);
		}
	);

	m_ButtonSizer->Add(m_CancelButton, 1, wxALL | wxEXPAND, FromDIP(10));

	m_MainSizer->Add(m_ButtonSizer, 0, wxALL | wxEXPAND, FromDIP(0));

	m_Panel->SetSizer(m_MainSizer);

	m_CancelButton->SetFocus();
}