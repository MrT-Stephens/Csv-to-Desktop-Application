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

	template <class _StrType>
	_StrType AddWhiteSpaceToString(_StrType _str, size_t _amount, int _style, typename _StrType::value_type _fillCharacter = ' ');

	enum AsciiTableGenerator_Styles
	{
		Ascii_Table_MySQL = 0,
		Ascii_Table_Dots = 1,
		Ascii_Table_Dots_Curved = 2,
		Ascii_Table_Compact = 3,
		Ascii_Table_Seperated_Header = 4,
		Ascii_Table_Simple = 5,
		Ascii_Table_Wavy = 6,
		ExtendedAscii_Table_MySQL = 7,
		UTF_Table_Single = 8,
		UTF_Table_Double = 9,
		UTF_Table_DoubleInside_SingleOutside = 10,
		UTF_Table_SingleInside_DoubleOutside = 11
	};

	  /***********************************/
	 /* AsciiTableGenerator Declaration */
	/***********************************/

	template <class _StrType>
	class AsciiTableGenerator
	{
	private:
		const mrt::CSVData<_StrType>* const m_CSVData;
	public:
		// Type Aliases
		using ValueType = typename _StrType::value_type;
		using OStream = typename std::basic_ostream<ValueType>;
		using OStrStream = typename std::basic_ostringstream<ValueType>;

		// Constructors
		AsciiTableGenerator(const mrt::CSVData<_StrType>* const _csvData);

		// Copy & Assignment
		AsciiTableGenerator(const AsciiTableGenerator& _other) = delete;
		AsciiTableGenerator(AsciiTableGenerator&& _other) noexcept = delete;
		AsciiTableGenerator& operator=(const AsciiTableGenerator& _other) = delete;

		// Member Functions
		void GenerateAsciiTable(OStream* stream, int _tableStyle, const _StrType& _commentCharacters, bool _forceRowSeparation, int _whiteSpaceStyle);

		_StrType GenerateDataLine(const std::vector<_StrType>& _rowVector, ValueType _separationCharacter, int _whiteSpaceStyle) const;

		_StrType GenerateSpacerLine(ValueType _dataSpaceCharacter, ValueType _edgesCharacter) const;

		_StrType GenerateUTFDataLine(const std::vector<_StrType>& _rowVector, ValueType _rightEdgeCharacter, ValueType _innerSeparationCharacter, ValueType _leftEdgeCharacter, int _whiteSpaceStyle) const;

		_StrType GenerateUTFSpacerLine(ValueType _rightEdgeCharacter, ValueType _innerEdgeCharacter, ValueType _leftEdgeCharacter, ValueType _innerDataSpaceCharacter) const;
	};

	enum MarkdownTableGenerator_Styles
	{
		Markdown_Table_Normal = 0,
		Markdown_Table_Simple = 1
	};

	  /**************************************/
	 /* MarkdownTableGenerator Declaration */
	/**************************************/

	template <class _StrType>
	class MarkdownTableGenerator
	{
	private:
		const mrt::CSVData<_StrType>* const m_CSVData;
	public:
		// Type Aliases
		using ValueType = typename _StrType::value_type;
		using OStream = typename std::basic_ostream<ValueType>;
		using OStrStream = typename std::basic_ostringstream<ValueType>;

		// Constructors
		MarkdownTableGenerator(const mrt::CSVData<_StrType>* const _csvData);

		// Copy & Assignment
		MarkdownTableGenerator(const MarkdownTableGenerator& _other) = delete;
		MarkdownTableGenerator(MarkdownTableGenerator&& _other) noexcept = delete;
		MarkdownTableGenerator& operator=(const MarkdownTableGenerator& _other) = delete;

		// Member Functions
		void GenerateMarkdownTable(OStream* stream, int _tableStyle, int _whiteSpaceStyle, bool _boldFirstRow, bool _boldFirstCol);

		_StrType GenerateDataLine(const std::vector<_StrType>& _rowVector, const std::vector<size_t>& _columnWidths, int _tableStyle, int _whiteSpaceStyle) const;
	};
}



template <class _StrType>
_StrType mrt::AddWhiteSpaceToString(_StrType _str, size_t _amount, int _style, typename _StrType::value_type _fillCharacter)
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

  /**************************************/
 /* AsciiTableGenerator Implementation */
/**************************************/

template <class _StrType>
mrt::AsciiTableGenerator<_StrType>::AsciiTableGenerator(const mrt::CSVData<_StrType>* const _csvData)
	: m_CSVData(_csvData) { }

template <class _StrType>
void mrt::AsciiTableGenerator<_StrType>::GenerateAsciiTable(OStream* stream, int _tableStyle, const _StrType& _commentCharacters, bool _forceRowSeparation, int _whiteSpaceStyle)
{
	{
		const std::vector<_StrType>& columnNames = m_CSVData->GetHeaderNames();

		// Generates the header lines for the table.
		switch (_tableStyle)
		{
		case Ascii_Table_MySQL:
			*stream << _commentCharacters << GenerateSpacerLine('-', '+') << "\n" << _commentCharacters
				<< GenerateDataLine(columnNames, '|', _whiteSpaceStyle) << "\n" << _commentCharacters << GenerateSpacerLine('-', '+') << "\n";
			break;
		case Ascii_Table_Dots:
			*stream << _commentCharacters << GenerateSpacerLine('.', '.') << "\n" << _commentCharacters
				<< GenerateDataLine(columnNames, ':', _whiteSpaceStyle) << "\n" << _commentCharacters << GenerateSpacerLine('.', '.') << "\n";
			break;
		case Ascii_Table_Dots_Curved:
			*stream << _commentCharacters << GenerateSpacerLine(8226, '.') << "\n" << _commentCharacters
				<< GenerateDataLine(columnNames, ':', _whiteSpaceStyle) << "\n" << _commentCharacters << GenerateSpacerLine(8226, 8226) << "\n";
			break;
		case Ascii_Table_Compact:
			*stream << _commentCharacters << GenerateDataLine(columnNames, ' ', _whiteSpaceStyle) << "\n" << _commentCharacters << GenerateSpacerLine('-', '-') << "\n";
			break;
		case Ascii_Table_Seperated_Header:
			*stream << _commentCharacters << GenerateSpacerLine('=', '+') << "\n" << _commentCharacters
				<< GenerateDataLine(columnNames, '|', _whiteSpaceStyle) << "\n" << _commentCharacters << GenerateSpacerLine('=', '+') << "\n";
			break;
		case Ascii_Table_Simple:
			*stream << _commentCharacters << GenerateSpacerLine('=', ' ') << "\n" << _commentCharacters
				<< GenerateDataLine(columnNames, ' ', _whiteSpaceStyle) << "\n" << _commentCharacters << GenerateSpacerLine('=', ' ') << "\n";
			break;
		case Ascii_Table_Wavy:
			*stream << _commentCharacters << GenerateSpacerLine('~', '+') << "\n" << _commentCharacters
				<< GenerateDataLine(columnNames, '|', _whiteSpaceStyle) << "\n" << _commentCharacters << GenerateSpacerLine('~', '+') << "\n";
			break;
		case ExtendedAscii_Table_MySQL:
			*stream << _commentCharacters << GenerateSpacerLine(9552, '+') << "\n" << _commentCharacters
				<< GenerateDataLine(columnNames, 9553, _whiteSpaceStyle) << "\n" << _commentCharacters << GenerateSpacerLine(9552, '+') << "\n";
			break;
		case UTF_Table_Single:
			*stream << _commentCharacters << GenerateUTFSpacerLine(9488, 9516, 9484, 9472) << "\n" << _commentCharacters
				<< GenerateUTFDataLine(columnNames, 9474, 9474, 9474, _whiteSpaceStyle) << "\n" << _commentCharacters << GenerateUTFSpacerLine(9508, 9532, 9500, 9472) << "\n";
			break;
		case UTF_Table_Double:
			*stream << _commentCharacters << GenerateUTFSpacerLine(9559, 9574, 9556, 9552) << "\n" << _commentCharacters
				<< GenerateUTFDataLine(columnNames, 9553, 9553, 9553, _whiteSpaceStyle) << "\n" << _commentCharacters << GenerateUTFSpacerLine(9571, 9580, 9568, 9552) << "\n";
			break;
		case UTF_Table_DoubleInside_SingleOutside:
			*stream << _commentCharacters << GenerateUTFSpacerLine(9488, 9573, 9484, 9472) << "\n" << _commentCharacters
				<< GenerateUTFDataLine(columnNames, 9474, 9553, 9474, _whiteSpaceStyle) << "\n" << _commentCharacters << GenerateUTFSpacerLine(9569, 9580, 9566, 9552) << "\n";
			break;
		case UTF_Table_SingleInside_DoubleOutside:
			*stream << _commentCharacters << GenerateUTFSpacerLine(9559, 9572, 9556, 9552) << "\n" << _commentCharacters
				<< GenerateUTFDataLine(columnNames, 9553, 9474, 9553, _whiteSpaceStyle) << "\n" << _commentCharacters << GenerateUTFSpacerLine(9570, 9532, 9567, 9472) << "\n";
			break;
		}
	}

	// Generates the data lines for the table.
	for (size_t i = 0; i < m_CSVData->GetRowCount(); ++i)
	{
		switch (_tableStyle)
		{
		case Ascii_Table_MySQL:
			*stream << _commentCharacters << GenerateDataLine(m_CSVData->GetRowData(i), '|', _whiteSpaceStyle) << "\n";

			if (_forceRowSeparation || (i == m_CSVData->GetRowCount() - 1))
			{
				*stream << _commentCharacters << GenerateSpacerLine('-', '+') << "\n";
			}
			break;
		case Ascii_Table_Dots:
			*stream << _commentCharacters << GenerateDataLine(m_CSVData->GetRowData(i), ':', _whiteSpaceStyle) << "\n";

			if (_forceRowSeparation || (i == m_CSVData->GetRowCount() - 1))
			{
				*stream << _commentCharacters << GenerateSpacerLine('.', '.') << "\n";
			}
			break;
		case Ascii_Table_Dots_Curved:
			*stream << _commentCharacters << GenerateDataLine(m_CSVData->GetRowData(i), ':', _whiteSpaceStyle) << "\n";

			if (_forceRowSeparation || (i == m_CSVData->GetRowCount() - 1))
			{
				*stream << _commentCharacters << GenerateSpacerLine(8226, 8226) << "\n";
			}
			break;
		case Ascii_Table_Compact:
			*stream << _commentCharacters << GenerateDataLine(m_CSVData->GetRowData(i), ' ', _whiteSpaceStyle) << "\n";

			if (_forceRowSeparation && i != m_CSVData->GetRowCount() - 1)
			{
				*stream << _commentCharacters << GenerateSpacerLine('-', '-') << "\n";
			}
			break;
		case Ascii_Table_Seperated_Header:
			*stream << _commentCharacters << GenerateDataLine(m_CSVData->GetRowData(i), '|', _whiteSpaceStyle) << "\n";

			if (_forceRowSeparation || (i == m_CSVData->GetRowCount() - 1))
			{
				*stream << _commentCharacters << GenerateSpacerLine('-', '+') << "\n";
			}
			break;
		case Ascii_Table_Simple:
			*stream << _commentCharacters << GenerateDataLine(m_CSVData->GetRowData(i), ' ', _whiteSpaceStyle) << "\n";

			if (_forceRowSeparation || (i == m_CSVData->GetRowCount() - 1))
			{
				*stream << _commentCharacters << GenerateSpacerLine('=', ' ') << "\n";
			}
			break;
		case Ascii_Table_Wavy:
			*stream << _commentCharacters << GenerateDataLine(m_CSVData->GetRowData(i), '|', _whiteSpaceStyle) << "\n";

			if (_forceRowSeparation || (i == m_CSVData->GetRowCount() - 1))
			{
				*stream << _commentCharacters << GenerateSpacerLine('~', '+') << "\n";
			}
			break;
		case ExtendedAscii_Table_MySQL:
			*stream << _commentCharacters << GenerateDataLine(m_CSVData->GetRowData(i), 9553, _whiteSpaceStyle) << "\n";

			if (_forceRowSeparation || (i == m_CSVData->GetRowCount() - 1))
			{
				*stream << _commentCharacters << GenerateSpacerLine(9552, '+') << "\n";
			}
			break;
		case UTF_Table_Single:
			*stream << _commentCharacters << GenerateUTFDataLine(m_CSVData->GetRowData(i), 9474, 9474, 9474, _whiteSpaceStyle) << "\n";

			if (_forceRowSeparation && (i < m_CSVData->GetRowCount() - 1))
			{
				*stream << _commentCharacters << GenerateUTFSpacerLine(9508, 9532, 9500, 9472) << "\n";
			}
			else if (i == m_CSVData->GetRowCount() - 1)
			{
				*stream << _commentCharacters << GenerateUTFSpacerLine(9496, 9524, 9492, 9472) << "\n";
			}
			break;
		case UTF_Table_Double:
			*stream << _commentCharacters << GenerateUTFDataLine(m_CSVData->GetRowData(i), 9553, 9553, 9553, _whiteSpaceStyle) << "\n";

			if (_forceRowSeparation && (i < m_CSVData->GetRowCount() - 1))
			{
				*stream << _commentCharacters << GenerateUTFSpacerLine(9571, 9580, 9568, 9552) << "\n";
			}
			else if (i == m_CSVData->GetRowCount() - 1)
			{
				*stream << _commentCharacters << GenerateUTFSpacerLine(9565, 9577, 9562, 9552) << "\n";
			}
			break;
		case UTF_Table_DoubleInside_SingleOutside:
			*stream << _commentCharacters << GenerateUTFDataLine(m_CSVData->GetRowData(i), 9474, 9553, 9474, _whiteSpaceStyle) << "\n";

			if (_forceRowSeparation && (i < m_CSVData->GetRowCount() - 1))
			{
				*stream << _commentCharacters << GenerateUTFSpacerLine(9569, 9580, 9566, 9552) << "\n";
			}
			else if (i == m_CSVData->GetRowCount() - 1)
			{
				*stream << _commentCharacters << GenerateUTFSpacerLine(9496, 9576, 9492, 9472) << "\n";
			}
			break;
		case UTF_Table_SingleInside_DoubleOutside:
			*stream << _commentCharacters << GenerateUTFDataLine(m_CSVData->GetRowData(i), 9553, 9474, 9553, _whiteSpaceStyle) << "\n";

			if (_forceRowSeparation && (i < m_CSVData->GetRowCount() - 1))
			{
				*stream << _commentCharacters << GenerateUTFSpacerLine(9570, 9532, 9567, 9472) << "\n";
			}
			else if (i == m_CSVData->GetRowCount() - 1)
			{
				*stream << _commentCharacters << GenerateUTFSpacerLine(9565, 9575, 9562, 9552) << "\n";
			}
			break;
		}
	}
}

template <class _StrType>
_StrType mrt::AsciiTableGenerator<_StrType>::GenerateDataLine(const std::vector<_StrType>& _rowVector, ValueType _separationCharacter, int _whiteSpaceStyle) const
{
	OStrStream oss;

	for (size_t i = 0; i < _rowVector.size(); ++i)
	{
		oss << _separationCharacter << " " << AddWhiteSpaceToString<_StrType>(_rowVector[i], (m_CSVData->GetMaxColumnWidth(i) - _rowVector[i].size()), _whiteSpaceStyle) << " ";
	}
	oss << _separationCharacter;

	return std::forward<_StrType>(oss.str());
}

template <class _StrType>
_StrType mrt::AsciiTableGenerator<_StrType>::GenerateSpacerLine(ValueType _dataSpaceCharacter, ValueType _edgesCharacter) const
{
	OStrStream oss;

	for (size_t i = 0; i < m_CSVData->GetColumnCount(); ++i)
	{
		oss << _edgesCharacter << AddWhiteSpaceToString<_StrType>(_StrType(), m_CSVData->GetMaxColumnWidth(i) + 2, WhiteSpaceStyle::LEFT, _dataSpaceCharacter);
	}
	oss << _edgesCharacter;

	return std::forward<_StrType>(oss.str());
}

template <class _StrType>
_StrType mrt::AsciiTableGenerator<_StrType>::GenerateUTFDataLine(const std::vector<_StrType>& _rowVector, ValueType _rightEdgeCharacter, ValueType _innerSeparationCharacter, ValueType _leftEdgeCharacter, int _whiteSpaceStyle) const
{
	OStrStream oss;

	oss << _leftEdgeCharacter;

	for (size_t i = 0; i < _rowVector.size(); ++i)
	{
		if (i != 0)
		{
			oss << _innerSeparationCharacter;
		}

		oss << " " << AddWhiteSpaceToString<_StrType>(_rowVector[i], (m_CSVData->GetMaxColumnWidth(i) - _rowVector[i].size()), _whiteSpaceStyle) << " ";
	}
	oss << _rightEdgeCharacter;

	return std::forward<_StrType>(oss.str());
}

template <class _StrType>
_StrType mrt::AsciiTableGenerator<_StrType>::GenerateUTFSpacerLine(ValueType _rightEdgeCharacter, ValueType _innerEdgeCharacter, ValueType _leftEdgeCharacter, ValueType _innerDataSpaceCharacter) const
{
	OStrStream oss;

	oss << _leftEdgeCharacter;

	for (size_t i = 0; i < m_CSVData->GetColumnCount(); ++i)
	{
		if (i != 0)
		{
			oss << _innerEdgeCharacter;
		}

		oss << AddWhiteSpaceToString<_StrType>(_StrType(), m_CSVData->GetMaxColumnWidth(i) + 2, WhiteSpaceStyle::LEFT, _innerDataSpaceCharacter);
	}
	oss << _rightEdgeCharacter;

	return std::forward<_StrType>(oss.str());
}

  /*****************************************/
 /* MarkdownTableGenerator Implementation */
/*****************************************/

template <class _StrType>
mrt::MarkdownTableGenerator<_StrType>::MarkdownTableGenerator(const mrt::CSVData<_StrType>* const _csvData)
	: m_CSVData(_csvData) { }

template <class _StrType>
void mrt::MarkdownTableGenerator<_StrType>::GenerateMarkdownTable(OStream* stream, int _tableStyle, int _whiteSpaceStyle, bool _boldFirstRow, bool _boldFirstCol)
{
	std::vector<size_t> columnWidths = m_CSVData->GetMaxColumnWidths();

	{
		std::vector<_StrType> headerRow = m_CSVData->GetHeaderNames();

		if (_boldFirstRow)
		{
			for (size_t i = 0; i < headerRow.size(); ++i)
			{
				headerRow[i] = _StrType(2, '*') + headerRow[i] + _StrType(2, '*');

				columnWidths[i] += 4;
			}
		}
		else if (_boldFirstCol)
		{
			headerRow[0] = _StrType(2, '*') + headerRow[0] + _StrType(2, '*');

			columnWidths[0] += 4;
		}

		*stream << GenerateDataLine(headerRow, columnWidths, _tableStyle, _whiteSpaceStyle) << "\n";
	}

	for (size_t i = 0; i < m_CSVData->GetColumnCount(); ++i)
	{
		switch (_tableStyle)
		{
		case Markdown_Table_Normal:
			*stream << _StrType(1, '|') << AddWhiteSpaceToString<_StrType>(_StrType(), columnWidths[i] + 2, WhiteSpaceStyle::LEFT, '-') << ((i == m_CSVData->GetColumnCount() - 1) ? "|\n" : "");
			break;
		case Markdown_Table_Simple:
			*stream << AddWhiteSpaceToString<_StrType>(_StrType(), columnWidths[i] + 2, WhiteSpaceStyle::LEFT, '-') << ((i == m_CSVData->GetColumnCount() - 1) ? '\n' : '|');
			break;
		}
	}

	for (size_t i = 0; i < m_CSVData->GetRowCount(); ++i)
	{
		std::vector<_StrType> row = m_CSVData->GetRowData(i);

		if (_boldFirstCol)
		{
			row[0] = _StrType(2, '*') + row[0] + _StrType(2, '*');
		}

		*stream << GenerateDataLine(row, columnWidths, _tableStyle, _whiteSpaceStyle) << "\n";
	}
}

template <class _StrType>
_StrType mrt::MarkdownTableGenerator<_StrType>::GenerateDataLine(const std::vector<_StrType>& _rowVector, const std::vector<size_t>& _columnWidths, int _tableStyle, int _whiteSpaceStyle) const
{
	OStrStream oss;

	switch (_tableStyle)
	{
	case Markdown_Table_Normal:
		for (size_t i = 0; i < _rowVector.size(); ++i)
		{
			oss << "| " << AddWhiteSpaceToString<_StrType>(_rowVector[i], (_columnWidths[i] - _rowVector[i].size()), _whiteSpaceStyle) << ' ';
		}
		oss << '|';
		break;
	case Markdown_Table_Simple:
		for (size_t i = 0; i < _rowVector.size(); ++i)
		{
			oss << ' ' << AddWhiteSpaceToString<_StrType>(_rowVector[i], (_columnWidths[i] - _rowVector[i].size()), _whiteSpaceStyle) << ' ';

			if (i != m_CSVData->GetColumnCount() - 1)
			{
				oss << '|';
			}
		}
		break;
	}

	return std::forward<_StrType>(oss.str());
}