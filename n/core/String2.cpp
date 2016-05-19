/*******************************
Copyright (C) 2013-2015 gregoire ANGERAND

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
#include "String2.h"
#include <cstring>
#include <n/core/Array.h>
#include <ostream>

namespace n {
namespace core {

// --------------------------------------------------- LONG ---------------------------------------------------

String2::LongData::LongData() : data(0), length(0) {
}

String2::LongData::LongData(const LongData &l) : LongData(l.data, l.length) {
}

String2::LongData::LongData(LongData &&l) : data(l.data), length(l.length) {
	l.data = 0;
}

String2::LongData::LongData(const char *str, uint len) : data(allocLong(len)), length(len) {
	if(str) {
		memcpy(data, str, len);
	}
	data[len] = 0;
}


// --------------------------------------------------- SHORT ---------------------------------------------------

String2::ShortData::ShortData() : data{0}, length(0) {
}

String2::ShortData::ShortData(const ShortData &s) {
	memcpy(this, &s, sizeof(ShortData));
}

String2::ShortData::ShortData(const char *str, uint len) : length(len) {
	if(str) {
		memcpy(data, str, len);
	}
	data[len] = 0;
}

String2::ShortData &String2::ShortData::operator=(const ShortData &s) {
	memcpy(this, &s, sizeof(ShortData));
	return *this;
}


// --------------------------------------------------- ALLOC ---------------------------------------------------

char *String2::allocLong(uint len) {
	char *a = new char[len + 1];
	return a;
}

void String2::freeLong(LongData &d) {
	delete[] d.data;
}




// --------------------------------------------------- STRING ---------------------------------------------------

String2::String2() : s(ShortData()) {
}

String2::String2(const String2 &str) {
	if(str.isLong()) {
		new(&l) LongData(str.l);
	} else {
		s = str.s;
	}
}

String2::String2(String2 &&str) {
	if(str.isLong()) {
		new(&l) LongData(std::move(str.l));
	} else {
		s = str.s;
	}
}

String2::String2(char c) : String2(&c, 1) {
}

String2::String2(const char *str) : String2(str, strlen(str)) {
}

String2::String2(const char *str, uint len) {
	if(len <= MaxShortSize) {
		s = ShortData(str, len);
	} else {
		new(&l) LongData(str, len);
	}
}

String2::String2(const char *beg, const char *end) : String2(beg, end - beg) {
}

uint String2::size() const {
	return isLong() ? l.length : s.length;
}

bool String2::isEmpty() const {
	return !size();
}

void String2::clear() {
	operator=("");
}

bool String2::isLong() const {
	return l.length.isLong;
}

bool String2::beginsWith(const String2 &str) const {
	uint s = str.size();
	if(size() < s) {
		return false;
	}
	const char *d = str.data();
	const char *td = data();
	for(uint i = 0; i != s; i++) {
		if(d[i] != td[i]) {
			return false;
		}
	}
	return true;
}

bool String2::endsWith(const String2 &str) const {
	uint s = str.size();
	uint ts = size();
	if(ts < s) {
		return false;
	}
	ts--;
	const char *d = str.data();
	const char *td = data();
	for(uint i = 0; i != s; i++) {
		uint index = ts - i;
		if(d[i] != td[index]) {
			return false;
		}
	}
	return true;
}

String2::const_iterator String2::find(const String2 &str, uint from) const {
	return const_iterator(const_cast<String2 *>(this)->find(str, from));
}

String2::const_iterator String2::find(const String2 &str, const_iterator from) const {
	return const_iterator(const_cast<String2 *>(this)->find(str, from));
}

String2::iterator String2::find(const String2 &str, uint from) {
	uint strSize = str.size();
	uint len = size();
	if(from + strSize > len) {
		return end();
	}
	const char *d = str.data();
	const char *td = data();
	for(uint i = from, l = len - strSize; i <= l; i++) {
		bool matched = true;
		for(uint j = 0; j != strSize; j++) {
			if(td[i + j] != d[j]) {
				matched = false;
				break;
			}
		}
		if(matched) {
			return begin() + i;
		}
	}
	return end();
}

String2::iterator String2::find(const String2 &str, const_iterator from) {
	return find(str, from - begin());
}

bool String2::contains(const String2 &str) const {
	return find(str) != end();
}

String2 String2::subString(const_iterator beg, uint len) {
	if(!len) {
		return String2();
	}
	return String2(beg, len);
}

String2 String2::subString(const_iterator beg) {
	return String2(beg);
}

String2 String2::subString(uint beg, uint len) const {
	if(!len) {
		return String2();
	}
	return String2(data() + beg, len);
}

String2 String2::subString(uint beg) const {
	return subString(beg, size() - beg);
}

String2 String2::replaced(const String2 &oldS, const String2 &newS) const {
	Array<String2> concat;
	const_iterator it = find(oldS);
	if(it != end()) {
		const_iterator from = 0;
		uint offset = oldS.size();
		do {
			concat += String2(from, it);
			concat += newS;
			from = it + offset;
			it = find(oldS, from);
		} while(it != end());
		concat += subString(from);
	} else {
		return *this;
	}
	return String2(concat);
}

String2 String2::toLower() const {
	return mapped([](char c) -> char { return tolower(c); });
}

String2 String2::toUpper() const {
	return mapped([](char c) -> char { return toupper(c); });
}

String2 String2::trimmed() const {
	if(isEmpty()) {
		return *this;
	}
	uint t = 0;
	uint si = size();
	const char *d = data();
	while(isspace(d[t])) {
		if(++t == si) {
			return "";
		}
	}
	uint e = 0;
	while(isspace(d[si - e - 1])) {
		e++;
	}
	return subString(t, (si - e) - t);
}

char *String2::data() {
	return isLong() ? l.data : s.data;
}

const char *String2::data() const {
	return isLong() ? l.data : s.data;
}

String2::iterator String2::begin() {
	return data();
}

String2::iterator String2::end() {
	if(isLong()) {
		return l.data + l.length;
	}
	return s.data + s.length;
}

String2::const_iterator String2::begin() const {
	return data();
}

String2::const_iterator String2::end() const {
	if(isLong()) {
		return l.data + l.length;
	}
	return s.data + s.length;
}

String2::const_iterator String2::cbegin() const {
	return data();
}

String2::const_iterator String2::cend() const {
	if(isLong()) {
		return l.data + l.length;
	}
	return s.data + s.length;
}

char &String2::operator[](uint i) {
	return data()[i];
}

char String2::operator[](uint i) const {
	return data()[i];
}

void String2::swap(String2 &str) {
	byte tmp[sizeof(ShortData)];
	unused(tmp);
	memcpy(tmp, &str.s, sizeof(ShortData));
	memcpy(&str.s, &s, sizeof(ShortData));
	memcpy(&s, tmp, sizeof(ShortData));
}

String2 &String2::operator=(const String2 &str) {
	if(isLong()) {
		freeLong(l);
	}
	if(str.isLong()) {
		new(&l) LongData(str.l);
	} else {
		s = str.s;
	}
	return *this;
}

String2 &String2::operator=(String2 &&str) {
	swap(str);
	return *this;
}

String2 &String2::operator+=(const String2 &rhs) {
	return operator=(operator+(rhs));
}

String2 &String2::operator<<(const String2 &rhs) {
	return operator+=(rhs);
}

String2 String2::operator+(const String2 &rhs) const {
	uint lhsize = size();
	uint rhsize = rhs.size();
	String2 str(0, lhsize + rhsize);
	char *d = str.data();
	memcpy(d, data(), lhsize);
	memcpy(d + lhsize, rhs.data(), rhsize);
	return str;
}

bool String2::operator==(const String2 &str) const {
	uint s = str.size();
	if(s != size()) {
		return false;
	}
	const char *lhs = data();
	const char *rhs = str.data();
	for(uint i = 0; i != s; i++) {
		if(lhs[i] != rhs[i]) {
			return false;
		}
	}
	return true;
}

bool String2::operator!=(const String2 &str) const {
	return !operator==(str);
}

bool String2::operator<(const String2 &s) const {
	uint lhsize = size();
	uint rhsize = s.size();
	uint min = std::min(lhsize, rhsize);
	const char *lhs = data();
	const char *rhs = s.data();
	for(uint i = 0; i != min; i++) {
		if(lhs[i] < rhs[i]) {
			return true;
		} else if(lhs[i] > rhs[i]) {
			return false;
		}
	}
	return lhsize < rhsize;
}

bool String2::operator>(const String2 &s) const {
	uint lhsize = size();
	uint rhsize = s.size();
	uint min = std::min(lhsize, rhsize);
	const char *lhs = data();
	const char *rhs = s.data();
	for(uint i = 0; i != min; i++) {
		if(lhs[i] > rhs[i]) {
			return true;
		} else if(lhs[i] < rhs[i]) {
			return false;
		}
	}
	return lhsize > rhsize;
}

}
}


std::istream &operator>>(std::istream &s, n::core::String2 &str) {
	std::string st;
	s>>st;
	str = n::core::String2(st.c_str());
	return s;
}

std::ostream &operator<<(std::ostream &s, const n::core::String2 &str) {
	s.write(str.data(), str.size());
	return s;
}

