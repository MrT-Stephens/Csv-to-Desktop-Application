#include "TableGenerators.h"

std::string mrt::AddWhiteSpaceToString(std::string _str, unsigned int _amount, int _style, unsigned char _fillCharacter)
{
	switch (_style)
	{
	case WhiteSpaceStyle::LEFT:
		_str.insert(_str.size(), _amount, _fillCharacter);
		break;
	case WhiteSpaceStyle::BOTH:
		_str.insert(0, std::floor(static_cast<double>(_amount) / 2), _fillCharacter);
		_str.insert(_str.size(), std::ceil(static_cast<double>(_amount) / 2), _fillCharacter);
		break;
	case WhiteSpaceStyle::RIGHT:
		_str.insert(0, _amount, _fillCharacter);
		break;
	}
	return _str;
}

mrt::AsciiTableGenerator::AsciiTableGenerator(
	const std::vector<std::string>& _columnNames,
	const std::vector<std::vector<std::string>>& _tableData,
	const std::vector<unsigned int>& _columnMaxWidths)
	: m_ColumnNames(_columnNames), m_TableData(_tableData), m_ColumnMaxWidths(_columnMaxWidths)
{

}

void mrt::AsciiTableGenerator::GenerateAsciiTable(int _tableStyle, const std::string _commentCharacters, bool _forceRowSeparation, int _whiteSpaceStyle)
{
	// Generates the header lines for the table.
	switch (_tableStyle)
	{
	case Ascii_Table_MySQL:
		m_AsciiTableText << _commentCharacters << GenerateSpacerLine(m_ColumnMaxWidths, '-', '+') << "\n" << _commentCharacters
			<< GenerateDataLine(m_ColumnNames, m_ColumnMaxWidths, '|', _whiteSpaceStyle) << "\n" << _commentCharacters << GenerateSpacerLine(m_ColumnMaxWidths, '-', '+') << "\n";
		break;
	case Ascii_Table_Dots:
		m_AsciiTableText << _commentCharacters << GenerateSpacerLine(m_ColumnMaxWidths, '·', '·') << "\n" << _commentCharacters
			<< GenerateDataLine(m_ColumnNames, m_ColumnMaxWidths, ':', _whiteSpaceStyle) << "\n" << _commentCharacters << GenerateSpacerLine(m_ColumnMaxWidths, '·', '·') << "\n";
		break;
	case Ascii_Table_Dots_Curved:
		m_AsciiTableText << _commentCharacters << GenerateSpacerLine(m_ColumnMaxWidths, '·', '.') << "\n" << _commentCharacters
			<< GenerateDataLine(m_ColumnNames, m_ColumnMaxWidths, ':', _whiteSpaceStyle) << "\n" << _commentCharacters << GenerateSpacerLine(m_ColumnMaxWidths, '·', '·') << "\n";
		break;
	case Ascii_Table_Compact:
		m_AsciiTableText << _commentCharacters << GenerateDataLine(m_ColumnNames, m_ColumnMaxWidths, ' ', _whiteSpaceStyle)
			<< "\n" << _commentCharacters << GenerateSpacerLine(m_ColumnMaxWidths, '-', '-') << "\n";
		break;
	case Ascii_Table_Seperated_Header:
		m_AsciiTableText << _commentCharacters << GenerateSpacerLine(m_ColumnMaxWidths, '=', '+') << "\n" << _commentCharacters
			<< GenerateDataLine(m_ColumnNames, m_ColumnMaxWidths, '|', _whiteSpaceStyle) << "\n" << _commentCharacters << GenerateSpacerLine(m_ColumnMaxWidths, '=', '+') << "\n";
		break;
	case Ascii_Table_Simple:
		m_AsciiTableText << _commentCharacters << GenerateSpacerLine(m_ColumnMaxWidths, '=', ' ') << "\n" << _commentCharacters
			<< GenerateDataLine(m_ColumnNames, m_ColumnMaxWidths, ' ', _whiteSpaceStyle) << "\n" << _commentCharacters << GenerateSpacerLine(m_ColumnMaxWidths, '=', ' ') << "\n";
		break;
	case Ascii_Table_Wavy:
		m_AsciiTableText << _commentCharacters << GenerateSpacerLine(m_ColumnMaxWidths, '~', '+') << "\n" << _commentCharacters
			<< GenerateDataLine(m_ColumnNames, m_ColumnMaxWidths, '¦', _whiteSpaceStyle) << "\n" << _commentCharacters << GenerateSpacerLine(m_ColumnMaxWidths, '~', '+') << "\n";
		break;
	case ExtendedAscii_Table_MySQL:
		m_AsciiTableText << _commentCharacters << GenerateSpacerLine(m_ColumnMaxWidths, '-', '+') << "\n" << _commentCharacters
			<< GenerateDataLine(m_ColumnNames, m_ColumnMaxWidths, '¦', _whiteSpaceStyle) << "\n" << _commentCharacters << GenerateSpacerLine(m_ColumnMaxWidths, '-', '+') << "\n";
		break;
	}

	// Generates the data lines for the table.
	for (size_t i = 0; i < m_TableData.size(); ++i)
	{
		switch (_tableStyle)
		{
		case Ascii_Table_MySQL:
			m_AsciiTableText << _commentCharacters << GenerateDataLine(m_TableData[i], m_ColumnMaxWidths, '|', _whiteSpaceStyle) << "\n";

			if (_forceRowSeparation || (i == m_TableData.size() - 1))
			{
				m_AsciiTableText << _commentCharacters << GenerateSpacerLine(m_ColumnMaxWidths, '-', '+') << "\n";
			}
			break;
		case Ascii_Table_Dots:
			m_AsciiTableText << _commentCharacters << GenerateDataLine(m_TableData[i], m_ColumnMaxWidths, ':', _whiteSpaceStyle) << "\n";

			if (_forceRowSeparation || (i == m_TableData.size() - 1))
			{
				m_AsciiTableText << _commentCharacters << GenerateSpacerLine(m_ColumnMaxWidths, '·', '·') << "\n";
			}
			break;
		case Ascii_Table_Dots_Curved:
			m_AsciiTableText << _commentCharacters << GenerateDataLine(m_TableData[i], m_ColumnMaxWidths, ':', _whiteSpaceStyle) << "\n";

			if (_forceRowSeparation || (i == m_TableData.size() - 1))
			{
				m_AsciiTableText << _commentCharacters << GenerateSpacerLine(m_ColumnMaxWidths, '·', '·') << "\n";
			}
			break;
		case Ascii_Table_Compact:
			m_AsciiTableText << _commentCharacters << GenerateDataLine(m_TableData[i], m_ColumnMaxWidths, ' ', _whiteSpaceStyle) << "\n";

			if (_forceRowSeparation && i != m_TableData.size() - 1)
			{
				m_AsciiTableText << _commentCharacters << GenerateSpacerLine(m_ColumnMaxWidths, '-', '-') << "\n";
			}
			break;
		case Ascii_Table_Seperated_Header:
			m_AsciiTableText << _commentCharacters << GenerateDataLine(m_TableData[i], m_ColumnMaxWidths, '|', _whiteSpaceStyle) << "\n";

			if (_forceRowSeparation || (i == m_TableData.size() - 1))
			{
				m_AsciiTableText << _commentCharacters << GenerateSpacerLine(m_ColumnMaxWidths, '-', '+') << "\n";
			}
			break;
		case Ascii_Table_Simple:
			m_AsciiTableText << _commentCharacters << GenerateDataLine(m_TableData[i], m_ColumnMaxWidths, ' ', _whiteSpaceStyle) << "\n";

			if (_forceRowSeparation || (i == m_TableData.size() - 1))
			{
				m_AsciiTableText << _commentCharacters << GenerateSpacerLine(m_ColumnMaxWidths, '=', ' ') << "\n";
			}
			break;
		case Ascii_Table_Wavy:
			m_AsciiTableText << _commentCharacters << GenerateDataLine(m_TableData[i], m_ColumnMaxWidths, '¦', _whiteSpaceStyle) << "\n";

			if (_forceRowSeparation || (i == m_TableData.size() - 1))
			{
				m_AsciiTableText << _commentCharacters << GenerateSpacerLine(m_ColumnMaxWidths, '~', '+') << "\n";
			}
			break;
		case ExtendedAscii_Table_MySQL:
			m_AsciiTableText << _commentCharacters << GenerateDataLine(m_TableData[i], m_ColumnMaxWidths, '¦', _whiteSpaceStyle) << "\n";

			if (_forceRowSeparation || (i == m_TableData.size() - 1))
			{
				m_AsciiTableText << _commentCharacters << GenerateSpacerLine(m_ColumnMaxWidths, '-', '+') << "\n";
			}
			break;
		}
	}
}

std::ostringstream& mrt::AsciiTableGenerator::GetAsciiTableStream()
{
	return m_AsciiTableText;		// Returns a string stream object, can call .str() on the function call to get the table string.
}

const std::ostringstream& mrt::AsciiTableGenerator::GetAsciiTableStream() const
{
	return m_AsciiTableText;		// Returns a constant string stream object, can call .str() on the function call to get the table string.
}

std::string mrt::AsciiTableGenerator::GenerateDataLine(const std::vector<std::string>& _rowVector, const std::vector<unsigned int>& _columnMaxWidths, unsigned char _separationCharacter, int _style) const
{
	std::ostringstream oss;

	for (size_t i = 0; i < _rowVector.size(); ++i)
	{
		oss << _separationCharacter << " " << AddWhiteSpaceToString(_rowVector[i], (_columnMaxWidths[i] - _rowVector[i].size()), _style) << " ";
	}
	oss << _separationCharacter;

	return std::forward<std::string>(oss.str());
}

std::string mrt::AsciiTableGenerator::GenerateSpacerLine(const std::vector<unsigned int>& _columnMaxWidths, unsigned char _dataSpaceCharacter, unsigned char _edgesCharacter) const
{
	std::ostringstream oss;

	for (size_t i = 0; i < _columnMaxWidths.size(); ++i)
	{
		oss << _edgesCharacter << AddWhiteSpaceToString("", _columnMaxWidths[i] + 2, WhiteSpaceStyle::LEFT, _dataSpaceCharacter);
	}
	oss << _edgesCharacter;

	return std::forward<std::string>(oss.str());
}

mrt::MarkdownTableGenerator::MarkdownTableGenerator(
	const std::vector<std::string>& _columnNames, 
	const std::vector<std::vector<std::string>>& _tableData,
	const std::vector<unsigned int>& _columnMaxWidths)
	: m_ColumnNames(_columnNames), m_ColumnMaxWidths(_columnMaxWidths), m_TableData(_tableData)
{
	
}

void mrt::MarkdownTableGenerator::GenerateMarkdownTable(int _tableStyle, int _whiteSpaceStyle, bool _boldFirstRow, bool _boldFirstCol)
{
	// Generates the header lines for the table.
	switch (_tableStyle)
	{
	case Markdown_Table_Normal:
		for (size_t i = 0; i < m_ColumnNames.size(); ++i)
		{
			m_MarkdownTableText << "|" << " " << AddWhiteSpaceToString(((_boldFirstCol && i == 0) ? std::format("**{}**", m_ColumnNames[i]) : ((_boldFirstRow) ? std::format("**{}**", m_ColumnNames[i]) : m_ColumnNames[i])),
				(m_ColumnMaxWidths[i] - m_ColumnNames[i].size()), _whiteSpaceStyle) << " ";
		}
		m_MarkdownTableText << "|\n";

		for (size_t i = 0; i < m_ColumnNames.size(); ++i)
		{
			m_MarkdownTableText << '|' << AddWhiteSpaceToString("", m_ColumnMaxWidths[i] + 2, WhiteSpaceStyle::LEFT, ' ');
		}
		m_MarkdownTableText << "|\n";

		break;
	case Markdown_Table_Simple:
		for (size_t i = 0; i < m_ColumnNames.size(); ++i)
		{
			m_MarkdownTableText << " " << AddWhiteSpaceToString(((_boldFirstCol && i == 0) ? std::format("**{}**", m_ColumnNames[i]) : ((_boldFirstRow) ? std::format("**{}**", m_ColumnNames[i]) : m_ColumnNames[i])),
				(m_ColumnMaxWidths[i] - m_ColumnNames[i].size()), _whiteSpaceStyle) << " " << ((i == m_ColumnNames.size() - 1) ? '\n' : '|');
		}

		for (size_t i = 0; i < m_ColumnNames.size(); ++i)
		{
			m_MarkdownTableText << AddWhiteSpaceToString("", m_ColumnMaxWidths[i] + 2, WhiteSpaceStyle::LEFT, ' ') << ((i == m_ColumnNames.size() - 1) ? '\n' : '|');
		}

		break;
	}

	// Generates the data lines for the table.
	for (const std::vector<std::string>& row : m_TableData)
	{
		switch (_tableStyle)
		{
		case Markdown_Table_Normal:
			for (size_t i = 0; i < row.size(); ++i)
			{
				m_MarkdownTableText << "|" << " " << AddWhiteSpaceToString(((_boldFirstCol && i == 0) ? std::format("**{}**", row[i]) : row[i]), (m_ColumnMaxWidths[i] - row[i].size()), _whiteSpaceStyle) << " ";
			}
			m_MarkdownTableText << "|\n";
	
			break;
		case Markdown_Table_Simple:
			for (size_t i = 0; i < row.size(); ++i)
			{
				m_MarkdownTableText << " " << AddWhiteSpaceToString(((_boldFirstCol && i == 0) ? std::format("**{}**", row[i]) : row[i]), (m_ColumnMaxWidths[i] - row[i].size()), _whiteSpaceStyle)
					<< " " << ((i == row.size() - 1) ? '\n' : '|');
			}

			break;
		}
	}
}

std::ostringstream& mrt::MarkdownTableGenerator::GetMarkdownTableStream()
{
	return m_MarkdownTableText;			// Returns a string stream object, can call .str() on the function call to get the table string.
}

const std::ostringstream& mrt::MarkdownTableGenerator::GetMarkdownTableStream() const
{
	return m_MarkdownTableText;			// Returns a constant string stream object, can call .str() on the function call to get the table string.
}