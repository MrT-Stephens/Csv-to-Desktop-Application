#pragma once

#include <numeric>
#include <sstream>
#include <vector>
#include <string>
#include <format>

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

		const std::vector<std::string>& m_ColumnNames;
		const std::vector<unsigned int>& m_ColumnMaxWidths;
		const std::vector<std::vector<std::string>>& m_TableData;
	public:
		AsciiTableGenerator(const std::vector<std::string>& _columnNames, const std::vector<std::vector<std::string>>& _tableData, const std::vector<unsigned int>& _columnMaxWidths);

		std::ostringstream& GetAsciiTableStream();
		const std::ostringstream& GetAsciiTableStream() const;

		void GenerateAsciiTable(int _tableStyle, const std::string _commentCharacters, bool _forceRowSeparation, int _whiteSpaceStyle);

		std::string GenerateDataLine(const std::vector<std::string>& _rowVector, const std::vector<unsigned int>& _columnMaxWidths, unsigned char _separationCharacter, int _style) const;

		std::string GenerateSpacerLine(const std::vector<unsigned int>& _columnMaxWidths, unsigned char _dataSpaceCharacter, unsigned char _edgesCharacter) const;
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

		const std::vector<std::string>& m_ColumnNames;
		const std::vector<unsigned int>& m_ColumnMaxWidths;
		const std::vector<std::vector<std::string>>& m_TableData;
	public:
		MarkdownTableGenerator(const std::vector<std::string>& _columnNames, const std::vector<std::vector<std::string>>& _tableData, const std::vector<unsigned int>& _columnMaxWidths);

		std::ostringstream& GetMarkdownTableStream();
		const std::ostringstream& GetMarkdownTableStream() const;

		void GenerateMarkdownTable(int _tableStyle, int _whiteSpaceStyle, bool _boldFirstRow, bool _boldFirstCol);
	};
}