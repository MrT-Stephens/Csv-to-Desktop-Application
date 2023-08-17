#pragma once

#include "CSVData.h"

#include <string>
#include <vector>
#include <sstream>

namespace mrt
{
	  /******************************/
     /* Ruby_Generator Declaration */
    /******************************/

	template <class _StrType>
	class Ruby_Generator
	{
	private:
		const mrt::CSVData<_StrType>* const m_CSVData;
	public:
		// Type Aliases
		using ValueType = typename _StrType::value_type;
		using OStream = typename std::basic_ostream<ValueType>;
		using OStrStream = typename std::basic_ostringstream<ValueType>;

		// Constructors
		Ruby_Generator(const mrt::CSVData<_StrType>* const csvData);

		// Copy & Assignment
		Ruby_Generator(const Ruby_Generator<_StrType>& other) = delete;
		Ruby_Generator(Ruby_Generator<_StrType>&& other) noexcept = delete;
		Ruby_Generator<_StrType>& operator=(const Ruby_Generator<_StrType>& other) = delete;

		// Member Functions
		void WriteToStream(OStream* stream);

		static _StrType GetValueString(size_t index, const _StrType& str);
	};
}

  /*********************************/
 /* Ruby_Generator Implementation */
/*********************************/

template <class _StrType>
mrt::Ruby_Generator<_StrType>::Ruby_Generator(const mrt::CSVData<_StrType>* const csvData) : m_CSVData(csvData) { }

template <class _StrType>
void mrt::Ruby_Generator<_StrType>::WriteToStream(OStream* stream)
{
	*stream << "[\n\t{";

	{
		const std::vector<_StrType>& headers = m_CSVData->GetHeaderNames();

		for (size_t i = 0; i < headers.size(); ++i)
		{
			*stream << std::format(L"\"val{}\"=>\"{}\"", i, headers[i]) << (i == headers.size() - 1 ? "}\n" : ",");
		}
	}

	{
		for (size_t i0 = 0; i0 < m_CSVData->GetRowCount(); ++i0)
		{
			const std::vector<_StrType>& row = m_CSVData->GetRowData(i0);

			*stream << "\t{";
			for (size_t i1 = 0; i1 < row.size(); ++i1)
			{
				*stream << std::format(L"\"val{}\"=>\"{}\"", i1, row[i1]) << (i1 == row.size() - 1 ? "}\n" : ",");
			}
		}
	}

	*stream << "];";
}

template <class _StrType>
_StrType mrt::Ruby_Generator<_StrType>::GetValueString(size_t index, const _StrType& str)
{
	typename std::basic_ostringstream<typename _StrType::value_type> stream;
	stream << "\"val" << index << "\"=>\"" << str << "\"";
	return stream.str();
}