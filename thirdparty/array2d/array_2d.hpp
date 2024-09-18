#pragma once
#ifndef _ARRAY_2D_
#define _ARRAY_2D_
#include <yvals_core.h>
#include <algorithm>
#if _STL_COMPILER_PREPROCESSOR
#include <xmemory>

#if _HAS_CXX17
#include <xpolymorphic_allocator.h>
#endif // _HAS_CXX17

#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

_STD_BEGIN
template <class _Size_t>
struct _Array_2d_range {
    _Array_2d_range() = default;

    _NODISCARD _CONSTEXPR20 bool _In_range(const _Size_t _Row, const _Size_t _Col) const noexcept {
        return _Col_begin <= _Col && _Col < _Col_end && _Row_begin <= _Row && _Row < _Row_end;
    }

    _NODISCARD _CONSTEXPR20 _Size_t _Row_size() const noexcept {
        return _Row_end - _Row_begin;
    }

    _NODISCARD _CONSTEXPR20 _Size_t _Col_size() const noexcept {
        return _Col_end - _Col_begin;
    }

    _NODISCARD _CONSTEXPR20 bool operator==(const _Array_2d_range& _Right) const noexcept {
        if (_Row_begin != _Right._Row_begin || _Row_end != _Right._Row_end) return false;
        if (_Col_begin != _Right._Col_begin || _Col_end != _Right._Col_end) return false;
        return true;
    }

    _Size_t _Row_begin;
    _Size_t _Row_end;
    _Size_t _Col_begin;
    _Size_t _Col_end;
};

template <class _Myarr>
class _Array_2d_iterator_base : public _Iterator_base {
private:
    using size_type = typename _Myarr::size_type;
    using pointer   = typename _Myarr::pointer;

public:
#if _ITERATOR_DEBUG_LEVEL == 2
    _NODISCARD virtual _CONSTEXPR20 bool _In_range(const _Array_2d_range<size_type>& _Range) const noexcept = 0;

    _NODISCARD virtual _CONSTEXPR20 bool _In_range(pointer _First, pointer _Last) const noexcept = 0;
#endif // _ITERATOR_DEBUG_LEVEL == 2
};

template <class _Myarr>
class _Array_2d_iterator : public _Array_2d_iterator_base<_Myarr> {
private:
    template <class, class>
    friend class array_2d;

public:
    using iterator_category = random_access_iterator_tag;
    using value_type        = typename _Myarr::value_type;
    using size_type         = typename _Myarr::size_type;
    using difference_type   = typename _Myarr::difference_type;
    using pointer           = typename _Myarr::pointer;
    using reference         = value_type&;

    using _Tptr = typename _Myarr::pointer;

    _CONSTEXPR20 _Array_2d_iterator() noexcept : _Ptr() {}

    _CONSTEXPR20 _Array_2d_iterator(_Tptr _Parg, const _Container_base* _Parray) noexcept : _Ptr(_Parg) {
        this->_Adopt(_Parray);
    }

    _NODISCARD _CONSTEXPR20 reference operator*() const noexcept {
#if _ITERATOR_DEBUG_LEVEL != 0
        const auto _Mycont = static_cast<const _Myarr*>(this->_Getcont());
        _STL_VERIFY(_Ptr, "can't dereference value-initialized array_2d iterator");
        _STL_VERIFY(_Mycont->_Myfirst <= _Ptr && _Ptr < _Mycont->_Mylast, 
            "can't dereference out of range array_2d iterator");
#endif // _ITERATOR_DEBUG_LEVEL != 0

        return *_Ptr;
    }

    _NODISCARD _CONSTEXPR20 pointer operator->() const noexcept {
#if _ITERATOR_DEBUG_LEVEL != 0
        const auto _Mycont = static_cast<const _Myarr*>(this->_Getcont());
        _STL_VERIFY(_Ptr, "can't dereference value-initialized array_2d iterator");
        _STL_VERIFY(
            _Mycont->_Myfirst <= _Ptr && _Ptr < _Mycont->_Mylast, 
            "can't dereference out of range array_2d iterator");
#endif // _ITERATOR_DEBUG_LEVEL != 0

        return _Ptr;
    }

    _CONSTEXPR20 _Array_2d_iterator& operator++() noexcept {
#if _ITERATOR_DEBUG_LEVEL != 0
        const auto _Mycont = static_cast<const _Myarr*>(this->_Getcont());
        _STL_VERIFY(_Ptr, "can't increment value-initialized array_2d iterator");
        _STL_VERIFY(_Ptr < _Mycont->_Mylast, "can't increment array_2d iterator past end");
#endif // _ITERATOR_DEBUG_LEVEL != 0

        ++_Ptr;
        return *this;
    }

    _CONSTEXPR20 _Array_2d_iterator operator++(int) noexcept {
        _Array_2d_iterator _Tmp = *this;
        ++*this;
        return _Tmp;
    }

    _CONSTEXPR20 _Array_2d_iterator& operator--() noexcept {
#if _ITERATOR_DEBUG_LEVEL != 0
        const auto _Mycont = static_cast<const _Myarr*>(this->_Getcont());
        _STL_VERIFY(_Ptr, "can't decrement value-initialized array_2d iterator");
        _STL_VERIFY(_Mycont->_Myfirst < _Ptr, "can't decrement array_2d iterator before begin");
#endif // _ITERATOR_DEBUG_LEVEL != 0

        --_Ptr;
        return *this;
    }

    _CONSTEXPR20 _Array_2d_iterator operator--(int) noexcept {
        _Array_2d_iterator _Tmp = *this;
        --*this;
        return _Tmp;
    }

    _CONSTEXPR20 void _Verify_offset(const difference_type _Off) const noexcept {
#if _ITERATOR_DEBUG_LEVEL == 0
        (void)_Off;
#else // ^^^ _ITERATOR_DEBUG_LEVEL == 0 ^^^ // vvv _ITERATOR_DEBUG_LEVEL != 0 vvv
        const auto _Mycont = static_cast<const _Myarr*>(this->_Getcont());
        _STL_VERIFY(_Off == 0 || _Ptr, "cannot seek value-initialized array_2d iterator");
        if (_Off < 0) {
            _STL_VERIFY(_Off >= _Mycont->_Myfirst - _Ptr, "cannot seek array_2d iterator before begin");
        }

        if (_Off > 0) {
            _STL_VERIFY(_Off <= _Mycont->_Mylast - _Ptr, "cannot seek array_2d iterator after end");
        }
#endif // _ITERATOR_DEBUG_LEVEL == 0
    }

    _CONSTEXPR20 _Array_2d_iterator& operator+=(const difference_type _Off) noexcept {
        _Verify_offset(_Off);
        _Ptr += _Off;
        return *this;
    }

    _NODISCARD _CONSTEXPR20 _Array_2d_iterator operator+(const difference_type _Off) const noexcept {
        _Array_2d_iterator _Tmp = *this;
        _Tmp += _Off;
        return _Tmp;
    }

    _CONSTEXPR20 _Array_2d_iterator& operator-=(const difference_type _Off) noexcept {
        return *this += -_Off;
    }

    _NODISCARD _CONSTEXPR20 _Array_2d_iterator operator-(const difference_type _Off) const noexcept {
        _Array_2d_iterator _Tmp = *this;
        _Tmp -= _Off;
        return _Tmp;
    }

    _NODISCARD _CONSTEXPR20 difference_type operator-(const _Array_2d_iterator& _Right) const noexcept {
        _Compat(_Right);
        return _Ptr - _Right._Ptr;
    }

    _NODISCARD _CONSTEXPR20 reference operator[](const difference_type _Off) const noexcept {
        return *(*this + _Off);
    }

    _NODISCARD _CONSTEXPR20 bool operator==(const _Array_2d_iterator& _Right) const noexcept {
        _Compat(_Right);
        return _Ptr == _Right._Ptr;
    }

    _NODISCARD bool operator!=(const _Array_2d_iterator& _Right) const noexcept {
        return !(*this == _Right);
    }

    _NODISCARD bool operator<(const _Array_2d_iterator& _Right) const noexcept {
        _Compat(_Right);
        return _Ptr < _Right._Ptr;
    }

    _NODISCARD bool operator>(const _Array_2d_iterator& _Right) const noexcept {
        return _Right < *this;
    }

    _NODISCARD bool operator<=(const _Array_2d_iterator& _Right) const noexcept {
        return !(_Right < *this);
    }

    _NODISCARD bool operator>=(const _Array_2d_iterator& _Right) const noexcept {
        return !(*this < _Right);
    }

    _CONSTEXPR20 void _Compat(const _Array_2d_iterator& _Right) const noexcept {
        // test for compatible iterator pair
#if _ITERATOR_DEBUG_LEVEL == 0
        (void)_Right;
#else // ^^^ _ITERATOR_DEBUG_LEVEL == 0 ^^^ // vvv _ITERATOR_DEBUG_LEVEL != 0 vvv
        _STL_VERIFY(this->_Getcont() == _Right._Getcont(), "array_2d iterators incompatible");
#endif // _ITERATOR_DEBUG_LEVEL == 0
    }

    friend _CONSTEXPR20 void _Verify_range(
        const _Array_2d_iterator& _First, const _Array_2d_iterator& _Last) noexcept {
        _STL_VERIFY(_First._Getcont() == _Last._Getcont(), "array_2d iterators in range are from different containers");
        _STL_VERIFY(_First._Ptr <= _Last._Ptr, "array_2d iterator range transposed");
    }

    _NODISCARD _CONSTEXPR20 value_type* _Unwrapped() const noexcept {
        return _Unfancy(_Ptr);
    }

    _CONSTEXPR20 void _Seek_to(const value_type* _It) noexcept {
        _Ptr = _Refancy<_Tptr>(const_cast<value_type*>(_It));
    }

#if _ITERATOR_DEBUG_LEVEL == 2
    _NODISCARD virtual _CONSTEXPR20 bool _In_range(const _Array_2d_range<size_type>& _Range) const noexcept {
        const auto _Mycont   = static_cast<const _Myarr*>(this->_Getcont());
        const auto _Distance = _Ptr - _Mycont->_Myfirst;
        const auto _Col      = _Mycont->_Col;
        return _Range._In_range(_Distance / _Col, _Distance % _Col);
    }

    _NODISCARD virtual _CONSTEXPR20 bool _In_range(pointer _First, pointer _Last) const noexcept {
        return _First <= _Ptr && _Ptr <= _Last;
    }
#endif // _ITERATOR_DEBUG_LEVEL == 2

protected:
    _Tptr _Ptr; // pointer to element in array_2d
};

template <class _Myarr>
_NODISCARD _CONSTEXPR20 _Array_2d_iterator<_Myarr> operator+(
    typename _Array_2d_iterator<_Myarr>::difference_type _Off, _Array_2d_iterator<_Myarr> _Next) noexcept {
    _Next += _Off;
    return _Next;
}

template <class _Myarr>
class _Array_2d_iterator_ex_base : public _Array_2d_iterator_base<_Myarr> {
private:
    template <class>
    friend class _Array_2d_limited_iter;

    using _Mybase = _Array_2d_iterator<_Myarr>;

public:
    using value_type        = typename _Myarr::value_type;
    using size_type         = typename _Myarr::size_type;
    using difference_type   = typename _Myarr::difference_type;
    using pointer           = typename _Myarr::pointer;
    using reference         = value_type&;

    using _Tptr = typename _Myarr::pointer;

public:
    _CONSTEXPR20 _Array_2d_iterator_ex_base() noexcept : _Row(), _Col() {}

    _CONSTEXPR20 _Array_2d_iterator_ex_base(const _Array_2d_iterator_ex_base& _Right)
        noexcept : _Row(_Right._Row), _Col(_Right._Col) {

        this->_Adopt(_Right._Getcont());
        _Update_Ptr();
    }

    _CONSTEXPR20 _Array_2d_iterator_ex_base(const size_type _Row, const size_type _Col, const _Container_base* _Parray) 
        noexcept : _Row(_Row), _Col(_Col) {
#if _ITERATOR_DEBUG_LEVEL != 0
        const auto _Mycont = static_cast<const _Myarr*>(_Parray);
        _STL_VERIFY(_Row <= _Mycont->_Row, "array_2d extended iterator constructor out of range");
        _STL_VERIFY(_Col <= _Mycont->_Col, "array_2d extended iterator constructor out of range");
#endif // _ITERATOR_DEBUG_LEVEL != 0

        this->_Adopt(_Parray);
        _Update_Ptr();
    }

    _NODISCARD _CONSTEXPR20 reference operator*() const noexcept {
#if _ITERATOR_DEBUG_LEVEL != 0
        const auto _Mycont = static_cast<const _Myarr*>(this->_Getcont());
        _STL_VERIFY(_Ptr, "can't dereference value-initialized array_2d iterator");
        _STL_VERIFY(_Col < _Mycont->_Col && _Row < _Mycont->_Row,
            "can't dereference out of range array_2d iterator");
#endif // _ITERATOR_DEBUG_LEVEL != 0

        return *_Ptr;
    }

    _NODISCARD _CONSTEXPR20 pointer operator->() const noexcept {
#if _ITERATOR_DEBUG_LEVEL != 0
        const auto _Mycont = static_cast<const _Myarr*>(this->_Getcont());
        _STL_VERIFY(_Ptr, "can't dereference value-initialized array_2d iterator");
        _STL_VERIFY(_Col < _Mycont->_Col && _Row < _Mycont->_Row,
            "can't dereference out of range array_2d iterator");
#endif // _ITERATOR_DEBUG_LEVEL != 0

        return _Ptr;
    }

    _NODISCARD _CONSTEXPR20 size_type idx_row() const noexcept {
        return _Row;
    }

    _NODISCARD _CONSTEXPR20 size_type idx_col() const noexcept {
        return _Col;
    }

    friend _CONSTEXPR20 void _Verify_range(
        const _Array_2d_iterator_ex_base& _First, const _Array_2d_iterator_ex_base& _Last) noexcept {
        _STL_VERIFY(_First._Getcont() == _Last._Getcont(), "array_2d iterators in range are from different containers");
        _STL_VERIFY(_First._Ptr <= _Last._Ptr, "array_2d iterator range transposed");
    }

    _CONSTEXPR20 void _Seek_to(const value_type* _It) noexcept {
        _Ptr = _Refancy<_Tptr>(const_cast<value_type*>(_It));
    }

#if _ITERATOR_DEBUG_LEVEL == 2
    _NODISCARD virtual _CONSTEXPR20 bool _In_range(const _Array_2d_range<size_type>& _Range) const noexcept {
        return _Range._In_range(_Row, _Col);
    }

    _NODISCARD virtual _CONSTEXPR20 bool _In_range(pointer _First, pointer _Last) const noexcept {
        return _First <= _Ptr && _Ptr <= _Last;
    }
#endif // _ITERATOR_DEBUG_LEVEL == 2

protected:
    struct { size_type _Row, _Col; };

    _CONSTEXPR20 void _Update_Ptr() noexcept {
#if _ITERATOR_DEBUG_LEVEL != 0
        const auto _Mycont = static_cast<const _Myarr*>(this->_Getcont());
        _STL_VERIFY(_Row <= _Mycont->_Row, "array_2d pointer out of range");
        _STL_VERIFY(_Col <= _Mycont->_Col, "array_2d pointer out of range");
#endif // _ITERATOR_DEBUG_LEVEL != 0
        _Ptr = _Mycont->_Myfirst + (_Row * _Mycont->_Col + _Col);
    }

    _NODISCARD _CONSTEXPR20 pointer _Get_ptr() const noexcept {
        return this->_Ptr;
    }

private:
    _Tptr _Ptr;
};

template <class _Myarr>
class _Array_2d_h_iterator : public _Array_2d_iterator_ex_base<_Myarr> {
public:
    using _Mybase = _Array_2d_iterator_ex_base<_Myarr>;

    using iterator_category = random_access_iterator_tag;
    using value_type        = typename _Myarr::value_type;
    using size_type         = typename _Myarr::size_type;
    using difference_type   = typename _Myarr::difference_type;
    using pointer           = typename _Myarr::pointer;
    using reference         = value_type&;

    using _Tptr = typename _Myarr::pointer;

    using _Mybase::_Mybase;

    _CONSTEXPR20 _Array_2d_h_iterator(const size_type _Row, const size_type _Col, const _Container_base* _Parray)
        noexcept : _Mybase(_Row, _Col, _Parray) {
#if _ITERATOR_DEBUG_LEVEL != 0
        const auto _Mycont = static_cast<const _Myarr*>(_Parray);
        _STL_VERIFY(_Row < _Mycont->_Row, "array_2d hiterator constructor out of range");
#endif // _ITERATOR_DEBUG_LEVEL != 0
    }

    _NODISCARD _CONSTEXPR20 reference operator*() const noexcept {
        return _Mybase::operator*();
    }

    _NODISCARD _CONSTEXPR20 pointer operator->() const noexcept {
        return _Mybase::operator->();
    }

    _CONSTEXPR20 _Array_2d_h_iterator& operator++() noexcept {
#if _ITERATOR_DEBUG_LEVEL != 0
        const auto _Mycont = static_cast<const _Myarr*>(this->_Getcont());
        _STL_VERIFY(this->_Col < _Mycont->_Col, "can't increment array_2d horizontal iterator past end");
#endif // _ITERATOR_DEBUG_LEVEL != 0

        ++this->_Col;
        this->_Update_Ptr();
        return *this;
    }

    _CONSTEXPR20 _Array_2d_h_iterator operator++(int) noexcept {
        _Array_2d_h_iterator _Tmp = *this;
        ++*this;
        return _Tmp;
    }

    _CONSTEXPR20 _Array_2d_h_iterator& operator--() noexcept {
#if _ITERATOR_DEBUG_LEVEL != 0
        const auto _Mycont = static_cast<const _Myarr*>(this->_Getcont());
        _STL_VERIFY(this->_Col >= size_type(), "can't decrement array_2d horizontal iterator before begin");
#endif // _ITERATOR_DEBUG_LEVEL != 0

        --this->_Col;
        this->_Update_Ptr();
        return *this;
    }

    _CONSTEXPR20 _Array_2d_h_iterator operator--(int) noexcept {
        _Array_2d_h_iterator _Tmp = *this;
        --*this;
        return _Tmp;
    }

    _CONSTEXPR20 void _Verify_offset(const difference_type _Off) const noexcept {
#if _ITERATOR_DEBUG_LEVEL == 0
        (void)_Off;
#else // ^^^ _ITERATOR_DEBUG_LEVEL == 0 ^^^ // vvv _ITERATOR_DEBUG_LEVEL != 0 vvv
        const auto _Mycont = static_cast<const _Myarr*>(this->_Getcont());
        if (_Off < 0) {
            _STL_VERIFY(static_cast<size_type>(-_Off) <= this->_Col,
                "cannot seek array_2d horizontal iterator before begin");
        }

        if (_Off > 0) {
            _STL_VERIFY(static_cast<size_type>(_Off) < _Mycont->_Col - this->_Col,
                "cannot seek array_2d horizontal iterator after end");
        }
#endif // _ITERATOR_DEBUG_LEVEL == 0
    }

    _CONSTEXPR20 _Array_2d_h_iterator& operator+=(const difference_type _Off) noexcept {
        _Verify_offset(_Off);
        this->_Col += _Off;
        this->_Update_Ptr();
        return *this;
    }

    _NODISCARD _CONSTEXPR20 _Array_2d_h_iterator operator+(const difference_type _Off) const noexcept {
        _Array_2d_h_iterator _Tmp = *this;
        _Tmp += _Off;
        return _Tmp;
    }

    _CONSTEXPR20 _Array_2d_h_iterator& operator-=(const difference_type _Off) noexcept {
        return *this += -_Off;
    }

    _NODISCARD _CONSTEXPR20 _Array_2d_h_iterator operator-(const difference_type _Off) const noexcept {
        _Array_2d_h_iterator _Tmp = *this;
        _Tmp -= _Off;
        return _Tmp;
    }

    _NODISCARD _CONSTEXPR20 difference_type operator-(const _Array_2d_h_iterator& _Right) const noexcept {
        _Compat(_Right);
        return this->_Col - _Right._Col;
    }

    _NODISCARD _CONSTEXPR20 reference operator[](const difference_type _Off) const noexcept {
        return *(*this + _Off);
    }

    _NODISCARD _CONSTEXPR20 bool operator==(const _Array_2d_h_iterator& _Right) const noexcept {
        _Compat(_Right);
        return this->_Col == _Right._Col;
    }

    _NODISCARD bool operator!=(const _Array_2d_h_iterator& _Right) const noexcept {
        return !(*this == _Right);
    }

    _NODISCARD bool operator<(const _Array_2d_h_iterator& _Right) const noexcept {
        _Compat(_Right);
        return this->_Col < _Right._Col;
    }

    _NODISCARD bool operator>(const _Array_2d_h_iterator& _Right) const noexcept {
        return _Right < *this;
    }

    _NODISCARD bool operator<=(const _Array_2d_h_iterator& _Right) const noexcept {
        return !(_Right < *this);
    }

    _NODISCARD bool operator>=(const _Array_2d_h_iterator& _Right) const noexcept {
        return !(*this < _Right);
    }

    _CONSTEXPR20 void _Compat(const _Array_2d_h_iterator& _Right) const noexcept {
        // test for compatible iterator pair
#if _ITERATOR_DEBUG_LEVEL == 0
        (void)_Right;
#else // ^^^ _ITERATOR_DEBUG_LEVEL == 0 ^^^ // vvv _ITERATOR_DEBUG_LEVEL != 0 vvv
        _STL_VERIFY(this->_Getcont() == _Right._Getcont(), "array_2d iterators incompatible");
        _STL_VERIFY(this->_Row == _Right._Row, "array_2d iterators incompatible");
#endif // _ITERATOR_DEBUG_LEVEL == 0
    }

    friend _CONSTEXPR20 void _Verify_range(
        const _Array_2d_h_iterator& _First, const _Array_2d_h_iterator& _Last) noexcept {
        const auto _First_cont = static_cast<const _Myarr*>(_First._Getcont());
        const auto _Last_cont = static_cast<const _Myarr*>(_Last._Getcont());
        _STL_VERIFY(_First_cont == _Last_cont, "array_2d iterators in range are from different containers");
        _STL_VERIFY(_First_cont->_Col <= _Last_cont->_Col, "array_2d iterator range transposed");
    }

    _CONSTEXPR20 void _Seek_to(const value_type* _It) noexcept {
        this->_Ptr = _Refancy<_Tptr>(const_cast<value_type*>(_It));
    }
};

template <class _Myarr>
_NODISCARD _CONSTEXPR20 _Array_2d_h_iterator<_Myarr> operator+(
    typename _Array_2d_h_iterator<_Myarr>::difference_type _Off, _Array_2d_h_iterator<_Myarr> _Next) noexcept {
    _Next += _Off;
    return _Next;
}

template <class _Myarr>
class _Array_2d_v_iterator : public _Array_2d_iterator_ex_base<_Myarr> {
public:
    using _Mybase = _Array_2d_iterator_ex_base<_Myarr>;

    using iterator_category = random_access_iterator_tag;
    using value_type        = typename _Myarr::value_type;
    using size_type         = typename _Myarr::size_type;
    using difference_type   = typename _Myarr::difference_type;
    using pointer           = typename _Myarr::pointer;
    using reference         = value_type&;

    using _Tptr = typename _Myarr::pointer;

    using _Mybase::_Mybase;

    _CONSTEXPR20 _Array_2d_v_iterator(const size_type _Row, const size_type _Col, const _Container_base* _Parray)
        noexcept : _Mybase(_Row, _Col, _Parray) {
#if _ITERATOR_DEBUG_LEVEL != 0
        const auto _Mycont = static_cast<const _Myarr*>(_Parray);
        _STL_VERIFY(_Col < _Mycont->_Col, "array_2d viterator constructor out of range");
#endif // _ITERATOR_DEBUG_LEVEL != 0
    }

    _NODISCARD _CONSTEXPR20 reference operator*() const noexcept {
        return _Mybase::operator*();
    }

    _NODISCARD _CONSTEXPR20 pointer operator->() const noexcept {
        return _Mybase::operator->();
    }

    _CONSTEXPR20 _Array_2d_v_iterator& operator++() noexcept {
#if _ITERATOR_DEBUG_LEVEL != 0
        const auto _Mycont = static_cast<const _Myarr*>(this->_Getcont());
        _STL_VERIFY(this->_Row < _Mycont->_Row, "can't increment array_2d vertival iterator past end");
#endif // _ITERATOR_DEBUG_LEVEL != 0

        ++this->_Row;
        this->_Update_Ptr();
        return *this;
    }

    _CONSTEXPR20 _Array_2d_v_iterator operator++(int) noexcept {
        _Array_2d_v_iterator _Tmp = *this;
        ++*this;
        return _Tmp;
    }

    _CONSTEXPR20 _Array_2d_v_iterator& operator--() noexcept {
#if _ITERATOR_DEBUG_LEVEL != 0
        const auto _Mycont = static_cast<const _Myarr*>(this->_Getcont());
        _STL_VERIFY(this->_Row >= size_type(), "can't decrement array_2d vertical iterator before begin");
#endif // _ITERATOR_DEBUG_LEVEL != 0

        --this->_Row;
        this->_Update_Ptr();
        return *this;
    }

    _CONSTEXPR20 _Array_2d_v_iterator operator--(int) noexcept {
        _Array_2d_v_iterator _Tmp = *this;
        --*this;
        return _Tmp;
    }

    _CONSTEXPR20 void _Verify_offset(const difference_type _Off) const noexcept {
#if _ITERATOR_DEBUG_LEVEL == 0
        (void)_Off;
#else // ^^^ _ITERATOR_DEBUG_LEVEL == 0 ^^^ // vvv _ITERATOR_DEBUG_LEVEL != 0 vvv
        const auto _Mycont = static_cast<const _Myarr*>(this->_Getcont());
        if (_Off < 0) {
            _STL_VERIFY(static_cast<size_type>(-_Off) <= this->_Row,
                "cannot seek array_2d vertical iterator before begin");
        }

        if (_Off > 0) {
            _STL_VERIFY(static_cast<size_type>(_Off) < _Mycont->_Row - this->_Row,
                "cannot seek array_2d vertical iterator after end");
        }
#endif // _ITERATOR_DEBUG_LEVEL == 0
    }

    _CONSTEXPR20 _Array_2d_v_iterator& operator+=(const difference_type _Off) noexcept {
        _Verify_offset(_Off);
        this->_Row += _Off;
        this->_Update_Ptr();
        return *this;
    }

    _NODISCARD _CONSTEXPR20 _Array_2d_v_iterator operator+(const difference_type _Off) const noexcept {
        _Array_2d_v_iterator _Tmp = *this;
        _Tmp += _Off;
        return _Tmp;
    }

    _CONSTEXPR20 _Array_2d_v_iterator& operator-=(const difference_type _Off) noexcept {
        return *this += -_Off;
    }

    _NODISCARD _CONSTEXPR20 _Array_2d_v_iterator operator-(const difference_type _Off) const noexcept {
        _Array_2d_v_iterator _Tmp = *this;
        _Tmp -= _Off;
        return _Tmp;
    }

    _NODISCARD _CONSTEXPR20 difference_type operator-(const _Array_2d_v_iterator& _Right) const noexcept {
        _Compat(_Right);
        return this->_Row - _Right._Row;
    }

    _NODISCARD _CONSTEXPR20 reference operator[](const difference_type _Off) const noexcept {
        return *(*this + _Off);
    }

    _NODISCARD _CONSTEXPR20 bool operator==(const _Array_2d_v_iterator& _Right) const noexcept {
        _Compat(_Right);
        return this->_Row == _Right._Row;
    }

    _NODISCARD bool operator!=(const _Array_2d_v_iterator& _Right) const noexcept {
        return !(*this == _Right);
    }

    _NODISCARD bool operator<(const _Array_2d_v_iterator& _Right) const noexcept {
        _Compat(_Right);
        return this->_Row < _Right._Row;
    }

    _NODISCARD bool operator>(const _Array_2d_v_iterator& _Right) const noexcept {
        return _Right < *this;
    }

    _NODISCARD bool operator<=(const _Array_2d_v_iterator& _Right) const noexcept {
        return !(_Right < *this);
    }

    _NODISCARD bool operator>=(const _Array_2d_v_iterator& _Right) const noexcept {
        return !(*this < _Right);
    }

    _CONSTEXPR20 void _Compat(const _Array_2d_v_iterator& _Right) const noexcept {
        // test for compatible iterator pair
#if _ITERATOR_DEBUG_LEVEL == 0
        (void)_Right;
#else // ^^^ _ITERATOR_DEBUG_LEVEL == 0 ^^^ // vvv _ITERATOR_DEBUG_LEVEL != 0 vvv
        const auto _Mycont = static_cast<const _Myarr*>(this->_Getcont());
        const auto _Right_cont = static_cast<const _Myarr*>(_Right._Getcont());
        _STL_VERIFY(this->_Getcont() == _Right._Getcont(), "array_2d iterators incompatible");
        _STL_VERIFY(_Mycont->_Col == _Right_cont->_Col, "array_2d iterators incompatible");
#endif // _ITERATOR_DEBUG_LEVEL == 0
    }

    friend _CONSTEXPR20 void _Verify_range(
        const _Array_2d_v_iterator& _First, const _Array_2d_v_iterator& _Last) noexcept {
        const auto _First_cont = static_cast<const _Myarr*>(_First._Getcont());
        const auto _Last_cont = static_cast<const _Myarr*>(_Last._Getcont());
        _STL_VERIFY(_First_cont == _Last_cont, "array_2d iterators in range are from different containers");
        _STL_VERIFY(_First_cont->_Row <= _Last_cont->_Row, "array_2d iterator range transposed");
    }

    _CONSTEXPR20 void _Seek_to(const value_type* _It) noexcept {
        this->_Ptr = _Refancy<_Tptr>(const_cast<value_type*>(_It));
    }
};

template <class _Myarr>
_NODISCARD _CONSTEXPR20 _Array_2d_v_iterator<_Myarr> operator+(
    typename _Array_2d_v_iterator<_Myarr>::difference_type _Off, _Array_2d_v_iterator<_Myarr> _Next) noexcept {
    _Next += _Off;
    return _Next;
}

template <class _BidIt>
class _Array_2d_limited_iter {
public:
    using iterator_type = _BidIt;

    using iterator_category = _Iter_cat_t<_BidIt>;
    using value_type        = _Iter_value_t<_BidIt>;
    using size_type         = typename _BidIt::size_type;
    using difference_type   = _Iter_diff_t<_BidIt>;
    using pointer           = typename iterator_traits<_BidIt>::pointer;
    using reference         = _Iter_ref_t<_BidIt>;

    using _Range_t = _Array_2d_range<size_type>;

    template <class>
    friend class _Array_2d_limited_iter; 

    _CONSTEXPR17 explicit _Array_2d_limited_iter(_BidIt _Right, const _Range_t& _Range) 
        noexcept(is_nothrow_move_constructible_v<_BidIt>) // strengthened
        : current(_STD move(_Right)), _Range(_Range) {}

    _CONSTEXPR17 explicit _Array_2d_limited_iter(const size_type _Row, const size_type _Col, const _Container_base* _Parray,
        const _Range_t& _Range) // strengthened
        : current(_Row, _Col, _Parray), _Range(_Range) {}

    template <class _Other>
    _CONSTEXPR17 _Array_2d_limited_iter(const _Array_2d_limited_iter<_Other>& _Right) noexcept(
        is_nothrow_constructible_v<_BidIt, const _Other&>) // strengthened
        : current(_Right.current) {}

    template <class _Other>
    _CONSTEXPR17 _Array_2d_limited_iter& operator=(const _Array_2d_limited_iter<_Other>& _Right) {
        current = _Right.current;
        return *this;
    }

    _NODISCARD _CONSTEXPR17 _BidIt base() const {
        return current;
    }

    _NODISCARD _CONSTEXPR20 reference operator*() const {
#if _ITERATOR_DEBUG_LEVEL != 0
        _STL_VERIFY(_Range._In_range(current._Row, current._Col),
            "can't dereference out of range array_2d limited iterator");
#endif // _ITERATOR_DEBUG_LEVEL != 0

        return current.operator*();
    }

    _NODISCARD _CONSTEXPR20 pointer operator->() const {
#if _ITERATOR_DEBUG_LEVEL != 0
        _STL_VERIFY(_Range._In_range(current._Row, current._Col),
            "can't dereference out of range array_2d limited iterator");
#endif // _ITERATOR_DEBUG_LEVEL != 0

        return current.operator->();
    }

    _CONSTEXPR20 _Array_2d_limited_iter operator++() noexcept {
#if _ITERATOR_DEBUG_LEVEL != 0 
        _STL_VERIFY(current._Col < _Range._Col_end, "can't increment array_2d limited iterator past end");
        _STL_VERIFY(current._Row < _Range._Row_end, "can't increment array_2d limited iterator past end");
#endif // _ITERATOR_DEBUG_LEVEL != 0

        ++current;
        return *this;
    }

    _CONSTEXPR20 _Array_2d_limited_iter operator++(int) noexcept {
        _Array_2d_limited_iter _Tmp = *this;
        ++* this;
        return _Tmp;
    }

    _CONSTEXPR20 _Array_2d_limited_iter& operator--() noexcept {
#if _ITERATOR_DEBUG_LEVEL != 0 
        _STL_VERIFY(current._Col > _Range._Col_begin, "can't decrement array_2d limited iterator before begin");
        _STL_VERIFY(current._Row > _Range._Row_begin, "can't decrement array_2d limited iterator before begin");
#endif // _ITERATOR_DEBUG_LEVEL != 0

        --current;
        return *this;
    }

    _CONSTEXPR20 _Array_2d_limited_iter operator--(int) noexcept {
        _Array_2d_limited_iter _Tmp = *this;
        --* this;
        return _Tmp;
    }

    _CONSTEXPR20 _Array_2d_limited_iter& operator+=(const difference_type _Off) noexcept {
        _Verify_offset(_Off);
        current += _Off;
        _Verify_rc_range();
        return *this;
    }

    _NODISCARD _CONSTEXPR20 _Array_2d_limited_iter operator+(const difference_type _Off) const noexcept {
        _Array_2d_limited_iter _Tmp = *this;
        _Tmp += _Off;
        return _Tmp;
    }

    _CONSTEXPR20 _Array_2d_limited_iter& operator-=(const difference_type _Off) noexcept {
        _Verify_offset(_Off);
        current += -_Off;
        _Verify_rc_range();
        return *this;
    }

    _NODISCARD _CONSTEXPR20 _Array_2d_limited_iter operator-(const difference_type _Off) const noexcept {
        _Array_2d_limited_iter _Tmp = *this;
        _Tmp -= _Off;
        return _Tmp;
    }

    _NODISCARD _CONSTEXPR20 reference operator[](const difference_type _Off) const noexcept {
        _Verify_offset(_Off);
#if _ITERATOR_DEBUG_LEVEL != 0 
        auto _Tmp = current[_Off];
        _Verify_rc_range();
        return _Tmp;
#else
        return current[_Off];
#endif // _ITERATOR_DEBUG_LEVEL != 0
    }

    using _Prevent_inheriting_unwrap = _Array_2d_limited_iter; 

    template <class _BidIt2 = _BidIt, enable_if_t<_Offset_verifiable_v<_BidIt2>, int> = 0>
    constexpr void _Verify_offset(const difference_type _Off) const {
        _STL_VERIFY(_Off != _Min_possible_v<difference_type>, "integer overflow");
        current._Verify_offset(_Off);
    }

    _CONSTEXPR20 void _Verify_rc_range() const noexcept {
#if _ITERATOR_DEBUG_LEVEL != 0 
        _STL_VERIFY(_Range._In_range(current._Row, current._Col),
            "array_2d limited iterator out of range");
#endif // _ITERATOR_DEBUG_LEVEL != 0
    }

    template <class _BidIt2 = _BidIt, enable_if_t<_Unwrappable_v<const _BidIt2&>, int> = 0>
    _NODISCARD constexpr _Array_2d_limited_iter<_Unwrapped_t<const _BidIt2&>> _Unwrapped() const {
        return static_cast<_Array_2d_limited_iter<_Unwrapped_t<const _BidIt2&>>>(current._Unwrapped());
    }

    template <class _Src, enable_if_t<_Wrapped_seekable_v<_BidIt, const _Src&>, int> = 0>
    constexpr void _Seek_to(const _Array_2d_limited_iter<_Src>& _It) {
        current._Seek_to(_It.current);
    }

    _NODISCARD constexpr const _BidIt& _Get_current() const noexcept {
        return current;
    }

protected:
    _BidIt current;

private:
    _Range_t _Range;
};

template <class _BidIt1, class _BidIt2>
_NODISCARD _CONSTEXPR17 bool operator==(const _Array_2d_limited_iter<_BidIt1>& _Left,
    const _Array_2d_limited_iter<_BidIt2>& _Right) {
    return _Left._Get_current() == _Right._Get_current();
}

template <class _BidIt1, class _BidIt2>
_NODISCARD _CONSTEXPR17 bool operator!=(const _Array_2d_limited_iter<_BidIt1>& _Left,
    const _Array_2d_limited_iter<_BidIt2>& _Right) {
    return _Left._Get_current() != _Right._Get_current();
}

template <class _BidIt1, class _BidIt2>
_NODISCARD _CONSTEXPR17 bool operator<(const _Array_2d_limited_iter<_BidIt1>& _Left,
    const _Array_2d_limited_iter<_BidIt2>& _Right) {
    return _Left._Get_current() > _Right._Get_current();
}

template <class _BidIt1, class _BidIt2>
_NODISCARD _CONSTEXPR17 bool operator>(const _Array_2d_limited_iter<_BidIt1>& _Left,
    const _Array_2d_limited_iter<_BidIt2>& _Right) {
    return _Left._Get_current() < _Right._Get_current();
}

template <class _BidIt1, class _BidIt2>
_NODISCARD _CONSTEXPR17 bool operator<=(const _Array_2d_limited_iter<_BidIt1>& _Left,
    const _Array_2d_limited_iter<_BidIt2>& _Right) {
    return _Left._Get_current() >= _Right._Get_current();
}

template <class _BidIt1, class _BidIt2>
_NODISCARD _CONSTEXPR17 bool operator>=(const _Array_2d_limited_iter<_BidIt1>& _Left,
    const _Array_2d_limited_iter<_BidIt2>& _Right) {
    return _Left._Get_current() <= _Right._Get_current();
}

template <class _BidIt1, class _BidIt2>
_NODISCARD _CONSTEXPR17 auto operator-(const _Array_2d_limited_iter<_BidIt1>& _Left,
    const _Array_2d_limited_iter<_BidIt2>& _Right) -> decltype(_Right._Get_current() - _Left._Get_current()) {
    return _Right._Get_current() - _Left._Get_current();
}

template <class _BidIt>
_NODISCARD _CONSTEXPR17 _Array_2d_limited_iter<_BidIt> operator+(
    typename _Array_2d_limited_iter<_BidIt>::difference_type _Off, const _Array_2d_limited_iter<_BidIt>& _Right) {
    return _Right + _Off;
}

template <class _BidIt>
class _Array_2d_const_iter : public _BidIt {
public:
    using iterator_type = _BidIt;

    using _Iter_traits = iterator_traits<_BidIt>;

    using iterator_category = _Iter_cat_t<_BidIt>;
    using value_type        = _Iter_value_t<_BidIt>;
    using difference_type   = _Iter_diff_t<_BidIt>;
    using pointer           = typename _Iter_traits::pointer;
    using const_pointer     = const value_type*;
    using reference         = const value_type&;

    template <class>
    friend class _Array_2d_const_iter;

    using _BidIt::_BidIt;

    _CONSTEXPR17 _Array_2d_const_iter() = default;

    _CONSTEXPR17 _Array_2d_const_iter(const _BidIt& _Right) noexcept(is_nothrow_copy_constructible_v<_BidIt>) // strengthened
        : _BidIt(_Right) {}

    template <class _Other, enable_if_t<is_convertible_v<_BidIt, _Other>, int> = 0>
    _CONSTEXPR17 _Array_2d_const_iter(const _Array_2d_const_iter<_Other>& _Right) noexcept(is_nothrow_constructible_v<_BidIt, const _Other&>) // strengthened
        : _BidIt(_Right) {}

    template <class _Other>
    _CONSTEXPR17 _Array_2d_const_iter& operator=(const _Array_2d_const_iter<_Other>& _Right) {
        _BidIt::operator=(_Right);
        return *this;
    }

    _NODISCARD _CONSTEXPR17 reference operator*() const {
        return _BidIt::operator*();
    }

    _NODISCARD _CONSTEXPR17 pointer operator->() const {
        return _BidIt::operator->();
    }

    _CONSTEXPR17 _Array_2d_const_iter operator++() noexcept {
        _BidIt::operator++();
        return *this;
    }

    _CONSTEXPR17 _Array_2d_const_iter operator++(int) noexcept {
        _Array_2d_const_iter _Tmp = *this;
        _BidIt::operator++();
        return _Tmp;
    }

    _CONSTEXPR17 _Array_2d_const_iter& operator--() noexcept {
        _BidIt::operator--();
        return *this;
    }

    _CONSTEXPR17 _Array_2d_const_iter operator--(int) noexcept {
        _Array_2d_const_iter _Tmp = *this;
        _BidIt::operator--();
        return _Tmp;
    }

    _NODISCARD _CONSTEXPR17 _Array_2d_const_iter operator+(const difference_type _Off) const noexcept {
        return _Array_2d_const_iter(_BidIt::operator+(_Off));
    }


    _CONSTEXPR17 _Array_2d_const_iter& operator+=(const difference_type _Off) noexcept {
        _BidIt::operator+=(_Off);
        return *this;
    }

    _NODISCARD _CONSTEXPR17 _Array_2d_const_iter operator-(const difference_type _Off) const noexcept {
        return _Array_2d_const_iter(_BidIt::operator-(_Off));
    }

    _CONSTEXPR17 _Array_2d_const_iter& operator-=(const difference_type _Off) noexcept {
        _BidIt::operator-=(_Off);
        return *this;
    }

    template <class _BidIt2>
    _NODISCARD _CONSTEXPR17 difference_type operator-(const _BidIt2& _Right) const noexcept {
        return _BidIt::operator-(_Right);
    }

    _NODISCARD _CONSTEXPR17 reference operator[](const difference_type _Off) const noexcept {
        return _BidIt::operator[](_Off);
    }

    template <class _BidIt2>
    _NODISCARD _CONSTEXPR17 bool operator==(const _BidIt2& _Right) const noexcept {
        return _BidIt::operator==(_Right);
    }

    template <class _BidIt2>
    _NODISCARD _CONSTEXPR17 bool operator!=(const _BidIt2& _Right) const noexcept {
        return _BidIt::operator!=(_Right);
    }

    template <class _BidIt2>
    _NODISCARD _CONSTEXPR17 bool operator<(const _BidIt2& _Right) const noexcept {
        return _BidIt::operator<(_Right);
    }

    template <class _BidIt2>
    _NODISCARD _CONSTEXPR17 bool operator>(const _BidIt2& _Right) const noexcept {
        return _BidIt::operator>(_Right);
    }

    template <class _BidIt2>
    _NODISCARD _CONSTEXPR17 bool operator<=(const _BidIt2& _Right) const noexcept {
        return _BidIt::operator<=(_Right);
    }

    template <class _BidIt2>
    _NODISCARD _CONSTEXPR17 bool operator>=(const _BidIt2& _Right) const noexcept {
        return _BidIt::operator>=(_Right);
    }

    using _Prevent_inheriting_unwrap = _Array_2d_const_iter;

    template <class _BidIt2 = _BidIt, enable_if_t<_Offset_verifiable_v<_BidIt2>, int> = 0>
    constexpr void _Verify_offset(const difference_type _Off) const {
        _BidIt::_Verify_offset(_Off);
    }

    template <class _BidIt2 = _BidIt, enable_if_t<_Unwrappable_v<const _BidIt2&>, int> = 0> 
    _NODISCARD constexpr _Array_2d_const_iter<_Unwrapped_t<const _BidIt2&>> _Unwrapped() const {
        return static_cast<_Array_2d_const_iter<_Unwrapped_t<const _BidIt2&>>>(_BidIt::_Unwrapped());
    }

    template <class _Src, enable_if_t<_Wrapped_seekable_v<_BidIt, const _Src&>, int> = 0>
    constexpr void _Seek_to(const _Array_2d_const_iter<_Src>& _It) {
        _BidIt::_Seek_to(static_cast<_BidIt>(_It));
    }
};

template <class _BidIt>
_NODISCARD _CONSTEXPR17 _Array_2d_const_iter<_BidIt> operator+(
    typename _Array_2d_const_iter<_BidIt>::difference_type _Off, const _Array_2d_const_iter<_BidIt>& _Right) {
    return _Right + _Off;
}

template <class _BidIt1, class _BidIt2>
constexpr void _Verify_range(const _Array_2d_const_iter<_BidIt1>& _First, const _Array_2d_const_iter<_BidIt2>& _Last) {
    _Verify_range(static_cast<_BidIt1>(_First), static_cast<_BidIt2>(_Last));
}

template <class _Myarr, class _Ref_t>
class _Array_2d_indexor 
{
private:
    using _This_type = _Array_2d_indexor;

public:
    using value_type = typename _Myarr::value_type;
    using size_type  = typename _Myarr::size_type;
    using reference  = _Ref_t;

    using _Tptr = typename _Myarr::pointer;

private:
    template<class, class>
    friend class array_2d;

    _Array_2d_indexor() = delete;

#if _CONTAINER_DEBUG_LEVEL > 0
    _CONSTEXPR20 _Array_2d_indexor(_Tptr _Ptr, size_type _Col_size) noexcept
        : _Ptr(_Ptr), _Col_size(_Col_size) {}

public:
    _CONSTEXPR20 reference operator[](size_type _Col) const noexcept {
        _STL_VERIFY(_Col < _Col_size, "array_2d subscript out of range");
        return *(_Ptr + _Col);
    }

protected:
    size_type _Col_size;
#else 
    _CONSTEXPR20 _Array_2d_indexor(_Tptr _Ptr) noexcept
        : _Ptr(_Ptr) {}

public:
    _CONSTEXPR20 reference operator[](size_type _Col) const noexcept {
        return *(_Ptr + _Col);
    }

protected:
#endif // _CONTAINER_DEBUG_LEVEL > 0
    value_type* const _Ptr;
};

template <class _Elem> 
class _Init_list_2d_iterator {
public:
    using _This_type = _Init_list_2d_iterator;

    using iterator_category = forward_iterator_tag;

    using value_type      = _Elem;
    using difference_type = ptrdiff_t;
    using pointer         = const _Elem*;
    using const_pointer   = const _Elem*;
    using reference       = const _Elem&;
    using const_reference = const _Elem&;
    using size_type       = size_t;

    using _Init_list_2d = _STD initializer_list<_STD initializer_list<_Elem>>;
    using _Iter_row     = typename _Init_list_2d::iterator;
    using _Iter_col     = typename _STD initializer_list<_Elem>::iterator;

private:
    _Init_list_2d_iterator(_Iter_row _Row, size_type _Col_size) noexcept
        : _IRow(_Row), _ICol(_Row->begin()), _Col_size(_Col_size), _Col_now(0), _Default_val() {}

public:
    template <class _Elem>
    friend _STD pair<_Init_list_2d_iterator<_Elem>, _Init_list_2d_iterator<_Elem>>
        _Make_Init_list_2d_iterator_pair(_STD initializer_list<_STD initializer_list<_Elem>> _Ilist) noexcept;
 
    _NODISCARD _CONSTEXPR20 reference operator*() const noexcept {
        if (_ICol != _IRow->end()) {
            return *_ICol;
        }
        return _Default_val;
    }

    _CONSTEXPR20 _Init_list_2d_iterator& operator++() noexcept {
        if (_ICol != _IRow->end()) {
            ++_ICol;
        }

        if (_Col_now < _Col_size - 1) {
            ++_Col_now;
        } else {
            _Col_now = 0;
            ++_IRow;
            _ICol = _IRow->begin();
        }

        return *this;
    }

    _NODISCARD _CONSTEXPR20 bool operator!=(const _Init_list_2d_iterator& _Right) {
        return (_IRow != _Right._IRow || _Col_now != _Right._Col_now) && _Col_size;
    }

    _NODISCARD _CONSTEXPR20 size_type col_size() const noexcept {
        return _Col_size;
    }

private:
    _Iter_row _IRow;
    _Iter_col _ICol;
    const size_type _Col_size;
    size_type _Col_now;

    const _Elem _Default_val;
};

template <class _Elem>
_NODISCARD _CONSTEXPR20 _STD pair<_Init_list_2d_iterator<_Elem>, _Init_list_2d_iterator<_Elem>> 
    _Make_Init_list_2d_iterator_pair(_STD initializer_list<_STD initializer_list<_Elem>> _Ilist) noexcept {
    static auto _Max_size = [](auto& a, auto& b) { return a.size() < b.size(); };

    size_t _Row = _Ilist.size();
    size_t _Col = max_element(_Ilist.begin(), _Ilist.end(), _Max_size)->size();

    return make_pair(_Init_list_2d_iterator<_Elem>(_Ilist.begin(), _Col),
        _Init_list_2d_iterator<_Elem>(_Ilist.end(), _Col));
}

template <class _Value_type, class _Size_type, class _Difference_type, class _Pointer, class _Const_pointer,
    class _Reference, class _Const_reference>
struct _Arr2d_iter_types {
    using value_type      = _Value_type;
    using size_type       = _Size_type;
    using difference_type = _Difference_type;
    using pointer         = _Pointer;
    using const_pointer   = _Const_pointer;
};

template <class _Val_types>
class _Array_2d_val : public _Container_base {
public:
    using value_type      = typename _Val_types::value_type;
    using size_type       = typename _Val_types::size_type;
    using difference_type = typename _Val_types::difference_type;
    using pointer         = typename _Val_types::pointer;
    using const_pointer   = typename _Val_types::const_pointer;
    using reference       = value_type&;
    using const_reference = const value_type&;

    _CONSTEXPR20 _Array_2d_val() noexcept 
        : _Myfirst(), _Mylast(), _Myend(), _Row(), _Col() {}

    _CONSTEXPR20 _Array_2d_val(size_type _Row, size_type _Col, pointer _First, pointer _Last, 
        pointer _End) noexcept
        : _Row(_Row), _Col(_Col), _Myfirst(_First), _Mylast(_Last), _Myend(_End) {}

    _CONSTEXPR20 void _Swap_val(_Array_2d_val& _Right) noexcept {
        this->_Swap_proxy_and_iterators(_Right);
        _Swap_adl(_Row, _Right._Row);
        _Swap_adl(_Col, _Right._Col);
        _Swap_adl(_Myfirst, _Right._Myfirst);
        _Swap_adl(_Mylast, _Right._Mylast);
        _Swap_adl(_Myend, _Right._Myend);
    }

    _CONSTEXPR20 void _Take_contents(_Array_2d_val& _Right) noexcept {
        this->_Swap_proxy_and_iterators(_Right);
        _Row     = _Right._Row;
        _Col     = _Right._Col;
        _Myfirst = _Right._Myfirst;
        _Mylast  = _Right._Mylast;
        _Myend   = _Right._Myend;

        _Right._Row     = size_type();
        _Right._Col     = size_type();
        _Right._Myfirst = nullptr;
        _Right._Mylast  = nullptr;
        _Right._Myend   = nullptr;
    }

    struct { size_type _Row, _Col; };
    pointer _Myfirst; // pointer to beginning of array
    pointer _Mylast; // pointer to current end of sequence
    pointer _Myend; // pointer to end of array
};

template <class _Ty, class _Alloc = allocator<_Ty>>
class array_2d { // varying size 2-dimentional array of values
private:
    friend _Tidy_guard<array_2d>;

    using _Alty        = _Rebind_alloc_t<_Alloc, _Ty>;
    using _Alty_traits = allocator_traits<_Alty>;

public:
    static_assert(!_ENFORCE_MATCHING_ALLOCATORS || is_same_v<_Ty, typename _Alloc::value_type>,
        _MISMATCHED_ALLOCATOR_MESSAGE("array_2d<T, Allocator>", "T"));

    using value_type      = _Ty;
    using allocator_type  = _Alloc;
    using pointer         = typename _Alty_traits::pointer;
    using const_pointer   = typename _Alty_traits::const_pointer;
    using reference       = _Ty&;
    using const_reference = const _Ty&;
    using size_type       = typename _Alty_traits::size_type;
    using difference_type = typename _Alty_traits::difference_type;

    using _Range_t      = _Array_2d_range<size_type>;
    using _Init_list_2d = _STD initializer_list<_STD initializer_list<_Ty>>;

private:
    using _Scary_val = _Array_2d_val<conditional_t<_Is_simple_alloc_v<_Alty>, _Simple_types<_Ty>,
        _Arr2d_iter_types<_Ty, size_type, difference_type, pointer, const_pointer, _Ty&, const _Ty&>>>;

public:
    using indexor       = _Array_2d_indexor<_Scary_val, reference>;
    using const_indexor = _Array_2d_indexor<_Scary_val, const reference>;

    using iterator               = _Array_2d_iterator<_Scary_val>;
    using const_iterator         = _Array_2d_const_iter<iterator>;
    using reverse_iterator       = _STD reverse_iterator<iterator>;
    using const_reverse_iterator = _STD reverse_iterator<const_iterator>;

    using hiterator               = _Array_2d_h_iterator<_Scary_val>;
    using const_hiterator         = _Array_2d_const_iter<hiterator>;
    using reverse_hiterator       = _STD reverse_iterator<hiterator>;
    using const_reverse_hiterator = _STD reverse_iterator<const_hiterator>;

    using viterator               = _Array_2d_v_iterator<_Scary_val>;
    using const_viterator         = _Array_2d_const_iter<viterator>;
    using reverse_viterator       = _STD reverse_iterator<viterator>;
    using const_reverse_viterator = _STD reverse_iterator<const_viterator>;

    _CONSTEXPR20 array_2d() noexcept(is_nothrow_default_constructible_v<_Alty>)
        : _Mypair(_Zero_then_variadic_args_t{}) {
        _Mypair._Myval2._Alloc_proxy(_GET_PROXY_ALLOCATOR(_Alty, _Getal()));
    }

    _CONSTEXPR20 explicit array_2d(const _Alloc& _Al) noexcept : _Mypair(_One_then_variadic_args_t{}, _Al) {
        _Mypair._Myval2._Alloc_proxy(_GET_PROXY_ALLOCATOR(_Alty, _Getal()));
    }

    _CONSTEXPR20 explicit array_2d(_CRT_GUARDOVERFLOW const size_type _Row, _CRT_GUARDOVERFLOW const size_type _Col,
        const _Alloc& _Al = _Alloc()) : _Mypair(_One_then_variadic_args_t{}, _Al) {
        _Construct_n(_Row, _Col);
    }

    _CONSTEXPR20 array_2d(_CRT_GUARDOVERFLOW const size_type _Row, _CRT_GUARDOVERFLOW const size_type _Col,
        const _Ty& _Val, const _Alloc& _Al = _Alloc()) : _Mypair(_One_then_variadic_args_t{}, _Al) {
        _Construct_n(_Row, _Col, _Val);
    }

    template <class _BidIt, enable_if_t<_Is_iterator_v<_BidIt>, int> = 0>
    _CONSTEXPR20 array_2d(_CRT_GUARDOVERFLOW const size_type _Row, _CRT_GUARDOVERFLOW const size_type _Col,
        _BidIt _First, _BidIt _Last, const _Alloc& _Al = _Alloc()) : _Mypair(_One_then_variadic_args_t{}, _Al) {
#if _CONTAINER_DEBUG_LEVEL > 0
        _STL_VERIFY(_Row * _Col == static_cast<size_t>(_STD distance(_First, _Last)),
            "array_2d iterators have imcompatible distance");
#endif // _CONTAINER_DEBUG_LEVEL > 0

        _Adl_verify_range(_First, _Last);
        auto _UFirst = _Get_unwrapped(_First);
        auto _ULast = _Get_unwrapped(_Last);
        _Construct_n(_Row, _Col, _STD move(_UFirst), _STD move(_ULast));
    }

private:
    _CONSTEXPR20 array_2d(const array_2d& _Arr, const _Range_t& _Range, const _Alloc& _Al_)
        : _Mypair(_One_then_variadic_args_t{}, _Al_) {
        _Construct_ranged_copy(_Arr, _Range);
    }

public:
    _CONSTEXPR20 array_2d(_Init_list_2d _Ilist, const _Alloc& _Al = _Alloc())
        : _Mypair(_One_then_variadic_args_t{}, _Al) {
        auto pair = _Make_Init_list_2d_iterator_pair(_Ilist);
        auto _Row = _Ilist.size();
        auto _Col = pair.first.col_size();
        _Construct_n(_Row, _Col, pair.first, pair.second);
    }

    _CONSTEXPR20 array_2d(const array_2d& _Right)
        : _Mypair(_One_then_variadic_args_t{}, _Alty_traits::select_on_container_copy_construction(_Right._Getal())) {
        const auto& _Right_data = _Right._Mypair._Myval2;
        _Construct_n(_Right_data._Row, _Right_data._Col, _Right_data._Myfirst, _Right_data._Mylast);
    }

    _CONSTEXPR20 array_2d(const array_2d& _Right, const _Identity_t<_Alloc>& _Al)
        : _Mypair(_One_then_variadic_args_t{}, _Al) {
        const auto& _Right_data = _Right._Mypair._Myval2;
        _Construct_n(_Right_data._Row, _Right_data._Col, _Right_data._Myfirst, _Right_data._Mylast);
    }

    _CONSTEXPR20 array_2d(array_2d&& _Right) noexcept
        : _Mypair(_One_then_variadic_args_t{}, _STD move(_Right._Getal()),
            _STD exchange(_Right._Mypair._Myval2._Row, size_type()),
            _STD exchange(_Right._Mypair._Myval2._Col, size_type()),
            _STD exchange(_Right._Mypair._Myval2._Myfirst, nullptr),
            _STD exchange(_Right._Mypair._Myval2._Mylast, nullptr),
            _STD exchange(_Right._Mypair._Myval2._Myend, nullptr)) {
        _Mypair._Myval2._Alloc_proxy(_GET_PROXY_ALLOCATOR(_Alty, _Getal()));
        _Mypair._Myval2._Swap_proxy_and_iterators(_Right._Mypair._Myval2);
    }

    _CONSTEXPR20 array_2d(array_2d&& _Right, const _Identity_t<_Alloc>& _Al_) noexcept(
        _Alty_traits::is_always_equal::value) // strengthened
        : _Mypair(_One_then_variadic_args_t{}, _Al_) {
        _Alty& _Al = _Getal();
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Al);
        auto& _My_data = _Mypair._Myval2;
        auto& _Right_data = _Right._Mypair._Myval2;
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _My_data);

        if constexpr (!_Alty_traits::is_always_equal::value) {
            if (_Al != _Right._Getal()) {
                const auto _Count = static_cast<size_type>(_Right_data._Mylast - _Right_data._Myfirst);
                if (_Count != 0) {
                    _Buy_raw(_Count);
                    _Tidy_guard<vector> _Guard{ this };
                    _My_data._Mylast =
                        _Uninitialized_move(_Right_data._Myfirst, _Right_data._Mylast, _My_data._Myfirst, _Al);

                    _Guard._Target = nullptr;
                }
                _Proxy._Release();
                return;
            }
        }

        _My_data._Take_contents(_Right_data);
        _Proxy._Release();
    }

    _CONSTEXPR20 array_2d& operator=(array_2d&& _Right) noexcept(
        _Choose_pocma_v<_Alty> != _Pocma_values::_No_propagate_allocators) {
        if (this == _STD addressof(_Right)) {
            return *this;
        }

        _Alty& _Al = _Getal();
        _Alty& _Right_al = _Right._Getal();
        constexpr auto _Pocma_val = _Choose_pocma_v<_Alty>;
        if constexpr (_Pocma_val == _Pocma_values::_No_propagate_allocators) {
            if (_Al != _Right_al) {
                _Move_assign_unequal_alloc(_Right);
                return *this;
            }
        }

        _Tidy();
#if _ITERATOR_DEBUG_LEVEL != 0
        if constexpr (_Pocma_val == _Pocma_values::_Propagate_allocators) {
            if (_Al != _Right_al) {
                _Mypair._Myval2._Reload_proxy(_GET_PROXY_ALLOCATOR(_Alty, _Al), _GET_PROXY_ALLOCATOR(_Alty, _Right_al));
            }
        }
#endif // _ITERATOR_DEBUG_LEVEL != 0          

        _Pocma(_Al, _Right_al);
        _Mypair._Myval2._Take_contents(_Right._Mypair._Myval2);
        return *this;
    }

    _CONSTEXPR20 ~array_2d() noexcept {
        _Tidy();
#if _ITERATOR_DEBUG_LEVEL != 0
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Getal());
        _Delete_plain_internal(_Alproxy, _STD exchange(_Mypair._Myval2._Myproxy, nullptr));
#endif // _ITERATOR_DEBUG_LEVEL != 0
    }

private:
    template <class... _Valty>
    _CONSTEXPR20 void _Emplace_one_at_hback(_Valty&&... _Val) { 
        // insert by perfectly forwarding into element at horizontal end, provide strong guarantee 
        auto& _My_data  = _Mypair._Myval2;
        pointer _Mylast = _My_data._Mylast;
        auto& _Row      = _My_data._Row;
        auto _Col       = _My_data._Col;

        if (_Mylast + _My_data._Col <= _My_data._Myend && _Col > 0) {
            _Emplace_hback_with_unused_capacity(_STD forward<_Valty>(_Val)...);
        } else if (_Col > 0) {
            _Emplace_h_reallocate(row(), _STD forward<_Valty>(_Val)...);
        } else {
            ++_Row;
        }
    }

    template <class... _Valty>
    _CONSTEXPR20 void _Emplace_one_at_vback(_Valty&&... _Val) { 
        // insert by perfectly forwarding into element at vertical end, provide basic guarantee
        auto& _My_data  = _Mypair._Myval2;
        pointer _Mylast = _My_data._Mylast;
        auto _Row       = _My_data._Row;
        auto& _Col      = _My_data._Col;

        if (_Mylast + _Row <= _My_data._Myend && _Row > 0) {
            _Emplace_vback_with_unused_capacity(_STD forward<_Valty>(_Val)...);
        } else if (_Row > 0) {
            _Emplace_v_reallocate(col(), _STD forward<_Valty>(_Val)...);
        } else {
            ++_Col;
        }
    }

    template <class... _Valty>
    _CONSTEXPR20 void _Emplace_hback_with_unused_capacity(_Valty&&... _Val) { 
        // insert by perfectly forwarding into element at horizontal end, provide strong guarantee
        auto& _My_data   = _Mypair._Myval2;
        const auto _Row  = _My_data._Row;
        const auto _Col  = _My_data._Col;
        pointer& _Mylast = _My_data._Mylast;
        pointer _Newlast = _Mylast + _Col;
        // check that we have unused capacity
        _STL_INTERNAL_CHECK(_Col <= static_cast<size_type>(_My_data._Myend - _Mylast)); 
        if constexpr (conjunction_v<is_nothrow_constructible<_Ty, _Valty...>,
            _Uses_default_construct<_Alloc, _Ty*, _Valty...>>) {
            for (auto _Iter = _Mylast; _Iter < _Newlast; ++_Iter) {
                _Construct_in_place(*_Iter, _STD forward<_Valty>(_Val)...);
            }
        } else {
            _Uninitialized_backout_al<_Alloc> _Backout{ _Mylast,  _Getal() };
            for (size_type _Count = _Col; 0 < _Count; --_Count) {
                _Backout._Emplace_back(_STD forward<_Valty>(_Val)...);
            }
            _Backout._Release();
        }

        _Orphan_range({ _Row, _Row + 1, 0, _Col });
        _Mylast = _Newlast;
        ++_My_data._Row;
    }

    template <class... _Valty>
    _CONSTEXPR20 void _Emplace_vback_with_unused_capacity(_Valty&&... _Val) {
        // insert by perfectly forwarding into element at vertical end, provide basic guarantee
        auto& _Al = _Getal();
        auto& _My_data = _Mypair._Myval2;
        const auto _Row = _My_data._Row;
        const auto _Col = _My_data._Col;
        const auto _Size = _Row * _Col;
        pointer _Myfirst = _My_data._Myfirst;
        pointer _Mylast = _My_data._Mylast;
        pointer _Newlast = _Mylast + _Col;
        // check that we have unused capacity
        _STL_INTERNAL_CHECK(_Row <= static_cast<size_type>(_My_data._Myend - _Mylast));
        _Alloc_temporary2<_Alty> _Obj(_Al, _STD forward<_Valty>(_Val)...);
        auto& _Tmp = _Obj._Get_value();

        pointer _New_last = _Mylast + _Row;

        auto _Constructed_last = _New_last;
        auto _Constructed_first = _Constructed_last;
        auto _Source_first = _Mylast - _Col;

        _TRY_BEGIN
        _Alty_traits::construct(_Al, _Unfancy(_Constructed_first - 1), _STD forward<_Valty>(_Val)...);
        _Constructed_first -= 1;
        if (_Constructed_first > _Mylast) {
            for (; _Constructed_first - _Col > _Mylast; _Source_first -= _Col) {
                _Uninitialized_move(_Source_first, _Source_first + _Col, _Constructed_first - _Col, _Al);
                _Constructed_first -= _Col;
                _Alty_traits::construct(_Al, _Unfancy(_Constructed_first - 1), _STD forward<_Valty>(_Val)...);
                _Constructed_first -= 1;
            }
            if (_Constructed_first == _Mylast + _Col + 1) {
                _Uninitialized_move(_Source_first, _Source_first + _Col, _Constructed_first - _Col, _Al);
                _Constructed_first -= _Col;
                _Alty_traits::construct(_Al, _Unfancy(_Constructed_first - 1), _STD forward<_Valty>(_Val)...);
                _Constructed_first -= 1;
            } else {
                auto _Spilled = _Constructed_first - _Mylast;
                _Uninitialized_move(_Source_first + _Col - _Spilled, _Source_first + _Col, _Mylast, _Al);
                _Constructed_first -= _Spilled;
                _Move_backward_unchecked(_Source_first, _Source_first + _Col - _Spilled, _Mylast);
                _Constructed_first -= _Col - _Spilled;
                *(_Constructed_first - 1) = _Tmp;
                _Constructed_first -= 1;
            }
            _Source_first -= _Col;
        } 
        for (auto _Iter = _Constructed_first - _Col - 1; _Iter >= _Myfirst; _Iter -= _Col + 1) {
            _Move_backward_unchecked(_Source_first, _Source_first + _Col, _Iter + _Col + 1);
            _Source_first -= _Col;
            *_Iter = _Tmp;
        }
        _CATCH_ALL
        _Destroy_range(_STD max(_Constructed_first, _Mylast), _Constructed_last, _Al);
        _RERAISE;
        _CATCH_END

        _Orphan_range({ 0, _Row, _Col, _Col + 1 });
        ++_My_data._Col; 
        _My_data._Mylast += _Row;
    }

    template <class... _Valty>
    _CONSTEXPR20 void _Emplace_h_reallocate(const size_type _Where_row, _Valty&&... _Val) { 
        // reallocate and insert by perfectly forwarding _Val at row of _Where_row
        _Alty& _Al       = _Getal();
        auto& _My_data   = _Mypair._Myval2;
        const auto _Row  = _My_data._Row;
        const auto _Col  = _My_data._Col;
        pointer _Myfirst = _My_data._Myfirst;
        pointer _Mylast  = _My_data._Mylast;

        const auto _Oldsize  = static_cast<size_type>(_Mylast - _Myfirst);

        const size_type _Newsize     = _Oldsize + _Col;
        const size_type _Newcapacity = _Calculate_growth(_Newsize);
        // check that we have no unused capacity
        _STL_INTERNAL_CHECK(_Newsize > static_cast<size_type>(_My_data._Myend - _Myfirst));

        if (_Oldsize == max_size()) {
            _Xlength();
        }

        const pointer _Newarr      = _Al.allocate(_Newcapacity);
        pointer _Constructed_first = _Newarr + _Where_row * _Col;
        pointer _Constructed_last  = _Constructed_first;
        const auto _Mid1           = _Myfirst + _Where_row * _Col;
        const auto _Mid2           = _Newarr + (_Where_row + 1) * _Col;

        _TRY_BEGIN
        for (; _Constructed_last < _Mid2; ++_Constructed_last) {
            _Alty_traits::construct(_Al, _Unfancy(_Constructed_last), _STD forward<_Valty>(_Val)...);
        }
        if (_Where_row == _Row) { // at back, provide strong guarantee
            if constexpr (is_nothrow_move_constructible_v<_Ty> || !is_copy_constructible_v<_Ty>) {
                _Uninitialized_move(_Myfirst, _Mylast, _Newarr, _Al);
            } else {
                _Uninitialized_copy(_Myfirst, _Mylast, _Newarr, _Al);
            }
        } else { // provide basic guarantee
            _Uninitialized_move(_Myfirst, _Mid1, _Newarr, _Al);
            _Constructed_first = _Newarr;
            _Uninitialized_move(_Mid1, _Mylast, _Mid2, _Al);
        }
        _CATCH_ALL 
        _Destroy_range(_Constructed_first, _Constructed_last, _Al);
        _Al.deallocate(_Newarr, _Newcapacity);
        _RERAISE;
        _CATCH_END

        _Change_array(_Newarr, _Row + 1, _Col, _Newcapacity);
    }

    template <class... _Valty>
    _CONSTEXPR20 void _Emplace_v_reallocate(const size_type _Where_col, _Valty&&... _Val) {
        // reallocate and insert by perfectly forwarding _Val at column of _Where_col
        _Alty& _Al       = _Getal();
        auto& _My_data   = _Mypair._Myval2;
        const auto _Row  = _My_data._Row;
        const auto _Col  = _My_data._Col;
        pointer _Myfirst = _My_data._Myfirst;
        pointer _Mylast  = _My_data._Mylast;

        const auto _Oldsize = static_cast<size_type>(_Mylast - _Myfirst);

        const size_type _Newsize     = _Oldsize + _Row;
        const size_type _Newcapacity = _Calculate_growth(_Newsize);
        // check that we have no unused capacity
        _STL_INTERNAL_CHECK(_Newsize > static_cast<size_type>(_My_data._Myend - _Myfirst));

        if (_Oldsize == max_size()) {
            _Xlength();
        }

        const pointer _Newarr      = _Al.allocate(_Newcapacity);
        const auto _Mid_off       = _Where_col;
        pointer _Constructed_first = _Newarr;
        pointer _Constructed_last  = _Constructed_first;
        pointer _Src_first         = _Myfirst + _Mid_off;
        pointer _Last              = _Newarr + _Newsize - _Col - 1;

        _TRY_BEGIN
        _Uninitialized_move(_Myfirst, _Myfirst + _Mid_off, _Constructed_last, _Al);
        _Constructed_last += _Mid_off;
        _Alty_traits::construct(_Al, _Unfancy(_Constructed_last), _STD forward<_Valty>(_Val)...);
        _Constructed_last += 1;
        for (; _Constructed_last <= _Last; _Src_first += _Col) {
            _Uninitialized_move(_Src_first, _Src_first + _Col, _Constructed_last, _Al);
            _Constructed_last += _Col;
            _Alty_traits::construct(_Al, _Unfancy(_Constructed_last), _STD forward<_Valty>(_Val)...);
            _Constructed_last += 1;
        }    
        _Uninitialized_move(_Src_first, _Mylast, _Constructed_last, _Al);
        _CATCH_ALL //provide basic guarantee
        _Destroy_range(_Constructed_first, _Constructed_last, _Al);
        _Al.deallocate(_Newarr, _Newcapacity);
        _RERAISE;
        _CATCH_END

        _Change_array(_Newarr, _Row, _Col + 1, _Newcapacity);
    }

public:
    template <class... _Valty>
    _CONSTEXPR20 hiterator emplace_hback(_Valty&&... _Val) {
        _Emplace_one_at_hback(_STD forward<_Valty>(_Val)...);
        return hbegin(row() - 1);
    }

    template <class... _Valty>
    _CONSTEXPR20 viterator emplace_vback(_Valty&&... _Val) {
        _Emplace_one_at_vback(_STD forward<_Valty>(_Val)...);
        return vbegin(col() - 1);
    }

    _CONSTEXPR20 void push_hback(const _Ty& _Val) {
        emplace_hback(_Val);
    }

    _CONSTEXPR20 void push_vback(const _Ty& _Val) {
        emplace_vback(_Val);
    }

    template <class... _Valty>
    _CONSTEXPR20 hiterator emplace_h(const size_type _Where_row, _Valty&&... _Val) {
        // insert by perfectly forwarding _Val at _Where_row TODO:check comment
        auto& _My_data         = _Mypair._Myval2;
        const auto _Row        = _My_data._Row;
        const auto _Col        = _My_data._Col;
        const pointer _Myfirst = _My_data._Myfirst;
        const pointer _Oldlast = _My_data._Mylast;
#if _CONTAINER_DEBUG_LEVEL > 0
        _STL_VERIFY(_Where_row <= _Row, "array_2d emplace outside range");
#endif // _CONTAINER_DEBUG_LEVEL > 0
        if (_Col == 0) {
            ++_My_data._Row;
        } else if (_Oldlast + _Col <= _My_data._Myend) {
            if (_Where_row == _Row) { // at back, provide strong guarantee
                _Emplace_hback_with_unused_capacity(_STD forward<_Valty>(_Val)...);
            } else {
                auto& _Al       = _Getal();
                const auto _Mid = _Myfirst + _Where_row * _Col;
                _Alloc_temporary2<_Alty> _Obj(_Al, _STD forward<_Valty>(_Val)...); // handle aliasing
                // after constructing _Obj, provide basic guarantee
                _Orphan_range(_Mid, _Oldlast);
                _Uninitialized_move(_Oldlast - _Col, _Oldlast , _Oldlast, _Al);
                _Move_backward_unchecked(_Mid, _Oldlast - _Col, _Oldlast);
                _STD fill_n(_Mid, _Col, _Obj._Get_value());

                ++_My_data._Row;
                _My_data._Mylast += _Col;
            }
        } else {
            _Emplace_h_reallocate(_Where_row, _STD forward<_Valty>(_Val)...);
        }

        return hbegin(_Where_row);
    }

    template <class... _Valty>
    _CONSTEXPR20 viterator emplace_v(const size_type _Where_col, _Valty&&... _Val) {
        // insert by perfectly forwarding _Val at _Where TODO:check comment
        auto& _My_data          = _Mypair._Myval2;
        const auto _Row         = _My_data._Row;
        const auto _Col         = _My_data._Col;
        const pointer _Myfirst = _My_data._Myfirst;
        const pointer _Mylast  = _My_data._Mylast;
#if _CONTAINER_DEBUG_LEVEL > 0
        _STL_VERIFY(_Where_col <= _Col, "array_2d emplace outside range");
#endif // _CONTAINER_DEBUG_LEVEL > 0

        if (_Row == 0) {
            ++_My_data._Col;
        } else if (_Mylast + _Row <= _My_data._Myend) {
            if (_Where_col == _Col) {
                _Emplace_vback_with_unused_capacity(_STD forward<_Valty>(_Val)...);
            } else {
                auto& _Al = _Getal();
                _Alloc_temporary2<_Alty> _Obj(_Al, _STD forward<_Valty>(_Val)...);
                auto& _Tmp = _Obj._Get_value();

                pointer _New_last   = _Mylast + _Row;
                const auto _Mid_off_rev = _Col - _Where_col;

                auto _Constructed_last = _New_last;
                auto _Constructed_first = _Constructed_last;
                auto _Source_first = _Mylast - _Mid_off_rev;

                _TRY_BEGIN
                if (_Constructed_first - _Mid_off_rev > _Mylast) {
                    _Uninitialized_move(_Source_first, _Mylast, _Constructed_first - _Mid_off_rev, _Al);
                    _Constructed_first -= _Mid_off_rev;
                    _Alty_traits::construct(_Al, _Unfancy(_Constructed_first - 1), _STD forward<_Valty>(_Val)...);
                    _Constructed_first -= 1;
                    _Source_first -= _Col;

                    for (; _Constructed_first - _Col - 1 > _Mylast; _Source_first -= _Col) {
                        _Uninitialized_move(_Source_first, _Source_first + _Col, _Constructed_first - _Col, _Al);
                        _Constructed_first -= _Col;
                        _Alty_traits::construct(_Al, _Unfancy(_Constructed_first - 1), _STD forward<_Valty>(_Val)...);
                        _Constructed_first -= 1;
                    }

                    if (_Constructed_first - _Col - 1 == _Mylast) {
                        _Uninitialized_move(_Source_first, _Source_first + _Col, _Mylast + 1, _Al);
                        _Constructed_first -= _Col;
                        _Alty_traits::construct(_Al, _Unfancy(_Constructed_first - 1), _STD forward<_Valty>(_Val)...);
                        _Constructed_first -= 1;
                    }
                    else {
                        auto _Spilled = _Constructed_first - _Mylast;
                        _Uninitialized_move(_Source_first + _Col - _Spilled, _Source_first + _Col, _Mylast, _Al);
                        _Constructed_first -= _Spilled;
                        _Move_backward_unchecked(_Source_first, _Source_first + _Col - _Spilled, _Mylast);
                        _Constructed_first -= _Col - _Spilled;
                        *(_Constructed_first - 1) = _Tmp;
                        _Constructed_first -= 1;
                    }
                } else {
                    auto _Unspilled = _Mid_off_rev - _Row;
                    _Uninitialized_move(_Source_first + _Unspilled, _Mylast, _Mylast, _Al);
                    _Constructed_first -= _Mylast - _Source_first - _Unspilled;
                    _Move_backward_unchecked(_Source_first, _Source_first + _Unspilled, _Mylast);
                    _Constructed_first -= _Unspilled;
                    *(_Constructed_first - 1) = _Tmp;
                    _Constructed_first -= 1;
                }
                _Source_first -= _Col;
                for (auto _Iter = _Constructed_first - _Col - 1; _Iter >= _Myfirst; _Iter -= _Col + 1) {
                    _Move_backward_unchecked(_Source_first, _Source_first + _Col, _Iter + _Col + 1);
                    _Source_first -= _Col;
                    *_Iter = _Tmp;
                }
                _CATCH_ALL
                _Destroy_range(_STD max(_Constructed_first, _Mylast), _Constructed_last, _Al);
                _RERAISE;
                _CATCH_END

                _Orphan_range({ 0, _Row, _Col, _Col + 1 });
                ++_My_data._Col;
            }
        } else {
            _Emplace_v_reallocate(_Where_col, _STD forward<_Valty>(_Val)...);
        }

        return vbegin(_Where_col);
    }

    _CONSTEXPR20 hiterator insert(const_hiterator _Where_first, const_hiterator _Where_last, const _Ty& _Val) { 
#if _ITERATOR_DEBUG_LEVEL == 2
        auto& _My_data = _Mypair._Myval2;
        _STL_VERIFY(_Where_first._Getcont() == _STD addressof(_My_data), "array_2d imcompatible iterator");
        _STL_VERIFY(_Where_last._Getcont() == _STD addressof(_My_data), "array_2d imcompatible iterator");
        _STL_VERIFY(_Where_first.idx_row() <= _Where_last.idx_row(), "array_2d iterator reversed");
        _STL_VERIFY(_Where_first.idx_col() == 0 && _Where_last.idx_col() == _My_data._Col,
            "array_2d iterators have imcompatible distance");
#endif // _ITERATOR_DEBUG_LEVEL == 2

        const auto _Count = _Where_last.idx_row() - _Where_first.idx_row();
        return insert_h(_Where_first.idx_row(), _Count, _Val);
    }

    _CONSTEXPR20 viterator insert(const_viterator _Where_first, const_viterator _Where_last, const _Ty& _Val) { 
#if _ITERATOR_DEBUG_LEVEL == 2
        auto& _My_data = _Mypair._Myval2;
        _STL_VERIFY(_Where_first._Getcont() == _STD addressof(_My_data), "array_2d imcompatible iterator");
        _STL_VERIFY(_Where_last._Getcont() == _STD addressof(_My_data), "array_2d imcompatible iterator");
        _STL_VERIFY(_Where_first.idx_col() <= _Where_last.idx_col(), "array_2d iterator reversed");
        _STL_VERIFY(_Where_first.idx_row() == 0 && _Where_last.idx_row() == _My_data._Row,
            "array_2d iterators have imcompatible distance");
#endif // _ITERATOR_DEBUG_LEVEL == 2

        const auto _Count = _Where_last.idx_col() - _Where_first.idx_col();
        return insert_v(_Where_first.idx_col(), _Count, _Val);
    }

    _CONSTEXPR20 hiterator insert_h(const size_type _Where_row, const _Ty& _Val) {
        return emplace_h(_Where_row, _Val);
    }

    _CONSTEXPR20 viterator insert_v(const size_type _Where_col, const _Ty& _Val) {  
        return emplace_v(_Where_col, _Val);
    }

    _CONSTEXPR20 hiterator insert_h(const size_type _Where_row, const size_type _Count, const _Ty& _Val) {
        // insert _Val at _Count line of rows begin with _Where_row 
        auto& _Al        = _Getal();
        auto& _My_data   = _Mypair._Myval2;
        pointer _Myfirst = _My_data._Myfirst;
        pointer& _Mylast = _My_data._Mylast;
        const auto _Row  = _My_data._Row;
        const auto _Col  = _My_data._Col;

        pointer _Oldlast  = _Mylast;
#if _ITERATOR_DEBUG_LEVEL == 2
        _STL_VERIFY(_Where_row <= _Row, "array_2d insert outside range");
#endif // _ITERATOR_DEBUG_LEVEL == 2

        const auto _New_elements    = _Count * _Col;
        const auto _Unused_capacity = static_cast<size_type>(_My_data._Myend - _Oldlast);
        const bool _One_at_back     = _Count == 1 && _Where_row == _Row;

        if (_Count == 0) { // nothing to do, avoid invalidating iterators
        } else if (_Col == 0) {
            _My_data._Row += _Count;
        } else if (_New_elements > _Unused_capacity) { // reallocate
            const auto _Oldsize = static_cast<size_type>(_Oldlast - _Myfirst);

            if (_New_elements > max_size() - _Oldsize) {
                _Xlength();
            }

            const size_type _Newsize     = _Oldsize + _New_elements;
            const size_type _Newcapacity = _Calculate_growth(_Newsize);

            pointer _Newarr            = _Al.allocate(_Newcapacity);
            const auto _Mid1_off       = _Where_row * _Col;
            const auto _Mid2_off       = _Mid1_off + _New_elements;
            pointer _Constructed_first = _Newarr + _Mid1_off;
            pointer _Constructed_last  = _Constructed_first;

            _TRY_BEGIN 
            _Constructed_last = _Uninitialized_fill_n(_Newarr + _Mid1_off, _New_elements, _Val, _Al);
            if (_One_at_back) {  // provide strong guarantee
                if constexpr (is_nothrow_move_constructible_v<_Ty> || !is_copy_constructible_v<_Ty>) {
                    _Uninitialized_move(_Myfirst, _Oldlast, _Newarr, _Al);
                } else {
                    _Uninitialized_copy(_Myfirst, _Oldlast, _Newarr, _Al);
                }
            } else { // provide basic guarantee
                _Uninitialized_move(_Myfirst, _Myfirst + _Mid1_off, _Newarr, _Al);
                _Constructed_first = _Newarr;
                _Uninitialized_move(_Myfirst + _Mid1_off, _Oldlast, _Newarr + _Mid2_off, _Al);
            }
            _CATCH_ALL
            _Destroy_range(_Constructed_first, _Constructed_last, _Al);
            _Al.deallocate(_Newarr, _Newcapacity);
            _RERAISE;
            _CATCH_END

            _Change_array(_Newarr, _Row + _Count, _Col, _Newcapacity);
        } else if (_One_at_back) { // provide strong guarantee
            _Emplace_hback_with_unused_capacity(_Val);
        } else { // provide basic guarantee
            const _Alloc_temporary2<_Alty> _Tmp_storage(_Al, _Val); 
            const auto& _Tmp              = _Tmp_storage._Get_value();
            const pointer _Mid_ptr        = _Myfirst + _Where_row * _Col;
            const auto _Affected_elements = static_cast<size_type>(_Oldlast - _Mid_ptr);

            _Orphan_range(_Mid_ptr, _Oldlast);

            if (_New_elements > _Affected_elements) { // new stuff spills off end
                _Mylast = _Uninitialized_fill_n(_Oldlast, _New_elements - _Affected_elements, _Tmp, _Al);
                _Mylast = _Uninitialized_move(_Mid_ptr, _Oldlast, _Mylast, _Al);
                _STD fill(_Mid_ptr, _Oldlast, _Tmp);
            } else { // new stuff can all be assigned
                _Mylast = _Uninitialized_move(_Oldlast - _New_elements, _Oldlast, _Oldlast, _Al);
                _Move_backward_unchecked(_Mid_ptr, _Oldlast - _New_elements, _Oldlast);
                _STD fill(_Mid_ptr, _Mid_ptr + _New_elements, _Tmp);
            }

            _My_data._Row += _Count;
        }

        return hbegin(_Where_row);
    }

    _CONSTEXPR20 viterator insert_v(const size_type _Where_col, const size_type _Count, const _Ty& _Val) {
        // insert _Val at _Count line of columns begin with _Where_col
        auto& _Al        = _Getal();
        auto& _My_data   = _Mypair._Myval2;
        pointer _Myfirst = _My_data._Myfirst;
        pointer& _Mylast = _My_data._Mylast;
        const auto _Row  = _My_data._Row;
        const auto _Col  = _My_data._Col;

        const pointer _Oldlast  = _Mylast;
#if _ITERATOR_DEBUG_LEVEL == 2
        _STL_VERIFY(_Where_col <= _Col, "array_2d insert outside range");
#endif // _ITERATOR_DEBUG_LEVEL == 2

        const auto _New_elements    = _Count * _Row;
        const auto _Unused_capacity = static_cast<size_type>(_My_data._Myend - _Oldlast);
        const bool _One_at_back     = _Count == 1 && _Where_col == _Col;

        if (_Count == 0) { // nothing to do, avoid invalidating iterators
        } else if (_Row == 0) {
            _My_data._Col += _Count;
        } else if (_New_elements > _Unused_capacity) { // reallocate
            const auto _Oldsize = static_cast<size_type>(_Oldlast - _Myfirst);

            if (_New_elements > max_size() - _Oldsize) {
                _Xlength();
            }

            const size_type _Newsize     = _Oldsize + _New_elements;
            const size_type _Newcapacity = _Calculate_growth(_Newsize);

            const pointer _Newarr            = _Al.allocate(_Newcapacity);
            const pointer _Constructed_first = _Newarr;
            pointer _Constructed_last        = _Newarr;

            const auto _Mid1_off = _Where_col;
            const auto _Last     = _Newarr + _Newsize - _Col - _Count;
            pointer _Src_first   = _Myfirst + _Mid1_off;

            _TRY_BEGIN //provide basic guarantee
            _Uninitialized_move(_Myfirst, _Myfirst + _Mid1_off, _Constructed_last, _Al);
            _Constructed_last += _Mid1_off;
            _Uninitialized_fill_n(_Constructed_last, _Count, _Val, _Al);
            _Constructed_last += _Count;
            for (; _Constructed_last <= _Last; _Src_first += _Col) {
                _Uninitialized_move(_Src_first, _Src_first + _Col, _Constructed_last, _Al);
                _Constructed_last += _Col;
                _Uninitialized_fill_n(_Constructed_last, _Count, _Val, _Al);
                _Constructed_last += _Count;
            }
            _Uninitialized_move(_Src_first, _Mylast, _Constructed_last, _Al);

            _CATCH_ALL
            _Destroy_range(_Constructed_first, _Constructed_last, _Al);
            _Al.deallocate(_Newarr, _Newcapacity);
            _RERAISE;
            _CATCH_END

            _Change_array(_Newarr, _Row, _Col + _Count, _Newcapacity);
        } else if (_One_at_back) { // provide basic guarantee
            _Emplace_vback_with_unused_capacity(_Val);
        } else { // provide basic guarantee
            auto& _Al = _Getal();
            pointer _New_last       = _Mylast + _Row * _Count;
            const auto _Mid_off_rev = _Col - _Where_col;

            auto _Constructed_last  = _New_last;
            auto _Constructed_first = _Constructed_last;
            auto _Source_first      = _Mylast - _Mid_off_rev;

            _TRY_BEGIN
            if (_Constructed_first - _Mid_off_rev - _Count > _Mylast) {
                _Uninitialized_move(_Source_first, _Mylast, _Constructed_first - _Mid_off_rev, _Al);
                _Constructed_first -= _Mid_off_rev;
                _Uninitialized_fill_n(_Constructed_first - _Count, _Count, _Val, _Al);
                _Constructed_first -= _Count;
                _Source_first -= _Col;

                for (; _Constructed_first - _Col - _Count > _Mylast; _Source_first -= _Col) {
                    _Uninitialized_move(_Source_first, _Source_first + _Col, _Constructed_first - _Col, _Al);
                    _Constructed_first -= _Col;
                    _Uninitialized_fill_n(_Constructed_first - _Count, _Count, _Val, _Al);
                    _Constructed_first -= _Count;
                }

                auto _Spilled = _Constructed_first - _Mylast;
                if (_Spilled <= _Col) {
                    _Uninitialized_move(_Source_first + _Col - _Spilled, _Source_first + _Col, _Mylast, _Al);
                    _Constructed_first -= _Spilled;
                    _Move_backward_unchecked(_Source_first, _Source_first + _Col - _Spilled, _Mylast);
                    _Constructed_first -= _Col - _Spilled;
                    _STD fill_n(_Constructed_first - _Count, _Count, _Val);
                    _Constructed_first -= _Count;
                }
                else {
                    auto _New_spilled = _Spilled - _Col;
                    _Uninitialized_move(_Source_first, _Source_first + _Col, _Mylast + _New_spilled, _Al);
                    _Constructed_first -= _Col;
                    _Uninitialized_fill_n(_Mylast, _New_spilled, _Val, _Al);
                    _Constructed_first -= _New_spilled;
                    _STD fill_n(_Mylast - _Count + _New_spilled, _Count - _New_spilled, _Val);
                    _Constructed_first -= _Count - _New_spilled;
                }
            } else {
                auto _Unspilled = _Mid_off_rev + _Count - _New_elements;
                if (_Unspilled  > _Count) {
                    _Uninitialized_move(_Source_first + _Unspilled - _Count, _Mylast, _Mylast, _Al);
                    _Constructed_first -= _Mid_off_rev + _Count - _Unspilled;
                    _Move_backward_unchecked(_Source_first, _Source_first + _Unspilled - _Count, _Mylast);
                    _Constructed_first -= _Unspilled - _Count;
                    _STD fill_n(_Constructed_first - _Count, _Count, _Val);
                    _Constructed_first -= _Count;
                } else {
                    auto _New_spilled = _Count - _Unspilled;
                    _Uninitialized_move(_Source_first, _Mylast, _Mylast + _New_spilled, _Al);
                    _Constructed_first -= _Mid_off_rev;
                    _Uninitialized_fill_n(_Mylast, _New_spilled, _Val, _Al);
                    _Constructed_first -= _New_spilled;
                    _STD fill_n(_Mylast - _Unspilled, _Unspilled, _Val);
                    _Constructed_first -= _Unspilled;
                }
            }
            _Source_first -= _Col;
            for (auto _Iter = _Constructed_first - _Col - _Count; _Iter >= _Myfirst; _Iter -= _Col + _Count) {
                _Move_backward_unchecked(_Source_first, _Source_first + _Col, _Iter + _Col + _Count);
                _Source_first -= _Col;
                _STD fill_n(_Iter, _Count, _Val);
                _Constructed_first -= _Count;
            }
            _CATCH_ALL
            _Destroy_range(_STD max(_Constructed_first, _Mylast), _Constructed_last, _Al);
            _RERAISE;
            _CATCH_END

            _Orphan_range(_Myfirst + _Where_col, _Oldlast);
            _Mylast += _New_elements;
            _My_data._Col += _Count;
        }

        return vbegin(_Where_col);
    }

    _CONSTEXPR20 void assign(const size_type _New_row, const size_type _New_col, const _Ty& _Val) { 
        // clear and assign with _Val
        auto& _Al         = _Getal();
        auto& _My_data    = _Mypair._Myval2;
        pointer& _Myfirst = _My_data._Myfirst;
        pointer& _Mylast  = _My_data._Mylast;

        const auto _Newsize = _New_row * _New_col;

        _My_data._Orphan_all();
        const auto _Oldcapacity = static_cast<size_type>(_My_data._Myend - _Myfirst);
        if (_Newsize > _Oldcapacity) { // reallocate
            _Clear_and_reserve_geometric(_Newsize);
            _Mylast = _Uninitialized_fill_n(_Myfirst, _Newsize, _Val, _Al);
            _My_data._Row = _New_row;
            _My_data._Col = _New_col;
            return;
        }

        const auto _Oldsize = static_cast<size_type>(_Mylast - _Myfirst);
        if (_Newsize > _Oldsize) {
            _STD fill(_Myfirst, _Mylast, _Val);
            _Mylast = _Uninitialized_fill_n(_Mylast, _Newsize - _Oldsize, _Val, _Al);
        } else {
            const pointer _Newlast = _Myfirst + _Newsize;
            _STD fill(_Myfirst, _Newlast, _Val);
            _Destroy_range(_Newlast, _Mylast, _Al);
            _Mylast = _Newlast;
        }
        _My_data._Row = _New_row;
        _My_data._Col = _New_col;
    }

private:
    template <class _Iter>
    _CONSTEXPR20 void _Assign_range(const size_type _Row, const size_type _Col, _Iter _First, _Iter _Last) { 
        // assign forward range [_First, _Last)
        auto& _Al           = _Getal();
        auto& _My_data      = _Mypair._Myval2;
        pointer& _Myfirst   = _My_data._Myfirst;
        pointer& _Mylast    = _My_data._Mylast;
        pointer& _Myend     = _My_data._Myend;

#if _CONTAINER_DEBUG_LEVEL > 0
        const auto _Iter_size = _Convert_size<size_type>(static_cast<size_t>(_STD distance(_First, _Last)));
        _STL_VERIFY(_Row * _Col == _Iter_size, "array_2d invalid shape");
#endif // _CONTAINER_DEBUG_LEVEL > 0

        _My_data._Orphan_all();

        const auto _Oldcapacity = static_cast<size_type>(_Myend - _Myfirst);
        const auto _Newsize     = _Row * _Col;

        if (_Newsize > _Oldcapacity) {
            _Clear_and_reserve_geometric(_Newsize);
            _Mylast = _Uninitialized_copy(_First, _Last, _Myfirst, _Al);
            _My_data._Row = _Row;
            _My_data._Col = _Col;
            return;
        }

        const auto _Oldsize = static_cast<size_type>(_Mylast - _Myfirst);
        if (_Newsize > _Oldsize) {
            const _Iter _Mid = _STD next(_First, static_cast<difference_type>(_Oldsize));

            _Copy_unchecked(_First, _Mid, _Myfirst);
            _Mylast = _Uninitialized_copy(_Mid, _Last, _Mylast, _Al);           
        } else {
            const pointer _Newlast = _Myfirst + _Newsize;

            _Copy_unchecked(_First, _Last, _Myfirst);
            _Destroy_range(_Newlast, _Mylast, _Al);
            _Mylast = _Newlast;
        }

        _My_data._Row = _Row;
        _My_data._Col = _Col;
    }

public:
    template <class _Iter, enable_if_t<_Is_iterator_v<_Iter>, int> = 0>
    _CONSTEXPR20 void assign(const size_type _Row, const size_type _Col, _Iter _First, _Iter _Last) {
        _Adl_verify_range(_First, _Last);
        _Assign_range(_Row, _Col, _Get_unwrapped(_First), _Get_unwrapped(_Last));
    }

    _CONSTEXPR20 void assign(_Init_list_2d _Ilist) {
        auto pair = _Make_Init_list_2d_iterator_pair(_Ilist);
        auto _Row = _Ilist.size();     
        auto _Col = pair.first.col_size();
        _Assign_range(_Row, _Col, pair.first, pair.second);
    }

    _CONSTEXPR20 array_2d& operator=(const array_2d& _Right) { 
        if (this == _STD addressof(_Right)) {
            return *this;
        }

        auto& _Al       = _Getal();
        auto& _My_data  = _Mypair._Myval2;
        auto& _Right_al = _Right._Getal();
        if constexpr (_Choose_pocca_v<_Alty>) {
            if (_Al != _Right_al) {
                _Tidy();
                _My_data._Reload_proxy(_GET_PROXY_ALLOCATOR(_Alty, _Al), _GET_PROXY_ALLOCATOR(_Alty, _Right_al));
            }
        }

        _Pocca(_Al, _Right_al);
        auto& _Right_data = _Right._Mypair._Myval2;
        assign(_Right_data._Row, _Right_data._Col, _Right_data._Myfirst, _Right_data._Mylast);

        return *this;
    }

    _CONSTEXPR20 array_2d& operator=(_Init_list_2d _Ilist) { 
        auto pair = _Make_Init_list_2d_iterator_pair(_Ilist);
        auto _Row = _Ilist.size();
        auto _Col = pair.first.col_size();
        _Assign_range(_Row, _Col, pair.first, pair.second);
        return *this;
    }

private:
    template <class _Ty2>
    _CONSTEXPR20 void _Resize_reallocate(const size_type _New_row, const size_type _New_col, const _Ty2& _Val) { 
        auto& _Al        = _Getal();
        auto& _My_data   = _Mypair._Myval2;
        auto _Row        = _My_data._Row;
        auto _Col        = _My_data._Col;

        _My_data._Orphan_all();
        const size_type _Newcapacity = _Calculate_growth(_New_row * _New_col);
        const pointer _Newarr        = _Al.allocate(_Newcapacity);

        pointer _Constructed_first = _Newarr; 
        pointer _Constructed_last  = _Newarr; 
        pointer _Src_first         = _My_data._Myfirst;

        const auto _Col_moved  = _STD min(_Col, _New_col);
        const auto _Col_filled = _STD max(_New_col, _Col) - _Col;

        _TRY_BEGIN
        for (size_t i = 0; i < _STD min(_Row, _New_row); ++i) {
            _Constructed_last = _Uninitialized_move(_Src_first, _Src_first + _Col_moved, _Constructed_last, _Al);
            _Src_first += _Col;
            if constexpr (is_same_v<_Ty2, _Ty>) {
                _Constructed_last = _Uninitialized_fill_n(_Constructed_last, _Col_filled, _Val, _Al);
            } else {
                _STL_INTERNAL_STATIC_ASSERT(is_same_v<_Ty2, _Value_init_tag>);
                _Constructed_last = _Uninitialized_value_construct_n(_Constructed_last, _Col_filled, _Al);
            }
        }
        if (_New_row > _Row) {
            auto _Row_filled = _New_col * (_New_row - _Row);
            if constexpr (is_same_v<_Ty2, _Ty>) {
                _Uninitialized_fill_n(_Constructed_last, _Row_filled, _Val, _Al);
            } else {
                _STL_INTERNAL_STATIC_ASSERT(is_same_v<_Ty2, _Value_init_tag>);
                _Uninitialized_value_construct_n(_Constructed_last, _Row_filled, _Al);
            }
        }
        _CATCH_ALL 
        _Destroy_range(_Constructed_first, _Constructed_last, _Al);
        _Al.deallocate(_Newarr, _Newcapacity);
        _RERAISE;
        _CATCH_END

        _Change_array(_Newarr, _New_row, _New_col, _Newcapacity);
    }

    template <class _Ty2>
    _CONSTEXPR20 void _Resize_column_extended(const size_type _New_row, const size_type _New_col, const _Ty2& _Val) {
        auto& _Al         = _Getal();
        auto& _My_data    = _Mypair._Myval2;
        auto& _Row        = _My_data._Row;
        auto& _Col        = _My_data._Col;
        pointer& _Myfirst = _My_data._Myfirst;
        pointer& _Mylast  = _My_data._Mylast;

        const auto _Newsize = _New_row * _New_col;
        const auto _Newlast = _Myfirst + _Newsize;

        auto _Row_filled = (_New_row - _Row) * _New_col;

        const auto _Count = _New_col - _Col;

        auto _Constructed_last  = _Newlast;
        auto _Constructed_first = _Constructed_last;
        auto _Src_first         = _Myfirst + _STD min(_Row, _New_row) * _Col;

        _Orphan_range(_Myfirst + _Col + 1, _Mylast);
        _TRY_BEGIN
        if constexpr (is_same_v<_Ty2, _Ty>) {
            if (_New_row > _Row) {
                if (_Newlast - _Row_filled > _Mylast) {
                    _Uninitialized_fill_n(_Newlast - _Row_filled, _Row_filled, _Val, _Al);
                } else if (_Newlast > _Mylast) {
                    _Uninitialized_fill_n(_Mylast, _Newlast - _Mylast, _Val, _Al);
                    _STD fill(_Newlast - _Row_filled, _Mylast, _Val);
                } else {
                    _STD fill(_Newlast - _Row_filled, _Newlast, _Val);
                }
                _Constructed_first -= _Row_filled;
            }
            if (_Constructed_first - _Count > _Mylast) {
                _Uninitialized_fill_n(_Constructed_first - _Count, _Count, _Val, _Al);
                _Constructed_first -= _Count;
                _Src_first -= _Col;

                for (; _Constructed_first - _Col - _Count > _Mylast; _Src_first -= _Col) {
                    _Uninitialized_move(_Src_first, _Src_first + _Col, _Constructed_first - _Col, _Al);
                    _Constructed_first -= _Col;
                    _Uninitialized_fill_n(_Constructed_first - _Count, _Count, _Val, _Al);
                    _Constructed_first -= _Count;
                }

                auto _Spilled = _Constructed_first - _Mylast;
                if (_Spilled <= _Col) {
                    _Uninitialized_move(_Src_first + _Col - _Spilled, _Src_first + _Col, _Mylast, _Al);
                    _Constructed_first -= _Spilled;
                    _Move_backward_unchecked(_Src_first, _Src_first + _Col - _Spilled, _Mylast);
                    _Constructed_first -= _Col - _Spilled;
                    _STD fill_n(_Constructed_first - _Count, _Count, _Val);
                    _Constructed_first -= _Count;
                } else {
                    auto _New_spilled = _Spilled - _Col;
                    _Uninitialized_move(_Src_first, _Src_first + _Col, _Mylast + _New_spilled, _Al);
                    _Constructed_first -= _Col;
                    _Uninitialized_fill_n(_Mylast, _New_spilled, _Val, _Al);
                    _Constructed_first -= _New_spilled;
                    _STD fill_n(_Mylast - _Count + _New_spilled, _Count - _New_spilled, _Val);
                    _Constructed_first -= _Count - _New_spilled;
                }
            } else if (_Constructed_first > _Mylast) {
                auto _Spilled = _Constructed_first - _Mylast;
                _Uninitialized_fill_n(_Mylast, _Spilled, _Val, _Al);
                _Constructed_first -= _Count;
                _STD fill_n(_Constructed_first - _Count, _Count - _Spilled, _Val);
                _Move_backward_unchecked(_Src_first - _Count, _Src_first, _Constructed_first - _Count);
            }
            _Src_first -= _Col;
            for (auto _Iter = _Constructed_first - _Col - _Count; _Iter >= _Myfirst; _Iter -= _Col + _Count) {
                _STD fill_n(_Iter + _Col, _Count, _Val);
                _Move_backward_unchecked(_Src_first, _Src_first + _Col, _Iter + _Col);
                _Src_first -= _Col;
            }
        } else {
            _STL_INTERNAL_STATIC_ASSERT(is_same_v<_Ty2, _Value_init_tag>);
            _Alloc_temporary2<_Alty> _Obj(_Al);
            const auto& _Temp = _Obj._Get_value();

            if (_New_row > _Row) {
                if (_Newlast - _Row_filled > _Mylast) {
                    _Uninitialized_value_construct_n(_Newlast - _Row_filled, _Row_filled, _Al);
                } else if (_Newlast > _Mylast) {
                    _Uninitialized_value_construct_n(_Mylast, _Newlast - _Mylast, _Al);
                    _STD fill(_Newlast - _Row_filled, _Mylast, _Temp);
                } else {
                    _STD fill(_Newlast - _Row_filled, _Newlast, _Temp);
                }
                _Constructed_first -= _Row_filled;
            }
            if (_Constructed_first - _Count > _Mylast) {
                _Uninitialized_value_construct_n(_Constructed_first - _Count, _Count, _Al);
                _Constructed_first -= _Count;
                _Src_first -= _Col;

                for (; _Constructed_first - _Col - _Count > _Mylast; _Src_first -= _Col) {
                    _Uninitialized_move(_Src_first, _Src_first + _Col, _Constructed_first - _Col, _Al);
                    _Constructed_first -= _Col;
                    _Uninitialized_value_construct_n(_Constructed_first - _Count, _Temp, _Al);
                    _Constructed_first -= _Count;
                 }

                auto _Spilled = _Constructed_first - _Mylast;
                if (_Spilled <= _Col) {
                    _Uninitialized_move(_Src_first + _Col - _Spilled, _Src_first + _Col, _Mylast, _Al);
                    _Constructed_first -= _Spilled;
                    _Move_backward_unchecked(_Src_first, _Src_first + _Col - _Spilled, _Mylast);
                    _Constructed_first -= _Col - _Spilled;
                    _STD fill_n(_Constructed_first - _Count, _Count, _Temp);
                    _Constructed_first -= _Count;
                }
                else {
                    auto _New_spilled = _Spilled - _Col;
                    _Uninitialized_move(_Src_first, _Src_first + _Col, _Mylast + _New_spilled, _Al);
                    _Constructed_first -= _Col;
                    _Uninitialized_value_construct_n(_Mylast, _New_spilled, _Al);
                    _Constructed_first -= _New_spilled;
                    _STD fill_n(_Mylast - _Count + _New_spilled, _Count - _New_spilled, _Temp);
                    _Constructed_first -= _Count - _New_spilled;
                }
            }
            else if (_Constructed_first > _Mylast) {
                auto _Spilled = _Constructed_first - _Mylast;
                _Uninitialized_value_construct_n(_Mylast, _Spilled, _Al);
                _Constructed_first -= _Count;
                _STD fill_n(_Constructed_first - _Count, _Count - _Spilled, _Temp);
                _Move_backward_unchecked(_Src_first - _Count, _Src_first, _Constructed_first - _Count);
            }
            _Src_first -= _Col;
            for (auto _Iter = _Constructed_first - _Col - _Count; _Iter >= _Myfirst; _Iter -= _Col + _Count) {
                _STD fill_n(_Iter + _Col, _Count, _Temp);
                _Move_backward_unchecked(_Src_first, _Src_first + _Col, _Iter + _Col);
                _Src_first -= _Col;
            }
        }
        _CATCH_ALL
        _Destroy_range(_STD max(_Constructed_first, _Mylast), _Constructed_last, _Al);
        _RERAISE;
        _CATCH_END
    }

    template <class _Ty2>
    _CONSTEXPR20 void _Resize(const size_type _New_row, const size_type _New_col, const _Ty2& _Val) {
        // trim or append elements
        auto& _My_data    = _Mypair._Myval2;
        auto& _Row        = _My_data._Row;
        auto& _Col        = _My_data._Col;

        const auto _Newsize = _New_row * _New_col;

        if (_Row == _New_row && _Col == _New_col) { // nothing to do
        } else if (_Newsize > capacity()) { //reallocate, provide basic guarantee 
            _Resize_reallocate(_New_row, _New_col, _Val); 
        } else { // resize with current capacity, provide basic guarantee
            auto& _Al           = _Getal();
            pointer& _Myfirst   = _My_data._Myfirst;
            pointer& _Mylast    = _My_data._Mylast;
            const auto _Newlast = _Myfirst + _Newsize;

            if (!_New_row || !_New_col) {
            } else if (_New_col > _Col) { // column extended
                _Resize_column_extended(_New_row, _New_col, _Val);
            } else {
                pointer _Dest_first = _Myfirst;
                pointer _Src_last   = _Myfirst + _STD min(_Row, _New_row) * _Col;

                for (auto _Src_first = _Myfirst; _Src_first < _Src_last; _Src_first += _Col) {
                    _Dest_first = _Move_unchecked(_Src_first, _Src_first + _New_col, _Dest_first);
                }
                if (_New_row > _Row) {
                    if constexpr (is_same_v<_Ty2, _Ty>) {
                        if (_Newlast > _Mylast) {
                            _Uninitialized_fill_n(_Mylast, _Newlast - _Mylast, _Val, _Al);
                            _STD fill(_Dest_first, _Mylast, _Val);
                        } else {
                            _STD fill(_Dest_first, _Newlast, _Val);
                        }

                    } else {
                        _STL_INTERNAL_STATIC_ASSERT(is_same_v<_Ty2, _Value_init_tag>);
                        _Alloc_temporary2<_Alty> _Obj(_Al);
                        if (_Newlast > _Mylast) {
                            _Uninitialized_value_construct_n(_Mylast, _Newlast - _Mylast, _Al);
                            _STD fill(_Dest_first, _Mylast, _Obj._Get_value());
                        } else {
                            _STD fill(_Dest_first, _Newlast, _Obj._Get_value());
                        }
                    }
                }
            } 

            if (_Newlast < _Mylast) {
                _Destroy_range(_Newlast, _Mylast, _Al);
            }

            _Row = _New_row;
            _Col = _New_col;
            _Mylast = _Newlast;
        }
    }

public:
    _CONSTEXPR20 void resize(const size_type _New_row, const size_type _New_col) {
        _Resize(_New_row, _New_col, _Value_init_tag{});
    }

    _CONSTEXPR20 void resize(const size_type _New_row, const size_type _New_col, const _Ty& _Val) {
        _Resize(_New_row, _New_col, _Val);
    }

private:
    _CONSTEXPR20 void _Reallocate_exactly(const size_type _Newcapacity) {
        auto& _Al         = _Getal();
        auto& _My_data    = _Mypair._Myval2;
        pointer& _Myfirst = _My_data._Myfirst;
        pointer& _Mylast  = _My_data._Mylast;

        const auto _Size = static_cast<size_type>(_Mylast - _Myfirst);

        const pointer _Newarr = _Al.allocate(_Newcapacity);

        _TRY_BEGIN
        if constexpr (is_nothrow_move_constructible_v<_Ty> || !is_copy_constructible_v<_Ty>) {
            _Uninitialized_move(_Myfirst, _Mylast, _Newarr, _Al);
        } else {
            _Uninitialized_copy(_Myfirst, _Mylast, _Newarr, _Al);
        }
        _CATCH_ALL
        _Al.deallocate(_Newarr, _Newcapacity);
        _RERAISE;
        _CATCH_END

        _Change_array(_Newarr, _My_data._Row, _My_data._Col , _Newcapacity);
    }

    _CONSTEXPR20 void _Reserve(const size_type _Newcapacity) { 
        if (_Newcapacity > capacity()) {
            if (_Newcapacity > max_size()) {
                _Xlength();
            }

            _Reallocate_exactly(_Newcapacity);
        }
    }

public:
    _CONSTEXPR20 void reserve(const size_type _Newcapacity) {
        _Reserve(_Newcapacity);
    }

    _CONSTEXPR20 void reserve(const size_type _New_row, const size_type _New_col) {
        _Reserve(_New_row * _New_col);
    }

    _CONSTEXPR20 void shrink_to_fit() {
        auto& _My_data         = _Mypair._Myval2;
        const pointer _Oldlast = _My_data._Mylast;
        if (_Oldlast != _My_data._Myend) { // something to do
            const pointer _Oldfirst = _My_data._Myfirst;
            if (_Oldfirst == _Oldlast) {
                _Tidy();
            } else {
                _Reallocate_exactly(static_cast<size_type>(_Oldlast - _Oldfirst));
            }
        }
    }

    _CONSTEXPR20 void pop_hback() noexcept /* strengthened */ {
        auto& _Al        = _Getal();
        auto& _My_data   = _Mypair._Myval2;
        auto& _Row       = _My_data._Row;
        auto& _Col       = _My_data._Col;
        pointer& _Mylast = _My_data._Mylast;

#if _ITERATOR_DEBUG_LEVEL == 2
        _STL_VERIFY(_Row > 0, "array_2d empty row before pop");
#endif // _ITERATOR_DEBUG_LEVEL == 2

        _Orphan_range({ _Row - 1, _Row + 1, 0, _Col + 1 });
        _Destroy_range(_Mylast - _Col, _Mylast, _Al);

        _Mylast -= _Col;
        --_Row;
    }

    _CONSTEXPR20 void pop_vback() noexcept(is_nothrow_move_assignable_v<value_type>) {
        auto& _Al        = _Getal();
        auto& _My_data   = _Mypair._Myval2;
        auto& _Row       = _My_data._Row;
        auto& _Col       = _My_data._Col;
        pointer _Myfirst = _My_data._Myfirst;
        pointer& _Mylast = _My_data._Mylast;

#if _ITERATOR_DEBUG_LEVEL == 2
        _STL_VERIFY(_Col > 0, "array_2d empty column before pop");
#endif // _ITERATOR_DEBUG_LEVEL == 2

        pointer _Src_first  = _Myfirst + _Col;
        pointer _Dest_first = _Myfirst + _Col - 1;

        _Orphan_range({ 0, _Row + 1, _Col - 1, _Col + 1 });      
        for (; _Src_first < _Mylast; _Src_first += _Col, _Dest_first += _Col - 1) {
            _Move_unchecked(_Src_first, _Src_first + _Col - 1, _Dest_first);
        }
        _Destroy_range(_Mylast - _Col, _Mylast, _Al);

        _Mylast -= _Row;
        --_Col;
    }

    _CONSTEXPR20 hiterator erase(const_hiterator _Where_first, const_hiterator _Where_last) noexcept( 
        is_nothrow_move_assignable_v<value_type>) /* strengthened */ {
#if _ITERATOR_DEBUG_LEVEL == 2
        auto& _My_data = _Mypair._Myval2;
        _STL_VERIFY(_Where_first._Getcont() == _STD addressof(_My_data), "array_2d imcompatible iterator");
        _STL_VERIFY(_Where_last._Getcont() == _STD addressof(_My_data), "array_2d imcompatible iterator");
        _STL_VERIFY(_Where_first.idx_row() <= _Where_last.idx_row(), "array_2d iterator reversed");
        _STL_VERIFY(_Where_first.idx_col() == 0 && _Where_last.idx_col() == _My_data._Col,
            "array_2d iterators have imcompatible distance");
#endif // _ITERATOR_DEBUG_LEVEL == 2

        const auto _Count = _Where_last.idx_row() - _Where_first.idx_row();
        return erase_h(_Where_first.idx_row(), _Count);
    }

    _CONSTEXPR20 viterator erase(const_viterator _Where_first, const_viterator _Where_last) noexcept( 
        is_nothrow_move_assignable_v<value_type>) /* strengthened */ {
#if _ITERATOR_DEBUG_LEVEL == 2
        auto& _My_data = _Mypair._Myval2;
        _STL_VERIFY(_Where_first._Getcont() == _STD addressof(_My_data), "array_2d imcompatible iterator");
        _STL_VERIFY(_Where_last._Getcont() == _STD addressof(_My_data), "array_2d imcompatible iterator");
        _STL_VERIFY(_Where_first.idx_col() <= _Where_last.idx_col(), "array_2d iterator reversed");
        _STL_VERIFY(_Where_first.idx_row() == 0 && _Where_last.idx_row() == _My_data._Row,
            "array_2d iterators have imcompatible distance");
#endif // _ITERATOR_DEBUG_LEVEL == 2

        const auto _Count = _Where_last.idx_col() - _Where_first.idx_col();
        return erase_v(_Where_first.idx_row(), _Count);
    }

    _CONSTEXPR20 hiterator erase_h(const size_type _Where_row, const size_type _Count) noexcept(
        is_nothrow_move_assignable_v<value_type>) /* strengthened */ { 
        auto& _My_data = _Mypair._Myval2;
        auto& _Row     = _My_data._Row;
        auto& _Col     = _My_data._Col;
        auto _Myfirst  = _My_data._Myfirst;
        auto& _Mylast  = _My_data._Mylast;

#if _CONTAINER_DEBUG_LEVEL > 0
        _STL_VERIFY(_Where_row + _Count <= _Row, "array_2d erase outside range");
#endif // _CONTAINER_DEBUG_LEVEL > 0

        if (_Count == 0) {  // nothing to do, avoid invalidating iterators
        } else if (_Col == 0) {
            _Row -= _Count;
        } else {
            auto _Erased = _Count * _Col;
            pointer _Mid = _Myfirst + _Where_row * _Col;

            _Orphan_range({ _Where_row, _Row + 1, 0, _Col + 1 });
            _Move_unchecked(_Mid + _Erased, _Mylast, _Mid);
            _Destroy_range(_Mylast - _Erased, _Mylast, _Getal());

            _Mylast -= _Erased;
            _Row -= _Count;
        }

        return hbegin(_Where_row);
    }

    _CONSTEXPR20 viterator erase_v(const size_type _Where_col, const size_type _Count) noexcept(
        is_nothrow_move_assignable_v<value_type>) /* strengthened */ { 
        auto& _My_data = _Mypair._Myval2;
        auto& _Row     = _My_data._Row;
        auto& _Col     = _My_data._Col;
        auto _Myfirst  = _My_data._Myfirst;
        auto& _Mylast  = _My_data._Mylast;

#if _CONTAINER_DEBUG_LEVEL > 0
        _STL_VERIFY(_Where_col + _Count <= _Col, "array_2d erase outside range");
#endif // _CONTAINER_DEBUG_LEVEL > 0

        if (_Count == 0) {  // nothing to do, avoid invalidating iterators
        } else if (_Row == 0) {
            _Col -= _Count;
        } else {
            _Orphan_range(_Myfirst + _Where_col, _Mylast);

            const auto _New_col = _Col - _Count;
            pointer _Dest_first = _Myfirst + _Where_col;
            pointer _Src_first  = _Dest_first + _Count;

            for (;_Src_first + _New_col < _Mylast; _Src_first += _Col) {
                _Dest_first = _Move_unchecked(_Src_first, _Src_first + _New_col, _Dest_first);
            }
            _Dest_first = _Move_unchecked(_Src_first, _Src_first + _New_col - _Where_col, _Dest_first);
            _Destroy_range(_Dest_first, _Mylast, _Getal());

            _Mylast = _Dest_first;
            _Col = _New_col;
        }

        return vbegin(_Where_col);
    }

public:
    _CONSTEXPR20 void clear() noexcept { 
        auto& _My_data    = _Mypair._Myval2;
        pointer& _Myfirst = _My_data._Myfirst;
        pointer& _Mylast  = _My_data._Mylast;

        if (_Myfirst != _Mylast) {
            _My_data._Orphan_all();
            _Destroy_range(_Myfirst, _Mylast, _Getal());
            _Mylast = _Myfirst;
        }

        _My_data._Row = size_type();
        _My_data._Col = size_type();
    }

    _CONSTEXPR20 void swap(array_2d& _Right) noexcept /* strengthened */ { 
        if (this != _STD addressof(_Right)) {
            _Pocs(_Getal(), _Right._Getal());
            _Mypair._Myval2._Swap_val(_Right._Mypair._Myval2);
        }
    }

    _NODISCARD _CONSTEXPR20 _Ty* data() noexcept {
        return _Unfancy_maybe_null(_Mypair._Myval2._Myfirst);
    }

    _NODISCARD _CONSTEXPR20 const _Ty* data() const noexcept {
        return _Unfancy_maybe_null(_Mypair._Myval2._Myfirst);
    }

    _NODISCARD _CONSTEXPR20 iterator begin() noexcept { 
        auto& _My_data = _Mypair._Myval2;
        return iterator(_My_data._Myfirst, _STD addressof(_My_data));
    }

    _NODISCARD _CONSTEXPR20 const_iterator begin() const noexcept { 
        return const_iterator(const_cast<array_2d*>(this)->begin());
    }

    _NODISCARD _CONSTEXPR20 iterator end() noexcept { 
        auto& _My_data = _Mypair._Myval2;
        return iterator(_My_data._Mylast, _STD addressof(_My_data));
    }

    _NODISCARD _CONSTEXPR20 const_iterator end() const noexcept { 
        return const_iterator(const_cast<array_2d*>(this)->end());
    }

    _NODISCARD _CONSTEXPR20 reverse_iterator rbegin() noexcept { 
        return reverse_iterator(end());
    }

    _NODISCARD _CONSTEXPR20 const_reverse_iterator rbegin() const noexcept { 
        return const_reverse_iterator(end());
    }

    _NODISCARD _CONSTEXPR20 reverse_iterator rend() noexcept { 
        return reverse_iterator(begin());
    }
     
    _NODISCARD _CONSTEXPR20 const_reverse_iterator rend() const noexcept { 
        return const_reverse_iterator(begin());
    }

    _NODISCARD _CONSTEXPR20 const_iterator cbegin() const noexcept { 
        return begin();
    }

    _NODISCARD _CONSTEXPR20 const_iterator cend() const noexcept { 
        return end();
    }

    _NODISCARD _CONSTEXPR20 const_reverse_iterator crbegin() const noexcept { 
        return rbegin();
    }

    _NODISCARD _CONSTEXPR20 const_reverse_iterator crend() const noexcept { 
        return rend();
    }
    //horizontal iterators
    _NODISCARD _CONSTEXPR20 hiterator hbegin(size_type _Row) noexcept {
        auto& _My_data = _Mypair._Myval2;
        return hiterator(_Row, 0, _STD addressof(_My_data));
    }

    _NODISCARD _CONSTEXPR20 const_hiterator hbegin(size_type _Row) const noexcept { 
        return const_hiterator(const_cast<array_2d*>(this)->hbegin(_Row)); 
    }
     
    _NODISCARD _CONSTEXPR20 hiterator hend(size_type _Row) noexcept { 
        auto& _My_data = _Mypair._Myval2;
        return hiterator(_Row, col(), _STD addressof(_My_data));
    }

    _NODISCARD _CONSTEXPR20 const_hiterator hend(size_type _Row) const noexcept { 
        return const_hiterator(const_cast<array_2d*>(this)->hend(_Row));
    }

    _NODISCARD _CONSTEXPR20 reverse_hiterator rhbegin(size_type _Row) noexcept { 
        return reverse_hiterator(hend(_Row));
    }

    _NODISCARD _CONSTEXPR20 const_reverse_hiterator rhbegin(size_type _Row) const noexcept { 
        return const_reverse_hiterator(hend(_Row));
    }

    _NODISCARD _CONSTEXPR20 reverse_hiterator rhend(size_type _Row) noexcept { 
        return reverse_hiterator(hbegin(_Row)); 
    }

    _NODISCARD _CONSTEXPR20 const_reverse_hiterator rhend(size_type _Row) const noexcept { 
        return const_reverse_hiterator(hbegin(_Row));
    }

    _NODISCARD _CONSTEXPR20 const_hiterator chbegin(size_type _Row) const noexcept { 
        return hbegin(_Row);
    }

    _NODISCARD _CONSTEXPR20 const_hiterator chend(size_type _Row) const noexcept { 
        return hend(_Row);
    }

    _NODISCARD _CONSTEXPR20 const_reverse_hiterator crhbegin(size_type _Row) const noexcept { 
        return rhbegin(_Row);
    }

    _NODISCARD _CONSTEXPR20 const_reverse_hiterator crhend(size_type _Row) const noexcept { 
        return rhend(_Row);
    }
    //vertical iterators
    _NODISCARD _CONSTEXPR20 viterator vbegin(size_type _Col) noexcept { 
        auto& _My_data = _Mypair._Myval2;
        return viterator(0, _Col, _STD addressof(_My_data));
    }

    _NODISCARD _CONSTEXPR20 const_viterator vbegin(size_type _Col) const noexcept { 
        return const_viterator(const_cast<array_2d*>(this)->vbegin(_Col));
    }

    _NODISCARD _CONSTEXPR20 viterator vend(size_type _Col) noexcept { 
        auto& _My_data = _Mypair._Myval2;
        return viterator(row(), _Col, _STD addressof(_My_data));
    }

    _NODISCARD _CONSTEXPR20 const_viterator vend(size_type _Col) const noexcept { 
        return const_viterator(const_cast<array_2d*>(this)->vend(_Col));
    }

    _NODISCARD _CONSTEXPR20 reverse_viterator rvbegin(size_type _Col) noexcept { 
        return reverse_viterator(vend(_Col));
    }

    _NODISCARD _CONSTEXPR20 const_reverse_viterator rvbegin(size_type _Col) const noexcept { 
        return const_reverse_viterator(vend(_Col)); 
    }

    _NODISCARD _CONSTEXPR20 reverse_viterator rvend(size_type _Col) noexcept { 
        return reverse_viterator(vbegin(_Col));
    }

    _NODISCARD _CONSTEXPR20 const_reverse_viterator rvend(size_type _Col) const noexcept { 
        return const_reverse_viterator(vbegin(_Col));
    }

    _NODISCARD _CONSTEXPR20 const_viterator cvbegin(size_type _Col) const noexcept { 
        return vbegin(_Col);
    }

    _NODISCARD _CONSTEXPR20 const_viterator cvend(size_type _Col) const noexcept { 
        return vend(_Col);
    }

    _NODISCARD _CONSTEXPR20 const_reverse_viterator crvbegin(size_type _Col) const noexcept { 
        return rvbegin(_Col);
    }

    _NODISCARD _CONSTEXPR20 const_reverse_viterator crvend(size_type _Col) const noexcept { 
        return rvend(_Col);
    }

    _NODISCARD _CONSTEXPR20 pointer _Unchecked_begin() noexcept { 
        return _Mypair._Myval2._Myfirst;
    }

    _NODISCARD _CONSTEXPR20 const_pointer _Unchecked_begin() const noexcept { 
        return _Mypair._Myval2._Myfirst;
    }

    _NODISCARD _CONSTEXPR20 pointer _Unchecked_end() noexcept { 
        return _Mypair._Myval2._Mylast;
    }

    _NODISCARD _CONSTEXPR20 const_pointer _Unchecked_end() const noexcept { 
        return _Mypair._Myval2._Mylast;
    }

public:
    template <class _Alloc2>
    _NODISCARD _CONSTEXPR20 array_2d slice_size(const size_type _Row_begin, const size_type _Row_size,
        const size_type _Col_begin, const size_type _Col_size, const _Alloc2& _Al) const noexcept {
        _Range_t _Range { _Row_begin, _Row_begin + _Row_size, _Col_begin, _Col_begin + _Col_size };
        return array_2d<_Ty, _Alloc2>(*this, _Range, _Al);
    }

    _NODISCARD _CONSTEXPR20 array_2d slice_size(const size_type _Row_begin, const size_type _Row_size,
        const size_type _Col_begin, const size_type _Col_size) const noexcept {
        return slice_size(_Row_begin, _Col_begin, _Row_size, _Col_size, _Getal());
    }

    template <class _Alloc2>
    _NODISCARD _CONSTEXPR20 array_2d slice(const size_type _Row_begin, const size_type _Row_end,
        const size_type _Col_begin, const size_type _Col_end, const _Alloc2 &_Al) const noexcept {
        _Range_t _Range{ _Row_begin, _Row_end, _Col_begin, _Col_end };
        return array_2d<_Ty, _Alloc2>(*this, _Range, _Al);
    }

    _NODISCARD _CONSTEXPR20 array_2d slice(const size_type _Row_begin, const size_type _Row_end,
        const size_type _Col_begin, const size_type _Col_end) const noexcept {
        return slice(_Row_begin, _Row_end, _Col_begin, _Col_end, _Getal());
    }

    template<class _Ty2, class _Alloc2, enable_if_t<is_convertible_v<_Ty2, _Ty>, int> = 0>
    _CONSTEXPR20 void patch(const array_2d<_Ty2, _Alloc2>& _Src, const size_type _Where_row, const size_type _Where_col,  
        const size_type _Width, const size_type _Height) {
        auto& _My_data  = _Mypair._Myval2;
        auto& _Src_data = _Src._Mypair._Myval2;
        auto _My_col    = _My_data._Col;
        auto _Src_col   = _Src_data._Col;

#if _CONTAINER_DEBUG_LEVEL > 0
        const auto _My_row = _My_data._Row;
        _STL_VERIFY(_Where_row + _Width <= _My_row, "array_2d patch outside range");
        _STL_VERIFY(_Where_col + _Height <= _My_col, "array_2d patch outside range");
#endif // _CONTAINER_DEBUG_LEVEL > 0

        pointer _Dest  = _My_data._Myfirst + _Where_row * _My_col + _Where_col;
        pointer _First = _Src_data._Myfirst;
        pointer _Last  = _First + _Height * _Src_col;

        for (auto _Iter = _First; _Iter < _Last; _Iter += _Src_col) {
            _STD copy_n(_Iter, _Width, _Dest);
            _Dest += _My_col;
        }
    }

    template<class _Ty2, class _Alloc2, enable_if_t<is_convertible_v<_Ty2, _Ty>, int> = 0>
    _CONSTEXPR20 void patch(const array_2d<_Ty2, _Alloc2>& _Src, const size_type _Where_row, const size_type _Where_col) {
        patch(_Src, _Where_row, _Where_col, _Src.row(), _Src.col());
    }

public:
    _NODISCARD _CONSTEXPR20 bool empty() const noexcept { 
        auto& _My_data = _Mypair._Myval2;
        return _My_data._Myfirst == _My_data._Mylast;
    }

    _NODISCARD _CONSTEXPR20 size_type size() const noexcept {
        auto& _My_data = _Mypair._Myval2; 
        return static_cast<size_type>(_My_data._Mylast - _My_data._Myfirst);
    }

    _NODISCARD _CONSTEXPR20 pair<size_type, size_type> shape() const noexcept {
        auto& _My_data = _Mypair._Myval2;
        return make_pair(_My_data._Row, _My_data._Col);
    }

    _NODISCARD _CONSTEXPR20 size_type row() const noexcept { 
        return _Mypair._Myval2._Row;
    }

    _NODISCARD _CONSTEXPR20 size_type col() const noexcept { 
        return _Mypair._Myval2._Col;
    }

    _NODISCARD _CONSTEXPR20 size_type max_size() const noexcept {
        return _STD min(
            static_cast<size_type>(numeric_limits<difference_type>::max()), _Alty_traits::max_size(_Getal()));
    }

    _NODISCARD _CONSTEXPR20 size_type capacity() const noexcept {
        auto& _My_data = _Mypair._Myval2;
        return static_cast<size_type>(_My_data._Myend - _My_data._Myfirst);
    }

    _NODISCARD _CONSTEXPR20 indexor operator[](const size_type _Row) noexcept {
        auto& _My_data  = _Mypair._Myval2;
        const auto _Col = _My_data._Col;
#if _CONTAINER_DEBUG_LEVEL > 0
        _STL_VERIFY(_Row < _My_data._Row, "array_2d subscript out of range");
        return indexor(_My_data._Myfirst + _Row * _Col, _Col);
#else
        return indexor(_My_data._Myfirst + _Row * _Col);
#endif // _CONTAINER_DEBUG_LEVEL > 0
    }

    _NODISCARD _CONSTEXPR20 const const_indexor operator[](const size_type _Row) const noexcept { 
        auto& _My_data  = _Mypair._Myval2;
        const auto _Col = _My_data._Col;
#if _CONTAINER_DEBUG_LEVEL > 0
        _STL_VERIFY(_Row < _My_data._Row, "array_2d subscript out of range");
        return const_indexor(_My_data._Myfirst + _Row * _Col, _Col);
#else
        return const_indexor(_My_data._Myfirst + _Row * _Col);
#endif // _CONTAINER_DEBUG_LEVEL > 0
    }

    _NODISCARD _CONSTEXPR20 _Ty& at(const size_type _Row, const size_type _Col) { 
        auto& _My_data = _Mypair._Myval2;
        if (static_cast<size_type>(_My_data._Row) <= _Row || 
            static_cast<size_type>(_My_data._Col) <= _Col) {
            _Xrange();
        }
 
        return (_My_data._Myfirst + _Row * _My_data._Col)[_Col];
    }

    _NODISCARD _CONSTEXPR20 const _Ty& at(const size_type _Row, const size_type _Col) const {
        auto& _My_data = _Mypair._Myval2;
        if (static_cast<size_type>(_My_data._Row) <= _Row ||
            static_cast<size_type>(_My_data._Col) <= _Col) {
            _Xrange();
        }

        return (_My_data._Myfirst + _Row * _My_data._Col)[_Col];
    }

    _NODISCARD _CONSTEXPR20 allocator_type get_allocator() const noexcept {
        return static_cast<allocator_type>(_Getal());
    }

private:
    _CONSTEXPR20 void _Clear_and_reserve_geometric(const size_type _Newsize) { 
        auto& _Al         = _Getal();
        auto& _My_data    = _Mypair._Myval2;
        pointer& _Myfirst = _My_data._Myfirst;
        pointer& _Mylast  = _My_data._Mylast;
        pointer& _Myend   = _My_data._Myend;

#if _ITERATOR_DEBUG_LEVEL != 0 && defined(_ENABLE_STL_INTERNAL_CHECK)
        _STL_INTERNAL_CHECK(_Newsize != 0);
        _Check_all_orphaned();
#endif // _ITERATOR_DEBUG_LEVEL != 0 && defined(_ENABLE_STL_INTERNAL_CHECK)

        if (_Newsize > max_size()) {
            _Xlength();
        }

        const size_type _Newcapacity = _Calculate_growth(_Newsize);

        if (_Myfirst) { // destroy and deallocate old array
            _Destroy_range(_Myfirst, _Mylast, _Al);
            _Al.deallocate(_Myfirst, static_cast<size_type>(_Myend - _Myfirst));

            _My_data._Row = size_type();
            _My_data._Col = size_type();
            _Myfirst      = nullptr;
            _Mylast       = nullptr;
            _Myend        = nullptr;
        }

        _Buy_raw(_Newcapacity);
    }

    _CONSTEXPR20 size_type _Calculate_growth(const size_type _Newsize) const {
        // given _Oldcapacity and _Newsize, calculate geometric growth
        const size_type _Oldcapacity = capacity();
        const auto _Max              = max_size();

        if (_Oldcapacity > _Max - _Oldcapacity / 2) {
            return _Max; // geometric growth would overflow
        }

        const size_type _Geometric = _Oldcapacity + _Oldcapacity / 2;

        if (_Geometric < _Newsize) {
            return _Newsize; // geometric growth would be insufficient
        }

        return _Geometric; // geometric growth is sufficient
    }

    _CONSTEXPR20 void _Buy_raw(const size_type _Newcapacity) {
        // allocate array with _Newcapacity elements
        auto& _My_data    = _Mypair._Myval2;
        size_type _Row    = _My_data._Row;
        size_type _Col    = _My_data._Col;
        pointer& _Myfirst = _My_data._Myfirst;
        pointer& _Mylast  = _My_data._Mylast;
        pointer& _Myend   = _My_data._Myend;

        _STL_INTERNAL_CHECK(_Row * _Col == static_cast<size_type>(_Mylast - _Myfirst));
        _STL_INTERNAL_CHECK(!_Myfirst && !_Mylast && !_Myend); // check that *this is tidy
        _STL_INTERNAL_CHECK(0 < _Newcapacity && _Newcapacity <= max_size());

        const auto _Newarr = _Getal().allocate(_Newcapacity);
        _Myfirst           = _Newarr;
        _Mylast            = _Newarr;
        _Myend             = _Newarr + _Newcapacity;
    }

    _CONSTEXPR20 void _Buy_nonzero(const size_type _Newcapacity) {
        // allocate array with _Newcapacity elements
#ifdef _ENABLE_STL_INTERNAL_CHECK
        auto& _My_data = _Mypair._Myval2;
        size_type _Row = _My_data._Row;
        size_type _Col = _My_data._Col;
        pointer& _Myfirst = _My_data._Myfirst;
        pointer& _Mylast  = _My_data._Mylast;
        pointer& _Myend   = _My_data._Myend;

        _STL_INTERNAL_CHECK(_Row * _Col == static_cast<size_type>(_Mylast - _Myfirst));
        _STL_INTERNAL_CHECK(!_Myfirst && !_Mylast && !_Myend); // check that *this is tidy
        _STL_INTERNAL_CHECK(0 < _Newcapacity);
#endif // _ENABLE_STL_INTERNAL_CHECK

        if (_Newcapacity > max_size()) {
            _Xlength();
        }

        _Buy_raw(_Newcapacity);
    }

    _CONSTEXPR20 void _Change_array(const pointer _Newvec, const size_type _Row, const size_type _Col, 
        const size_type _Newcapacity) {
        // orphan all iterators, discard old array, acquire new array
        auto& _Al         = _Getal();
        auto& _My_data    = _Mypair._Myval2;
        pointer& _Myfirst = _My_data._Myfirst;
        pointer& _Mylast  = _My_data._Mylast;
        pointer& _Myend   = _My_data._Myend;
        auto _Newsize     = _Row * _Col;

        _My_data._Orphan_all();

        if (_Myfirst) { // destroy and deallocate old array
            _Destroy_range(_Myfirst, _Mylast, _Al);
            _Al.deallocate(_Myfirst, static_cast<size_type>(_Myend - _Myfirst));
        }

        _My_data._Row = _Row;
        _My_data._Col = _Col;
        _Myfirst      = _Newvec;
        _Mylast       = _Newvec + _Newsize;
        _Myend        = _Newvec + _Newcapacity;
    }

    _CONSTEXPR20 void _Tidy() noexcept { // free all storage
        auto& _Al         = _Getal();
        auto& _My_data    = _Mypair._Myval2;
        pointer& _Myfirst = _My_data._Myfirst;
        pointer& _Mylast  = _My_data._Mylast;
        pointer& _Myend   = _My_data._Myend;

        _My_data._Orphan_all();

        if (_Myfirst) { // destroy and deallocate old array
            _Destroy_range(_Myfirst, _Mylast, _Al);
            _Al.deallocate(_Myfirst, static_cast<size_type>(_Myend - _Myfirst));

            _My_data._Row = size_type();
            _My_data._Col = size_type();
            _Myfirst      = nullptr;
            _Mylast       = nullptr;
            _Myend        = nullptr;
        }
    }

    template <class... _Valty>
    _CONSTEXPR20 void _Construct_n(_CRT_GUARDOVERFLOW const size_type _Row, _CRT_GUARDOVERFLOW const size_type _Col, _Valty&&... _Val) { 
        // Dispatches between the three sized constructions.
        // 1-arg -> value-construction, e.g. array_2d(3, 5)
        // 2-arg -> fill, e.g. array2d(3, 5, "meow")
        // 3-arg -> sized range construction, e.g. array2d{ { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } }

        auto& _Al         = _Getal();
        auto&& _Alproxy   = _GET_PROXY_ALLOCATOR(_Alty, _Al);
        auto& _My_data    = _Mypair._Myval2;
        const auto _Count = _Row * _Col;

        _My_data._Row = _Row;
        _My_data._Col = _Col;
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _My_data);
        if (_Count != 0) {
            _Buy_nonzero(_Count);
            _Tidy_guard<array_2d> _Guard{this};
            if constexpr (sizeof...(_Val) == 0) {
                _My_data._Mylast = _Uninitialized_value_construct_n(_My_data._Myfirst, _Count, _Al);
            } else if constexpr (sizeof...(_Val) == 1) {
                _STL_INTERNAL_STATIC_ASSERT(is_same_v<_Valty..., const _Ty&>);
                _My_data._Mylast = _Uninitialized_fill_n(_My_data._Myfirst, _Count, _Val..., _Al);
            } else if constexpr (sizeof...(_Val) == 2) {
#if _CONTAINER_DEBUG_LEVEL > 0
                const auto _Iter_dist = _Convert_size<size_type>(static_cast<size_t>(_STD distance(_STD forward<_Valty>(_Val)...)));
                _STL_VERIFY(_Row * _Col == _Iter_dist, "array_2d invalid shape");
#endif // _CONTAINER_DEBUG_LEVEL > 0
                _My_data._Mylast = _Uninitialized_copy(_STD forward<_Valty>(_Val)..., _My_data._Myfirst, _Al);
            } else {
                static_assert(_Always_false<_Ty>, "Should be unreachable");
            }
            _Guard._Target = nullptr;
        }
        _Proxy._Release();
    }

    _CONSTEXPR20 void _Construct_ranged_copy(const array_2d& _Arr, const _Range_t& _Range) {
        auto& _Al       = _Getal();
        auto&& _Alproxy = _GET_PROXY_ALLOCATOR(_Alty, _Al);
        auto& _My_data  = _Mypair._Myval2;
        auto& _Arr_data = _Arr._Mypair._Myval2;
        auto& _Mylast   = _My_data._Mylast;

        const size_t _Row_begin = _Range._Row_begin;
        const size_t _Row_size  = _Range._Row_end - _Row_begin;
        const size_t _Col_begin = _Range._Col_begin;
        const size_t _Col_size  = _Range._Col_end - _Col_begin;
        const size_t _Capacity  = _Row_size * _Col_size;
        const size_t _Arr_col   = _Arr_data._Col;

#if _CONTAINER_DEBUG_LEVEL > 0
        const size_t _Arr_row = _Arr_data._Row;
        _STL_VERIFY(_Row_begin + _Row_size <= _Arr_row, "array_2d outside range");
        _STL_VERIFY(_Col_begin + _Col_size <= _Arr_col, "array_2d outside range");
#endif // _CONTAINER_DEBUG_LEVEL > 0

        _My_data._Row = _Row_size;
        _My_data._Col = _Col_size;
        _Container_proxy_ptr<_Alty> _Proxy(_Alproxy, _My_data);

        if (_Capacity != 0) {
            _Buy_nonzero(_Capacity);
            _Tidy_guard<array_2d> _Guard{ this };
            
            pointer _First = _Arr_data._Myfirst + _Row_begin * _Arr_col + _Col_begin;
            pointer _Last  = _First + _Row_size * _Arr_col;
            
            for (auto _Iter = _First; _Iter != _Last; _Iter += _Arr_col) {
                _Mylast = _Uninitialized_copy(_Iter, _Iter + _Col_size, _Mylast, _Al);
            }
            _Guard._Target = nullptr;
        }
        _Proxy._Release();
    }

    _CONSTEXPR20 void _Move_assign_unequal_alloc(array_2d& _Right) {
        auto& _Al         = _Getal();
        auto& _My_data    = _Mypair._Myval2;
        auto& _Right_data = _Right._Mypair._Myval2;

        const pointer _First = _Right_data._Myfirst;
        const pointer _Last  = _Right_data._Mylast;
        const auto _Newsize  = static_cast<size_type>(_Last - _First);

        pointer& _Myfirst = _My_data._Myfirst;
        pointer& _Mylast  = _My_data._Mylast;

        _My_data._Orphan_all();
        const auto _Oldcapacity = static_cast<size_type>(_My_data._Myend - _Myfirst);
        if (_Newsize > _Oldcapacity) {
            _Clear_and_reserve_geometric(_Newsize);
            _Mylast = _Uninitialized_move(_First, _Last, _Myfirst, _Al);
            return;
        }

        const auto _Oldsize = static_cast<size_type>(_Mylast - _Myfirst);
        if (_Newsize > _Oldsize) {
            const pointer _Mid = _First + _Oldsize;
            _Move_unchecked(_First, _Mid, _Myfirst);
            _Mylast = _Uninitialized_move(_Mid, _Last, _Mylast, _Al);
        } else {
            const pointer _Newlast = _Myfirst + _Newsize;
            _Move_unchecked(_First, _Last, _Myfirst);
            _Destroy_range(_Newlast, _Mylast, _Al);
            _Mylast = _Newlast;
        }
    }

    [[noreturn]] static void _Xlength() { 
        _Xlength_error("array_2d too long");
    }

    [[noreturn]] static void _Xrange() { 
        _Xout_of_range("invalid array_2d subscript");
    }

#if _ITERATOR_DEBUG_LEVEL == 2
    template <class... _Valty>
    _CONSTEXPR20 void _Orphan_range_unlocked(_Valty&&... _Range) const noexcept {
        _Iterator_base12** _Pnext = &_Mypair._Myval2._Myproxy->_Myfirstiter;
        while (*_Pnext) {
            const auto _Pnext_iter = static_cast<iterator&>(**_Pnext);
            if (_Pnext_iter._In_range(_STD forward<_Valty>(_Range)...)) { // orphan the iterator
                const auto _Temp = *_Pnext;
                _Temp->_Myproxy = nullptr;
                *_Pnext = _Temp->_Mynextiter;
            } else { // skip the iterator
                const auto _Temp = *_Pnext;
                _Pnext = &_Temp->_Mynextiter;
            }
        }
    }

    template <class... _Valty>
    void _Orphan_range_locked(_Valty&&... _Range) const {
        _Lockit _Lock(_LOCK_DEBUG);
        _Orphan_range_unlocked(_STD forward<_Valty>(_Range)...);
    }

    _CONSTEXPR20 void _Orphan_range(const _Range_t& _Range) const {
        // orphan iterators within specified range
        _Orphan_range_locked(_Range);
    }

    _CONSTEXPR20 void _Orphan_range(pointer _First, pointer _Last) const {
        // orphan iterators within specified range
        _Orphan_range_locked(_First, _Last);
    }
#else // ^^^ _ITERATOR_DEBUG_LEVEL == 2 ^^^ // vvv _ITERATOR_DEBUG_LEVEL != 2 vvv
    _CONSTEXPR20 void _Orphan_range(const _Range_t& _Range) const {}

    _CONSTEXPR20 void _Orphan_range(pointer _First, pointer _Last) const {}
#endif // _ITERATOR_DEBUG_LEVEL != 2
    _NODISCARD _CONSTEXPR20 _Alty& _Getal() noexcept {
        return _Mypair._Get_first();
    }
     
    _NODISCARD _CONSTEXPR20 const _Alty& _Getal() const noexcept {
        return _Mypair._Get_first();
    }

    _Compressed_pair<_Alty, _Scary_val> _Mypair;
};

template <class _Ty, class _Alloc>
_NODISCARD _CONSTEXPR20 bool operator==(const array_2d<_Ty, _Alloc>& _Left, const array_2d<_Ty, _Alloc>& _Right) {
    if (_STD addressof(_Left) == _STD addressof(_Right)) {
        return true;
    }
    if (_Left.shape() != _Right.shape()) {
        return false;
    }
    return _STD equal(_Left._Unchecked_begin(), _Left._Unchecked_end(), _Right._Unchecked_begin());
}

template <class _Ty, class _Alloc>
_NODISCARD _CONSTEXPR20 bool operator!=(const array_2d<_Ty, _Alloc>& _Left, const array_2d<_Ty, _Alloc>& _Right) {
    return !(_Left == _Right);
}

template <class _Ty, class _Alloc>
_CONSTEXPR20 void swap(array_2d<_Ty, _Alloc>& _Left, array_2d<_Ty, _Alloc>& _Right) noexcept /* strengthened */ {
    _Left.swap(_Right);
}

_STD_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)
#endif // _STL_COMPILER_PREPROCESSOR
#endif // _ARRAY_2D_