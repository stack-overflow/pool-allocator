#include "MemoryPool.h"

//#define PRECISE_MEMORY_CHECK

MemoryPool::MemoryPool(size_t _num_blocks, size_t _block_size) :
    m_num_blocks(_num_blocks),
    m_block_header_size(sizeof(PTR_INT)),
    m_block_size(_block_size + sizeof(PTR_INT)),
    m_num_allocated_blocks(0)
{
    size_t num_bytes_to_alloc = (size_t)(m_num_blocks * m_block_size);
    m_mem = m_head = (PTR_INT*) malloc(num_bytes_to_alloc);

    prepare_memory();
}

MemoryPool::~MemoryPool()
{
    free(m_mem);
}

void* MemoryPool::allocate()
{
    if (m_num_allocated_blocks < m_num_blocks)
    {
        // Get pointer to data, from current free block
        void* to_return = data_pointer(m_head);
        // Set head of the list to the next address
        m_head = next_free_block_address(m_head);
        ++m_num_allocated_blocks;
        return to_return;
    }

    // No free blocks available
    return NULL;
}

bool MemoryPool::deallocate(void *p)
{
    if (is_from_here(p))
    {
        PTR_INT *to_deallocate = (PTR_INT*) pointer_header(p);

        if (!is_valid_pointer(to_deallocate))
        {
            return false;
        }
        (*to_deallocate) = (PTR_INT)m_head;
        m_head = to_deallocate;
        --m_num_allocated_blocks;

        return true;
    }

    return false;
}

bool MemoryPool::full() const
{
    return (m_num_allocated_blocks == m_num_blocks) ? true : false;
}

bool MemoryPool::empty() const
{
    return (m_num_allocated_blocks > 0) ? false : true;
}

int MemoryPool::empty_slots() const
{
    return m_num_blocks - m_num_allocated_blocks;
}

bool MemoryPool::is_from_here(void *ptr) const
{
#ifndef PRECISE_MEMORY_CHECK
    return ((ptr > (BYTE*)m_mem) && (ptr <= (BYTE*)m_mem + (m_num_blocks * m_block_size)));
#else
    PTR_INT *tmp = m_mem;
    for (int i = 0; i < m_num_blocks; ++i)
    {
        if (ptr == data_pointer(tmp))
        {
            return true;
        }

        tmp = (PTR_INT*)get_next_block(tmp);
    }
    return false;
#endif // PRECISE_MEMORY_CHECK
}

void MemoryPool::clear()
{
    m_num_allocated_blocks = 0;
    prepare_memory();
}

// private ====
BYTE *MemoryPool::get_next_block(PTR_INT *block) const
{
    return (BYTE*) block + m_block_size;
}

BYTE *MemoryPool::pointer_header(void *pointer) const
{
    return (BYTE*) pointer - m_block_header_size;
}

void *MemoryPool::data_pointer(PTR_INT *pointer) const
{
    return (void*) ((BYTE*) pointer + m_block_header_size);
}

PTR_INT *MemoryPool::next_free_block_address(PTR_INT *block) const
{
    return (PTR_INT*) (*block);
}

bool MemoryPool::is_valid_pointer(void *pointer) const
{
    // Get 
    int delta = (BYTE*) pointer - (BYTE*) m_mem;
    int delta_modulo = abs(delta) % m_block_size;

    return (delta_modulo == 0 || delta_modulo == m_block_size);
}

void MemoryPool::prepare_memory()
{
    PTR_INT *tmp_head = m_mem;
    for (int i = 0; i < m_num_blocks; ++i)
    {
        // Get next block in linear memory and set the header to it
        (*tmp_head) = (PTR_INT) get_next_block(tmp_head);
        // Enter the next block
        tmp_head = next_free_block_address(tmp_head);
    }
    m_head = m_mem;
}
