#include "cache.h"

QueryRecord* Cache::genQuery(char t, unsigned int memAddr, int f)
{
    int block_num_in_mem =  (int) (memAddr/(words_per_block*4));
    int block_offset = (int)( memAddr % (words_per_block*4));
    int index = block_num_in_mem % n_sets;
    int tag = block_num_in_mem/n_sets;

    QueryRecord* q = new QueryRecord(t, index, tag);
    return q;
}

void Cache::execute (QueryRecord* q)
{
    // std::cout << "entered execute fn\n";
    if (q->isTypeRead)
    {
        handleRead(q);
    }
    else
    {
        handleWrite(q);
    }
}

void Cache::handleRead (QueryRecord* q)
{
    // std::cout << "entered handleRead fn\n";
    
    Set* s = container[q->index];
    int bl = s->search(q);

    if (bl == -1)
    {
        handleReadMiss(q, s);
    }
    else
    {
        q->gotHit = true;
        handleReadHit(q, s);
    }
}

void Cache::handleWrite (QueryRecord* q)
{
    // std::cout << "entered handleWrite fn\n";
    Set* s = container[q->index];
    int bl = s->search(q);

    if (bl == -1)
    {
        handleWriteMiss(q, s);
    }
    else
    {
        q->gotHit = true;
        handleWriteHit(q, s);
    }
}

void Cache::handleReadHit (QueryRecord* q, Set* s)
{
    // std::cout << "entered handleReadHit fn\n";
    q->n_cycles += CACHE_RW_CYCLES_PER_WORD;
}

void Cache::handleReadMiss (QueryRecord* q, Set* s)
{
    // std::cout << "entered handleReadMiss fn\n";
    //correction
    
    fetchFromMem(q);
    // s->writeNewToCache(q);
    bool dirtyEviction = s->writeNewToCache(q, false);
    if (!isWriteThru && dirtyEviction) q->n_cycles += MEM_RW_CYCLES_PER_WORD*words_per_block;
    q->n_cycles += CACHE_RW_CYCLES_PER_WORD;
}

void Cache::handleWriteHit (QueryRecord* q, Set* s)
{
    // std::cout << "entered handleWriteHit fn\n";
    if (isWriteThru)
    {
        s->writeExistingToCache(q);
        q->n_cycles += CACHE_RW_CYCLES_PER_WORD;
        writeToMem(q);
    }
    else
    {
        s->writeExistingToCache(q);
        q->n_cycles += CACHE_RW_CYCLES_PER_WORD;
    }
}

void Cache::handleWriteMiss (QueryRecord* q, Set* s)
{
    // std::cout << "entered handleWriteMiss fn\n";
    if(isWriteAlloc)
    {
        if(isWriteThru)
        {
            fetchFromMem(q);
            s->writeNewToCache(q, true);
            q->n_cycles += CACHE_RW_CYCLES_PER_WORD;
            writeToMem(q);
        }
        else
        {
            //correction
            writeBlockToMem(q); 
            bool dirtyEviction = s->writeNewToCache(q, true);
            if (dirtyEviction) q->n_cycles += MEM_RW_CYCLES_PER_WORD*words_per_block;
            q->n_cycles += CACHE_RW_CYCLES_PER_WORD;
        }
    }
    else
    {
        writeToMem(q);
    }
}

void Cache::fetchFromMem (QueryRecord* q)
{
    // std::cout << "entered fetchFromMem fn\n";
    q->n_cycles += MEM_RW_CYCLES_PER_WORD*words_per_block;
}

void Cache::writeToMem (QueryRecord* q)
{
    // std::cout << "entered writeToMem fn\n";
    q->n_cycles += MEM_RW_CYCLES_PER_WORD*1;
}

void Cache::writeBlockToMem (QueryRecord* q)
{
    // std::cout << "entered writeToMem fn\n";
    q->n_cycles += MEM_RW_CYCLES_PER_WORD*words_per_block;
}

Cache::~Cache()
{
    for(int i = 0; i<n_sets; i++)
    {
        delete container[i];
        container[i] = nullptr;
    }
    container.clear();
}