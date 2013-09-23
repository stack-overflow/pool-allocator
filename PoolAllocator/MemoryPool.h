#include <list>
#include <memory>
#include <cstdint>

#ifdef _WIN64
typedef unsigned long long PTR_INT;
#else
typedef unsigned long PTR_INT;
#endif
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
    BYTE *get_next_block(PTR_INT *block) const;

    BYTE *pointer_header(void *pointer) const;

    void *data_pointer(PTR_INT *pointer) const;

    PTR_INT *next_free_block_address(PTR_INT *block) const;

    bool is_valid_pointer(void *pointer) const;

    void prepare_memory();

private:
    PTR_INT *m_head;
    PTR_INT *m_mem;
    int m_block_header_size;
    int m_num_blocks;
    int m_block_size;
    int m_num_allocated_blocks;
};
