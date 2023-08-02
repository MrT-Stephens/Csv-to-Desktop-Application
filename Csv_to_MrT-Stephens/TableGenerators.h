#pragma once

#include <numeric>
#include <sstream>
#include <vector>
#include <string>
#include <format>

#include "CSVData.h"

namespace mrt
{
	enum WhiteSpaceStyle
	{
		LEFT = 0, BOTH = 1, RIGHT = 2
	};

	std::string AddWhiteSpaceToString(std::string _str, unsigned int _amount, int _style, unsigned char _fillCharacter = ' ');

	enum AsciiTableGenerator_Styles
	{
		Ascii_Table_MySQL = 0,
		Ascii_Table_Dots = 1,
		Ascii_Table_Dots_Curved = 2,
		Ascii_Table_Compact = 3,
		Ascii_Table_Seperated_Header = 4,
		Ascii_Table_Simple = 5,
		Ascii_Table_Wavy = 6,
		ExtendedAscii_Table_MySQL = 7
	};

	class AsciiTableGenerator
	{
	private:
		std::ostringstream m_AsciiTableText;

		const mrt::CSVData<std::string>* const m_CSVData;

		bool m_ForceRowSeparation;
		std::string m_CommentCharacters;
		int m_TableStyle, m_WhiteSpaceStyle;
	public:
		AsciiTableGenerator(const mrt::CSVData<std::string>* const _csvData, int _tableStyle, const std::string& _commentCharacters, bool _forceRowSeparation, int _whiteSpaceStyle);

		void GenerateAsciiTable();

		std::ostringstream& GetAsciiTableStream();
		const std::ostringstream& GetAsciiTableStream() const;

		std::string GenerateDataLine(const std::vector<std::string>& _rowVector, unsigned char _separationCharacter) const;

		std::string GenerateSpacerLine(unsigned char _dataSpaceCharacter, unsigned char _edgesCharacter) const;
	};

	enum MarkdownTableGenerator_Styles
	{
		Markdown_Table_Normal = 0,
		Markdown_Table_Simple = 1
	};

	class MarkdownTableGenerator
	{
	private:
		std::ostringstream m_MarkdownTableText;

		const mrt::CSVData<std::string>* const m_CSVData;

		bool m_BoldFirstRow, m_BoldFirstCol;
		int m_TableStyle, m_WhiteSpaceStyle;
	public:
		MarkdownTableGenerator(const mrt::CSVData<std::string>* const _csvData, int _tableStyle, int _whiteSpaceStyle, bool _boldFirstRow, bool _boldFirstCol);

		void GenerateMarkdownTable();

		std::ostringstream& GetMarkdownTableStream();
		const std::ostringstream& GetMarkdownTableStream() const;

		std::string GenerateDataLine(const std::vector<std::string>& _rowVector, const std::vector<size_t>& _columnWidths) const;
	};
}