//
// Created by Olga on 26.03.2018.
//
#include <vector>
#include <iostream>

#ifndef MALLOC_MY_ALLOCATOR_H
#define MALLOC_MY_ALLOCATOR_H

using namespace std;

struct MemoryControlBlock {
    MemoryControlBlock(int size, void *ptr, MemoryControlBlock *prev, MemoryControlBlock *next) {
        this->size = size;
        this->memoryPointer = ptr;
        this->prev = prev;
        this->next = next;
        this->isFree = true;
    }

    bool isFree;
    int size;
    void *memoryPointer;
    MemoryControlBlock *next;
    MemoryControlBlock *prev;

    string ToString() {
        // print something from v to str, e.g: Str << v.getX();
        string freedom = "Is free: " + isFree;
        string size = "\nTotal size: " + size + "\n";
        return freedom + size;
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
    int chunkSize = 2048;
    int chunksCount = 10;
    int maxSize = chunkSize * chunksCount;
    int totalSize = chunkSize * chunksCount;
    void *firstAddr;
    void *currentAddr;
    vector<MemoryControlBlock *> memoryBlocks;
public:
    my_allocator() {
        firstAddr = malloc(static_cast<size_t>(maxSize));
        currentAddr = firstAddr;
    }

    void *alloc(int num_bytes) {
        if (totalSize + num_bytes > maxSize) {
            throw bad_alloc();
        }
        MemoryControlBlock *block = NULL;
        auto result = currentAddr;
        if (memoryBlocks.empty()) {
            block = new MemoryControlBlock(num_bytes, result, nullptr, nullptr);
        } else {
            block = new MemoryControlBlock(num_bytes, result, &memoryBlocks[memoryBlocks.size() - 1], nullptr);
        }

        currentAddr += num_bytes;
        totalSize -= num_bytes;
        memoryBlocks.push_back(block);
        return result;
    }

    void free(void *ptr) {
        int idx = -1;
        MemoryControlBlock *blockToFree = NULL;
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
        totalSize -= blockToFree.GetSize();
        auto nextBlock = blockToFree.next;
        auto prevBlock = blockToFree.prev;
        prevBlock->next = nextBlock;
        nextBlock->prev = prevBlock;
    }

    void Dump() {
        for (int i = 0; i < memoryBlocks.capacity(); ++i) {
            cout << memoryBlocks[i].ToString() << endl;
        }
    }
};


#endif //MALLOC_MY_ALLOCATOR_H
