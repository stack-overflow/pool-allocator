#include <iostream>
#include <map>
#include <vector>
#include <ctime>

#include "StlPoolAllocator.h"

//#define PRECISE_MEMORY_CHECK
#define SPEED_CHECK
//#define STL_MAP_TEST
#define FULL_POOL_ALLOCATOR

class MapPoolAllocator
{
public:
	MapPoolAllocator() :
		m_default_num_elements(1024)
	{}

	void *allocate(size_t byte_size)
	{
		std::map<size_t, AllocationEngine*>::iterator it = m_alloc_engines.find(byte_size);
		if (it == m_alloc_engines.end())
		{
			it = m_alloc_engines.insert(m_alloc_engines.begin(), std::pair<size_t, AllocationEngine*>(byte_size, new AllocationEngine(m_default_num_elements, byte_size)));
		}
		return (*it).second->allocate();
	}

	void deallocate(void *p, size_t byte_size)
	{
		if (m_alloc_engines.find(byte_size) != m_alloc_engines.end())
		{
			m_alloc_engines[byte_size]->deallocate(p);
		}
	}

private:
	std::map<size_t, AllocationEngine*> m_alloc_engines;
	int m_default_num_elements;
};

struct vec2f
{
	float x;
	float y;
	//float z;
	//float w;
};
int main()
{
	int N_ELEMENTS = 19;
#ifdef SPEED_CHECK
	N_ELEMENTS = 1999999;
#endif
	
#ifdef FULL_POOL_ALLOCATOR
	VectorPoolAllocator pool = VectorPoolAllocator(512);
#else
	AllocationEngine pool = new AllocationEngine(1024, sizeof(vec2f));
#endif

	std::vector<vec2f*> vecs(N_ELEMENTS);

#ifdef FULL_POOL_ALLOCATOR
#ifdef SPEED_CHECK
	clock_t begin, end;
	double elapsed_secs;

// Pool alloc ====
	begin = clock();
	for (int i = 0; i < N_ELEMENTS; ++i)
	{
		vec2f* v = (vec2f*)pool.allocate(sizeof(vec2f));
		v->x = float(i);
		v->y = float(i);
		vecs[i] = v;
	}
	end = clock();
    elapsed_secs = double(end - begin);

	std::cout << "Allocation of " << N_ELEMENTS << ": " << elapsed_secs << std::endl;
	//getchar();

	begin = clock();
	for (int i = 0; i < N_ELEMENTS; ++i)
	{
		pool.deallocate(vecs[i], sizeof(vec2f));
	}
	//pool->clear();
	end = clock();
    elapsed_secs = double(end - begin);

	std::cout << "Deallocation of " << N_ELEMENTS << ": " << elapsed_secs << std::endl;

	vecs.clear();
	vecs.resize(N_ELEMENTS);
	//getchar();

// System malloc ====
	begin = clock();
	for (int i = 0; i < N_ELEMENTS; ++i)
	{
		vec2f* v = (vec2f*)malloc(sizeof(vec2f));
		v->x = float(i);
		v->y = float(i);
		vecs[i] = v;
	}
	end = clock();
    elapsed_secs = double(end - begin);

	std::cout << "System allocation of " << N_ELEMENTS << ": " << elapsed_secs << std::endl;
	//getchar();
	begin = clock();
	for (int i = 0; i < N_ELEMENTS; ++i)
	{
		free(vecs[i]);
	}
	end = clock();
    elapsed_secs = double(end - begin);

	std::cout << "System deallocation of " << N_ELEMENTS << ": " << elapsed_secs << std::endl;

	vecs.clear();
	//getchar();
#else
	for (int i = 0; i < N_ELEMENTS; ++i)
	{
		vec2f* v = (vec2f*)pool.allocate(sizeof(vec2f));
		v->x = float(i);
		//v->y = float(i);
		if (i == 8)
		{
			pool.deallocate(vecs[4], sizeof(vec2f));
		}
		vecs[i] = v;
	}

	for (int i = 0; i < N_ELEMENTS; ++i)
	{
		std::cout << (int)vecs[i]->x << " " << (int)vecs[i]->x << std::endl;
	}

	for (int i = 0; i < N_ELEMENTS; ++i)
	{
		pool.deallocate(vecs[i], sizeof(vec2f));
	}

	vecs.clear();
	vecs.resize(N_ELEMENTS);

	for (int i = 0; i < N_ELEMENTS; ++i)
	{
		vec2f* v = (vec2f*)pool.allocate(sizeof(vec2f));
		v->x = float(i);
		//v->y = float(i);
		vecs[i] = v;
	}

	for (int i = 0; i < N_ELEMENTS; ++i)
	{
		std::cout << (int)vecs[i]->x << " " << (int)vecs[i]->x << std::endl;
	}

	for (int i = 0; i < N_ELEMENTS; ++i)
	{
		pool.deallocate(vecs[i], sizeof(vec2f));
	}
	
	vecs.clear();
#endif

#else
#ifdef SPEED_CHECK
	clock_t begin, end;
	double elapsed_secs;

// Pool alloc ====
	begin = clock();
	for (int i = 0; i < N_ELEMENTS; ++i)
	{
		vec2f* v = (vec2f*)pool->allocate();
		v->x = float(i);
		v->y = float(i);
		vecs[i] = v;
	}
	end = clock();
    elapsed_secs = double(end - begin);

	std::cout << "Allocation of " << N_ELEMENTS << ": " << elapsed_secs << std::endl;
	//getchar();

	begin = clock();
	for (int i = 0; i < N_ELEMENTS; ++i)
	{
		pool->deallocate(vecs[i]);
	}
	//pool->clear();
	end = clock();
    elapsed_secs = double(end - begin);

	std::cout << "Deallocation of " << N_ELEMENTS << ": " << elapsed_secs << std::endl;

	vecs.clear();
	vecs.resize(N_ELEMENTS);
	//getchar();

// System malloc ====
	begin = clock();
	for (int i = 0; i < N_ELEMENTS; ++i)
	{
		vec2f* v = (vec2f*)malloc(sizeof(vec2f));
		v->x = float(i);
		v->y = float(i);
		vecs[i] = v;
	}
	end = clock();
    elapsed_secs = double(end - begin);

	std::cout << "System allocation of " << N_ELEMENTS << ": " << elapsed_secs << std::endl;
	//getchar();
	begin = clock();
	for (int i = 0; i < N_ELEMENTS; ++i)
	{
		free(vecs[i]);
	}
	end = clock();
    elapsed_secs = double(end - begin);

	std::cout << "System deallocation of " << N_ELEMENTS << ": " << elapsed_secs << std::endl;

	vecs.clear();
	//getchar();
#else
	for (int i = 0; i < N_ELEMENTS; ++i)
	{
		vec2f* v = (vec2f*)pool->allocate(sizeof(vec2f));
		v->x = float(i);
		v->y = float(i);
		if (i == 8)
		{
			pool->deallocate(vecs[4], sizeof(vec2f));
		}
		vecs[i] = v;
	}

	for (int i = 0; i < N_ELEMENTS; ++i)
	{
		std::cout << vecs[i]->x << " " << vecs[i]->y << std::endl;
	}

	for (int i = 0; i < N_ELEMENTS; ++i)
	{
		pool->deallocate(vecs[i], sizeof(vec2f));
	}

	vecs.clear();
	vecs.resize(N_ELEMENTS);

	for (int i = 0; i < N_ELEMENTS; ++i)
	{
		vec2f* v = (vec2f*)pool->allocate(sizeof(vec2f));
		v->x = float(i);
		v->y = float(i);
		vecs[i] = v;
	}

	for (int i = 0; i < N_ELEMENTS; ++i)
	{
		std::cout << vecs[i]->x << " " << vecs[i]->y << std::endl;
	}

	for (int i = 0; i < N_ELEMENTS; ++i)
	{
		pool->deallocate(vecs[i], sizeof(vec2f));
	}
	
	vecs.clear();
#endif
#endif


	//delete pool;


#ifdef STL_MAP_TEST
std::map<int, std::string, std::less<int>, StlPoolAllocator<std::string> > vvv;
N_ELEMENTS = 200;

clock_t begin, end;
double elapsed_secs;

begin = clock();
for (int i = 0; i < N_ELEMENTS; ++i)
{
	vvv.insert(std::pair<int, std::string>(i, "adss"));
}
end = clock();
elapsed_secs = double(end - begin);
std::cout << "Allocation of " << N_ELEMENTS << ": " << elapsed_secs << std::endl;

std::map<int, std::string> vvvNormalAllocator;

begin = clock();
for (int i = 0; i < N_ELEMENTS; ++i)
{
	vvvNormalAllocator.insert(std::pair<int, std::string>(i, "adss"));
}
end = clock();
elapsed_secs = double(end - begin);
std::cout << "Normal allocator allocation of " << N_ELEMENTS << ": " << elapsed_secs << std::endl;

//long long x = 0;
begin = clock();
for (int i = 0; i < N_ELEMENTS; ++i)
{
	auto it = vvv.find(i);
	//x += it->second;
}
end = clock();
elapsed_secs = double(end - begin);
std::cout << "Finding of " << N_ELEMENTS << ": " << elapsed_secs << std::endl;
//std::cout << x << std::endl;

//x = 0;
begin = clock();
for (int i = 0; i < N_ELEMENTS; ++i)
{
	auto it = vvvNormalAllocator.find(i);
	//x += it->second;
}

for (int i = 0; i < N_ELEMENTS; ++i)
{
	vvv.erase(i);
}

end = clock();
elapsed_secs = double(end - begin);
std::cout << "Normal allocator finding of " << N_ELEMENTS << ": " << elapsed_secs << std::endl;
//std::cout << x << std::endl;  
#endif // STL_MAP_TEST


	return 0;
}
