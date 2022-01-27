#include "VirtualMemory.h"
#include "PhysicalMemory.h"
#include <math.h>

#define _CEIL(VARIABLE) ( (VARIABLE - (int)VARIABLE)==0 ? (int)VARIABLE : (int)VARIABLE+1 )

/**
 * A struct for the cyclic operation data to be saved.
 */
typedef struct Cyclic
{
    uint64_t frameAddress;
    uint64_t page;
    uint64_t distance;
} Cyclic;

/**
 * A function that clears the given frame and puts 0 in it.
 * @param frameIndex The number of the frame to clear.
 */
void clearTable(uint64_t frameIndex)
{
    for (uint64_t i = 0; i < PAGE_SIZE; ++i)
    {
        PMwrite(frameIndex * PAGE_SIZE + i, 0);
    }
}

/**
 * A function that initializes the virtual memory.
 */
void VMinitialize()
{
    clearTable(0);
}

/**
 * A function that separates addresses.
 * @param arr The pointer to the array to store the numbers in.
 * @param addr The given virtual address to separate.
 * @param size The size of the address.
 */
void separateAddr(uint64_t *arr, uint64_t addr, unsigned int size)
{
    uint64_t mask = PAGE_SIZE - 1;
    for (unsigned int i = 0; i < size; i++)
    {
        arr[size - i - 1] = addr & mask;
        addr >>= OFFSET_WIDTH;
    }
}

/**
 * A helper function that traverses the entire tree to find a free space for the find frame function.
 * @param addrParent The number representation of the parent address.
 * @param maxFrame The pointer to the maximal frame number we reached.
 * @param cyclic The pointer to the cyclic struct.
 * @param stPage The address of the page we wish to read/write to/from.
 * @param depth The depth we reached in the traversal.
 * @param currPage The number representation of the current page.
 * @param lastFrame The number of the frame last visited.
 * @param freeFrame The pointer to the free frame we found.
 */
void memoryTraversal(uint64_t addrParent, uint64_t *maxFrame, Cyclic *cyclic,
                     uint64_t &stPage, int depth, uint64_t currPage, word_t &lastFrame,
                     word_t *freeFrame)
{
    word_t subTreeRoot = 0;

    if (depth != 0)
    {
        PMread(addrParent, (word_t *) &subTreeRoot);
    }
    if (depth == TABLES_DEPTH)
    {
        long long dist = abs((long long) (currPage - stPage));
        long long min = (NUM_PAGES - dist) > dist ? dist : NUM_PAGES - dist;
        if ((long long) cyclic->distance < min)
        {
            cyclic->frameAddress = addrParent;
            cyclic->distance = min;
            cyclic->page = currPage;
        }
        return;
    }
    int nullChildes = 0;
    word_t subTreeChildes[PAGE_SIZE];

    for (uint64_t i = 0; i < PAGE_SIZE; i++)
    {
        PMread(subTreeRoot * PAGE_SIZE + i, (word_t *) subTreeChildes + i);
        if (subTreeChildes[i] == 0)
        {
            ++nullChildes;
        }
    }
    if (nullChildes == PAGE_SIZE && subTreeRoot != 0 && subTreeRoot != lastFrame)
    {
        *freeFrame = subTreeRoot;
        PMwrite(addrParent, 0);
        return;
    }
    currPage <<= OFFSET_WIDTH;
    for (long i = 0; i < PAGE_SIZE; ++i)
    {
        if (uint64_t(subTreeChildes[i]) > *maxFrame)
        {
            *maxFrame = subTreeChildes[i];
        }
        if (subTreeChildes[i])
        {
            memoryTraversal(subTreeRoot * PAGE_SIZE + i, maxFrame,
                            cyclic, stPage, depth + 1,
                            currPage + i, lastFrame, freeFrame);
        }
    }
}

/**
 * A function that finds the free frame to put the table in.
 * @param frame The pointer to the frame that called the function.
 * @param page The given page we wish to read/write to/from.
 * @param lastFrame The number of the frame last visited.
 */
void findFrame(word_t *frame, uint64_t page, word_t lastFrame)
{
    Cyclic cyclic = {0, 0, 0};
    uint64_t maxFrameInd = 0;
    word_t freeFrame = 0;
    memoryTraversal(0, &maxFrameInd, &cyclic, page, 0, 0, lastFrame, &freeFrame);
    if (freeFrame)
    {
        *frame = freeFrame;
        return;
    } else if (maxFrameInd + 1 < NUM_FRAMES)
    {
        *frame = maxFrameInd + 1;
        return;
    } else  // Situation 3
    {
        word_t value;
        PMread(cyclic.frameAddress, &value);
        PMevict(value, cyclic.page);
        PMwrite(cyclic.frameAddress, 0);
        *frame = value;
    }
}

/**
 * A function that receives the virtual address and returns the address to read/write to.
 * @param virtualAddress The number that represents the virtual address.
 * @return The number representation of the address to read/write to.
 */
uint64_t getPMAddr(uint64_t virtualAddress)
{
    auto page = virtualAddress >> OFFSET_WIDTH;
    uint64_t arr[_CEIL(float(VIRTUAL_ADDRESS_WIDTH) / OFFSET_WIDTH)]; // Array of offsets.
    separateAddr(arr, virtualAddress, _CEIL(float(VIRTUAL_ADDRESS_WIDTH) / OFFSET_WIDTH)); // Get offsets.
    uint64_t parent = 0UL;
    word_t frame_val, last_frame = 0;
    for (int i = 0; i < TABLES_DEPTH; i++)
    {
        PMread(parent * PAGE_SIZE + arr[i], &frame_val);
        if (frame_val == 0) //
        {
            findFrame(&frame_val, page, last_frame);
            if (i + 1 != TABLES_DEPTH)
            {
                clearTable(frame_val);
            } else
            {
                PMrestore(frame_val, page);
            }
            PMwrite(parent * PAGE_SIZE + arr[i], frame_val);
        }
        parent = frame_val;
        last_frame = frame_val;
    }

    return parent * PAGE_SIZE + arr[_CEIL(float(VIRTUAL_ADDRESS_WIDTH) / OFFSET_WIDTH) - 1];
}

/**
 * A function that reads a word from the given virtual address and puts its content in *value.
 * @param virtualAddress The number that represents the virtual address.
 * @param value The value that we wish to put.
 * @return returns 1 on success. returns 0 on failure (if the address cannot be mapped to a physical
 * address for any reason).
 */
int VMread(uint64_t virtualAddress, word_t *value)
{
    if (virtualAddress >= VIRTUAL_MEMORY_SIZE)
    {
        return 0;
    }
    uint64_t addr = getPMAddr(virtualAddress);
    PMread(addr, value);
    return 1;
}

/**
 * A function that writes a word to the given virtual address.
 *  @param virtualAddress The number that represents the virtual address.
 * @param value The value that we wish to put.
 * @return returns 1 on success. returns 0 on failure (if the address cannot be mapped to a physical
 * address for any reason).
 */
int VMwrite(uint64_t virtualAddress, word_t value)
{
    if (virtualAddress >= VIRTUAL_MEMORY_SIZE)
    {
        return 0;
    }
    uint64_t addr = getPMAddr(virtualAddress);
    PMwrite(addr, value);
    return 1;
}