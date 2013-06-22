#include "VectorPoolAllocator.h"

template <class T>
class StlPoolAllocator
{
public:
	typedef size_t    size_type;
	typedef ptrdiff_t difference_type;
	typedef T*        pointer;
	typedef const T*  const_pointer;
	typedef T&        reference;
	typedef const T&  const_reference;
	typedef T         value_type;

	template <class U>
	struct rebind
	{
		typedef StlPoolAllocator<U> other;
	};

	template<typename U>
	friend class StlPoolAllocator;

	StlPoolAllocator() :
		m_allocator(std::make_shared<VectorPoolAllocator>())
	{}

	inline explicit StlPoolAllocator(const StlPoolAllocator& a)
	{
		m_allocator = a.m_allocator;
	}

    template<typename U>
	inline explicit StlPoolAllocator(const StlPoolAllocator<U> & a)
	{
		m_allocator = a.m_allocator;
	}

	~StlPoolAllocator()
	{}

	pointer address(reference r)
	{
		return &r;
	}
	const_pointer address(reference r) const
	{
		return &r;
	}
	inline pointer allocate(size_type size)
	{
		return (pointer)m_allocator->allocate(size * sizeof(T));
	}
	inline void deallocate(pointer p, size_type size)
	{
		m_allocator->deallocate(p, size * sizeof(T));
	}
	size_type max_size() const
	{
		return std::numeric_limits<size_type>::max() / sizeof(T);
	}
	inline void construct(pointer p, const T& t)
	{
		new(p) T(t);
	}
	inline void destroy(pointer p)
	{
		p->~T();
	}

	inline bool operator==(StlPoolAllocator const&) { return true; }
	inline bool operator!=(StlPoolAllocator const& a) { return !operator==(a); }

public:
	std::shared_ptr<VectorPoolAllocator> m_allocator;
};