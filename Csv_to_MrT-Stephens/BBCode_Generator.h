#pragma once

#pragma once

#include "CSVData.h"

#include <string>
#include <vector>
#include <sstream>

namespace mrt
{
	  /********************************/
     /* BBCode_Generator Declaration */
    /********************************/

	template <class _StrType>
	class BBCode_Generator
	{
	private:
		const mrt::CSVData<_StrType>* const m_CSVData;
	public:
		// Type Aliases
		using ValueType = typename _StrType::value_type;
		using OStream = typename std::basic_ostream<ValueType>;
		using OStrStream = typename std::basic_ostringstream<ValueType>;

		// Constructors
		BBCode_Generator(const mrt::CSVData<_StrType>* const csvData);

		// Copy & Assignment
		BBCode_Generator(const BBCode_Generator<_StrType>& other) = delete;
		BBCode_Generator(BBCode_Generator<_StrType>&& other) noexcept = delete;
		BBCode_Generator<_StrType>& operator=(const BBCode_Generator<_StrType>& other) = delete;

		// Member Functions
		void WriteToStream(OStream* stream, bool newLines);

		static _StrType getTabs(size_t amount, bool newLines);

		static _StrType getNewLine(bool newLines);
	};
}

  /***********************************/
 /* BBCode_Generator Implementation */
/***********************************/

template <class _StrType>
mrt::BBCode_Generator<_StrType>::BBCode_Generator(const mrt::CSVData<_StrType>* const csvData) : m_CSVData(csvData) { }

template <class _StrType>
void mrt::BBCode_Generator<_StrType>::WriteToStream(OStream* stream, bool newLines)
{
	*stream << "[table]" << getNewLine(newLines);

	*stream << "[tr]" << getNewLine(newLines);

	{
		const std::vector<_StrType>& header = m_CSVData->GetHeaderNames();

		for (const _StrType& headerName : header)
		{
			*stream << getTabs(1, newLines) << "[th]" << headerName << "[/th]" << getNewLine(newLines);
		}
	}

	*stream << "[/tr]" << getNewLine(newLines);

	{
		for (size_t i = 0; i < m_CSVData->GetRowCount(); ++i)
		{
			*stream << "[tr]" << getNewLine(newLines);

			for (const _StrType& rowValue : m_CSVData->GetRowData(i))
			{
				*stream << getTabs(1, newLines) << "[td]" << rowValue << "[/td]" << getNewLine(newLines);
			}

			*stream << "[/tr]" << getNewLine(newLines);
		}
	}

	*stream << "[/table]" << getNewLine(newLines);
}

template <class _StrType>
_StrType mrt::BBCode_Generator<_StrType>::getTabs(size_t amount, bool newLines)
{
	return newLines ? _StrType(amount, '\t') : _StrType();
}

template <class _StrType>
_StrType mrt::BBCode_Generator<_StrType>::getNewLine(bool newLines)
{
	return newLines ? _StrType(1, '\n') : _StrType();
}