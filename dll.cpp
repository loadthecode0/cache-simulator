#include "dll.h"
#include <iostream>

Node::Node()
{
    next = nullptr;
    prev = nullptr;
    // block = Block(-1);
}

Node::Node(Block bl)
{
    next = nullptr;
    prev = nullptr;
    block = bl;
}

Node::Node(Block bl, Node* n, Node* p)
{
    next = n;
    prev = p;
    block = bl;
}

Node::~Node()
{
    // next = prev = nullptr;
    // delete block;
}

DLL::DLL()
{
    size = 0;
    head = nullptr;
    tail = nullptr;
}

Node* DLL::getHead()
{
    return head;
}

void DLL::removeFromTail()
{
    if (size == 0) return;
    if (size == 1)
    {
        delete tail;
        head = tail = nullptr;
        size--;
        return;
    }

    Node* temp = tail;
    tail = tail -> prev;
    tail -> next = nullptr;
    delete temp; temp = nullptr;
    size--;
}

void DLL::insertAtHead(Node* node)
{
    if (size == 0)
    {
        head = tail = node;
        size++;
        return;
    }

    head -> prev = node;        
    node -> next = head;
    head = node;
    size++;
}

void DLL::moveToHead (Node* node)
{
    if (size <= 1 || node == head) return;

    if (node->prev) 
    {
        node->prev->next = node->next;
    }
    if (node->next) 
    {
        node->next->prev = node->prev;
    }

    if (node == tail) {
        tail = node->prev;
    }
    node->prev = nullptr;
    if(head) head->prev = node;
    node->next = head;
    head = node;
}

int DLL::getSize()
{
    return size;
}

int DLL::getLRUTag()
{
    return tail->block.getTag();
}

int DLL::getLRUDirty()
{
    return tail->block.isDirty();
}

DLL::~DLL()
{
    if (size == 1)
    {
        delete head; head = nullptr;
    }
    else
    {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;  // Free the memory of the node
            current = next;
        }
        head = nullptr;
        tail = nullptr;
    }
    // delete this;
}