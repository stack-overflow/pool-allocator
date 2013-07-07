#include <list>
#include <memory>
#include <cstdint>

typedef unsigned long DWORD;
typedef uint8_t       BYTE;

class MemoryPool
{
public:
	MemoryPool(size_t _num_blocks, size_t _block_size);
	~MemoryPool();

	void* allocate();

	bool deallocate(void *p);

	bool full() const;

	bool empty() const;

	int empty_slots() const;

	bool is_from_here(void *ptr) const;

	

	// Make memory available for reuse. Does not actually free memory.
	void clear();

private:
	BYTE *next_block(DWORD *block) const;

	BYTE *pointer_header(void *pointer) const;

	void *data_pointer(DWORD *pointer) const;

	DWORD *next_block_address(DWORD *block) const;

	bool is_valid_pointer(void *pointer);

	void prepare_memory();

private:
	DWORD *m_head;
	DWORD *m_mem;
	int m_block_header_size;
	int m_num_blocks;
	int m_block_size;
	int m_num_allocated_blocks;
};
