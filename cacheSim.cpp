#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "cache.h"

int total_loads = 0;
int total_stores = 0;
int load_hits = 0;
int load_misses = 0;
int store_hits = 0;
int store_misses = 0;
int total_cycles = 0;

void solve(Cache* c, char ty, unsigned int hexMemAdd, int f)
{
    QueryRecord* q = c->genQuery(ty, hexMemAdd, f);

    c->execute(q);

    if (q->isTypeRead)
    {
        total_loads++;
        if (q->gotHit) load_hits++; else load_misses++;
    }
    else
    {
        total_stores++;
        if (q->gotHit) store_hits++; else store_misses++;
    }
    total_cycles += q->n_cycles;
    delete q;
    // q->display();
}

int main(int argc, char *argv[])
{

    std::string s1 = argv[4];
    std::string s2 = argv[5];
    std::string s3 = argv[6];

    bool a = (s1 == "write-allocate") ? true : false;
    bool b = (s2 == "write-through") ? true : false;
    bool c = (s3 == "lru") ? true : false;

    for (int i = 0; i < 7; i++)
    {
        std::cout << argv[i] << " ";

    }
    std::cout << "\n";

    Cache* cac = new Cache(std::stoi(argv[1]), std::stoi(argv[2]), std::stoi(argv[3]), a, b, c);

    char ty;
    while (std::cin >> ty) {
        unsigned int hexMemAdd; int f;
        std::cin>> std::hex >> hexMemAdd >> std::dec >> f;  
        // std::cout << ty << " " << hexMemAdd << " " << f << "\n";
        solve(cac, ty, hexMemAdd, f);
    }

    delete cac;
    
    std::cout << "Total loads: " << total_loads << std::endl;
    std::cout << "Total stores: " << total_stores << std::endl;
    std::cout << "Load hits: " << load_hits << std::endl;
    std::cout << "Load misses: " << load_misses << std::endl;
    std::cout << "Store hits: " << store_hits << std::endl;
    std::cout << "Store misses: " << store_misses << std::endl;
    std::cout << "Total cycles: " << total_cycles << std::endl;

    return 0;
}

