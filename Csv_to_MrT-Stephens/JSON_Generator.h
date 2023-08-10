#pragma once

#include "CSVData.h"
#include "Str_Validator.h"

namespace mrt
{
	enum JSON_Generator_Styles
	{
		JSON_Array_Of_Objects = 0,
		JSON_2D_Arrays = 1,
		JSON_Column_Arrays = 2,
		JSON_Keyed_Arrays = 3,
		JSONLines_Objects = 4,
		JSONLines_Arrays = 5
	};

	  /******************************/
	 /* JSON_Generator Declaration */
	/******************************/

	template <class _StrType>
	class JSON_Generator
	{
	private:
		const mrt::CSVData<_StrType>* const m_CSVData;
	public:
		// Type Aliases
		using ValueType = typename _StrType::value_type;
		using OStream = typename std::basic_ostream<ValueType>;
		using OStrStream = typename std::basic_ostringstream<ValueType>;

		// Constructors
		JSON_Generator(const mrt::CSVData<_StrType>* const _csvData);

		// Copy & Assignment
		JSON_Generator(const JSON_Generator& _other) = delete;
		JSON_Generator(JSON_Generator&& _other) noexcept = delete;
		JSON_Generator& operator=(const JSON_Generator& _other) = delete;

		// Member Functions
		void GenerateJSON(OStream* stream, int jsonStyle, bool parsingJSON, bool wrapperByData, bool newLines);

		static _StrType getTabs(size_t amount, bool newLines);

		static _StrType getNewLine(bool newLines);
	};
}

  /*********************************/
 /* JSON_Generator Implementation */
/*********************************/

template <class _StrType>
mrt::JSON_Generator<_StrType>::JSON_Generator(const mrt::CSVData<_StrType>* const _csvData) : m_CSVData(_csvData) { }

template <class _StrType>
void mrt::JSON_Generator<_StrType>::GenerateJSON(OStream* stream, int jsonStyle, bool parsingJSON, bool wrapperByData, bool newLines)
{
	if (jsonStyle == JSONLines_Objects || jsonStyle == JSONLines_Arrays)   // JSON Lines objects and arrays generators.
	{
		mrt::Basic_Str_Validator<_StrType> validator(mrt::Basic_Str_Filter_Numeric);

		const std::vector<_StrType>& headers = m_CSVData->GetHeaderNames();

		if (jsonStyle == JSONLines_Arrays)
		{
			*stream << "[";

			for (size_t i = 0; i < headers.size(); ++i)
			{
				*stream << "\"" << headers[i] << "\"" << ((i == headers.size() - 1) ? "]\n" : ",");
			}
		}

		for (size_t i0 = 0; i0 < m_CSVData->GetRowCount(); ++i0)
		{
			const std::vector<_StrType>& row = m_CSVData->GetRowData(i0);

			*stream << (jsonStyle == JSONLines_Objects ? "{" : "[");

			for (size_t i1 = 0; i1 < m_CSVData->GetColumnCount(); ++i1)
			{
				if (jsonStyle == JSONLines_Objects)
				{
					*stream << "\"" << headers[i1] << "\":";
				}

				if (parsingJSON && validator.IsValid(row[i1]))
				{
					*stream << std::stoi(row[i1]);
				}
				else
				{
					*stream << "\"" << row[i1] << "\"";
				}

				*stream << ((i1 == m_CSVData->GetColumnCount() - 1) ? (jsonStyle == JSONLines_Objects ? "}\n" : "]\n") : ",");
			}
		}

		return;
	}

	{   // JSON Array of Objects, 2D Arrays, Column Arrays, Keyed Arrays generators.
		size_t tabs = 0;

		*stream << "[" << getNewLine(newLines);

		if (wrapperByData)
		{
			tabs = 1;

			*stream << getTabs(tabs, newLines) << "\"data\": [" << getNewLine(newLines);
		}

		{
			mrt::Basic_Str_Validator<_StrType> validator(mrt::Basic_Str_Filter_Numeric);

			const std::vector<_StrType>& headers = m_CSVData->GetHeaderNames();

			switch (jsonStyle)
			{
			case JSON_Array_Of_Objects:   // JSON Array of Objects generator.
			{
				for (size_t i0 = 0; i0 < m_CSVData->GetRowCount(); ++i0)
				{
					const std::vector<_StrType>& row = m_CSVData->GetRowData(i0);

					*stream << getTabs(tabs + 1, newLines) << "{" << getNewLine(newLines);

					for (size_t i1 = 0; i1 < m_CSVData->GetColumnCount(); ++i1)
					{
						*stream << getTabs(tabs + 2, newLines) << "\"" << headers[i1] << "\": ";

						if (parsingJSON && validator.IsValid(row[i1]))
						{
							*stream << std::stoi(row[i1]);
						}
						else
						{
							*stream << "\"" << row[i1] << "\"";
						}

						if (i1 != m_CSVData->GetColumnCount() - 1)
						{
							*stream << ",";
						}

						*stream << getNewLine(newLines);
					}

					*stream << getTabs(tabs + 1, newLines) << ((i0 == m_CSVData->GetRowCount() - 1) ? "}" : "},") << getNewLine(newLines);
				}
				break;
			}
			case JSON_2D_Arrays:   // JSON 2D Arrays generator.
			{
				*stream << getTabs(tabs + 1, newLines) << "[" << getNewLine(newLines);

				for (size_t i = 0; i < headers.size(); ++i)
				{
					*stream << getTabs(tabs + 2, newLines) << "\"" << headers[i] << "\"" << ((i == headers.size() - 1) ? "" : ",") << getNewLine(newLines);
				}

				*stream << getTabs(tabs + 1, newLines) << "]," << getNewLine(newLines);

				for (size_t i0 = 0; i0 < m_CSVData->GetRowCount(); ++i0)
				{
					const std::vector<_StrType>& row = m_CSVData->GetRowData(i0);

					*stream << getTabs(tabs + 1, newLines) << "[" << getNewLine(newLines);

					for (size_t i1 = 0; i1 < m_CSVData->GetColumnCount(); ++i1)
					{
						*stream << getTabs(tabs + 2, newLines);

						if (parsingJSON && validator.IsValid(row[i1]))
						{
							*stream << std::stoi(row[i1]);
						}
						else
						{
							*stream << "\"" << row[i1] << "\"";
						}

						if (i1 != m_CSVData->GetColumnCount() - 1)
						{
							*stream << ",";
						}

						*stream << getNewLine(newLines);
					}

					*stream << getTabs(tabs + 1, newLines) << ((i0 == m_CSVData->GetRowCount() - 1) ? "]" : "],") << getNewLine(newLines);
				}
				break;
			}
			case JSON_Column_Arrays:   // JSON Column Arrays generator.
			{
				for (size_t i0 = 0; i0 < m_CSVData->GetColumnCount(); ++i0)
				{
					*stream << getTabs(tabs + 1, newLines) << "{" << getNewLine(newLines) << getTabs(tabs + 2, newLines) << "\"" << headers[i0] << "\": [" << getNewLine(newLines);

					for (size_t i1 = 0; i1 < m_CSVData->GetRowCount(); ++i1)
					{
						const _StrType& cell = m_CSVData->GetRowData(i1)[i0];

						*stream << getTabs(tabs + 3, newLines);

						if (parsingJSON && validator.IsValid(cell))
						{
							*stream << std::stoi(cell);
						}
						else
						{
							*stream << "\"" << cell << "\"";
						}

						if (i1 != m_CSVData->GetRowCount() - 1)
						{
							*stream << ",";
						}

						*stream << getNewLine(newLines);
					}

					*stream << getTabs(tabs + 2, newLines) << "]" << getNewLine(newLines) << getTabs(tabs + 1, newLines) << ((i0 == m_CSVData->GetColumnCount() - 1) ? "}" : "},") << getNewLine(newLines);
				}
				break;
			}
			case JSON_Keyed_Arrays:   // JSON Keyed Arrays generator.
			{
				*stream << getTabs(tabs + 1, newLines) << "{" << getNewLine(newLines) << getTabs(tabs + 2, newLines) << "\"0\": [" << getNewLine(newLines);

				for (size_t i = 0; i < headers.size(); ++i)
				{
					*stream << getTabs(tabs + 3, newLines) << "\"" << headers[i] << "\"" << ((i == headers.size() - 1) ? "" : ",") << getNewLine(newLines);
				}

				*stream << getTabs(tabs + 2, newLines) << "]" << getNewLine(newLines) << getTabs(tabs + 1, newLines) << "}," << getNewLine(newLines);

				for (size_t i0 = 0; i0 < m_CSVData->GetRowCount(); ++i0)
				{
					const std::vector<_StrType>& row = m_CSVData->GetRowData(i0);

					*stream << getTabs(tabs + 1, newLines) << "{" << getNewLine(newLines) << getTabs(tabs + 2, newLines) << "\"" << i0 + 1 << "\": [" << getNewLine(newLines);

					for (size_t i1 = 0; i1 < m_CSVData->GetColumnCount(); ++i1)
					{
						*stream << getTabs(tabs + 3, newLines);

						if (parsingJSON && validator.IsValid(row[i1]))
						{
							*stream << std::stoi(row[i1]);
						}
						else
						{
							*stream << "\"" << row[i1] << "\"";
						}

						if (i1 != m_CSVData->GetColumnCount() - 1)
						{
							*stream << ",";
						}

						*stream << getNewLine(newLines);
					}

					*stream << getTabs(tabs + 2, newLines) << "]" << getNewLine(newLines) << getTabs(tabs + 1, newLines) << ((i0 == m_CSVData->GetRowCount() - 1) ? "}" : "},") << getNewLine(newLines);
				}
				break;
			}
			}
		}

		if (wrapperByData)
		{
			*stream << getTabs(tabs, newLines) << "]" << getNewLine(newLines);
		}

		*stream << "]";
	}
}

template <class _StrType>
_StrType mrt::JSON_Generator<_StrType>::getTabs(size_t amount, bool newLines)
{
	return newLines ? _StrType(amount, '\t') : _StrType();
}

template <class _StrType>
_StrType mrt::JSON_Generator<_StrType>::getNewLine(bool newLines)
{
	return newLines ? _StrType(1, '\n') : _StrType();
}