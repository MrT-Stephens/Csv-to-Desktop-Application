#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <numeric>
#include <locale>
#include <array>

namespace mrt
{
	enum class CSVData_Error
	{
		NONE = 0,
		CANNOT_OPEN_FILE = 1,
		FAILED_TO_READ_FILE = 2,
		FAILED_TO_WRITE_FILE = 3
	};

	enum class CSVData_UndoRedoState
	{
		CANT_UNDO = 0,
		CAN_UNDO = 1,
		CANT_REDO = 2,
		CAN_REDO = 3
	};

	  /**********************************/
     /* CSVData_Base Class Declaration */
    /**********************************/

	template <class _StrType>
	class CSVData_Base
	{
	protected:
		std::locale m_Locale;

		// Main Items
		std::vector<_StrType> m_HeaderNames;
		std::vector<std::vector<_StrType>> m_Data;
		std::vector<size_t> m_MaxColumnWidths;

		CSVData_Error m_Error = CSVData_Error::NONE;
	public:
		// Type Aliases
		using StrType = _StrType;
		using ValueType = typename _StrType::value_type;
		using IStream = typename std::basic_istream<ValueType>;
		using OStream = typename std::basic_ostream<ValueType>;
		using StrStream = typename std::basic_stringstream<ValueType>;
		using OStrStream = typename std::basic_ostringstream<ValueType>;
		using IStrStream = typename std::basic_istringstream<ValueType>;
		using OFstream = typename std::basic_ofstream<ValueType>;
		using IFstream = typename std::basic_ifstream<ValueType>;

		// Default Constructors
		CSVData_Base(const _StrType& filePath);
		explicit CSVData_Base(std::vector<std::vector<_StrType>>&& data = {}, std::vector<_StrType>&& headerNames = {});

		// Copy Constructors
		CSVData_Base(const CSVData_Base& other) = delete;
		CSVData_Base(CSVData_Base&& other) noexcept = delete;
		CSVData_Base& operator=(const CSVData_Base& other) = delete;

		// Destructor
		~CSVData_Base();

		// Member Functions
		std::vector<_StrType>& GetHeaderNames();
		const std::vector<_StrType>& GetHeaderNames() const;

		std::vector<_StrType>& GetRowData(size_t index);
		const std::vector<_StrType>& GetRowData(size_t index) const;

		std::vector<size_t>& GetMaxColumnWidths();
		const std::vector<size_t>& GetMaxColumnWidths() const;

		std::vector<std::vector<_StrType>>& GetTableData();
		const std::vector<std::vector<_StrType>>& GetTableData() const;

		size_t GetRowCount() const;
		size_t GetColumnCount() const;

		void SetDataValue(size_t row, size_t column, const _StrType& value);

		size_t GetMaxColumnWidth(size_t index) const;

		CSVData_Error GetError() const;

		const std::locale& GetLocale() const;
		void SetLocale(const std::locale& locale);

		// Static Member Functions
		static void CheckXYLengths(CSVData_Base<_StrType>* const csvData);

		static void CheckMaxColumnWidths(CSVData_Base<_StrType>* const csvData);

		static CSVData_Error LoadCsv(CSVData_Base<_StrType>* const csvData, const _StrType& fileDir);
		static CSVData_Error SaveCsv(const CSVData_Base<_StrType>* const csvData, const _StrType& fileDir, ValueType delimiter, bool includeHeader, bool addQuotes);
		static void SaveCsvToStream(const CSVData_Base<_StrType>* const csvData, OStream* stream, ValueType delimiter, bool includeHeader, bool addQuotes);

		static void ParseCsvLine(const _StrType& line, std::vector<_StrType>& row);
	};

	  /***********************/
     /* CSVData Declaration */
    /***********************/

	template <class _StrType>
	class CSVData : public CSVData_Base<_StrType>
	{
	protected:
		// Undo & Redo Items
		static const size_t m_MaxUndoRedo = 10;   // Max number of undo/redo states that can be stored in one CSVData object.

		std::array<std::vector<std::vector<_StrType>>, m_MaxUndoRedo> m_UndoData;
		std::array<std::vector<std::vector<_StrType>>, m_MaxUndoRedo> m_RedoData;

		std::array<std::vector<_StrType>, m_MaxUndoRedo> m_UndoHeaderNames;
		std::array<std::vector<_StrType>, m_MaxUndoRedo> m_RedoHeaderNames;

		size_t m_UndoIndex = 0, m_RedoIndex = 0;
	public:
		// Default Constructors
		CSVData(const _StrType& filePath);
		explicit CSVData(std::vector<std::vector<_StrType>>&& data = {}, std::vector<_StrType>&& headerNames = {});

		// Copy Constructors
		CSVData(const CSVData& other) = delete;
		CSVData(CSVData&& other) noexcept = delete;
		CSVData& operator=(const CSVData& other) = delete;

		CSVData_UndoRedoState Undo();

		CSVData_UndoRedoState Redo();

		void CreateUndo();

		void ClearUndoRedo();

		static void SortByColumn(CSVData_Base<_StrType>* const csvData, size_t index, bool asendingOrder);

		static void LowerUpperData(CSVData_Base<_StrType>* const csvData, bool includeHeader, bool lowercase);

		static void CapitalizeData(CSVData_Base<_StrType>* const csvData, bool includeHeader);

		static void RemoveWhiteSpace(CSVData_Base<_StrType>* const csvData, bool includeHeader);

		static void TransposeData(CSVData_Base<_StrType>* const csvData);
	};

	  /************************************/
	 /* MrT Global Functions Declaration */
	/************************************/

	template <class _StrType>
	static void CheckForBOM(_StrType* const fileString);
}

  /*******************************/
 /* CSVData_Base Implementation */
/*******************************/

template <class _StrType>
mrt::CSVData_Base<_StrType>::CSVData_Base(const _StrType& filePath)
{
	m_Locale = std::locale("");

	m_Error = LoadCsv(this, filePath);
}

template <class _StrType>
mrt::CSVData_Base<_StrType>::CSVData_Base(std::vector<std::vector<_StrType>>&& data, std::vector<_StrType>&& headerNames) :
	m_Data(std::forward<std::vector<std::vector<_StrType>>>(data)), m_HeaderNames(std::forward<std::vector<_StrType>>(headerNames))
{
	m_Locale = std::locale("");

	CheckXYLengths(this);
	CheckMaxColumnWidths(this);
}

template <class _StrType>
mrt::CSVData_Base<_StrType>::~CSVData_Base() { }

template <class _StrType>
std::vector<_StrType>& mrt::CSVData_Base<_StrType>::GetHeaderNames() { return m_HeaderNames; }

template <class _StrType>
const std::vector<_StrType>& mrt::CSVData_Base<_StrType>::GetHeaderNames() const { return m_HeaderNames; }

template <class _StrType>
std::vector<_StrType>& mrt::CSVData_Base<_StrType>::GetRowData(size_t index) { return m_Data[index]; }

template <class _StrType>
const std::vector<_StrType>& mrt::CSVData_Base<_StrType>::GetRowData(size_t index) const { return m_Data[index]; }

template <class _StrType>
std::vector<size_t>& mrt::CSVData_Base<_StrType>::GetMaxColumnWidths() { return m_MaxColumnWidths; }

template <class _StrType>
const std::vector<size_t>& mrt::CSVData_Base<_StrType>::GetMaxColumnWidths() const { return m_MaxColumnWidths; }

template <class _StrType>
std::vector<std::vector<_StrType>>& mrt::CSVData_Base<_StrType>::GetTableData() { return m_Data; }

template <class _StrType>
const std::vector<std::vector<_StrType>>& mrt::CSVData_Base<_StrType>::GetTableData() const { return m_Data; }

template <class _StrType>
size_t mrt::CSVData_Base<_StrType>::GetRowCount() const { return m_Data.size(); }

template <class _StrType>
size_t mrt::CSVData_Base<_StrType>::GetColumnCount() const { return m_HeaderNames.size(); }

template <class _StrType>
void mrt::CSVData_Base<_StrType>::SetDataValue(size_t row, size_t column, const _StrType& value) { m_Data[row][column] = value; }

template <class _StrType>
size_t mrt::CSVData_Base<_StrType>::GetMaxColumnWidth(size_t index) const { return m_MaxColumnWidths[index]; }

template <class _StrType>
mrt::CSVData_Error mrt::CSVData_Base<_StrType>::GetError() const { return m_Error; }

template <class _StrType>
const std::locale& mrt::CSVData_Base<_StrType>::GetLocale() const { return m_Locale; }

template <class _StrType>
void mrt::CSVData_Base<_StrType>::SetLocale(const std::locale& locale) { m_Locale = locale; }

template <class _StrType>
void mrt::CSVData_Base<_StrType>::CheckXYLengths(CSVData_Base<_StrType>* const csvData)
{
	size_t maxSize = std::max_element(csvData->GetTableData().begin(), csvData->GetTableData().end(), [](const std::vector<_StrType>& a, const std::vector<_StrType>& b)->bool
		{
			return (a.size() < b.size());
		}
	)->size();

	for (std::vector<_StrType>& row : csvData->GetTableData())
	{
		if (row.size() < maxSize)
		{
			row.resize(maxSize, _StrType());
		}
	}
}

template <class _StrType>
void mrt::CSVData_Base<_StrType>::CheckMaxColumnWidths(CSVData_Base<_StrType>* const csvData)
{
	// Resize the columns widths vector
	csvData->GetMaxColumnWidths().resize(csvData->GetColumnCount());

	std::vector<size_t>& maxColumnWidths = csvData->GetMaxColumnWidths();

	{
		const std::vector<_StrType>& headerNames = csvData->GetHeaderNames();

		// Set max column widths to header names string lengths
		for (size_t i = 0; i < csvData->GetColumnCount(); ++i)
		{
			maxColumnWidths[i] = headerNames[i].size();
		}
	}

	// Check for max column string length, by comparing with header names string lengths
	for (size_t i0 = 0; i0 < csvData->GetRowCount(); ++i0)
	{
		const std::vector<_StrType>& row = csvData->GetRowData(i0);

		for (size_t i1 = 0; i1 < csvData->GetColumnCount(); ++i1)
		{
			if (row[i1].size() > maxColumnWidths[i1])
			{
				maxColumnWidths[i1] = row[i1].size();
			}
		}
	}
}

template <class _StrType>
mrt::CSVData_Error mrt::CSVData_Base<_StrType>::LoadCsv(CSVData_Base<_StrType>* const csvData, const _StrType& fileDir)
{
	IFstream file(std::filesystem::path(fileDir), std::ios::in);

	if (!file.is_open())
	{
		return CSVData_Error::CANNOT_OPEN_FILE;
	}

	{
		_StrType line;
		bool firstLoop = true;

		while (std::getline(file, line))
		{
			if (firstLoop)
			{
				firstLoop = false;
				CheckForBOM<_StrType>(&line);					// Check for BOM and remove it if it exists
				ParseCsvLine(line, csvData->GetHeaderNames());  // Parse the header names
			}
			else
			{
				std::vector<_StrType> row;

				ParseCsvLine(line, row);						// Parse the row data
				csvData->GetTableData().push_back(row);			// Add the row data to the table data
			}

			if (file.fail()) { return CSVData_Error::FAILED_TO_READ_FILE; }
		}
	}

	file.close();
	CheckXYLengths(csvData);									// Checks the row lengths to make sure they are all the same, otherwise will insert empty strings.
																// If the row lengths are not the same, it will cause errors within the application.

	CheckMaxColumnWidths(csvData);								// Checks the max column widths. Used when printing tables.
	return CSVData_Error::NONE;
}

template <class _StrType>
mrt::CSVData_Error mrt::CSVData_Base<_StrType>::SaveCsv(const CSVData_Base<_StrType>* const csvData, const _StrType& fileDir, ValueType delimiter, bool includeHeader, bool addQuotes)
{
	OFstream file(std::filesystem::path(fileDir), std::ios::out);

	if (!file.is_open())
	{
		return CSVData_Error::CANNOT_OPEN_FILE;
	}

	if (includeHeader)
	{
		const std::vector<_StrType>& headerNames = csvData->GetHeaderNames();

		// Write Header Names
		for (size_t i = 0; i < csvData->GetColumnCount(); ++i)
		{
			file << (addQuotes ? (_StrType(1, '\"') + headerNames[i] + _StrType(1, '\"')) : headerNames[i]) << ((i != csvData->GetColumnCount() - 1) ? _StrType(1, delimiter) : _StrType(1, '\n'));
		}
	}

	for (const std::vector<_StrType>& row : csvData->GetTableData())
	{
		// Write Row Data
		for (size_t i1 = 0; i1 < row.size(); ++i1)
		{
			file << (addQuotes ? (_StrType(1, '\"') + row[i1] + _StrType(1, '\"')) : row[i1]) << ((i1 != row.size() - 1) ? _StrType(1, delimiter) : _StrType(1, '\n'));
		}
	}

	file.close();
	return CSVData_Error::NONE;
}

template <class _StrType>
void mrt::CSVData_Base<_StrType>::SaveCsvToStream(const CSVData_Base<_StrType>* const csvData, OStream* stream, ValueType delimiter, bool includeHeader, bool addQuotes)
{
	if (includeHeader)
	{
		const std::vector<_StrType>& headerNames = csvData->GetHeaderNames();

		// Write Header Names
		for (size_t i = 0; i < csvData->GetColumnCount(); ++i)
		{
			*stream << (addQuotes ? (_StrType(1, '\"') + headerNames[i] + _StrType(1, '\"')) : headerNames[i]) << ((i != csvData->GetColumnCount() - 1) ? _StrType(1, delimiter) : _StrType(1, '\n'));
		}
	}

	for (const std::vector<_StrType>& row : csvData->GetTableData())
	{
		// Write Row Data
		for (size_t i1 = 0; i1 < row.size(); ++i1)
		{
			*stream << (addQuotes ? (_StrType(1, '\"') + row[i1] + _StrType(1, '\"')) : row[i1]) << ((i1 != row.size() - 1) ? _StrType(1, delimiter) : _StrType(1, '\n'));
		}
	}
}

template <class _StrType>
void mrt::CSVData_Base<_StrType>::ParseCsvLine(const _StrType& line, std::vector<_StrType>& row)
{
	_StrType cell;
	bool inQuotes = false;

	for (const typename _StrType::value_type& c : line)
	{
		if (c == '\"' && !inQuotes)
		{
			inQuotes = true;
		}
		else if (c == '\"' && inQuotes)
		{
			inQuotes = false;
		}
		else if (c == ',' && !inQuotes)
		{
			row.push_back(cell);
			cell = _StrType();
		}
		else
		{
			cell += c;
		}
	}

	if (!cell.empty()) { row.push_back(cell); }
}

  /**************************/
 /* CSVData Implementation */
/**************************/

template <class _StrType>
mrt::CSVData<_StrType>::CSVData(const _StrType& fileDir) : CSVData_Base<_StrType>(fileDir) { }

template <class _StrType>
mrt::CSVData<_StrType>::CSVData(std::vector<std::vector<_StrType>>&& data, std::vector<_StrType>&& headerNames) :
	CSVData_Base<_StrType>(std::forward<std::vector<std::vector<_StrType>>>(data), std::forward<std::vector<_StrType>>(headerNames)) { }

template <class _StrType>
mrt::CSVData_UndoRedoState mrt::CSVData<_StrType>::Undo()
{
	if (m_UndoIndex > 0)
	{
		if (m_UndoData[m_UndoIndex - 1].empty())
		{
			return CSVData_UndoRedoState::CANT_UNDO;
		}

		if (m_RedoIndex < m_MaxUndoRedo)
		{
			m_RedoData[m_RedoIndex].swap(this->m_Data);
			m_RedoHeaderNames[m_RedoIndex].swap(this->m_HeaderNames);
			++m_RedoIndex;
		}
		else
		{
			m_RedoIndex = m_MaxUndoRedo - 1;

			for (size_t i = 1; i < m_MaxUndoRedo; ++i)
			{
				m_RedoData[i - 1].swap(m_RedoData[i]);
				m_RedoHeaderNames[i - 1].swap(m_RedoHeaderNames[i]);
			}

			m_RedoData[m_RedoIndex].swap(this->m_Data);
			m_RedoHeaderNames[m_RedoIndex].swap(this->m_HeaderNames);

			++m_RedoIndex;
		}

		--m_UndoIndex;

		this->m_Data = m_UndoData[m_UndoIndex];
		this->m_HeaderNames = m_UndoHeaderNames[m_UndoIndex];
	}
	else
	{
		return CSVData_UndoRedoState::CANT_UNDO;
	}

	return CSVData_UndoRedoState::CAN_UNDO;
}

template <class _StrType>
mrt::CSVData_UndoRedoState mrt::CSVData<_StrType>::Redo()
{
	if (m_RedoIndex > 0)
	{
		if (m_RedoData[m_RedoIndex - 1].empty())
		{
			return CSVData_UndoRedoState::CANT_REDO;
		}

		if (m_UndoIndex < m_MaxUndoRedo)
		{
			m_UndoData[m_UndoIndex].swap(this->m_Data);
			m_UndoHeaderNames[m_UndoIndex].swap(this->m_HeaderNames);
			++m_UndoIndex;
		}
		else
		{
			m_UndoIndex = m_MaxUndoRedo - 1;

			for (size_t i = 1; i < m_MaxUndoRedo; ++i)
			{
				m_UndoData[i - 1].swap(m_UndoData[i]);
				m_UndoHeaderNames[i - 1].swap(m_UndoHeaderNames[i]);
			}

			m_UndoData[m_UndoIndex].swap(this->m_Data);
			m_UndoHeaderNames[m_UndoIndex].swap(this->m_HeaderNames);

			++m_UndoIndex;
		}

		--m_RedoIndex;

		this->m_Data = m_RedoData[m_RedoIndex];
		this->m_HeaderNames = m_RedoHeaderNames[m_RedoIndex];
	}
	else
	{
		return CSVData_UndoRedoState::CANT_REDO;
	}

	return CSVData_UndoRedoState::CAN_REDO;
}

template <class _StrType>
void mrt::CSVData<_StrType>::CreateUndo()
{
	if (m_UndoIndex < m_MaxUndoRedo)
	{
		m_UndoData[m_UndoIndex] = this->m_Data;
		m_UndoHeaderNames[m_UndoIndex] = this->m_HeaderNames;
		++m_UndoIndex;
	}
	else
	{
		m_UndoIndex = m_MaxUndoRedo - 1;

		for (size_t i = 1; i < m_MaxUndoRedo; ++i)
		{
			m_UndoData[i - 1].swap(m_UndoData[i]);
			m_UndoHeaderNames[i - 1].swap(m_UndoHeaderNames[i]);
		}

		m_UndoData[m_UndoIndex] = this->m_Data;
		m_UndoHeaderNames[m_UndoIndex] = this->m_HeaderNames;

		++m_UndoIndex;
	}
}

template <class _StrType>
void mrt::CSVData<_StrType>::ClearUndoRedo()
{
	for (size_t i = 0; i < m_MaxUndoRedo; ++i)
	{
		m_UndoData[i].clear();
		std::vector<std::vector<_StrType>>().swap(m_UndoData[i]);

		m_RedoData[i].clear();
		std::vector<std::vector<_StrType>>().swap(m_RedoData[i]);

		m_UndoHeaderNames[i].clear();
		std::vector<_StrType>().swap(m_UndoHeaderNames[i]);

		m_RedoHeaderNames[i].clear();
		std::vector<_StrType>().swap(m_RedoHeaderNames[i]);
	}

	m_UndoIndex = 0;
	m_RedoIndex = 0;
}

template <class _StrType>
void mrt::CSVData<_StrType>::SortByColumn(CSVData_Base<_StrType>* const csvData, size_t index, bool asendingOrder)
{
	std::vector<std::vector<_StrType>>& data = csvData->GetTableData();

	std::sort(data.begin(), data.end(), [&index, asendingOrder](const std::vector<_StrType>& a, const std::vector<_StrType>& b)->bool
		{
			return ((asendingOrder) ? (a[index] < b[index]) : (a[index] > b[index]));
		}
	);
}

template <class _StrType>
void mrt::CSVData<_StrType>::LowerUpperData(CSVData_Base<_StrType>* const csvData, bool includeHeader, bool lowercase)
{
	if (includeHeader)
	{
		std::vector<_StrType>& headerNames = csvData->GetHeaderNames();

		for (auto& rowVal : headerNames)
		{
			std::transform(rowVal.begin(), rowVal.end(), rowVal.begin(), [&lowercase, &csvData](_StrType::value_type c)->_StrType::value_type
				{
					return (lowercase) ? std::tolower(c, csvData->GetLocale()) : std::toupper(c, csvData->GetLocale());
				}
			);
		}
	}

	std::vector<std::vector<_StrType>>& data = csvData->GetTableData();

	for (auto& row : data)
	{
		for (auto& rowVal : row)
		{
			std::transform(rowVal.begin(), rowVal.end(), rowVal.begin(), [&lowercase, &csvData](_StrType::value_type c)->_StrType::value_type
				{
					return (lowercase) ? std::tolower(c, csvData->GetLocale()) : std::toupper(c, csvData->GetLocale());
				}
			);
		}
	}
}

template <class _StrType>
void mrt::CSVData<_StrType>::CapitalizeData(CSVData_Base<_StrType>* const csvData, bool includeHeader)
{
	if (includeHeader)
	{
		std::vector<_StrType>& headerNames = csvData->GetHeaderNames();

		for (auto& rowVal : headerNames)
		{
			rowVal[0] = std::toupper(rowVal[0], csvData->GetLocale());
		}
	}

	std::vector<std::vector<_StrType>>& data = csvData->GetTableData();

	for (auto& row : data)
	{
		for (auto& rowVal : row)
		{
			rowVal[0] = std::toupper(rowVal[0], csvData->GetLocale());
		}
	}
}

template <class _StrType>
void mrt::CSVData<_StrType>::RemoveWhiteSpace(CSVData_Base<_StrType>* const csvData, bool includeHeader)
{
	if (includeHeader)
	{
		std::vector<_StrType>& headerNames = csvData->GetHeaderNames();

		for (auto& rowVal : headerNames)
		{
			rowVal.erase(std::remove_if(rowVal.begin(), rowVal.end(), [&csvData](_StrType::value_type c)->bool { return (std::isspace(c, csvData->GetLocale())) ? true : false;  }), rowVal.end());
		}
	}

	std::vector<std::vector<_StrType>>& data = csvData->GetTableData();

	for (auto& row : data)
	{
		for (auto& rowVal : row)
		{
			rowVal.erase(std::remove_if(rowVal.begin(), rowVal.end(), [&csvData](_StrType::value_type c)->bool { return (std::isspace(c, csvData->GetLocale())) ? true : false;  }), rowVal.end());
		}
	}

	csvData->CheckMaxColumnWidths(csvData);
}

template <class _StrType>
void mrt::CSVData<_StrType>::TransposeData(CSVData_Base<_StrType>* const csvData)
{
	std::vector<std::vector<_StrType>> transposedData(std::move(csvData->GetTableData()));
	transposedData.insert(transposedData.begin(), csvData->GetHeaderNames());

	{
		std::vector<_StrType>& headerNames = csvData->GetHeaderNames();
		std::vector<std::vector<_StrType>>& data = csvData->GetTableData();

		data.clear();
		headerNames.clear();

		data.resize(transposedData[0].size() - 1, std::vector<_StrType>(transposedData.size()));
		headerNames.resize(transposedData.size());

		for (size_t i0 = 0; i0 < transposedData.size(); ++i0)
		{
			headerNames[i0] = transposedData[i0][transposedData[0].size() - 1];

			for (size_t i1 = 0; i1 < transposedData[0].size() - 1; ++i1)
			{
				data[i1][i0] = transposedData[i0][i1];
			}
		}
	}

	csvData->CheckMaxColumnWidths(csvData);
}

  /***************************************/
 /* MrT Global Functions Implementation */
/***************************************/

template <>
void mrt::CheckForBOM<std::string>(std::string* const fileString)
{
	if ("\xEF\xBB\xBF" == fileString->substr(0, 3))   // UTF-8 Byte order mark.
	{
		fileString->erase(0, 3);
	}
	else if ("\xFE\xFF" == fileString->substr(0, 2))   // UTF-16 big-endian Byte order mark.
	{
		fileString->erase(0, 2);
	}
	else if ("\xFF\xFE" == fileString->substr(0, 2))   // UTF-16 little-endian Byte order mark.
	{
		fileString->erase(0, 2);
	}
	else if ("\x00\x00\xFE\xFF" == fileString->substr(0, 4))   // UTF-32 big-endian Byte order mark.
	{
		fileString->erase(0, 4);
	}
	else if ("\xFF\xFE\x00\x00" == fileString->substr(0, 4))   // UTF-32 little-endian Byte order mark.
	{
		fileString->erase(0, 4);
	}
}

template <>
void mrt::CheckForBOM<std::wstring>(std::wstring* const fileString)
{
	if (L"\xEF\xBB\xBF" == fileString->substr(0, 3))   // UTF-8 Byte order mark.
	{
		fileString->erase(0, 3);
	}
	else if (L"\xFE\xFF" == fileString->substr(0, 2))   // UTF-16 big-endian Byte order mark.
	{
		fileString->erase(0, 2);
	}
	else if (L"\xFF\xFE" == fileString->substr(0, 2))   // UTF-16 little-endian Byte order mark.
	{
		fileString->erase(0, 2);
	}
	else if (L"\x00\x00\xFE\xFF" == fileString->substr(0, 4))   // UTF-32 big-endian Byte order mark.
	{
		fileString->erase(0, 4);
	}
	else if (L"\xFF\xFE\x00\x00" == fileString->substr(0, 4))   // UTF-32 little-endian Byte order mark.
	{
		fileString->erase(0, 4);
	}
}

template <>
void mrt::CheckForBOM<std::u8string>(std::u8string* const fileString)
{
	if (u8"\xEF\xBB\xBF" == fileString->substr(0, 3))   // UTF-8 Byte order mark.
	{
		fileString->erase(0, 3);
	}
	else if (u8"\xFE\xFF" == fileString->substr(0, 2))   // UTF-16 big-endian Byte order mark.
	{
		fileString->erase(0, 2);
	}
	else if (u8"\xFF\xFE" == fileString->substr(0, 2))   // UTF-16 little-endian Byte order mark.
	{
		fileString->erase(0, 2);
	}
	else if (u8"\x00\x00\xFE\xFF" == fileString->substr(0, 4))   // UTF-32 big-endian Byte order mark.
	{
		fileString->erase(0, 4);
	}
	else if (u8"\xFF\xFE\x00\x00" == fileString->substr(0, 4))   // UTF-32 little-endian Byte order mark.
	{
		fileString->erase(0, 4);
	}
}