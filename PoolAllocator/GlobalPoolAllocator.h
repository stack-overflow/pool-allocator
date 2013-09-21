#pragma once
#include "FlexPoolAllocator.h"

class GlobalPoolAllocator
{
public:
    static GlobalPoolAllocator& get_instance()
    {
        static GlobalPoolAllocator globalPoolAlloc;
        return globalPoolAlloc;
    }

    inline
    void *allocate(size_t _num_bytes)
    {
        return m_allocator.allocate(_num_bytes);
    }

    inline
    void deallocate(void *_ptr, size_t _num_bytes)
    {
        m_allocator.deallocate(_ptr, _num_bytes);
    }

private:
    GlobalPoolAllocator() : 
        m_allocator(512)
    {}

    GlobalPoolAllocator(GlobalPoolAllocator&){};
    ~GlobalPoolAllocator() {};

    FlexPoolAllocator m_allocator;
};
