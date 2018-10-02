#ifndef UINT_VECTOR
#define UINT_VECTOR

#include <memory>
typedef unsigned int uint;


#ifdef WIN32
	int const SHORT_SIZE = 5;
#else	
	int const SHORT_SIZE = 6;
#endif

struct uint_vector {
    uint_vector();
    ~uint_vector();
    uint_vector(size_t sz, uint val = 0);
	uint_vector(uint_vector const& other);
	void reserve(size_t capacity);
	void resize(size_t sz);
	size_t size() const;
	bool empty() const;
	uint& operator[](size_t id);
	uint const& operator[](size_t id) const;
	uint_vector& operator = (uint_vector const& other);
	void pop_back();
	void push_back(const uint x);
	uint back() const;
	void swap(uint_vector & other) noexcept;
	friend bool operator == (const uint_vector &a, const uint_vector &b);
	void clear();
private:
    size_t _size;
    bool is_normal;
    size_t get_capacity() const;
    struct normal {
        size_t capacity;
        std::shared_ptr<uint[]> ptr;
        normal(uint* f, size_t cap);
        void swap(normal &other) noexcept;
    };
    union uint_vector_data {
        uint _short[SHORT_SIZE];
        normal _normal;
        uint_vector_data() {}
        ~uint_vector_data() {}
    } data;
    void make_normal(size_t cap);
    void set_capacity(size_t cap);
    void swap_different(uint_vector_data & a, uint_vector_data & b) noexcept;
    size_t new_capacity(size_t cap);
};
#endif
