#include "block.h"

class Node
{
    private:

    public:
    Node* next;
    Node* prev;
    Block block;
    Node();
    Node(Block bl);
    Node(Block bl, Node* n, Node* p);

    ~Node();
};

class DLL
{
    private:
    

    public:
    int size;
    Node* head;
    Node* tail;
    DLL();
    Node* getHead();
    void removeFromTail ();
    void insertAtHead (Node* node);
    void moveToHead (Node* node);
    int getSize();
    int getLRUTag();
    int getLRUDirty();
    ~DLL();
};