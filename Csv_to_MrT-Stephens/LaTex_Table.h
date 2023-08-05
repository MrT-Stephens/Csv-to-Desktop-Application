#pragma once

#include "CSVData.h"

namespace mrt
{
	enum LaTex_Allignment
	{
		ALLIGN_LEFT = 0,
		ALLIGN_CENTER = 1,
		ALLIGN_RIGHT = 2
	};

	enum LaTex_Table_Border
	{
		LaTex_Table_Border_All = 0,
		LaTex_Table_Border_MySql = 1,
		LaTex_Table_Border_Excel = 2,
		Latex_Table_Border_Horizontal = 3,
		Latex_Table_Border_Markdown = 4,
		Latex_Table_Border_None = 5
	};

	  /***************************/
	 /* LaTex_Table Declaration */
	/***************************/

	template <class _StrType>
	class LaTex_Table
	{
	private:
		_StrType m_Label;
		_StrType m_Caption;
		bool m_CaptionLocationAbove;
		const mrt::CSVData<_StrType>* const m_CSVData;
	public:
		// Type Aliases
		using ValueType = typename _StrType::value_type;
		using OStream = typename std::basic_ostream<ValueType>;
		using OStrStream = typename std::basic_ostringstream<ValueType>;

		// Constructor
		LaTex_Table(const mrt::CSVData<_StrType>* const csvData, const _StrType& label, const _StrType& caption, bool captionLocationAbove = true);

		// Copy & Assignment
		LaTex_Table(const LaTex_Table& other) = delete;
		LaTex_Table(LaTex_Table&& other) noexcept = delete;
		LaTex_Table& operator=(const LaTex_Table& other) = delete;

		// Member Functions
		void WriteToStream(OStream* stream, int tableStyle, int tableAllignment, int textAllignment, bool hereOrTop, bool minimalWorking);

		static void WriteTabularToStream(OStream* stream, const mrt::CSVData<_StrType>* const csvData, int tableStyle, int textAllignment);

		static _StrType getTabs(size_t amount);
	};
}

  /******************************/
 /* LaTex_Table Implementation */
/******************************/

template <class _StrType>
mrt::LaTex_Table<_StrType>::LaTex_Table(const mrt::CSVData<_StrType>* const csvData, const _StrType& label, const _StrType& caption, bool captionLocationAbove) :
	m_CSVData(csvData), m_Label(label), m_Caption(caption), m_CaptionLocationAbove(captionLocationAbove) { }

template <class _StrType>
void mrt::LaTex_Table<_StrType>::WriteToStream(OStream* stream, int tableStyle, int tableAllignment, int textAllignment, bool hereOrTop, bool minimalWorking)
{
	if (minimalWorking)
	{
		*stream << "\\documentclass{article}\n\\begin{document}\n\n";
	}

	*stream << (hereOrTop ? "\\begin{table}[!ht]\n" : "\\begin{table}\n") << getTabs(1);

	switch (tableAllignment)
	{
	case ALLIGN_LEFT:
		*stream << "\\raggedleft\n";
		break;
	case ALLIGN_CENTER:
		*stream << "\\centering\n";
		break;
	case ALLIGN_RIGHT:
		*stream << "\\raggedright\n";
		break;
	}

	*stream << getTabs(1);

	if (m_CaptionLocationAbove && !m_Caption.empty())
	{
		*stream << "\\caption{" << m_Caption << "}\n" << getTabs(1);
	}

	WriteTabularToStream(stream, m_CSVData, tableStyle, textAllignment);

	if (!m_CaptionLocationAbove && !m_Caption.empty())
	{
		*stream << getTabs(1) << "\\caption{" << m_Caption << "}\n";
	}

	if (!m_Label.empty())
	{
		*stream << getTabs(1) << "\\label{" << m_Label << "}\n";
	}

	*stream << "\\end{table}\n";

	if (minimalWorking)
	{
		*stream << "\n\\end{document}";
	}
}

template <class _StrType>
void mrt::LaTex_Table<_StrType>::WriteTabularToStream(OStream* stream, const mrt::CSVData<_StrType>* const csvData, int tableStyle, int textAllignment)
{
	*stream << "\\begin{tabular}{";

	{
		char textAllign = ' ';

		switch (textAllignment)
		{
		case ALLIGN_LEFT:
			textAllign = 'l';
			break;
		case ALLIGN_CENTER:
			textAllign = 'c';
			break;
		case ALLIGN_RIGHT:
			textAllign = 'r';
			break;
		}

		for (size_t i = 0; i < csvData->GetColumnCount() + 1; ++i)
		{
			if (tableStyle == LaTex_Table_Border_All || tableStyle == LaTex_Table_Border_MySql || tableStyle == Latex_Table_Border_Markdown)
			{
				*stream << "|";
			}
			else if (tableStyle == LaTex_Table_Border_Excel)
			{
				if (i == 0 || i == 1 || i == csvData->GetColumnCount())
				{
					*stream << "|";
				}
			}

			if (i != csvData->GetColumnCount())
			{
				*stream << textAllign;
			}
		}
	}

	*stream << "}\n";

	if ((tableStyle != Latex_Table_Border_Markdown) && (tableStyle != Latex_Table_Border_None))
	{
		*stream << getTabs(1) << "\\hline\n";
	}

	{
		const std::vector<_StrType>& columnNames = csvData->GetHeaderNames();

		*stream << getTabs(2);

		for (size_t i = 0; i < columnNames.size(); ++i)
		{
			*stream << columnNames[i] << ((i == columnNames.size() - 1) ? " \\\\" : " & ");
		}

		if (tableStyle != Latex_Table_Border_None)
		{
			*stream << " \\hline";
		}

		*stream << "\n";

		for (size_t i0 = 0; i0 < csvData->GetRowCount(); ++i0)
		{
			const std::vector<_StrType>& row = csvData->GetRowData(i0);

			*stream << getTabs(2);

			for (size_t i1 = 0; i1 < row.size(); ++i1)
			{
				*stream << row[i1] << ((i1 == row.size() - 1) ? " \\\\" : " & ");
			}

			if (tableStyle == LaTex_Table_Border_All)
			{
				*stream << " \\hline";
			}
			else if ((tableStyle == LaTex_Table_Border_Excel || tableStyle == LaTex_Table_Border_MySql ||
				tableStyle == Latex_Table_Border_Horizontal) && i0 == csvData->GetRowCount() - 1)
			{
				*stream << " \\hline";
			}

			*stream << "\n";
		}
	}

	*stream << getTabs(1) << "\\end{tabular}\n";
}

template <class _StrType>
_StrType mrt::LaTex_Table<_StrType>::getTabs(size_t amount)
{
	return _StrType(amount, '\t');
}