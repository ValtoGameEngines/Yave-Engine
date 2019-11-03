/*******************************
Copyright (c) 2016-2019 Gr�goire Angerand

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
**********************************/
#ifndef Y_SERDE3_HEADERS_H
#define Y_SERDE3_HEADERS_H

#include <y/utils/name.h>
#include <y/utils/hash.h>

#include "serde.h"

namespace y {
namespace serde3 {
namespace detail {

struct TypeHeader {
	u32 name_hash;
	u32 type_hash;

	constexpr bool operator==(const TypeHeader& other) const {
		return name_hash == other.name_hash && type_hash == other.type_hash;
	}

};

struct MembersHeader {
	u32 member_hash;
	u32 count;

	constexpr bool operator==(const MembersHeader& other) const {
		return member_hash == other.member_hash && count == other.count;
	}
};

struct Header {
	TypeHeader type;
	MembersHeader members;

	constexpr bool operator==(const Header& other) const {
		return type == other.type && members == other.members;
	}

	constexpr bool operator!=(const Header& other) const {
		return !operator==(other);
	}
};

static_assert(sizeof(TypeHeader) == sizeof(u64));
static_assert(sizeof(MembersHeader) == sizeof(u64));
static_assert(sizeof(Header) == sizeof(TypeHeader) + sizeof(MembersHeader));



constexpr u32 ct_str_hash(std::string_view str) {
	u32 hash = 0x65766179;
	for(char c : str) {
		hash_combine(hash, u32(c));
	}
	return hash;
}

template<typename T>
constexpr u32 serde3_type_hash() {
	using naked = remove_cvref_t<T>;
	u32 hash = ct_str_hash(ct_type_name<naked>());
	//hash_combine(hash, u32(sizeof(naked)));
	//hash_combine(hash, u32(alignof(naked)));
	return hash;
}

template<typename T>
constexpr TypeHeader build_type_header(NamedObject<T> obj) {
	return TypeHeader{
		ct_str_hash(obj.name),
		serde3_type_hash<T>()
	};
}

template<usize I, typename... Args>
constexpr void hash_members(u32& hash, std::tuple<NamedObject<Args>...> objects) {
	unused(hash, objects);
	if constexpr(I < sizeof...(Args)) {
		TypeHeader tpe = build_type_header(std::get<I>(objects));
		hash_combine(hash, tpe.name_hash);
		hash_combine(hash, tpe.type_hash);

		hash_members<I + 1>(hash, objects);
	}
}

template<typename T>
constexpr MembersHeader build_members_header(NamedObject<T> obj) {
	u32 member_hash = 0x65766179;
	hash_members<0>(member_hash, members(obj.object));
	return MembersHeader {
		member_hash,
		member_count<T>(),
	};
}


template<typename T>
constexpr Header build_header(NamedObject<T> obj) {
	return Header{
		build_type_header(obj),
		build_members_header(obj)
	};
}

}
}
}

#endif // Y_SERDE3_HEADERS_H