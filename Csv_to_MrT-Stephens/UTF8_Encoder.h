#pragma once

#include <assert.h>
#include <stdint.h>
#include <string>

class UTF8_Encoder
{
public:
    static inline std::string From_wchar_t(int32_t Code)
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

    static inline std::string From_wstring(const std::wstring& str)
    {
        std::string result;

        for (const wchar_t& character : str)
        {
            result.append(From_wchar_t(character));
        }

        return result;
    }
};