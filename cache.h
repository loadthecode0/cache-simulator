#include <vector>
#include "set.h"

class Cache
{
    std::vector<Set*> container;
    
    public:
    int n_sets;
    int blocks_per_set ;
    int words_per_block;
    bool isWriteAlloc;
    bool isWriteThru;
    int MEM_RW_CYCLES_PER_WORD = 100;
    int CACHE_RW_CYCLES_PER_WORD = 1;

    Cache(int a, int b, int c, bool d, bool e, bool ifLRU)
    {
        n_sets = a;
        blocks_per_set  = b;
        words_per_block = c/4;
        isWriteAlloc = d;
        isWriteThru = e;

        container.resize(n_sets);

        if(ifLRU)
        {
            // std::cout << "Initializing LRU Sets...\n";
            for (int i = 0; i < n_sets; i++)
            {
                container[i] = new LRUSet(i, blocks_per_set);
            }
        }
        else
        {
            // std::cout << "Initializing FIFO Sets...\n";
            for (int i = 0; i < n_sets; i++)
            {
                container[i] = new FIFOSet(i, blocks_per_set);
            }
        }


        // std::cout << "Initialised ";
        // if(isWriteAlloc) std::cout << "Write-Allocate, "; else std::cout << "Not-Write-Allocate, ";
        // if(isWriteThru) std::cout << "Write-through "; else std::cout << "Write-back ";
        // if(ifLRU) std::cout << "LRU Cache "; else std::cout << "FIFO Cache ";
        // std::cout << "with " << n_sets<< " sets of " << blocks_per_set << " blocks each. \n";
    }

    QueryRecord* genQuery(char t, unsigned int memAddr, int f);

    void execute (QueryRecord* q);
    void handleRead (QueryRecord* q);
    void handleWrite (QueryRecord* q);
    void handleReadHit (QueryRecord* q, Set* s);
    void handleReadMiss (QueryRecord* q, Set* s);
    void handleWriteHit (QueryRecord* q, Set* s);
    void handleWriteMiss (QueryRecord* q, Set* s);
    void fetchFromMem (QueryRecord* q);
    void writeToMem (QueryRecord* q);
    void writeBlockToMem (QueryRecord* q);

    ~Cache();
};