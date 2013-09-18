#include <list>
#include <algorithm>

#include "MemoryPool.h"

class AllocationEngine
{
public:

	AllocationEngine(int _num_blocks, int _block_size);

	~AllocationEngine();

	void *allocate();

	void deallocate(void *p);

	void clear();

	size_t get_pool_size() const;

private:
	std::list<MemoryPool*> m_pools;
	int m_num_blocks;
	int m_block_size;
};
