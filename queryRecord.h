#include <vector>
#include <iostream>

class QueryRecord
{

    private:
    public:
    bool isTypeRead; //whether read(1) or write query(0)
    int index;//which set in the cache?
    int tag; //main way to search
    bool gotHit; //True if query hit, false if missed
    int n_cycles;

    QueryRecord(char ty, int idx, int t)
    {
        isTypeRead = (ty == 'l') ? true : false;
        index = idx; //to get set
        tag = t; //to search block in set
        gotHit = false;
        n_cycles = 0;
    }

    void display()
    {
        if (isTypeRead) std::cout << "(r) "; else std::cout << "(w) ";
        std::cout << "Index: " << index << ", Tag: " << tag << " | ";
        if (gotHit) std::cout << "Hit | "; else std::cout << "Miss | ";
        std::cout << "Cycles: " << n_cycles << "\n";
        std::cout << "----------------------------------------------\n";
    }

};

