#pragma once

#include "CSVData.h"

#include <string>
#include <vector>
#include <sstream>

namespace mrt
{
	template <class _StrType>
	class HTML_Table
	{
	private:
		const mrt::CSVData<_StrType>* const m_CSVData;
	public:
		// Type Aliases
		using ValueType = typename _StrType::value_type;
		using OStream = typename std::basic_ostream<ValueType>;
		using OStrStream = typename std::basic_ostringstream<ValueType>;

		// Constructors
		HTML_Table(const mrt::CSVData<_StrType>* const csvData);

		// Copy & Assignment
		HTML_Table(const HTML_Table<_StrType>& other) = delete;
		HTML_Table(HTML_Table<_StrType>&& other) noexcept = delete;
		HTML_Table<_StrType>& operator=(const HTML_Table<_StrType>& other) = delete;

		// Member Functions
		void WriteToStream(OStream* stream, bool tbody, bool newLines);

		static _StrType getTabs(size_t amount, bool newLines);

		static _StrType getNewLine(bool newLines);
	};
}

template <class _StrType>
mrt::HTML_Table<_StrType>::HTML_Table(const mrt::CSVData<_StrType>* const csvData) : m_CSVData(csvData) { }

template <class _StrType>
void mrt::HTML_Table<_StrType>::WriteToStream(OStream* stream, bool tbody, bool newLines)
{
	size_t tabs = 0;

	*stream << "<table>" << getNewLine(newLines);

	if (tbody)
	{
		tabs = 1;
		*stream << getTabs(tabs, newLines) << "<thread>" << getNewLine(newLines);
	}

	*stream << getTabs(tabs + 1, newLines) << "<tr>" << getNewLine(newLines);

	{
		const std::vector<_StrType>& header = m_CSVData->GetHeaderNames();

		for (const _StrType& headerName : header)
		{
			*stream << getTabs(tabs + 2, newLines) << "<th>" << headerName << "</th>" << getNewLine(newLines);
		}
	}

	*stream << getTabs(tabs + 1, newLines) << "</tr>" << getNewLine(newLines);

	if (tbody)
	{
		*stream << getTabs(tabs, newLines) << "</thread>" << getNewLine(newLines);
	}

	{
		for (size_t i = 0; i < m_CSVData->GetRowCount(); ++i)
		{
			if (tbody)
			{
				*stream << getTabs(tabs, newLines) << "<tbody>" << getNewLine(newLines);
			}

			*stream << getTabs(tabs + 1, newLines) << "<tr>" << getNewLine(newLines);

			for (const _StrType& rowValue : m_CSVData->GetRowData(i))
			{
				*stream << getTabs(tabs + 2, newLines) << "<td>" << rowValue << "</td>" << getNewLine(newLines);
			}

			*stream << getTabs(tabs + 1, newLines) << "</tr>" << getNewLine(newLines);

			if (tbody)
			{
				*stream << getTabs(tabs, newLines) << "</tbody>" << getNewLine(newLines);
			}
		}
	}

	*stream << "</table>" << getNewLine(newLines);
}

template <class _StrType>
_StrType mrt::HTML_Table<_StrType>::getTabs(size_t amount, bool newLines)
{
	return newLines ? _StrType(amount, '\t') : _StrType();
}

template <class _StrType>
_StrType mrt::HTML_Table<_StrType>::getNewLine(bool newLines)
{
	return newLines ? _StrType(1, '\n') : _StrType();
}