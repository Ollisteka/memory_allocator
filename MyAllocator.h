//
// Created by Olga on 26.03.2018.
//
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <map>

#ifndef MALLOC_MY_ALLOCATOR_H
#define MALLOC_MY_ALLOCATOR_H

using namespace std;

struct MemoryControlBlock {

    MemoryControlBlock() = default;

    MemoryControlBlock(int size, void *ptr, bool free = false) {
        this->size = size;
        this->memoryPointer = ptr;
        this->isFree = free;
    }

    bool isFree;
    int size;
    void *memoryPointer;
};

class MyAllocator {
private:
    int maxSize = 80;
    int sizeTaken = 0;
    void *firstAddress;
    void *currentAddress;
    unordered_map<void *, MemoryControlBlock> memory;
    unordered_map<void *, MemoryControlBlock> freeMemory;
    //vector<MemoryControlBlock> memoryBlocks;
    //vector<int> freeBlocksIndexes;
public:
    MyAllocator() {
        firstAddress = malloc(static_cast<size_t>(maxSize));
        currentAddress = firstAddress;
    }

    void *alloc(int num_bytes) {
        if (memory.empty() || freeMemory.empty())
            return insertNewBlock(num_bytes);
        //TODO: объединять больше, чем 2 блока, сделать побыстрее
        for (pair<void *, MemoryControlBlock> element : freeMemory) {
            MemoryControlBlock *freeBlock = &memory[element.first];
            if (freeBlock->size < num_bytes && !canMergeWithNextBlockToGetEnoughSpace(freeBlock, num_bytes))
                continue;
            void *result = freeBlock->memoryPointer;
            if (canMergeWithNextBlockToGetEnoughSpace(freeBlock, num_bytes))
                mergeTwoAdjacentBlocks(freeBlock);
            if (isNextBlockFree(freeBlock)) {
                addThisRemainderToNextBlock(freeBlock, freeBlock->size - num_bytes, result + num_bytes);
            }
                //дроблю текущий
            else if (freeBlock->size > num_bytes) {
                MemoryControlBlock newBlock = MemoryControlBlock(freeBlock->size - num_bytes,
                                                                 freeBlock->memoryPointer + num_bytes,
                                                                 true);
                memory[newBlock.memoryPointer] = newBlock;
                freeMemory[newBlock.memoryPointer] = newBlock;
            }
            sizeTaken += num_bytes;
            freeBlock->size = num_bytes;
            freeBlock->isFree = false;
            freeMemory.erase(freeBlock->memoryPointer);
            return result;
        }
        return insertNewBlock(num_bytes);
    }

    void free(void *ptr) {
        MemoryControlBlock *blockToFree = &memory[ptr];
        sizeTaken -= blockToFree->size;
        blockToFree->isFree = true;
        freeMemory[ptr] = memory[ptr];
        defragment(ptr);
    }

    void Dump() {
        cout << "Taken " + to_string(sizeTaken) + " out of " + to_string(maxSize) << endl;
        string outer_result;
        vector<void *> sortedMemory;
        for (pair<void *, MemoryControlBlock> memoryBlock : memory)
            sortedMemory.push_back(memoryBlock.first);

        sort(sortedMemory.begin(), sortedMemory.end());
        for (auto ptr : sortedMemory) {
            string insideResult = "|";
            string sep = memory[ptr].isFree ? "_" : "#";
            int upper = memory[ptr].size / 2;
            for (int j = 0; j < upper; ++j) {
                insideResult += sep;
            }
            insideResult += "|";
            outer_result += insideResult;
        }
        cout << outer_result << endl;
//        string free;
//        for (pair<void*, MemoryControlBlock> memoryBlock : freeMemory) {
//            free += to_string(memoryBlock.first) + " ";
//        }
//        cout << free << endl;
    }

private:
    void *insertNewBlock(int num_bytes) {
        if (sizeTaken + num_bytes > maxSize) {
            cout << "Out of memory!" << endl;
            throw bad_alloc();
        }
        void *result = currentAddress;
        memory[result] = MemoryControlBlock(num_bytes, result);
        currentAddress += num_bytes;
        sizeTaken += num_bytes;
        return result;
    }

    void defragment(void *startPtr) {
        MemoryControlBlock *currentFreeBlock = &freeMemory[startPtr];
        while (isNextBlockFree(currentFreeBlock)) {
            mergeTwoAdjacentBlocks(currentFreeBlock);
        }
    }

    bool isNextBlockFree(MemoryControlBlock *currentFreeBlock) {
        return freeMemory.count(currentFreeBlock->memoryPointer + currentFreeBlock->size) != 0;
    }


    bool canMergeWithNextBlockToGetEnoughSpace(MemoryControlBlock *currentFreeBlock, int size_needed) {
        if (!isNextBlockFree(currentFreeBlock))
            return false;
        int currentSize = currentFreeBlock->size;
        int nextSize = memory[getNextPointer(currentFreeBlock)].size;
        return currentSize + nextSize >= size_needed;

    }

    void *getNextPointer(MemoryControlBlock *currentBlock) {
        return currentBlock->memoryPointer + currentBlock->size;
    }

    void mergeTwoAdjacentBlocks(MemoryControlBlock *currentFreeBlock) {
        void *nextBlockPtr = getNextPointer(currentFreeBlock);
        MemoryControlBlock *nextFreeBlock = &memory[nextBlockPtr];
        freeMemory[currentFreeBlock->memoryPointer].size += nextFreeBlock->size;
        memory[currentFreeBlock->memoryPointer].size += nextFreeBlock->size;
        freeMemory.erase(nextBlockPtr);
        memory.erase(nextBlockPtr);
    }

    void addThisRemainderToNextBlock(MemoryControlBlock *currentFreeBlock, int size_to_add, void *newPtr) {
        void *nextBlockPtr = getNextPointer(currentFreeBlock);
        MemoryControlBlock nextFreeBlock = memory[nextBlockPtr];
        nextFreeBlock.memoryPointer = newPtr;
        nextFreeBlock.size += size_to_add;
        freeMemory.erase(nextBlockPtr);
        memory.erase(nextBlockPtr);
        freeMemory[newPtr] = nextFreeBlock;
        memory[newPtr] = nextFreeBlock;
    }

};


#endif //MALLOC_MY_ALLOCATOR_H
