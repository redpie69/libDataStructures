/*
 * stack.c
 * General Stack implementation (source)
 * Author: Alpay
 * Created: October 27, 2025
 *
 * Description:
 * This source file contains the implementation for the Stack abstraction
 * declared in stack.h. The stack stores elements as opaque binary blobs
 * and is responsible for allocation, alignment, push/pop semantics and
 * lifecycle management (CreateNewStack, DeleteStack, Push, Pop, etc.).
 *
 * Writer:
 * Alpay.
 *
 * Notes:
 * - Keep this implementation consistent with the declarations in stack.h.
 * - Ensure proper memory management to avoid leaks.
*/
#include <stdlib.h>
#include <string.h>
#include "stack.h"

#define CHECK_INITIALIZATION(error) if(!isInitialized)return error; 

// stacks are binay blobs of data 
// bottom pointer points to the first byte of stack
// top pointer points to the last byte of the stack
// index is the first byte of the current item
// element size is size of one element in bytes
typedef struct
{
    uint8_t* bottom;
    uint8_t* top;
    uint8_t* index;
    size_t elementSize;
}Stack_hndl_t;

static bool isInitialized = false;

static Stack_hndl_t* handles = NULL;
static unsigned int stackCount = 0;
static unsigned maxStackCount = 0;

int InitializeStackLibrary(size_t maxStackCountParam)
{
    size_t blockSize;

    CHECK_INITIALIZATION(1);

    if(maxStackCount < 1)
        return 1;
    maxStackCount = maxStackCountParam;
    stackCount = 0;
    handles = NULL;

    blockSize =  sizeof(Stack_hndl_t) * maxStackCount;
    handles = (Stack_hndl_t*)malloc(blockSize);
    if(handles == NULL)
        return 1;
    
    memset(handles,0,blockSize);
    isInitialized = true;
}

Stack_t CreateNewStack(size_t size,size_t elementSize)
{
    Stack_hndl_t* newHandle;

    CHECK_INITIALIZATION(1);
    if(stackCount >= maxStackCount)
        return 1;
    if(size < 1 || elementSize < 1)
        return 1;

    newHandle = &(handles[stackCount++]);
    newHandle->bottom = (uint8_t*)malloc(elementSize*size);
    if(newHandle->bottom == NULL)
        return 1;
    newHandle->top = newHandle->bottom + elementSize*size - 1;
    newHandle->elementSize = elementSize;
    newHandle->index = newHandle->bottom;

    return stackCount-1;
}
