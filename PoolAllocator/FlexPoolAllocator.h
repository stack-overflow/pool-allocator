#include <vector>

#include "AllocationEngine.h"

class FlexPoolAllocator
{
public:
    FlexPoolAllocator();
    FlexPoolAllocator(size_t num_elements_per_pool);


    ~FlexPoolAllocator();

    AllocationEngine* get_allocator(size_t byte_size);

    void *allocate(size_t byte_size);

    void deallocate(void *p, size_t byte_size);

    void clear();

private:
    std::vector<AllocationEngine*> m_alloc_engines;
    std::pair<size_t, AllocationEngine*> m_last_used_allocator;
    size_t m_default_num_elements;
};
