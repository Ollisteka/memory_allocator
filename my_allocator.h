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

    MemoryControlBlock(int size, void *ptr) {
        this->size = size;
        this->memoryPointer = ptr;
        this->isFree = false;
    }
    bool isFree;
    int size;
    void *memoryPointer;

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
            block = MemoryControlBlock(num_bytes, result);
        } else {

            block = MemoryControlBlock(num_bytes, result);
        }

        currentAddr += num_bytes;
        sizeTaken += num_bytes;
        memoryBlocks.push_back(block);
        return result;
    }

    void free(void *ptr) {
        int idx = -1;
        for (int i = 0; i < memoryBlocks.size(); ++i) {

            if (memoryBlocks[i].GetPointer() == ptr) {
                idx = i;
                sizeTaken -= memoryBlocks[i].GetSize();
                memoryBlocks[i].isFree = true;
                break;
            }
        }
        if (idx == -1)
            throw exception();
        // memoryBlocks.erase(memoryBlocks.begin() + idx);
        //  memoryBlocks.insert(memoryBlocks.begin() + idx,);

        auto a = 0;
    }

    void Dump() {
        cout << "NEW DUMP:" << endl;
        string outer_result;
        for (int i = 0; i < memoryBlocks.size(); ++i) {
            string insideResult = "|";
            string sep;
            if (memoryBlocks[i].isFree)
                sep = "_";
            else sep = "#";
            int upper = memoryBlocks[i].size / 2;
            for (int j = 0; j < upper; ++j) {
                insideResult += sep;
            }
            insideResult += "|";
            outer_result += insideResult;
        }
        cout << outer_result << endl;
//        cout << "NEW DUMP:" << endl;
//        cout << "Capacity: " + to_string(maxSize) << endl;
//        cout << "Size taken: " + to_string(sizeTaken) << endl;
//        cout << "Blocks in use: " + to_string(memoryBlocks.size()) << endl;
//        for (int i = 0; i < memoryBlocks.size(); ++i) {
//            cout << "BLOCK" + to_string(i) + ":\n" + memoryBlocks[i].ToString() << endl;
//        }
    }
};


#endif //MALLOC_MY_ALLOCATOR_H
