#include "block.h"

Block::Block()
{
    tag = -1;
    dirty = false;
}

Block::Block(int c) {
    tag = c;
    dirty = false;
}

Block::Block(int c, bool d) {
    tag = c;
    dirty = d;
}

int Block::getTag() {
    return tag;
}

bool Block::isDirty() {
    return dirty;
}

void Block::setDirty() {
    dirty = true;
}