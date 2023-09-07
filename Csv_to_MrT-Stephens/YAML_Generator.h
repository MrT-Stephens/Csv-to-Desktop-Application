#pragma once

#include "CSVData.h"
#include "Basic_Str_Validator.h"

#include <string>
#include <vector>
#include <sstream>

namespace mrt
{
	  /******************************/
     /* YAML_Generator Declaration */
    /******************************/

	template <class _StrType>
	class YAML_Generator
	{
	private:
		const mrt::CSVData<_StrType>* const m_CSVData;
	public:
		// Type Aliases
		using ValueType = typename _StrType::value_type;
		using OStream = typename std::basic_ostream<ValueType>;
		using OStrStream = typename std::basic_ostringstream<ValueType>;

		// Constructors
		YAML_Generator(const mrt::CSVData<_StrType>* const csvData);

		// Copy & Assignment
		YAML_Generator(const YAML_Generator<_StrType>& other) = delete;
		YAML_Generator(YAML_Generator<_StrType>&& other) noexcept = delete;
		YAML_Generator<_StrType>& operator=(const YAML_Generator<_StrType>& other) = delete;

		// Member Functions
		void WriteToStream(OStream* stream, bool explictStatement, _StrType quoteType);
	};
}

  /*********************************/
 /* YAML_Generator Implementation */
/*********************************/

template <class _StrType>
mrt::YAML_Generator<_StrType>::YAML_Generator(const mrt::CSVData<_StrType>* const csvData) : m_CSVData(csvData) { }

template <class _StrType>
void mrt::YAML_Generator<_StrType>::WriteToStream(OStream* stream, bool explictStatement, _StrType quoteType)
{
	*stream << "---\n";

	mrt::Basic_Str_Validator<_StrType> intValidator(mrt::Basic_Str_Filter_Digits | mrt::Basic_Str_Filter_Empty);
	mrt::Basic_Str_Validator<_StrType> floatValidator(mrt::Basic_Str_Filter_Numeric | mrt::Basic_Str_Filter_Empty);

	const std::vector<_StrType>& header = m_CSVData->GetHeaderNames();

	for (size_t i0 = 0; i0 < m_CSVData->GetRowCount(); ++i0)
	{
		*stream << "-\n";

		const std::vector<_StrType>& row = m_CSVData->GetRowData(i0);

		for (size_t i1 = 0; i1 < header.size(); ++i1)
		{
			if (intValidator.IsValid(row[i1]))
			{
				*stream << "    " << header[i1] << ": " << row[i1] << "\n";
			}
			else if (floatValidator.IsValid(row[i1]))
			{
				*stream << "    " << header[i1] << ": " << (explictStatement ? "!!float " : "") << row[i1] << "\n";
			}
			else
			{
				*stream << "    " << header[i1] << ": " << (explictStatement ? "!!str " : "") << quoteType << row[i1] << quoteType << "\n";
			}
		}
	}
}