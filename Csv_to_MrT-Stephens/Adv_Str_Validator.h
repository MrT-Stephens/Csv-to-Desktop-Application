#pragma once

#include <regex>

#include "Basic_Str_Validator.h"

namespace mrt
{
	enum Basic_Str_Validator_Filters
	{
		Adv_Str_Filter_None = 0,
		Adv_Str_Filter_Float = 1 << 20,
		Adv_Str_Filter_Int = 1 << 21
	};

	template <class _StrType>
	class Adv_Str_Validator : public Basic_Str_Validator<_StrType>
	{
	public:
		// Type Aliases
		using Regex = std::basic_regex<typename _StrType::value_type>;
	protected:

	public:
		// Constructors
		Adv_Str_Validator(long filters = Adv_Str_Filter_None, const _StrType& includeList = _StrType(), const _StrType& excludeList = _StrType());

		// Copy & Assignment
		Adv_Str_Validator(const Adv_Str_Validator& _other) = delete;
		Adv_Str_Validator(Adv_Str_Validator&& _other) noexcept = delete;
		Adv_Str_Validator& operator=(const Adv_Str_Validator& _other) = delete;

		// Member Functions
		[[nodiscard]] virtual bool IsValid(const _StrType& str) const override;

		[[nodiscard]] static inline bool IsFloat(const _StrType& str);
		[[nodiscard]] static inline bool IsInt(const _StrType& str);
		[[nodiscard]] static inline bool IsBool(const _StrType& str);
		[[nodiscard]] static inline bool IsDate(const _StrType& str);
		[[nodiscard]] static inline bool IsTime(const _StrType& str);
		[[nodiscard]] static inline bool IsDateTime(const _StrType& str);
	};
}