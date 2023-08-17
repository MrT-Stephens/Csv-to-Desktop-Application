#pragma once

#include <wx/wx.h>

#include <string>

#include "CSV_to_Logo.xpm"   // Icons array is called 'CSV_to_Logo'

  /*********************/
 /* MrT Global Macros */
/*********************/

#if defined(MRT_DEBUG)
#define MRT_DEBUG_LOG_MSG(msg) std::cout << "[MRT_DEBUG] " << "(Time: " << std::chrono::system_clock::now() << ") " << msg << ".\n"
#define MRT_DEBUG_LOG_TIME(msg, runTime, timeUnits) std::cout << "[MRT_DEBUG] " << msg << " (Time: " << runTime << " " << timeUnits << ").\n"
#endif

#define MAIN_FONT_TEXT(size) wxFont(size, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "", wxFONTENCODING_DEFAULT)
#define MAIN_FONT_BOLD(size) wxFont(size, wxFONTFAMILY_MODERN, wxFONTSTYLE_MAX, wxFONTWEIGHT_EXTRAHEAVY, false, "", wxFONTENCODING_DEFAULT)

  /*****************************/
 /* MrT App Global Infomation */
/*****************************/

namespace mrtApp
{
	extern const std::string APP_NAME;
	extern const std::string APP_VERSION;

	extern const std::string APP_TITLE;

	extern const std::string GITHUB_URL;

	struct AppColours
	{
		const wxColour BACKGROUND{ 32, 32, 32 };
		const wxColour FOREGROUND{ 255, 255, 255 };
		const wxColour PRIMARY{ 42, 42, 42 };
		const wxColour SECONDARY{ 28, 230, 224 };
	};
}

  /***************************/
 /* MrT Global Declarations */
/***************************/

namespace mrt
{
	template <class _Numeric>
	inline _Numeric RoundToNearest10(_Numeric number);
}

  /******************************/
 /* MrT Global Implementations */
/******************************/

template <class _Numeric>
_Numeric mrt::RoundToNearest10(_Numeric number)
{
	return (number + 10) / 10 * 10;
}