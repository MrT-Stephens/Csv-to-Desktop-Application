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

mrt::AsciiTableGenerator::AsciiTableGenerator(const mrt::CSVData<std::string>* const _csvData, int _tableStyle, const std::string& _commentCharacters, bool _forceRowSeparation, int _whiteSpaceStyle)
	: m_CSVData(_csvData), m_TableStyle(_tableStyle), m_CommentCharacters(_commentCharacters), m_ForceRowSeparation(_forceRowSeparation), m_WhiteSpaceStyle(_whiteSpaceStyle)
{
	GenerateAsciiTable();
}

void mrt::AsciiTableGenerator::GenerateAsciiTable()
{
	{
		const std::vector<std::string>& columnNames = m_CSVData->GetHeaderNames();

		// Generates the header lines for the table.
		switch (m_TableStyle)
		{
		case Ascii_Table_MySQL:
			m_AsciiTableText << m_CommentCharacters << GenerateSpacerLine('-', '+') << "\n" << m_CommentCharacters
				<< GenerateDataLine(columnNames, '|') << "\n" << m_CommentCharacters << GenerateSpacerLine('-', '+') << "\n";
			break;
		case Ascii_Table_Dots:
			m_AsciiTableText << m_CommentCharacters << GenerateSpacerLine('·', '·') << "\n" << m_CommentCharacters
				<< GenerateDataLine(columnNames, ':') << "\n" << m_CommentCharacters << GenerateSpacerLine('·', '·') << "\n";
			break;
		case Ascii_Table_Dots_Curved:
			m_AsciiTableText << m_CommentCharacters << GenerateSpacerLine('·', '.') << "\n" << m_CommentCharacters
				<< GenerateDataLine(columnNames, ':') << "\n" << m_CommentCharacters << GenerateSpacerLine('·', '·') << "\n";
			break;
		case Ascii_Table_Compact:
			m_AsciiTableText << m_CommentCharacters << GenerateDataLine(columnNames, ' ') << "\n" << m_CommentCharacters << GenerateSpacerLine('-', '-') << "\n";
			break;
		case Ascii_Table_Seperated_Header:
			m_AsciiTableText << m_CommentCharacters << GenerateSpacerLine('=', '+') << "\n" << m_CommentCharacters
				<< GenerateDataLine(columnNames, '|') << "\n" << m_CommentCharacters << GenerateSpacerLine('=', '+') << "\n";
			break;
		case Ascii_Table_Simple:
			m_AsciiTableText << m_CommentCharacters << GenerateSpacerLine('=', ' ') << "\n" << m_CommentCharacters
				<< GenerateDataLine(columnNames, ' ') << "\n" << m_CommentCharacters << GenerateSpacerLine('=', ' ') << "\n";
			break;
		case Ascii_Table_Wavy:
			m_AsciiTableText << m_CommentCharacters << GenerateSpacerLine('~', '+') << "\n" << m_CommentCharacters
				<< GenerateDataLine(columnNames, '¦') << "\n" << m_CommentCharacters << GenerateSpacerLine('~', '+') << "\n";
			break;
		case ExtendedAscii_Table_MySQL:
			m_AsciiTableText << m_CommentCharacters << GenerateSpacerLine('-', '+') << "\n" << m_CommentCharacters
				<< GenerateDataLine(columnNames, '¦') << "\n" << m_CommentCharacters << GenerateSpacerLine('-', '+') << "\n";
			break;
		}
	}

	// Generates the data lines for the table.
	for (size_t i = 0; i < m_CSVData->GetRowCount(); ++i)
	{
		switch (m_TableStyle)
		{
		case Ascii_Table_MySQL:
			m_AsciiTableText << m_CommentCharacters << GenerateDataLine(m_CSVData->GetRowData(i), '|') << "\n";

			if (m_ForceRowSeparation || (i == m_CSVData->GetRowCount() - 1))
			{
				m_AsciiTableText << m_CommentCharacters << GenerateSpacerLine('-', '+') << "\n";
			}
			break;
		case Ascii_Table_Dots:
			m_AsciiTableText << m_CommentCharacters << GenerateDataLine(m_CSVData->GetRowData(i), ':') << "\n";

			if (m_ForceRowSeparation || (i == m_CSVData->GetRowCount() - 1))
			{
				m_AsciiTableText << m_CommentCharacters << GenerateSpacerLine('·', '·') << "\n";
			}
			break;
		case Ascii_Table_Dots_Curved:
			m_AsciiTableText << m_CommentCharacters << GenerateDataLine(m_CSVData->GetRowData(i), ':') << "\n";

			if (m_ForceRowSeparation || (i == m_CSVData->GetRowCount() - 1))
			{
				m_AsciiTableText << m_CommentCharacters << GenerateSpacerLine('·', '·') << "\n";
			}
			break;
		case Ascii_Table_Compact:
			m_AsciiTableText << m_CommentCharacters << GenerateDataLine(m_CSVData->GetRowData(i), ' ') << "\n";

			if (m_ForceRowSeparation && i != m_CSVData->GetRowCount() - 1)
			{
				m_AsciiTableText << m_CommentCharacters << GenerateSpacerLine('-', '-') << "\n";
			}
			break;
		case Ascii_Table_Seperated_Header:
			m_AsciiTableText << m_CommentCharacters << GenerateDataLine(m_CSVData->GetRowData(i), '|') << "\n";

			if (m_ForceRowSeparation || (i == m_CSVData->GetRowCount() - 1))
			{
				m_AsciiTableText << m_CommentCharacters << GenerateSpacerLine('-', '+') << "\n";
			}
			break;
		case Ascii_Table_Simple:
			m_AsciiTableText << m_CommentCharacters << GenerateDataLine(m_CSVData->GetRowData(i), ' ') << "\n";

			if (m_ForceRowSeparation || (i == m_CSVData->GetRowCount() - 1))
			{
				m_AsciiTableText << m_CommentCharacters << GenerateSpacerLine('=', ' ') << "\n";
			}
			break;
		case Ascii_Table_Wavy:
			m_AsciiTableText << m_CommentCharacters << GenerateDataLine(m_CSVData->GetRowData(i), '¦') << "\n";

			if (m_ForceRowSeparation || (i == m_CSVData->GetRowCount() - 1))
			{
				m_AsciiTableText << m_CommentCharacters << GenerateSpacerLine('~', '+') << "\n";
			}
			break;
		case ExtendedAscii_Table_MySQL:
			m_AsciiTableText << m_CommentCharacters << GenerateDataLine(m_CSVData->GetRowData(i), '¦') << "\n";

			if (m_ForceRowSeparation || (i == m_CSVData->GetRowCount() - 1))
			{
				m_AsciiTableText << m_CommentCharacters << GenerateSpacerLine('-', '+') << "\n";
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

std::string mrt::AsciiTableGenerator::GenerateDataLine(const std::vector<std::string>& _rowVector, unsigned char _separationCharacter) const
{
	std::ostringstream oss;

	for (size_t i = 0; i < _rowVector.size(); ++i)
	{
		oss << _separationCharacter << " " << AddWhiteSpaceToString(_rowVector[i], (m_CSVData->GetMaxColumnWidth(i) - _rowVector[i].size()), m_WhiteSpaceStyle) << " ";
	}
	oss << _separationCharacter;

	return std::forward<std::string>(oss.str());
}

std::string mrt::AsciiTableGenerator::GenerateSpacerLine(unsigned char _dataSpaceCharacter, unsigned char _edgesCharacter) const
{
	std::ostringstream oss;

	for (size_t i = 0; i < m_CSVData->GetColumnCount(); ++i)
	{
		oss << _edgesCharacter << AddWhiteSpaceToString("", m_CSVData->GetMaxColumnWidth(i) + 2, WhiteSpaceStyle::LEFT, _dataSpaceCharacter);
	}
	oss << _edgesCharacter;

	return std::forward<std::string>(oss.str());
}

mrt::MarkdownTableGenerator::MarkdownTableGenerator(const mrt::CSVData<std::string>* const _csvData, int _tableStyle, int _whiteSpaceStyle, bool _boldFirstRow, bool _boldFirstCol)
	: m_CSVData(_csvData), m_TableStyle(_tableStyle), m_WhiteSpaceStyle(_whiteSpaceStyle), m_BoldFirstRow(_boldFirstRow), m_BoldFirstCol(_boldFirstCol)
{
	GenerateMarkdownTable();
}

void mrt::MarkdownTableGenerator::GenerateMarkdownTable()
{
	std::vector<size_t> columnWidths = m_CSVData->GetMaxColumnWidths();

	{
		std::vector<std::string> headerRow = m_CSVData->GetHeaderNames();

		if (m_BoldFirstRow)
		{
			for (size_t i = 0; i < headerRow.size(); ++i)
			{
				headerRow[i] = std::format("**{}**", headerRow[i]);

				columnWidths[i] += 4;
			}
		}
		else if (m_BoldFirstCol)
		{
			headerRow[0] = std::format("**{}**", headerRow[0]);

			columnWidths[0] += 4;
		}

		m_MarkdownTableText << GenerateDataLine(headerRow, columnWidths) << "\n";
	}

	for (size_t i = 0; i < m_CSVData->GetColumnCount(); ++i)
	{
		switch (m_TableStyle)
		{
		case Markdown_Table_Normal:
			m_MarkdownTableText << '|' << AddWhiteSpaceToString("", columnWidths[i] + 2, WhiteSpaceStyle::LEFT, '-') << ((i == m_CSVData->GetColumnCount() - 1) ? "|\n" : "");
			break;
		case Markdown_Table_Simple:
			m_MarkdownTableText << AddWhiteSpaceToString("", columnWidths[i] + 2, WhiteSpaceStyle::LEFT, '-') << ((i == m_CSVData->GetColumnCount() - 1) ? '\n' : '|');
			break;
		}
	}

	for (size_t i = 0; i < m_CSVData->GetRowCount(); ++i)
	{
		std::vector<std::string> row = m_CSVData->GetRowData(i);

		if (m_BoldFirstCol)
		{
			row[0] = std::format("**{}**", row[0]);
		}

		m_MarkdownTableText << GenerateDataLine(row, columnWidths) << "\n";
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

std::string mrt::MarkdownTableGenerator::GenerateDataLine(const std::vector<std::string>& _rowVector, const std::vector<size_t>& _columnWidths) const
{
	std::ostringstream oss;

	switch (m_TableStyle)
	{
	case Markdown_Table_Normal:
		for (size_t i = 0; i < _rowVector.size(); ++i)
		{
			oss << "| " << AddWhiteSpaceToString(_rowVector[i], (_columnWidths[i] - _rowVector[i].size()), m_WhiteSpaceStyle) << ' ';
		}
		oss << '|';
		break;
	case Markdown_Table_Simple:
		for (size_t i = 0; i < _rowVector.size(); ++i)
		{
			oss << ' ' << AddWhiteSpaceToString(_rowVector[i], (_columnWidths[i] - _rowVector[i].size()), m_WhiteSpaceStyle) << ' ';

			if (i != m_CSVData->GetColumnCount() - 1)
			{
				oss << '|';
			}
		}
		break;
	}

	return std::forward<std::string>(oss.str());
}