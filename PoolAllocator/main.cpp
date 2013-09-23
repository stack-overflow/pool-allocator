#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "StlPoolAllocator.h"

struct Vector4f
{
    float x;
    float y;
    float z;
    float w;

    void* operator new(std::size_t size)
    {
        return GlobalPoolAllocator::get_instance().allocate(size);
    }

    void operator delete(void* ptr, std::size_t size)
    {
        GlobalPoolAllocator::get_instance().deallocate(ptr, size);
    }
};

int main()
{
    int N_ELEMENTS = 32;
    
    std::vector<Vector4f*> v(N_ELEMENTS);
    for (int i = 0; i < N_ELEMENTS; ++i)
    {
        Vector4f *vec = new Vector4f;
        vec->x = vec->y = vec->z = vec->w = float(i);
        v[i] = vec;
    }

    for (int i = 0; i < N_ELEMENTS; ++i)
    {
        std::cout << v[i]->x << " " << v[i]->y << " " << v[i]->z << " " << v[i]->w << " " << std::endl;
    }

    for(int i = 0; i < N_ELEMENTS; ++i)
    {
        delete v[i];
    }

    std::map<int,
             std::string,
             std::less<int>,
             StlPoolAllocator<std::string> > sample_pool_map;

    for (int i = 0; i < N_ELEMENTS; ++i)
    {
        std::string str = "This is sample map container accelerated by pool allocator. " + std::to_string(i);
        sample_pool_map[i] = str;
    }

    for (int i = 0; i < N_ELEMENTS; ++i)
    {
        std::cout << sample_pool_map[i] << std::endl;
    }

    std::cout << sizeof (PTR_INT) << std::endl;
    std::cout << sizeof (PTR_INT*) << std::endl;

    return 0;
}
