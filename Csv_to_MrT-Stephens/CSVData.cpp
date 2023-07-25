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

mrt::CSVData_Error mrt::CSVData::LoadCsv(const std::string& fileDir, bool onlyAsciiCharacters) noexcept
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
	return CSVData_Error::NONE;
}

mrt::CSVData_Error mrt::CSVData::SaveCsv(const std::string& fileDir) const noexcept
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

void mrt::CSVData::CheckMaxColumnWidths() noexcept
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

std::vector<std::string>& mrt::CSVData::GetHeaderNames() noexcept
{
	return m_HeaderNames;
}

const std::vector<std::string>& mrt::CSVData::GetHeaderNames() const noexcept
{
	return m_HeaderNames;
}

std::vector<std::string>& mrt::CSVData::GetRowData(size_t index) noexcept
{
	return m_Data[index];
}

const std::vector<std::string>& mrt::CSVData::GetRowData(size_t index) const noexcept
{
	return m_Data[index];
}

void mrt::CSVData::SortByColumn(size_t index, bool ascendingOrder) noexcept
{
	std::sort(m_Data.begin(), m_Data.end(), [&index, ascendingOrder](const std::vector<std::string>& a, const std::vector<std::string>& b)->bool 
		{
			return ((ascendingOrder) ? (a[index] < b[index]) : (a[index] > b[index]));
		}
	);
}

std::vector<unsigned int>& mrt::CSVData::GetMaxColumnWidths() noexcept
{
	return m_MaxColumnWidths;
}

const std::vector<unsigned int>& mrt::CSVData::GetMaxColumnWidths() const noexcept
{
	return m_MaxColumnWidths;
}

unsigned int mrt::CSVData::GetMaxColumnWidth(size_t index) const noexcept
{
	return m_MaxColumnWidths[index];
}

void mrt::CSVData::LowerUpperData(bool lower) noexcept
{
	std::locale loc("");

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

void mrt::CSVData::CapitalizeData() noexcept
{
	std::locale loc("");

	for (auto& row : m_Data)
	{
		for (auto& rowVal : row)
		{
			rowVal[0] = std::toupper(rowVal[0], loc);
		}
	}
}

void mrt::CSVData::RemoveWhiteSpace() noexcept
{
	std::locale loc("");

	for (auto& row : m_Data)
	{
		for (auto& rowVal : row)
		{
			rowVal.erase(std::remove_if(rowVal.begin(), rowVal.end(), [&loc](auto c)->bool { return (std::isspace(c, loc)) ? true : false;  }), rowVal.end());
		}
	}

	CheckMaxColumnWidths();
}

void mrt::CSVData::TransposeData() noexcept
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

size_t mrt::CSVData::GetRowCount() const noexcept
{
	return m_Data.size();
}

size_t mrt::CSVData::GetColumnCount() const noexcept
{
	return m_HeaderNames.size();
}

std::vector<std::vector<std::string>>& mrt::CSVData::GetTableData() noexcept
{
	return m_Data;
}

const std::vector<std::vector<std::string>>& mrt::CSVData::GetTableData() const noexcept
{
	return m_Data;
}

void mrt::CSVData::SetValue(size_t row, size_t column, const std::string& value) noexcept
{
	m_Data[row][column] = value;
}

mrt::CSVData_Error mrt::CSVData::GetError() const noexcept
{
	return m_Error;
}