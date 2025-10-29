/*
 * A general Stack Implementation
 * Author: Alpay
 * Created: October 27, 2025
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "commonDefs.h"

// stack identifier
typedef unsigned int Stack_t;

// initializes shared resources and defines max numbers of stacks
// call this function first.
// maxStackCount should be a positive integer
// returns 0 if successful , else 1
int InitializeStackLibrary(size_t maxStackCount);

// Pushes elements into the given stack
// if stack is not full pushes the element and returns 0
// if stack is full returns 1
// if the size of the element is invalid for the given stack returns 2
int Push(void* element,size_t sizeOfElement,Stack_t stack);

// Pops Element from the given stack
// Resolve the address and copy the element right away because stack does not guarantee the existence of the given element after the pop.
// if stack is empty returns NULL;
void* Pop(Stack_t stack);

// Used create a new stack with a given type
// Only size of the elements is required since stack acts like elements are binary blobs
// Space alignment is done dynamically for the stack
// If space is available and sizes aren't invalid, returns the stack identifier . Otherwise returns -1.
Stack_t CreateNewStack(size_t size,size_t elementSize);

// Deletes the given stack if it exists
// When you are finished with the stacks delete them or they will cause memory leaks.
void DeleteStack(Stack_t stack);

// Returns whether the stack is full if the stack exists.
// if it isn't exist returns true anyway.
bool IsFull(Stack_t stack);

// Returns whether the given stack is Empty
// if the given stack doesn't exist, returns true anyway.
bool IsEmpty(Stack_t stack);