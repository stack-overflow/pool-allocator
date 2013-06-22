#include "VectorPoolAllocator.h"

VectorPoolAllocator::VectorPoolAllocator() :
	m_default_num_elements(128),
	m_last_used_allocator(std::pair<size_t, AllocationEngine*>(0, NULL))
{}

VectorPoolAllocator::VectorPoolAllocator(size_t num_elements_per_pool) :
	m_default_num_elements(num_elements_per_pool),
	m_last_used_allocator(std::pair<size_t, AllocationEngine*>(0, NULL))
{}

VectorPoolAllocator::~VectorPoolAllocator()
{
	clear();
}

AllocationEngine* VectorPoolAllocator::get_allocator(size_t byte_size)
{
	std::vector<AllocationEngine*>::iterator it = std::find_if(
		m_alloc_engines.begin(),
		m_alloc_engines.end(),
		[&] (AllocationEngine* ae) { return (ae->get_pool_size() == byte_size); });

	if (it == m_alloc_engines.end())
	{
		m_alloc_engines.push_back(new AllocationEngine(m_default_num_elements, byte_size));
		it = std::prev(m_alloc_engines.end());
	}

	m_last_used_allocator.first = byte_size;
	m_last_used_allocator.second = (*it);

	return (*it);
}

void *VectorPoolAllocator::allocate(size_t byte_size)
{
	if (m_last_used_allocator.first == byte_size)
	{
		return m_last_used_allocator.second->allocate();
	}

	return get_allocator(byte_size)->allocate();
}

void VectorPoolAllocator::deallocate(void *p, size_t byte_size)
{
	if (m_last_used_allocator.first == byte_size)
	{
		m_last_used_allocator.second->deallocate(p);
	}
	else
	{
		get_allocator(byte_size)->deallocate(p);
	}
}

void VectorPoolAllocator::clear()
{
	if (m_alloc_engines.size() > 0)
	std::for_each(
		m_alloc_engines.begin(),
		m_alloc_engines.end(),
		std::default_delete<AllocationEngine>());
}
