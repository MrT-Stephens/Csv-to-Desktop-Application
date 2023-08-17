#pragma once

#include "CSVData.h"

#include <string>
#include <vector>
#include <sstream>

namespace mrt
{
	  /*************************/
     /* PHP_Table Declaration */
    /*************************/

	template <class _StrType>
	class PHP_Generator
	{
	private:
		const mrt::CSVData<_StrType>* const m_CSVData;
	public:
		// Type Aliases
		using ValueType = typename _StrType::value_type;
		using OStream = typename std::basic_ostream<ValueType>;
		using OStrStream = typename std::basic_ostringstream<ValueType>;

		// Constructors
		PHP_Generator(const mrt::CSVData<_StrType>* const csvData);

		// Copy & Assignment
		PHP_Generator(const PHP_Generator<_StrType>& other) = delete;
		PHP_Generator(PHP_Generator<_StrType>&& other) noexcept = delete;
		PHP_Generator<_StrType>& operator=(const PHP_Generator<_StrType>& other) = delete;

		// Member Functions
		void WriteToStream(OStream* stream);

		static _StrType GetValueString(size_t index, const _StrType& str);
	};
}

  /****************************/
 /* PHP_Table Implementation */
/****************************/

template <class _StrType>
mrt::PHP_Generator<_StrType>::PHP_Generator(const mrt::CSVData<_StrType>* const csvData) : m_CSVData(csvData) { }

template <class _StrType>
void mrt::PHP_Generator<_StrType>::WriteToStream(OStream* stream)
{
	*stream << "array(\n\tarray(";

	{
		const std::vector<_StrType>& headers = m_CSVData->GetHeaderNames();

		for (size_t i = 0; i < headers.size(); ++i)
		{
			*stream << GetValueString(i, headers[i]) << (i == headers.size() - 1 ? ")\n" : ",");
		}
	}

	{
		for (size_t i0 = 0; i0 < m_CSVData->GetRowCount(); ++i0)
		{
			const std::vector<_StrType>& row = m_CSVData->GetRowData(i0);

			*stream << "\tarray(";
			for (size_t i1 = 0; i1 < row.size(); ++i1)
			{
				*stream << GetValueString(i1, row[i1]) << (i1 == row.size() - 1 ? ")\n" : ",");
			}
		}
	}

	*stream << ");";
}

template <class _StrType>
_StrType mrt::PHP_Generator<_StrType>::GetValueString(size_t index, const _StrType& str)
{
	typename std::basic_ostringstream<typename _StrType::value_type> stream;
	stream << "\"val" << index << "\"=>\"" << str << "\"";
	return stream.str();
}