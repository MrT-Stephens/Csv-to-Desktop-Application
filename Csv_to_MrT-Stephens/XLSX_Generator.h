#pragma once

#include "XML.h"
#include "external/ZIP.h"

#include <string>
#include <filesystem>

namespace mrt
{
	/*****************************/
   /* XLSX_DataCell Declaration */
  /*****************************/

	template <class _StrType>
	class XLSX_DataCell
	{
	private:
		_StrType m_Data;
	public:
		// Constructors
		XLSX_DataCell();
		XLSX_DataCell(const _StrType& data);

		// Copy & Assignment
		XLSX_DataCell(const XLSX_DataCell& other);
		XLSX_DataCell(XLSX_DataCell&& other) noexcept;
		XLSX_DataCell& operator=(const XLSX_DataCell& other);

		// Member Functions
		void SetData(const _StrType& data);
		_StrType GetData() const;
	};

	/****************************/
   /* XLSX_DataRow Declaration */
  /****************************/

	template <class _StrType>
	class XLSX_DataRow
	{
	private:
		std::vector<XLSX_DataCell<_StrType>> m_Cells;
	public:
		// Constructors
		XLSX_DataRow();
		XLSX_DataRow(const std::vector<XLSX_DataCell<_StrType>>& row);

		// Copy & Assignment
		XLSX_DataRow(const XLSX_DataRow& other);
		XLSX_DataRow(XLSX_DataRow&& other) noexcept;
		XLSX_DataRow& operator=(const XLSX_DataRow& other);

		// Member Functions
		void AddCell(const XLSX_DataCell<_StrType>& cell);

		XLSX_DataCell<_StrType>& GetCell(size_t index);
		const XLSX_DataCell<_StrType>& GetCell(size_t index) const;

		std::vector<XLSX_DataCell<_StrType>>& GetCells();
		const std::vector<XLSX_DataCell<_StrType>>& GetCells() const;

		size_t GetCellCount() const;
	};

	/*****************************/
   /* XLSX_Workbook Declaration */
  /*****************************/

	enum class XLSX_Errors
	{
		SUCCESS = 0,
		FAILED_TO_OPEN_EMPTY_FILE = 1,
		FAILED_TO_SAVE_FILE = 2
	};

	template <class _StrType>
	class XLSX_Generator
	{
	public:
		// Type Aliases
		using ValueType = typename _StrType::value_type;
		using OStrStream = typename std::basic_ostringstream<typename ValueType>;
	private:
		_StrType m_WorkbookDir;
		XML_Document<_StrType> m_Document;
		std::vector<XLSX_DataRow<_StrType>> m_Rows;
	public:
		// Constructors
		XLSX_Generator(const _StrType& emptyWorkbookDir);

		// Copy & Assignment
		XLSX_Generator(const XLSX_Generator&) = delete;
		XLSX_Generator(XLSX_Generator&&) noexcept = delete;
		XLSX_Generator& operator=(const XLSX_Generator&) = delete;

		// Member Functions
		void AddRow(const XLSX_DataRow<_StrType>& row);

		XLSX_Errors Save(const _StrType& fileName) const;

		void Generate();

		static _StrType GetDimension(size_t rows, size_t cols);
	};
}

/********************************/
/* XLSX_DataCell Implementation */
/********************************/

template <class _StrType>
mrt::XLSX_DataCell<_StrType>::XLSX_DataCell() { }

template <class _StrType>
mrt::XLSX_DataCell<_StrType>::XLSX_DataCell(const _StrType& data) : m_Data(data) { }

template <class _StrType>
mrt::XLSX_DataCell<_StrType>::XLSX_DataCell(const XLSX_DataCell& other) : m_Data(other.m_Data) { }

template <class _StrType>
mrt::XLSX_DataCell<_StrType>::XLSX_DataCell(XLSX_DataCell&& other) noexcept : m_Data(std::move(other.m_Data)) { }

template <class _StrType>
mrt::XLSX_DataCell<_StrType>& mrt::XLSX_DataCell<_StrType>::operator=(const XLSX_DataCell& other) { m_Data = other.m_Data; return *this; }

template <class _StrType>
void mrt::XLSX_DataCell<_StrType>::SetData(const _StrType& data) { m_Data = data; }

template <class _StrType>
_StrType mrt::XLSX_DataCell<_StrType>::GetData() const { return m_Data; }

/*******************************/
/* XLSX_DataRow Implementation */
/*******************************/

template <class _StrType>
mrt::XLSX_DataRow<_StrType>::XLSX_DataRow() { }

template <class _StrType>
mrt::XLSX_DataRow<_StrType>::XLSX_DataRow(const std::vector<XLSX_DataCell<_StrType>>& row) : m_Cells(row) { }

template <class _StrType>
mrt::XLSX_DataRow<_StrType>::XLSX_DataRow(const XLSX_DataRow& other) : m_Cells(other.m_Cells) { }

template <class _StrType>
mrt::XLSX_DataRow<_StrType>::XLSX_DataRow(XLSX_DataRow&& other) noexcept : m_Cells(std::move(other.m_Cells)) { }

template <class _StrType>
mrt::XLSX_DataRow<_StrType>& mrt::XLSX_DataRow<_StrType>::operator=(const XLSX_DataRow& other) { m_Cells = other.m_Cells; return *this; }

template <class _StrType>
void mrt::XLSX_DataRow<_StrType>::AddCell(const XLSX_DataCell<_StrType>& cell) { m_Cells.push_back(cell); }

template <class _StrType>
mrt::XLSX_DataCell<_StrType>& mrt::XLSX_DataRow<_StrType>::GetCell(size_t index) { return m_Cells[index]; }

template <class _StrType>
const mrt::XLSX_DataCell<_StrType>& mrt::XLSX_DataRow<_StrType>::GetCell(size_t index) const { return m_Cells[index]; }

template <class _StrType>
std::vector<mrt::XLSX_DataCell<_StrType>>& mrt::XLSX_DataRow<_StrType>::GetCells() { return m_Cells; }

template <class _StrType>
const std::vector<mrt::XLSX_DataCell<_StrType>>& mrt::XLSX_DataRow<_StrType>::GetCells() const { return m_Cells; }

template <class _StrType>
size_t mrt::XLSX_DataRow<_StrType>::GetCellCount() const { return m_Cells.size(); }

/********************************/
/* XLSX_Workbook Implementation */
/********************************/

template <class _StrType>
mrt::XLSX_Generator<_StrType>::XLSX_Generator(const _StrType& emptyWorkbookDir)
	: m_WorkbookDir(emptyWorkbookDir) { }

template <class _StrType>
void mrt::XLSX_Generator<_StrType>::AddRow(const XLSX_DataRow<_StrType>& row)
{
	m_Rows.push_back(row);
}

template <class _StrType>
mrt::XLSX_Errors mrt::XLSX_Generator<_StrType>::Save(const _StrType& fileName) const
{
	if (!std::filesystem::exists(m_WorkbookDir))
	{
		return XLSX_Errors::FAILED_TO_OPEN_EMPTY_FILE;
	}

	OStrStream xmlBuffer;
	m_Document.WriteDocumentToStream(&xmlBuffer, m_Document, true, false);

	// minizip only supports appending or writing to new files
	// so we must
	// - make a new file
	// - write any new files
	// - copy the old files

	// Create new zip file
	zip_t* new_zip = zip_open(mrt::string_cast<std::string>(fileName).c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');

	// Write out xl/worksheets/sheet1.xml
	zip_entry_open(new_zip, "xl/worksheets/sheet1.xml");

	std::string bu1 = mrt::string_cast<std::string>(xmlBuffer.str());
	const char* buf = bu1.c_str();

	zip_entry_write(new_zip, buf, strlen(buf));
	zip_entry_close(new_zip);

	std::string dir = mrt::string_cast<std::string>(m_WorkbookDir);

	const char* dir_c = dir.c_str();

	// Open the original zip and copy all files.
	zip_t* orig_zip = zip_open(dir_c, ZIP_DEFAULT_COMPRESSION_LEVEL, 'r');

	// Loop & copy each relevant entry in the original zip
	int orig_zip_entry_ct = zip_entries_total(orig_zip);
	for (int i = 0; i < orig_zip_entry_ct; ++i)
	{
		zip_entry_openbyindex(orig_zip, i);
		const char* name = zip_entry_name(orig_zip);

		// Skip copying the original file
		if (std::string(name) != std::string("xl/worksheets/sheet1.xml"))
		{
			// Read the old content
			void* entry_buf;
			size_t entry_buf_size;
			zip_entry_read(orig_zip, &entry_buf, &entry_buf_size);

			// Write into new zip
			zip_entry_open(new_zip, name);
			zip_entry_write(new_zip, entry_buf, entry_buf_size);
			zip_entry_close(new_zip);

			free(entry_buf);
		}

		zip_entry_close(orig_zip);
	}

	// Close both zips
	zip_close(orig_zip);
	zip_close(new_zip);

	return XLSX_Errors::SUCCESS;
}

template <class _StrType>
void mrt::XLSX_Generator<_StrType>::Generate()
{
	m_Document.GetRoot().SetName(L"worksheet");

	mrt::XML_Node<_StrType>& root = m_Document.GetRoot();

	root.EmplaceAttribute(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main").
		EmplaceAttribute(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");

	root.EmplaceChild(L"dimension").EmplaceAttribute(L"ref", GetDimension(m_Rows.size(), (*m_Rows.begin()).GetCellCount()));

	root.EmplaceChild(L"sheetViews").EmplaceChild(L"sheetView").EmplaceAttribute(L"workbookViewId", L"0");

	mrt::XML_Node<_StrType>& sheetData = root.EmplaceChild(L"sheetData");

	for (size_t i0 = 0; i0 < m_Rows.size(); ++i0)
	{
		mrt::XML_Node<_StrType>& row = sheetData.EmplaceChild(L"row").EmplaceAttribute(L"r", std::to_wstring(i0 + 1));

		for (size_t i1 = 0; i1 < m_Rows[i0].GetCellCount(); ++i1)
		{
			row.EmplaceChild(L"c").EmplaceAttribute(L"r", static_cast<wchar_t>(65 + i1) + std::to_wstring(i0 + 1)).
				EmplaceAttribute(L"t", L"str").
				EmplaceChild(L"v", m_Rows[i0].GetCell(i1).GetData());
		}
	}

	root.EmplaceChild(L"ignoredErrors").EmplaceChild(L"ignoredError").
		EmplaceAttribute(L"numberStoredAsText", L"1").
		EmplaceAttribute(L"sqref", GetDimension(m_Rows.size(), (*m_Rows.begin()).GetCellCount()));
}

template <class _StrType>
_StrType mrt::XLSX_Generator<_StrType>::GetDimension(size_t rows, size_t cols)
{
	_StrType dimension = L"A1:";

	if (cols > 26)
	{
		dimension += static_cast<char>(65 + (cols / 26) - 1);
	}

	dimension += static_cast<char>(65 + (cols % 26) - 1);

	dimension += std::to_wstring(rows);

	return dimension;
}