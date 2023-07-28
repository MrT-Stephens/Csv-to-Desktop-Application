#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
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

	class CSVData
	{
	private:
		// Main Items
		std::vector<std::string> m_HeaderNames;
		std::vector<std::vector<std::string>> m_Data;
		std::vector<unsigned int> m_MaxColumnWidths;

		CSVData_Error m_Error = CSVData_Error::NONE;

		// Undo/ Redo Items
		static const size_t m_MaxUndoRedo = 5;

		std::array<std::vector<std::vector<std::string>>, m_MaxUndoRedo> m_UndoData;
		std::array<std::vector<std::vector<std::string>>, m_MaxUndoRedo> m_RedoData;

		std::array<std::vector<std::string>, m_MaxUndoRedo> m_UndoHeaderNames;
		std::array<std::vector<std::string>, m_MaxUndoRedo> m_RedoHeaderNames;

		size_t m_UndoIndex = 0, m_RedoIndex = 0;
	public:
		// Default Constructors
		CSVData(const std::string& filePath, bool onlyAsciiCharacters = false);
		CSVData(std::vector<std::vector<std::string>>&& data = {}, std::vector<std::string>&& headerNames = {});

		// Copy Constructors
		CSVData(const CSVData& other) = delete;
		CSVData(CSVData&& other) noexcept = delete;
		CSVData& operator=(const CSVData& other) = delete;

		~CSVData();

		CSVData_Error LoadCsv(const std::string& fileDir, bool onlyAsciiCharacters) noexcept;
		CSVData_Error SaveCsv(const std::string& fileDir) const noexcept;

		std::vector<std::string>& GetHeaderNames() noexcept;
		const std::vector<std::string>& GetHeaderNames() const noexcept;

		std::vector<std::string>& GetRowData(size_t index) noexcept;
		const std::vector<std::string>& GetRowData(size_t index) const noexcept;

		void SortByColumn(size_t index, bool asendingOrder) noexcept;

		void LowerUpperData(bool includeHeader, bool lower = true) noexcept;

		void CapitalizeData(bool includeHeader) noexcept;

		void RemoveWhiteSpace(bool includeHeader) noexcept;

		void TransposeData() noexcept;

		void CheckMaxColumnWidths() noexcept;

		void CheckXYLengths() noexcept;

		size_t GetRowCount() const noexcept;

		size_t GetColumnCount() const noexcept;

		void SetValue(size_t row, size_t column, const std::string& value) noexcept;

		unsigned int GetMaxColumnWidth(size_t index) const noexcept;

		std::vector<unsigned int>& GetMaxColumnWidths() noexcept;
		const std::vector<unsigned int>& GetMaxColumnWidths() const noexcept;

		CSVData_Error GetError() const noexcept;

		std::vector<std::vector<std::string>>& GetTableData() noexcept;
		const std::vector<std::vector<std::string>>& GetTableData() const noexcept;

		void Undo() noexcept;

		void Redo() noexcept;

		void CreateUndo() noexcept;

		void ClearUndoRedo() noexcept;
	};
}