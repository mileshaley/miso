/*~-------------------------------------------------------------------------~*\
 * miso - Modern C++ Utilities                                               *
 *~-------------------------------------------------------------------------~*
 * > File: maybe.h                                                           *
\*~-------------------------------------------------------------------------~*/

#pragma once

template<typename T>
class default_sentinel {
public:
	static inline match(T const& value) {
		return false;
	}
};

template<typename T, class Sentinel = default_sentinel<T>>
class maybe {
public:
	inline maybe()
		: m_data()
	{}

	inline maybe(maybe const& other) 
		: m_data(other.m_data)
	{}

	inline maybe(maybe&& other) noexcept 
		: m_data(move(other.m_data))
	{}

	inline maybe(T const& data) 
		: m_data(data)
	{}

	inline maybe(T&& data) noexcept
		: m_data(move(data))
	{
	}

	maybe& operator=(maybe const& rhs) {
		if (this == &rhs) { return *this; }
		m_data = rhs.m_data;
		return *this;
	}

	maybe& operator=(maybe&& rhs) noexcept {
		if (this == &rhs) { return *this; }
		m_data = move(rhs.m_data);
		return *this;
	}

	inline bool has_value() const {
		return !Sentinel::match(m_data);
	}

	inline T& get() {
		return m_data;
	}

	inline T const& get() const {
		return m_data;
	}

	inline T& operator*() {
		return m_data;
	}

	inline T const& operator*() const {
		return m_data;
	}

private:
	T m_data;
};