#include "GlobalPoolAllocator.h"

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

    StlPoolAllocator()
    {}

    inline
    explicit StlPoolAllocator(const StlPoolAllocator& a)
    {}

    template<typename U>
    inline
    explicit StlPoolAllocator(const StlPoolAllocator<U> & a)
    {}

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
    inline
    pointer allocate(size_type size)
    {
        return (pointer)GlobalPoolAllocator::get_instance().allocate(size * sizeof(T));
    }
    inline
    void deallocate(pointer p, size_type size)
    {
        GlobalPoolAllocator::get_instance().deallocate(p, size * sizeof(T));
    }
    size_type max_size() const
    {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }
    inline
    void construct(pointer p, const T& t)
    {
        new(p) T(t);
    }
    inline
    void destroy(pointer p)
    {
        p->~T();
    }

    inline
    bool operator==(StlPoolAllocator const&) { return true; }
    inline
    bool operator!=(StlPoolAllocator const& a) { return !operator==(a); }
};