#pragma once

#include <wx/wx.h>

#include <string>
#include <sstream>

#include "CSV_to_Logo.xpm"		// Icons array is called 'CSV_to_Logo'

#define MAIN_FONT_TEXT(size) wxFont(size, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "", wxFONTENCODING_DEFAULT)
#define MAIN_FONT_BOLD(size) wxFont(size, wxFONTFAMILY_MODERN, wxFONTSTYLE_MAX, wxFONTWEIGHT_EXTRAHEAVY, false, "", wxFONTENCODING_DEFAULT)

namespace mrtApp
{
	const std::string APP_NAME = "CSV to...";
	const std::string APP_VERSION = "v0.1.1";

	const std::string APP_TITLE = APP_NAME + " " + APP_VERSION;

	const std::string GITHUB_URL = "https://github.com/MrT-Stephens";
	const std::string DONATE_URL = "";

	struct AppColours
	{
		const wxColour BACKGROUND{ 32, 32, 32 };
		const wxColour FOREGROUND{ 255, 255, 255 };
		const wxColour PRIMARY{ 52, 52, 52 };
		const wxColour SECONDARY{ 28, 230, 224 };
	};
}