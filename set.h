#include <vector>
#include <unordered_map>
#include <map>
#include <queue>
#include "queryRecord.h"
#include "dll.h"



class Set
{
    public:
    int index;
    int n_blocks;
    virtual int search(QueryRecord* q)  //return nullptr if not-found(miss), or corresponding Block* if found
    {
        std::cout<< "entered Base type set search\n"; 
        return -1; 
    }
    virtual void writeExistingToCache(QueryRecord* q)
    {
        std::cout<< "entered Base type set writeExistingToCache\n"; 
        return;
    }
    virtual bool writeNewToCache(QueryRecord* q, bool isWrite)
    {
        std::cout<< "entered Base type set writeNewToCache\n"; 
        return false;
    }
    virtual void updateMRU(int tag)
    {
        std::cout<< "entered Base type set updateMRU\n"; 
        return;
    }

    virtual ~Set()
    {

    }
};

class LRUSet : public Set
{
    DLL list; //DLL contains only valid entries
    std::unordered_map<int, Node*> table; //map from tag to Node containing Block, use for search

    public:
    LRUSet(int i, int n)
    {
        // list = new DLL();
        index = i;
        n_blocks = n;
    }
    //For new allocation, DLL size == n_blocks, 
    //then need to evict, 
    //otherwise search for empty space 

    void updateMRU(int tag) override
    {
        Node* blockNode = table[tag];
        list.moveToHead(blockNode);
    }

    int search (QueryRecord* q) override  //return nullptr if not-found(miss), or corresponding Block* if found
    {
        // std::cout<< "entered LRU set search\n";

        int tag = q->tag; 
        if (table.find(tag) != table.end())
        {
            updateMRU(tag);
            return table[tag]->block.getTag();
        }

        return -1; 
    }

    bool writeNewToCache(QueryRecord* q, bool isWrite) override
    {
        //when tag was already not present
        // std::cout<< "entered LRU writeNewToCache\n"; 
        bool dirtyEviction = false;
        if (list.getSize() == n_blocks) //evict LRU
        {
            int LRUtag = list.getLRUTag();
            dirtyEviction = list.getLRUDirty();
            list.removeFromTail();
            // delete table[LRUtag]; 
            table[LRUtag] = nullptr;
            table.erase(LRUtag);
        }
        // Node* newNode = new Node(Block(q->tag, isWrite)); //can remove bytes field from block!
        list.insertAtHead(new Node(Block(q->tag, isWrite)));
        table[q->tag] = list.getHead();

        return dirtyEviction;
    }

    void writeExistingToCache(QueryRecord* q) override
    {
        //when tag was already present --> move to front
        // std::cout<< "entered LRU writeExistingToCache\n"; 
        Node* MRUNode = table[q->tag];
        MRUNode->block.setDirty();
        list.moveToHead(MRUNode);
    }

    ~LRUSet() override
    {
        // delete list;

        Node* current = list.getHead();
        while (current != nullptr) {
            Node* next = current->next;
            delete current;  // Free the node
            current = next;
        }
        list.head = list.tail = nullptr;

        table.clear();
    }
};



class FIFOSet : public Set
{
    std::queue<int> queue; //for tags that are currently occupying, if st.size == n_blocks, evict
    std::unordered_map<int, Block> table; //map from tag to Block, use for search

    public:
    FIFOSet(int i, int n)
    {
        index = i;
        n_blocks = n;
    }

    int search(QueryRecord* q) override //return nullptr if not-found(miss), or corresponding Block* if found
    {
        // std::cout<< "entered FIFO set search\n"; 

        int tag = q->tag; 
        if (table.find(tag) != table.end())
        {
            return tag;
        }

        return -1;
    }

    bool writeNewToCache(QueryRecord* q, bool isWrite) override
    {
        //when tag was already not present
        // std::cout<< "entered FIFO writeNewToCache\n"; 
        bool dirtyEviction = false;
        if (queue.size() == n_blocks)
        {
            int firstTag = queue.front();
            Block blk = table[firstTag];
            dirtyEviction = blk.isDirty();
            // delete blk;
            // blk = nullptr;
            table.erase(firstTag);
            queue.pop();
        }
        queue.push(q->tag);
        table[q->tag] = Block(q->tag, isWrite);
        return dirtyEviction;
    }

    void writeExistingToCache(QueryRecord* q) override
    {
        table[q->tag].setDirty();
        //when tag was already present --> do nothing, just increase cycles
    }

    ~FIFOSet() override
    {
        // for (auto itr = table.begin(); itr != table.end(); itr++)
        // {
        //     delete itr->second;
        //     itr->second = nullptr;
        // }
        table.clear();
    }
};