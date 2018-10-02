#include "uint_vector.h"
#include <memory.h>

uint_vector::normal::normal(uint* p, size_t cap) : capacity(cap), ptr(p) {}

size_t uint_vector::new_capacity(size_t n) {
	if (n <= SHORT_SIZE) return SHORT_SIZE; else return 2 * n;
}

void uint_vector::normal::swap(normal & other) noexcept {
	std::swap(ptr, other.ptr);
	std::swap(capacity, other.capacity);
}

void uint_vector::swap_different(uint_vector::uint_vector_data & a, uint_vector::uint_vector_data & b) noexcept {
	uint temp[SHORT_SIZE];
	memcpy(temp, b._short, SHORT_SIZE * sizeof(uint));
    new(&b._normal) normal(a._normal);
    a._normal.~normal();
	memcpy(a._short, temp, SHORT_SIZE * sizeof(uint));
}

void uint_vector::swap(uint_vector & other) noexcept {
    std::swap(_size, other._size);
	if (!is_normal && !other.is_normal) {
        for (size_t i = 0; i < SHORT_SIZE; i++) std::swap(data._short[i], other.data._short[i]);
		return;
	}
	if (is_normal && other.is_normal) {
        std::swap(data._normal, other.data._normal);
		return;
	}
	if (is_normal) {
		swap_different(data, other.data);
	} else { 
		swap_different(other.data, data);
	}
    std::swap(is_normal, other.is_normal);
}

size_t uint_vector::get_capacity() const {
	if (is_normal) return data._normal.capacity; else return SHORT_SIZE;
}

uint* copy_data(const uint* src, size_t cnt, size_t cap) {
    //uint* res = (uint*)malloc(cap * sizeof(uint));
    uint* res = new uint[cap];
	memcpy(res, src, cnt * sizeof(uint));
	memset(res + cnt, 0, (cap - cnt) * sizeof(uint));
	return res;
}

uint_vector::uint_vector() : _size(0), is_normal(false) {}

uint_vector::~uint_vector() {
    if (is_normal) data._normal.~normal();
}

uint_vector::uint_vector(size_t sz, uint val) : _size(0), is_normal(false) {
	reserve(sz);
	_size = sz;
    if (is_normal) {

        for (size_t i = 0; i < sz; i++) *(data._normal.ptr.get() + i) = val;
    } else {
		for (size_t i = 0; i < SHORT_SIZE; i++) data._short[i] = val;
	}
}

uint_vector::uint_vector(uint_vector const& other) : _size(other._size), is_normal(other.is_normal) {
	if (other.is_normal) {
		new(&data._normal) normal(other.data._normal);
	} else {
		memcpy(data._short, other.data._short, SHORT_SIZE * sizeof(uint));
	}
}

void uint_vector::make_normal(size_t cap) {
	new(&data._normal) normal(copy_data(data._short, _size, cap), cap);
	is_normal = true;
	data._normal.capacity = cap;
}

void uint_vector::set_capacity(size_t cap) {
	if (is_normal || cap > SHORT_SIZE) {
		if (!is_normal) {
			make_normal(cap);
		} else {
            data._normal.ptr.reset(copy_data(data._normal.ptr.get(), _size, cap));
			data._normal.capacity = cap;
		}
	}
}

void uint_vector::reserve(size_t capacity) {
	if (capacity > get_capacity()) set_capacity(capacity);
}

void uint_vector::resize(size_t sz) {
	reserve(sz);
	_size = sz;
}

size_t uint_vector::size() const {
	return _size;
}

bool uint_vector::empty() const {
	return (_size == 0);
}

uint& uint_vector::operator[](size_t id) {
	if (is_normal) {
		return *(data._normal.ptr.get() + id);
	} else {
		return data._short[id];
	}
}

uint const& uint_vector::operator[](size_t id) const {
	if (is_normal) {
        return *(data._normal.ptr.get() + id);
	} else {
		return data._short[id];
	}
}

uint_vector& uint_vector::operator = (uint_vector const& other) {
	uint_vector tmp(other);
	swap(tmp);
	return *this;
}

void uint_vector::pop_back() {
	_size--;
}

void uint_vector::push_back(const uint x) {
	if (get_capacity() < _size + 1) reserve(new_capacity(_size + 1));
	if (is_normal) {
		*(data._normal.ptr.get() + _size) = x;
	} else {
		data._short[_size] = x;
	}
	_size++;
}

uint uint_vector::back() const {
	if (is_normal) {
		return *(data._normal.ptr.get() + _size - 1);
	} else {
		return data._short[_size - 1];
	}
}

bool operator == (const uint_vector &a, const uint_vector &b) {
	if (a._size != b._size) return false;
	if (a.is_normal) return (memcmp(a.data._normal.ptr.get(), b.data._normal.ptr.get(), a._size * sizeof(uint)) == 0);
	return (memcmp(a.data._short, b.data._short, SHORT_SIZE * sizeof(uint)) == 0);
}

void uint_vector::clear() {
	(*this) = uint_vector();
}
