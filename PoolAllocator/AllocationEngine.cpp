#include "AllocationEngine.h"

AllocationEngine::AllocationEngine(int _num_blocks, int _block_size) :
    m_num_blocks(_num_blocks),
    m_block_size(_block_size)
{
    m_pools.push_front(new MemoryPool(m_num_blocks, m_block_size));
}

AllocationEngine::~AllocationEngine()
{
    clear();
}

void *AllocationEngine::allocate()
{
    if (!m_pools.front()->full())
    {
        return m_pools.front()->allocate();
    }
    else
    {
        MemoryPool *front = m_pools.front();
        m_pools.pop_front();
        m_pools.push_back(front);

        // If second pool in line is full, then all of the remaining are.
        if (m_pools.front()->full())
        {
            m_pools.push_front(new MemoryPool(m_num_blocks, m_block_size));
        }

        return m_pools.front()->allocate();
    }
}
#include <iostream>
void AllocationEngine::deallocate(void *p)
{
    std::list<MemoryPool*>::iterator it = std::find_if(
        m_pools.begin(),
        m_pools.end(),
        [&] (MemoryPool *c) { return c->deallocate(p); });
        
    if (it != m_pools.end())
    {
        if (m_pools.size() > 1)
        {
            if ((*it)->empty())
            {
                delete (*it);
                m_pools.erase(it);
            }
            else
            {
                MemoryPool *block = (*it);
                m_pools.erase(it);
                m_pools.push_front(block);
            }
        }
    }
    else
    {
        // System delete is performed, only when it's certain,
        // the pointer is not invalid address from any of internal memory pools.
        it = std::find_if(
            m_pools.begin(),
            m_pools.end(),
            [&] (MemoryPool *c) { return c->is_from_here(p); });
            
        if (it == m_pools.end())
        {
            // Safely delete.
            delete p;
        }
    }
}

void AllocationEngine::clear()
{
    // Perform default delete on all memory pools, which will trigger destructors
    // and actual system memory release.
    for_each(
        m_pools.begin(),
        m_pools.end(),
        std::default_delete<MemoryPool>());
    m_pools.clear();
}

size_t AllocationEngine::get_pool_size() const
{
    return m_block_size;
}
