#include "commonDefs.h"

struct LinkedListNode
{
    void* data;
    size_t dataSize;
    struct LinkedListNode* next;
    struct LinkedListNode* previous;
};

typedef struct LinkedListNode LinkedListNode_t;

typedef void* LinkedListId;

ReturnCodes_e LnkLstInitialize();
ReturnCodes_e LnkLstCreateList(LinkedListId* idOut);
ReturnCodes_e LnkLstRemoveList(LinkedListId id);