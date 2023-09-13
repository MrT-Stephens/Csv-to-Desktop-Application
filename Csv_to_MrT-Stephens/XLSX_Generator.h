#pragma once

#include "XML.h"
#include "external/ZIP.h"

#include <string>

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
		FAILED_TO_OPEN_FILE = 1,
		FAILED_TO_SAVE_FILE = 2
	};

	template <class _StrType>
	class XLSX_Generator
	{
	public:
		// Type Aliases
		using ValueType = typename _StrType::value_type;
		using OStrStream = typename std::basic_ostringstream<ValueType>;
	private:
		_StrType m_WorkbookName;
		XML_Document<_StrType> m_Document;
		std::vector<XLSX_DataRow<_StrType>> m_Rows;
	public:
		// Constructors
		XLSX_Generator(const _StrType& workbookName);

		// Copy & Assignment
		XLSX_Generator(const XLSX_Generator&) = delete;
		XLSX_Generator(XLSX_Generator&&) noexcept = delete;
		XLSX_Generator& operator=(const XLSX_Generator&) = delete;

		// Member Functions
		void AddRow(const XLSX_DataRow<_StrType>& row);

		XLSX_Errors Open(const _StrType& fileName);
		XLSX_Errors Save(const _StrType& fileName) const;

		void Generate();
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
mrt::XLSX_Generator<_StrType>::XLSX_Generator(const _StrType& workbookName)
	: m_WorkbookName(workbookName) { }

template <class _StrType>
void mrt::XLSX_Generator<_StrType>::AddRow(const XLSX_DataRow<_StrType>& row)
{
	m_Rows.push_back(row);
}

template <class _StrType>
mrt::XLSX_Errors mrt::XLSX_Generator<_StrType>::Open(const _StrType& fileName)
{

}

template <class _StrType>
mrt::XLSX_Errors mrt::XLSX_Generator<_StrType>::Save(const _StrType& fileName) const
{

}

template <class _StrType>
void mrt::XLSX_Generator<_StrType>::Generate()
{

}