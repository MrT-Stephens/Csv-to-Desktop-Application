#pragma once

#include <string>

namespace mrt
{
	enum Basic_Str_Validator_Filters
	{
		Basic_Str_Filter_None = 0,
		Basic_Str_Filter_Empty = 1 << 0,
		Basic_Str_Filter_Ascii = 1 << 1,
		Basic_Str_Filter_Alpha = 1 << 2,
		Basic_Str_Filter_AlphaNumeric = 1 << 3,
		Basic_Str_Filter_Numeric = 1 << 4,
		Basic_Str_Filter_Digits = 1 << 5,
		Basic_Str_Filter_Symbols = 1 << 6
	};

	template <class _StrType>
	class Basic_Str_Validator
	{
	private:
		long m_Filters;
		_StrType m_IncludeCharList;
		_StrType m_ExcludeCharList;
	public:
		// Type Aliases
		using _ValueType = typename _StrType::value_type;

		// Constructors
		explicit Basic_Str_Validator(long filters = Basic_Str_Filter_None, const _StrType& includeList = _StrType(), const _StrType& excludeList = _StrType());

		// Copy & Assignment
		Basic_Str_Validator(const Basic_Str_Validator& _other) = delete;
		Basic_Str_Validator(Basic_Str_Validator&& _other) noexcept = delete;
		Basic_Str_Validator& operator=(const Basic_Str_Validator& _other) = delete;

		// Member Functions
		void AddToIncludeList(const _ValueType& c);

		void SetIncludeList(const _StrType& includeList);

		[[nodiscard]] _StrType GetIncludeList() const;

		void AddToExcludeList(const _ValueType& c);

		void SetExcludeList(const _StrType& excludeList);

		[[nodiscard]] _StrType GetExcludeList() const;

		virtual [[nodiscard]] bool IsValid(const _StrType& str) const;

		virtual [[nodiscard]] bool IsValidChar(const _ValueType& c) const;

		[[nodiscard]] bool IsIncluded(const _ValueType& c) const;

		[[nodiscard]] bool IsExcluded(const _ValueType& c) const;

		[[nodiscard]] inline bool HasFlag(const Basic_Str_Validator_Filters& filters) const;

		// Static Validation Functions
		[[nodiscard]] static inline bool IsAscii(_ValueType c);
		[[nodiscard]] static inline bool IsAlpha(_ValueType c);
		[[nodiscard]] static inline bool IsAlphaNumeric(_ValueType c);
		[[nodiscard]] static inline bool IsNumeric(_ValueType c);
		[[nodiscard]] static inline bool IsDigits(_ValueType c);
		[[nodiscard]] static inline bool IsSymbols(_ValueType c);
	};
}

  /**************************************/
 /* Basic_Str_Validator Implementation */
/**************************************/

template <class _StrType>
mrt::Basic_Str_Validator<_StrType>::Basic_Str_Validator(long filters, const _StrType& includeList, const _StrType& excludeList)
	: m_Filters(filters), m_IncludeCharList(includeList), m_ExcludeCharList(excludeList) { }

template <class _StrType>
void mrt::Basic_Str_Validator<_StrType>::AddToIncludeList(const _ValueType& c) { m_IncludeCharList.push_back(c); }

template <class _StrType>
void mrt::Basic_Str_Validator<_StrType>::SetIncludeList(const _StrType& includeList) { m_IncludeCharList = includeList; }

template <class _StrType>
[[nodiscard]] _StrType mrt::Basic_Str_Validator<_StrType>::GetIncludeList() const { return m_IncludeCharList; }

template <class _StrType>
void mrt::Basic_Str_Validator<_StrType>::AddToExcludeList(const _ValueType& c) { m_ExcludeCharList.push_back(c); }

template <class _StrType>
void mrt::Basic_Str_Validator<_StrType>::SetExcludeList(const _StrType& excludeList) { m_ExcludeCharList = excludeList; }

template <class _StrType>
[[nodiscard]] _StrType mrt::Basic_Str_Validator<_StrType>::GetExcludeList() const { return m_ExcludeCharList; }

template <class _StrType>
[[nodiscard]] bool mrt::Basic_Str_Validator<_StrType>::IsValid(const _StrType& str) const
{
	if (m_Filters == Basic_Str_Filter_None) 
		return true;

	if (HasFlag(Basic_Str_Filter_Empty) && str.empty()) return false;

	for (const auto& c : str)
	{
		if (IsIncluded(c)) 
			continue;

		if (!IsValidChar(c)) 
			return false;

		if (IsExcluded(c))
			return false;
	}

	return true;
}

template <class _StrType>
[[nodiscard]] bool mrt::Basic_Str_Validator<_StrType>::IsValidChar(const _ValueType& c) const
{
	if (HasFlag(Basic_Str_Filter_Ascii) && IsAscii(c)) 
		return true;
	if (HasFlag(Basic_Str_Filter_Alpha) && IsAlpha(c)) 
		return true;
	if (HasFlag(Basic_Str_Filter_AlphaNumeric) && IsAlphaNumeric(c)) 
		return true;
	if (HasFlag(Basic_Str_Filter_Numeric) && IsNumeric(c)) 
		return true;
	if (HasFlag(Basic_Str_Filter_Digits) && IsDigits(c)) 
		return true;
	if (HasFlag(Basic_Str_Filter_Symbols) && IsSymbols(c)) 
		return true;

	return false;
}

template <class _StrType>
[[nodiscard]] bool mrt::Basic_Str_Validator<_StrType>::IsIncluded(const _ValueType& c) const { return m_IncludeCharList.find(c) != _StrType::npos; }

template <class _StrType>
[[nodiscard]] bool mrt::Basic_Str_Validator<_StrType>::IsExcluded(const _ValueType& c) const { return m_ExcludeCharList.find(c) != _StrType::npos; }

template <class _StrType>
[[nodiscard]] inline bool mrt::Basic_Str_Validator<_StrType>::HasFlag(const Basic_Str_Validator_Filters& filter) const { return (m_Filters & filter) != 0; }

template <class _StrType>
[[nodiscard]] bool mrt::Basic_Str_Validator<_StrType>::IsAscii(_ValueType c) { return c >= 0 && c <= 127; }

template <class _StrType>
[[nodiscard]] bool mrt::Basic_Str_Validator<_StrType>::IsAlpha(_ValueType c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }

template <class _StrType>
[[nodiscard]] bool mrt::Basic_Str_Validator<_StrType>::IsAlphaNumeric(_ValueType c) { return IsAlpha(c) || IsNumeric(c); }

template <class _StrType>
[[nodiscard]] bool mrt::Basic_Str_Validator<_StrType>::IsNumeric(_ValueType c) { return IsDigits(c) || c == '-' || c == '+' || c == '.'; }

template <class _StrType>
[[nodiscard]] bool mrt::Basic_Str_Validator<_StrType>::IsDigits(_ValueType c) { return c >= '0' && c <= '9'; }

template <class _StrType>
[[nodiscard]] bool mrt::Basic_Str_Validator<_StrType>::IsSymbols(_ValueType c) { return (c >= 33 && c <= 47) || (c >= 58 && c <= 64) || (c >= 91 && c <= 96) || (c >= 123 && c <= 126); }