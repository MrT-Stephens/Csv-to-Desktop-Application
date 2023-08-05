#pragma once

#include <format>

#include <wx/wx.h>
#include <wx/statline.h>

#include "UtilityItems.h"

namespace mrt
{
	  /******************************/
	 /* MrT_InfoDialog Declaration */
	/******************************/

	class MrT_InfoDialog : public wxDialog
	{
	private:
		wxTextCtrl* m_Message;
		wxStaticBitmap* m_Logo;
		wxStaticLine* m_LogoTileLine;
		wxStaticText* m_Title, * m_AppName;
		wxBoxSizer* m_MainSizer, * m_ButtonSizer;
		wxButton* m_OkButton, * m_GitHubButton, * m_DonateButton;
	public:
		MrT_InfoDialog(wxWindow* parent, const std::string& title, const std::string& message, const mrtApp::AppColours* colours, const wxSize& size = wxDefaultSize);
	};

	enum MrT_UniDialogType							// Universal Dialog Types
	{
		MrT_UniDialogType_None = 0,
		MrT_UniDialogType_OK = 1 << 0,
		MrT_UniDialogType_YES = 1 << 1,
		MrT_UniDialogType_NO = 1 << 2,
		MrT_UniDialogType_CANCEL = 1 << 3,

		MrT_UniDialogType_OK_CANCEL = MrT_UniDialogType_OK | MrT_UniDialogType_CANCEL,
		MrT_UniDialogType_YES_NO = MrT_UniDialogType_YES | MrT_UniDialogType_NO,
		MrT_UniDialogType_YES_NO_CANCEL = MrT_UniDialogType_YES | MrT_UniDialogType_NO | MrT_UniDialogType_CANCEL
	};

	  /*****************************/
	 /* MrT_UniDialog Declaration */
	/*****************************/

	class MrT_UniDialog : public wxDialog			// Universal Dialog class for error, warning, and question dialogs
	{
	private:
		wxTextCtrl* m_Message;
		wxStaticText* m_Title;
		wxBoxSizer* m_MainSizer, * m_ButtonSizer;
		wxButton* m_OkButton, * m_CancelButton, * m_YesButton, * m_NoButton;
	public:
		MrT_UniDialog(wxWindow* parent, const std::string& title, const std::string& message, const mrtApp::AppColours* colours, 
			const wxIcon& icon, long dialogType = MrT_UniDialogType_None, const wxSize& size = wxDefaultSize);
	};

	  /**********************************/
	 /* MrT_DataEditDialog Declaration */
	/**********************************/

	class MrT_DataEditDialog : public wxDialog
	{
	private:
		// Main Items
		wxScrolled<wxPanel>* m_Panel;
		wxStaticText* m_Title;
		wxBoxSizer* m_MainSizer, * m_ButtonSizer;
		wxButton* m_ApplyButton, * m_CancelButton;

		// Variable amount items
		std::vector<wxStaticText*> m_DataEditTitles;
		std::vector<wxTextCtrl*> m_DataEditBoxes;
	public:
		MrT_DataEditDialog(wxWindow* parent, const std::string& title, const mrtApp::AppColours* colours, 
			const std::vector<std::string>& dataEditTitles, std::vector<std::string>& editableData, const wxSize& size = wxDefaultSize);
	};
}