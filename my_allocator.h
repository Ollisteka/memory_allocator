//
// Created by Olga on 26.03.2018.
//
#include <vector>
#include <iostream>

#ifndef MALLOC_MY_ALLOCATOR_H
#define MALLOC_MY_ALLOCATOR_H

using namespace std;

class MemoryControlBlock {


public:
    MemoryControlBlock() {

    }

    MemoryControlBlock(int size, void *ptr, MemoryControlBlock *prev, MemoryControlBlock *nextBlock) {
        this->size = size;
        this->memoryPointer = ptr;
        this->previous = prev;
        this->next = nextBlock;
        this->isFree = false;
    }
    bool isFree;
    int size;
    void *memoryPointer;
    MemoryControlBlock *next;
    MemoryControlBlock *previous;

    string ToString() {
        // print something from v to str, e.g: Str << v.getX();
        string freedom = "Is free: ";
        if (isFree) {
            freedom += "True";
        } else { freedom += "False"; }
        string size_taken = "\nTotal size: " + to_string(size) + "\n";
        return freedom + size_taken;
    }

    void *GetPointer() {
        return this->memoryPointer;
    }

    int GetSize() {
        return this->size;
    }
};

class my_allocator {
private:
    int chunkSize = 16;
    int chunksCount = 10;
    int maxSize = chunkSize * chunksCount;
    int sizeTaken = 0;
    void *firstAddr;
    void *currentAddr;
    vector<MemoryControlBlock> memoryBlocks;
public:
    my_allocator() {
        firstAddr = malloc(static_cast<size_t>(maxSize));
        currentAddr = firstAddr;
    }

    void *alloc(int num_bytes) {
        if (sizeTaken + num_bytes > maxSize) {
            cout << "Out of memory!" << endl;
            throw bad_alloc();
        }
        MemoryControlBlock block;
        auto result = currentAddr;
        if (memoryBlocks.empty()) {
            block = MemoryControlBlock(num_bytes, result, nullptr, nullptr);
        } else {
            ulong last = memoryBlocks.size() - 1;
            MemoryControlBlock lastBlock = memoryBlocks[last];
            block = MemoryControlBlock(num_bytes, result, &lastBlock, nullptr);
            lastBlock.next = &block;
        }

        currentAddr += num_bytes;
        sizeTaken += num_bytes;
        memoryBlocks.push_back(block);
        return result;
    }

    void free(void *ptr) {
        int idx = -1;
        MemoryControlBlock blockToFree;
        for (int i = 0; i < memoryBlocks.size(); ++i) {
            auto block = memoryBlocks[i];
            if (block.GetPointer() == ptr) {
                idx = i;
                blockToFree = block;
                break;
            }
        }
        if (idx == -1)
            throw exception();
        memoryBlocks.erase(memoryBlocks.begin() + idx);
        sizeTaken -= blockToFree.GetSize();
        blockToFree.isFree = true;

        MemoryControlBlock *nextBlock = blockToFree.next;
        MemoryControlBlock *prevBlock = blockToFree.previous;
        if (prevBlock != NULL) {
            prevBlock->next = nextBlock;
        }
        if (nextBlock != NULL) {
            nextBlock->previous = prevBlock;
        }
    }

    void Dump() {
        cout << "NEW DUMP:" << endl;
        cout << "Capacity: " + to_string(maxSize) << endl;
        cout << "Size taken: " + to_string(sizeTaken) << endl;
        cout << "Blocks in use: " + to_string(memoryBlocks.size()) << endl;
//        for (int i = 0; i < memoryBlocks.size(); ++i) {
//            cout << "BLOCK" + to_string(i) + ":\n" + memoryBlocks[i].ToString() << endl;
//        }
    }
};


#endif //MALLOC_MY_ALLOCATOR_H
