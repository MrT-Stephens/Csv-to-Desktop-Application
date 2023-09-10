#pragma once

#include "XML.h"
#include "external/ZIP.h"

#include <string>

namespace mrt
{
	  /*****************************/
     /* XLSX_Workbook Declaration */
    /*****************************/

	template <class _StrType>
	class XLSX_Workbook
	{
	private:
		_StrType m_WorkbookName;
		std::vector<XLSX_Worksheet<_StrType>> m_Worksheets;
	public:
		// Constructors
		XLSX_Workbook(const _StrType& workbookName);

		// Copy & Assignment
		XLSX_Workbook(const XLSX_Workbook&) = delete;
		XLSX_Workbook(XLSX_Workbook&&) noexcept = delete;
		XLSX_Workbook& operator=(const XLSX_Workbook&) = delete;

		// Member Functions
		void AddSheet(const XLSX_Worksheet<_StrType>& sheet);

		std::vector<XLSX_Worksheet<_StrType>>& GetWorksheets();
		const std::vector<XLSX_Worksheet<_StrType>>& GetWorksheets() const;

		// Static Functions
		static void Save(const _StrType& fileName, const XLSX_Workbook& workbook);
	};

	  /******************************/
     /* XLSX_Worksheet Declaration */
    /******************************/

	template <class _StrType>
	class XLSX_Worksheet
	{
	private:
		_StrType m_SheetName;
		mrt::XML_Document<_StrType> m_Xml_Document;
		std::vector<XLSX_DataRow<_StrType>> m_DataRows;
	public:
		// Constructors
		XLSX_Worksheet(const _StrType& sheetName);

		// Copy & Assignment
		XLSX_Worksheet(const XLSX_Worksheet&) = delete;
		XLSX_Worksheet(XLSX_Worksheet&&) noexcept = delete;
		XLSX_Worksheet& operator=(const XLSX_Worksheet&) = delete;

		// Member Functions
		_StrType GetSheetName() const;
		void SetSheetName(const _StrType& sheetName);

		void AddRow(const XLSX_DataRow<_StrType>& row);
	};

	  /****************************/
     /* XLSX_DataRow Declaration */
    /****************************/

	template <class _StrType>
	class XLSX_DataRow
	{
	private:
		std::vector<XLSX_DataCell<_StrType>> m_Data;
	public:
		// Constructors
		XLSX_DataRow() { }
		XLSX_DataRow(const std::vector<XLSX_DataCell<_StrType>>& row);

		// Copy & Assignment
		XLSX_DataRow(const XLSX_DataRow& other);
		XLSX_DataRow(XLSX_DataRow&& other) noexcept;
		XLSX_DataRow& operator=(const XLSX_DataRow& other);

		// Member Functions
		void AddCell(const XLSX_DataCell<_StrType>& cell);

		std::vector<XLSX_DataCell<_StrType>>& GetCells();
		const std::vector<XLSX_DataCell<_StrType>>& GetCells() const;
	};

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
}