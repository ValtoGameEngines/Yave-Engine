/*******************************
Copyright (C) 2013-2016 gregoire ANGERAND

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
**********************************/
#ifndef Y_CORE_RANGE_H
#define Y_CORE_RANGE_H

#include <y/utils.h>
#include "ReverseIterator.h"
#include "ValueIterator.h"
#include "MapIterator.h"
#include "FilterIterator.h"

namespace y {
namespace core {

template<typename Iter>
class Range {
	public:
		using Return = typename dereference<Iter>::type;
		using Element = typename std::decay<Return>::type;

		Range(const Iter& b, const Iter& e) : _beg(b), _en(e) {
		}

		const Iter& begin() const {
			return _beg;
		}

		const Iter& end() const {
			return _en;
		}

		ReverseIterator<Iter> rbegin() const {
			Iter i(_beg);
			return reverse_iterator(--i);
		}

		ReverseIterator<Iter> rend() const {
			Iter i(_en);
			return reverse_iterator(--i);
		}

		usize size() const {
			return _en - _beg;
		}

		Range<ReverseIterator<Iter>> reverse() const {
			return Range<ReverseIterator<Iter>>(rend(), rbegin());
		}

		template<typename F>
		Range<MapIterator<Iter, F>> map(const F& f) const {
			return Range<MapIterator<Iter, F>>(MapIterator<Iter, F>(begin(), f), MapIterator<Iter, F>(end(), f));
		}

		template<typename F>
		Range<FilterIterator<Iter, F>> filter(const F& f) const {
			return Range<FilterIterator<Iter, F>>(FilterIterator<Iter, F>(begin(), f), FilterIterator<Iter, F>(end(), f));
		}


		template<template<typename...> typename Coll>
		auto collect() const {
			Coll<Element> c;
			collect(c);
			return c;
		}

		template<typename Stream>
		auto collect(Stream& str) const {
			for(const auto& e : *this) {
				str << e;
			}
		}

		template<typename T>
		void foreach(const T& t) const {
			for(const auto& e : *this) {
				t(e);
			}
		}

		template<typename T>
		void foreach(T& t) const {
			for(const auto& e : *this) {
				t(e);
			}
		}

		template<typename T>
		bool contains(const T& t) const {
			for(const auto& e : *this) {
				if(e == t) {
					return true;
				}
			}
			return false;
		}

	private:
		Iter _beg;
		Iter _en;
};

namespace detail {

template<typename Iter>
inline auto range(const Iter& b, const Iter& e, std::true_type) {
	bool r = e < b;
	using RI = ValueIterator<Iter>;
	return Range<RI>(RI(b, r), RI(e, r));
}

template<typename Iter>
inline auto range(const Iter& b, const Iter& e, std::false_type) {
	return Range<Iter>(b, e);
}

}


template<typename Iter>
inline auto range(const Iter& b, const Iter& e) {
	return detail::range(b, e, bool_type<!is_dereferenceable<Iter>::value>());
}

template<typename Coll>
inline auto range(const Coll& c) {
	return range(c.begin(), c.end());
}

template<typename Coll>
inline auto range(Coll& c) {
	return range(c.begin(), c.end());
}

}
}

#endif // Y_CORE_RANGE_H
