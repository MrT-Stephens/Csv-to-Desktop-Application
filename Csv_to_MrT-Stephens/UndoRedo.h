#pragma once

#include "CSVData.h"

#include <array>
#include <algorithm>

namespace mrt
{
	template <class _Type, size_t _Size>
	class UndoRedo
	{
	private:
		std::array<_Type, _Size> m_UndoData;
		std::array<_Type, _Size> m_RedoData;

		size_t m_UndoIndex = 0, m_RedoIndex = 0;
	public:
		UndoRedo();

		constexpr size_t Size() const noexcept;

		void CreateUndoData(const _Type* const data) noexcept;

		void Undo(_Type* data) noexcept;

		void Redo(_Type* data) noexcept;

		void Clear() noexcept;
	};
}

template<class _Type, size_t _Size>
mrt::UndoRedo<_Type, _Size>::UndoRedo() : m_UndoData({ _Type() }), m_RedoData({ _Type() }) { }

template<class _Type, size_t _Size>
constexpr size_t mrt::UndoRedo<_Type, _Size>::Size() const noexcept
{
	return _Size;
}

template<class _Type, size_t _Size>
void mrt::UndoRedo<_Type, _Size>::CreateUndoData(const _Type* const data) noexcept
{
	if (m_UndoIndex < _Size)
	{
		m_UndoData[m_UndoIndex] = *data;
		++m_UndoIndex;
	}
	else
	{
		m_UndoIndex = _Size - 1;
		
		std::rotate(m_UndoData.begin(), m_UndoData.begin() + 1, m_UndoData.end());
		std::rotate(m_UndoData.begin(), m_UndoData.begin() + 3, m_UndoData.end());
		
		m_UndoData[m_UndoIndex] = *data;
		
		++m_UndoIndex;
	}
}

template<class _Type, size_t _Size>
void mrt::UndoRedo<_Type, _Size>::Undo(_Type* data) noexcept
{
	if (m_UndoIndex > 0)
	{
		*data = m_UndoData[m_UndoIndex - 1];

		if (m_RedoIndex < _Size)
		{
			std::swap(m_RedoData[m_RedoIndex], m_UndoData[m_UndoIndex]);
			++m_RedoIndex;
		}
		else
		{
			m_RedoIndex = _Size - 1;

			std::rotate(m_RedoData.begin(), m_RedoData.begin() + 1, m_RedoData.end());
			std::rotate(m_RedoData.begin(), m_RedoData.begin() + 3, m_RedoData.end());

			std::swap(m_RedoData[m_RedoIndex], m_UndoData[m_UndoIndex]);

			++m_RedoIndex;
		}

		--m_UndoIndex;
	}
}

template<class _Type, size_t _Size>
void mrt::UndoRedo<_Type, _Size>::Redo(_Type* data) noexcept
{
	if (m_RedoIndex > 0)
	{
		*data = m_RedoData[m_RedoIndex - 1];

		if (m_UndoIndex < _Size)
		{
			std::swap(m_UndoData[m_UndoIndex], m_RedoData[m_RedoIndex]);
			++m_UndoIndex;
		}
		else
		{
			m_UndoIndex = _Size - 1;

			std::rotate(m_UndoData.begin(), m_UndoData.begin() + 1, m_UndoData.end());
			std::rotate(m_UndoData.begin(), m_UndoData.begin() + 3, m_UndoData.end());

			std::swap(m_UndoData[m_UndoIndex], m_RedoData[m_RedoIndex]);

			++m_UndoIndex;
		}

		--m_RedoIndex;
	}
}

template<class _Type, size_t _Size>
void mrt::UndoRedo<_Type, _Size>::Clear() noexcept
{
	m_UndoIndex = 0;
	m_RedoIndex = 0;

	for (size_t i = 0; i < _Size; ++i)
	{
		_Type emptyUndoData;
		std::swap(emptyUndoData, m_UndoData[i]);

		_Type emptyRedoData;
		std::swap(emptyRedoData, m_RedoData[i]);
	}
}