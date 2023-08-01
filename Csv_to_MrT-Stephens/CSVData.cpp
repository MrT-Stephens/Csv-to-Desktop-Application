#include "CSVData.h"

mrt::CSVData::CSVData(const std::string& filePath, bool onlyAsciiCharacters)
{
	m_Error = LoadCsv(filePath, onlyAsciiCharacters);
}

mrt::CSVData::CSVData(std::vector<std::vector<std::string>>&& data, std::vector<std::string>&& headerNames) :
	m_Data(std::forward<std::vector<std::vector<std::string>>>(data)), m_HeaderNames(std::forward<std::vector<std::string>>(headerNames)) 
{
	CheckMaxColumnWidths();
}

mrt::CSVData::~CSVData() { }

mrt::CSVData_Error mrt::CSVData::LoadCsv(const std::string& fileDir, bool onlyAsciiCharacters)
{
	std::ifstream file(fileDir, std::ios::in);

	if (!file.is_open())
	{
		return CSVData_Error::CANNOT_OPEN_FILE;
	}

	std::string line, word;
	std::stringstream ss;

	// Get Header Names
	std::getline(file, line);
	ss << line;

	while (std::getline(ss, word, ','))
	{
		if (onlyAsciiCharacters)   // Remove non-ascii characters
		{
			word.erase(std::remove_if(word.begin(), word.end(), [](char c) {
				return (static_cast<int>(c) < 0) ? true : false; }), word.end());
		}

		m_HeaderNames.emplace_back(word);
		m_MaxColumnWidths.emplace_back(word.size());
	}

	// Get Data
	while (std::getline(file, line))
	{
		ss.clear();
		ss << line;

		std::vector<std::string> row(m_HeaderNames.size());

		for (size_t i = 0; i < m_HeaderNames.size(); ++i)
		{
			std::getline(ss, word, ',');

			if (onlyAsciiCharacters)   // Remove non-ascii characters
			{
				word.erase(std::remove_if(word.begin(), word.end(), [](char c) {
					return (static_cast<int>(c) < 0) ? true : false; }), word.end());
			}

			row[i] = word;

			if (word.size() > m_MaxColumnWidths[i])   // Check for max column string length
			{
				m_MaxColumnWidths[i] = word.size();
			}
		}

		m_Data.emplace_back(row);
	}

	file.close();
	CheckXYLengths();   // Checks the row lengths to make sure they are all the same, otherwise will insert empty strings
	return CSVData_Error::NONE;
}

mrt::CSVData_Error mrt::CSVData::SaveCsv(const std::string& fileDir) const
{
	std::ofstream file(fileDir, std::ios::out);

	if (!file.is_open())
	{
		return CSVData_Error::CANNOT_OPEN_FILE;
	}

	// Write Header Names
	for (size_t i = 0; i < m_HeaderNames.size(); ++i)
	{
		file << m_HeaderNames[i] << ((i != m_HeaderNames.size() - 1) ? "," : "\n");
	}

	for (const std::vector<std::string>& row : m_Data)
	{
		for (size_t i1 = 0; i1 < row.size(); ++i1)
		{
			file << row[i1] << ((i1 != row.size() - 1) ? "," : "\n");
		}
	}

	file.close();
	return CSVData_Error::NONE;
}

void mrt::CSVData::CheckMaxColumnWidths()
{
	// Resize the columns widths vector
	m_MaxColumnWidths.resize(m_HeaderNames.size());

	// Set max column widths to header names string lengths
	for (size_t i = 0; i < m_HeaderNames.size(); ++i)
	{
		m_MaxColumnWidths[i] = m_HeaderNames[i].size();
	}

	// Check for max column string length, by comparing with header names string lengths
	for (size_t i0 = 0; i0 < m_Data.size(); ++i0)
	{
		for (size_t i1 = 0; i1 < m_Data[0].size(); ++i1)
		{
			if (m_Data[i0][i1].size() > m_MaxColumnWidths[i1])
			{
				m_MaxColumnWidths[i1] = m_Data[i0][i1].size();
			}
		}
	}
}

void mrt::CSVData::CheckXYLengths()
{
	size_t maxSize = std::max_element(m_Data.begin(), m_Data.end(), [](const std::vector<std::string>& a, const std::vector<std::string>& b)->bool
		{
			return (a.size() < b.size());
		}
	)->size();

	for (std::vector<std::string>& row : m_Data)
	{
		if (row.size() < maxSize)
			row.resize(maxSize, "");
	}
}

std::vector<std::string>& mrt::CSVData::GetHeaderNames()
{
	return m_HeaderNames;
}

const std::vector<std::string>& mrt::CSVData::GetHeaderNames() const
{
	return m_HeaderNames;
}

std::vector<std::string>& mrt::CSVData::GetRowData(size_t index)
{
	return m_Data[index];
}

const std::vector<std::string>& mrt::CSVData::GetRowData(size_t index) const
{
	return m_Data[index];
}

void mrt::CSVData::SortByColumn(size_t index, bool ascendingOrder)
{
	std::sort(m_Data.begin(), m_Data.end(), [&index, ascendingOrder](const std::vector<std::string>& a, const std::vector<std::string>& b)->bool 
		{
			return ((ascendingOrder) ? (a[index] < b[index]) : (a[index] > b[index]));
		}
	);
}

std::vector<unsigned int>& mrt::CSVData::GetMaxColumnWidths()
{
	return m_MaxColumnWidths;
}

const std::vector<unsigned int>& mrt::CSVData::GetMaxColumnWidths() const
{
	return m_MaxColumnWidths;
}

unsigned int mrt::CSVData::GetMaxColumnWidth(size_t index) const
{
	return m_MaxColumnWidths[index];
}

void mrt::CSVData::LowerUpperData(bool includeHeader, bool lower)
{
	std::locale loc("");

	if (includeHeader)
	{
		for (auto& rowVal : m_HeaderNames)
		{
			std::transform(rowVal.begin(), rowVal.end(), rowVal.begin(), [&lower, &loc](auto c)->char
				{
					return (lower) ? std::tolower(c, loc) : std::toupper(c, loc);
				}
			);
		}
	}

	for (auto& row : m_Data)
	{
		for (auto& rowVal : row)
		{
			std::transform(rowVal.begin(), rowVal.end(), rowVal.begin(), [&lower, &loc](auto c)->char
				{
					return (lower) ? std::tolower(c, loc) : std::toupper(c, loc);
				}
			);
		}
	}
}

void mrt::CSVData::CapitalizeData(bool includeHeader)
{
	std::locale loc("");

	if (includeHeader)
	{
		for (auto& rowVal : m_HeaderNames)
		{
			rowVal[0] = std::toupper(rowVal[0], loc);
		}
	}

	for (auto& row : m_Data)
	{
		for (auto& rowVal : row)
		{
			rowVal[0] = std::toupper(rowVal[0], loc);
		}
	}
}

void mrt::CSVData::RemoveWhiteSpace(bool includeHeader)
{
	std::locale loc("");

	if (includeHeader)
	{
		for (auto& rowVal : m_HeaderNames)
		{
			rowVal.erase(std::remove_if(rowVal.begin(), rowVal.end(), [&loc](auto c)->bool { return (std::isspace(c, loc)) ? true : false;  }), rowVal.end());
		}
	}

	for (auto& row : m_Data)
	{
		for (auto& rowVal : row)
		{
			rowVal.erase(std::remove_if(rowVal.begin(), rowVal.end(), [&loc](auto c)->bool { return (std::isspace(c, loc)) ? true : false;  }), rowVal.end());
		}
	}

	CheckMaxColumnWidths();
}

void mrt::CSVData::TransposeData()
{
	std::vector<std::vector<std::string>> transposedData(std::move(m_Data));
	transposedData.insert(transposedData.begin(), m_HeaderNames);

	m_Data.clear();
	m_HeaderNames.clear();

	m_Data.resize(transposedData[0].size() - 1, std::vector<std::string>(transposedData.size()));
	m_HeaderNames.resize(transposedData.size());

	for (size_t i0 = 0; i0 < transposedData.size(); ++i0)
	{
		m_HeaderNames[i0] = transposedData[i0][transposedData[0].size() - 1];

		for (size_t i1 = 0; i1 < transposedData[0].size() - 1; ++i1)
		{
			m_Data[i1][i0] = transposedData[i0][i1];
		}
	}
	CheckMaxColumnWidths();
}

size_t mrt::CSVData::GetRowCount() const
{
	return m_Data.size();
}

size_t mrt::CSVData::GetColumnCount() const
{
	return m_HeaderNames.size();
}

std::vector<std::vector<std::string>>& mrt::CSVData::GetTableData()
{
	return m_Data;
}

const std::vector<std::vector<std::string>>& mrt::CSVData::GetTableData() const
{
	return m_Data;
}

void mrt::CSVData::SetValue(size_t row, size_t column, const std::string& value)
{
	m_Data[row][column] = value;
}

mrt::CSVData_Error mrt::CSVData::GetError() const
{
	return m_Error;
}

mrt::CSVData_UndoRedoState mrt::CSVData::Undo()
{
	if (m_UndoIndex > 0)
	{
		if (m_UndoData[m_UndoIndex - 1].empty())
		{
			return CSVData_UndoRedoState::CANT_UNDO;
		}

		if (m_RedoIndex < m_MaxUndoRedo)
		{
			m_RedoData[m_RedoIndex].swap(m_Data);
			m_RedoHeaderNames[m_RedoIndex].swap(m_HeaderNames);
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

			m_RedoData[m_RedoIndex].swap(m_Data);
			m_RedoHeaderNames[m_RedoIndex].swap(m_HeaderNames);

			++m_RedoIndex;
		}

		--m_UndoIndex;

		m_Data = m_UndoData[m_UndoIndex];
		m_HeaderNames = m_UndoHeaderNames[m_UndoIndex];
	}
	else
	{
		return CSVData_UndoRedoState::CANT_UNDO;
	}

	return CSVData_UndoRedoState::CAN_UNDO;
}

mrt::CSVData_UndoRedoState mrt::CSVData::Redo()
{
	if (m_RedoIndex > 0)
	{
		if (m_RedoData[m_RedoIndex - 1].empty())
		{
			return CSVData_UndoRedoState::CANT_REDO;
		}

		if (m_UndoIndex < m_MaxUndoRedo)
		{
			m_UndoData[m_UndoIndex].swap(m_Data);
			m_UndoHeaderNames[m_UndoIndex].swap(m_HeaderNames);
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

			m_UndoData[m_UndoIndex].swap(m_Data);
			m_UndoHeaderNames[m_UndoIndex].swap(m_HeaderNames);

			++m_UndoIndex;
		}

		--m_RedoIndex;

		m_Data = m_RedoData[m_RedoIndex];
		m_HeaderNames = m_RedoHeaderNames[m_RedoIndex];
	}
	else
	{
		return CSVData_UndoRedoState::CANT_REDO;
	}

	return CSVData_UndoRedoState::CAN_REDO;
}

void mrt::CSVData::CreateUndo()
{
	if (m_UndoIndex < m_MaxUndoRedo)
	{
		m_UndoData[m_UndoIndex] = m_Data;
		m_UndoHeaderNames[m_UndoIndex] = m_HeaderNames;
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

		m_UndoData[m_UndoIndex] = m_Data;
		m_UndoHeaderNames[m_UndoIndex] = m_HeaderNames;

		++m_UndoIndex;
	}
}

void mrt::CSVData::ClearUndoRedo()
{
	for (size_t i = 0; i < m_MaxUndoRedo; ++i)
	{
		m_UndoData[i].clear();
		std::vector<std::vector<std::string>>().swap(m_UndoData[i]);

		m_RedoData[i].clear();
		std::vector<std::vector<std::string>>().swap(m_RedoData[i]);

		m_UndoHeaderNames[i].clear();
		std::vector<std::string>().swap(m_UndoHeaderNames[i]);

		m_RedoHeaderNames[i].clear();
		std::vector<std::string>().swap(m_RedoHeaderNames[i]);
	}

	m_UndoIndex = 0;
	m_RedoIndex = 0;
}