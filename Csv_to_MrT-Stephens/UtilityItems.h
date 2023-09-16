#pragma once

#include <wx/wx.h>

#include <string>
#include <assert.h>
#include <stdint.h>

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
		const wxColour FOREGROUND{ 225, 225, 225 };
		const wxColour PRIMARY{ 42, 42, 42 };
		const wxColour SECONDARY{ 0, 230, 230 };
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

  /****************************/
 /* UTF8_Encoder Declaration */
/****************************/

class UTF8_Encoder
{
public:
    static inline std::string From_wchar_t(int32_t Code);
	static inline std::string From_wstring(const std::wstring& str);
};

  /******************************/
 /* MrT Global Implementations */
/******************************/

template <class _Numeric>
_Numeric mrt::RoundToNearest10(_Numeric number)
{
	return (number + 10) / 10 * 10;
}

  /*******************************/
 /* UTF8_Encoder Implementation */
/*******************************/

inline std::string UTF8_Encoder::From_wchar_t(int32_t Code)
{
    assert((Code >= 0) && "Code can not be negative.");

    if (Code <= 0x7F)
    {
        return std::string(1, static_cast<char>(Code));
    }
    else
    {
        unsigned char FirstByte = 0xC0;                     //First byte with mask
        unsigned char Buffer[7] = { 0 };                    //Buffer for UTF-8 bytes, null-ponter string
        char* Ptr = reinterpret_cast<char*>(&Buffer[5]);    //Ptr to Buffer, started from end
        unsigned char LastValue = 0x1F;                     //Max value for implement to the first byte

        while (true)
        {
            *Ptr = static_cast<char>((Code & 0x3F) | 0x80); //Making new value with format 10xxxxxx
            Ptr--;                                          //Move Ptr to new position
            Code >>= 6;                                     //Shifting Code
            if (Code <= LastValue)
            {
                break;                                      //if Code can set to FirstByte => break;
            }
            LastValue >>= 1;                                //Make less max value
            FirstByte = (FirstByte >> 1) | 0x80;            //Modify first byte
        }

        *Ptr = static_cast<char>(FirstByte | Code);         //Making first byte of UTF-8 sequence
        return std::string(Ptr);
    }
}

inline std::string UTF8_Encoder::From_wstring(const std::wstring& str)
{
    std::string result;

    for (const wchar_t& character : str)
    {
        result.append(From_wchar_t(character));
    }

    return result;
}