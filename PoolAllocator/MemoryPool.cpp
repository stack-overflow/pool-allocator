#include "MemoryPool.h"

MemoryPool::MemoryPool(int _num_blocks, int _block_size) :
	m_num_blocks(_num_blocks),
	m_block_header_size(sizeof(DWORD)),
	m_block_size(_block_size + sizeof(DWORD)),
	m_num_allocated_blocks(0)
{
	size_t num_bytes_to_alloc = m_num_blocks * m_block_size;
	m_mem = m_head = (DWORD*) malloc(num_bytes_to_alloc);

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
		void* to_return = data_pointer(m_head);
		m_head = next_block_address(m_head);
		++m_num_allocated_blocks;
		return to_return;
	}
	else
	{
		return NULL;
	}
}

bool MemoryPool::deallocate(void *p)
{
	if (is_from_here(p))
	{
		DWORD *to_deallocate = (DWORD*) pointer_header(p);

		int delta = (BYTE*) m_mem - (BYTE*) to_deallocate;
		int delta_modulo = abs(delta) % m_block_size;

		if (delta_modulo != 0 && delta_modulo != m_block_size)
		{
			return false;
		}

		(*to_deallocate) = (DWORD)m_head;
		m_head = to_deallocate;
		--m_num_allocated_blocks;

		return true;
	}
	else
	{
		return false;
	}
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
	DWORD *tmp = m_mem;
	for (int i = 0; i < m_num_blocks; ++i)
	{
		if (ptr == data_pointer(tmp))
		{
			return true;
		}

		tmp = (DWORD*)next_block(tmp);
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
BYTE *MemoryPool::next_block(DWORD *block) const
{
	return (BYTE*) block + m_block_size;
}

BYTE *MemoryPool::pointer_header(void *pointer) const
{
	return (BYTE*) pointer - m_block_header_size;
}

void *MemoryPool::data_pointer(DWORD *pointer) const
{
	return (void*) ((BYTE*) pointer + m_block_header_size);
}

DWORD *MemoryPool::next_block_address(DWORD *block) const
{
	return (DWORD*) (*block);
}

void MemoryPool::prepare_memory()
{
	DWORD *tmp_head = m_mem;
	for (int i = 0; i < m_num_blocks; ++i)
	{
		(*tmp_head) = (DWORD) next_block(tmp_head);
		tmp_head = next_block_address(tmp_head);
	}
	m_head = m_mem;
}
