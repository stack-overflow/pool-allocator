#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "StlPoolAllocator.h"

struct TestVector4f
{
	float x;
	float y;
	float z;
	float w;
};

int main()
{
	int N_ELEMENTS = 32;
	int N_ELEMENTS_PER_POOL = 512;

	VectorPoolAllocator *allocator = new VectorPoolAllocator(N_ELEMENTS_PER_POOL);
	
	std::vector<TestVector4f*> v(N_ELEMENTS);
	for (int i = 0; i < N_ELEMENTS; ++i)
	{
		TestVector4f *vec = (TestVector4f*) allocator->allocate(sizeof(TestVector4f));
		vec->x = vec->y = vec->z = vec->w = float(i);
		v[i] = vec;
	}

	for (int i = 0; i < N_ELEMENTS; ++i)
	{
		std::cout << v[i]->x << " " << v[i]->y << " " << v[i]->z << " " << v[i]->w << " " << std::endl;
	}

	for(int i = 0; i < N_ELEMENTS; ++i)
	{
		allocator->deallocate(v[i], sizeof(TestVector4f));
	}

	delete allocator;

	std::map<int, std::string, std::less<int>, StlPoolAllocator<std::string> > sample_pool_map;

	for (int i = 0; i < N_ELEMENTS; ++i)
	{
		std::string str = "This is sample map container accelerated by pool allocator. " + std::to_string(i);
		sample_pool_map[i] = str;
	}

	for (int i = 0; i < N_ELEMENTS; ++i)
	{
		std::cout << sample_pool_map[i] << std::endl;
	}

	return 0;
}
