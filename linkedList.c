#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "linkedList.h"

#define INIT_CHECK()            \
    if (isInitialized == false) \
    return R_NOT_INITIALIZED

#define GET_LIST(id) listPtr = FindList(id); if(listPtr == NULL)return R_NOT_EXIST;

typedef struct LinkedList
{
    LinkedListNode_t *head;
    LinkedListNode_t *tail;
    unsigned long nodeCount;
} LinkedList_t;

static LinkedList_t lists;
static bool isInitialized = false;

// private function declarations
static LinkedList_t *FindList(LinkedListId id);
static void EmptyList(LinkedList_t* listPtr);
static LinkedListNode_t* FindTheNode(void* data, size_t dataSize, LinkedList_t* listPtr);


ReturnCodes_e LnkLstInitialize()
{
    if (isInitialized)
        return R_ERROR;

    lists.nodeCount = 0;
    lists.head = NULL;
    lists.tail = NULL;
    isInitialized = true;

    return R_OK;
}

void LnkLstDeinitialize()
{

}

ReturnCodes_e LnkLstCreateList(LinkedListId *idOut)
{
    INIT_CHECK();

    if (lists.nodeCount == 0)
    {
        LinkedList_t *newList = (LinkedList_t *)malloc(sizeof(LinkedList_t));
        MALLOC_CHECK(newList);
        newList->head = NULL;
        newList->tail = NULL;
        newList->nodeCount = 0;

        LinkedListNode_t *ListsNode = (LinkedListNode_t *)malloc(sizeof(LinkedListNode_t));
        MALLOC_CHECK(ListsNode);
        ListsNode->data = newList;
        ListsNode->dataSize = sizeof(LinkedList_t);
        ListsNode->next = NULL;
        ListsNode->previous = NULL;

        lists.head = ListsNode;
        lists.tail = ListsNode;
        lists.nodeCount = 1;

        *idOut = newList;
        return R_OK;
    }
    else
    {
        LinkedList_t *newList;
        LinkedListNode_t *newListsNode;

        newList = (LinkedList_t *)malloc(sizeof(LinkedList_t));
        MALLOC_CHECK(newList);
        newList->head = NULL;
        newList->tail = NULL;
        newList->nodeCount = 0;

        newListsNode = (LinkedListNode_t *)malloc(sizeof(LinkedListNode_t));
        MALLOC_CHECK(newListsNode);
        newListsNode->data = newList;
        newListsNode->dataSize = sizeof(LinkedList_t);
        newListsNode->next = NULL;
        newListsNode->previous = lists.tail;

        lists.tail->next = newListsNode;
        lists.tail = newListsNode;
        lists.nodeCount++;

        *idOut = newList;
        return R_OK;
    }
}

ReturnCodes_e LnkLstRemoveList(LinkedListId id)
{
    LinkedListNode_t *nodeToDelete;
    bool isFound;

    INIT_CHECK();

    if (lists.nodeCount == 0)
    {
        return R_NOT_EXIST;
    }

    nodeToDelete = lists.head;
    isFound = true;

    while (nodeToDelete->data != id)
    {
        if (nodeToDelete->next == NULL)
        {
            isFound = false;
        }
        else
        {
            nodeToDelete = nodeToDelete->next;
        }
    }

    if (!isFound)
        return R_NOT_EXIST;

    if (nodeToDelete == lists.head)
    {
        if (nodeToDelete->next != NULL)
        {
            nodeToDelete->next->previous = NULL;
            lists.head = nodeToDelete->next;
        }
        else
        {
            lists.head = NULL;
            lists.tail = NULL;
        }
    }
    else if (nodeToDelete == lists.tail)
    {
        nodeToDelete->previous->next = NULL;
        lists.tail = nodeToDelete->previous;
    }
    else
    {
        nodeToDelete->previous->next = nodeToDelete->next;
        nodeToDelete->next->previous = nodeToDelete->previous;
    }

    EmptyList((LinkedList_t*)nodeToDelete->data);
    free(nodeToDelete->data);
    free(nodeToDelete);
    lists.nodeCount--;
}

ReturnCodes_e LnkLstInsertEnd(void *data, size_t dataSize, LinkedListId id)
{
    LinkedList_t *listPtr; // desired linked list
    LinkedListNode_t* newNodePtr; // the node to be added
    void* recordPtr; // new data will be recorded in here so user of the library doesn't have to think about lifetime of the data

    /*checks*/
    INIT_CHECK();
    NULL_CHECK(data);

    if(dataSize == 0)
        return R_ERROR;
    
    listPtr = FindList(id);
    if (listPtr == NULL)
        return R_NOT_EXIST;
    /*end of checks*/

    recordPtr = malloc(dataSize); // allocating enough memory to data
    memcpy(recordPtr,data,dataSize); // copying
    
    newNodePtr = (LinkedListNode_t*)malloc(sizeof(LinkedListNode_t)); // creating new node
    newNodePtr->data = recordPtr;
    newNodePtr->dataSize = dataSize;

    if(listPtr->tail == NULL) // if the first insert
    {
        if(listPtr->nodeCount != 0 ) // redundant control
        {
            DEBUG_PRINT("tail is null but node count is not 0");
        }    
        // set head and tail
        listPtr->head = newNodePtr;
        listPtr->tail = newNodePtr;
        
        //set newNode
        newNodePtr->next = NULL;
        newNodePtr->previous = NULL;
    }
    else // there is a tail
    {
        // make old tails next the new node and make the previous of newNode old tail
        listPtr->tail->next = newNodePtr;
        newNodePtr->previous = listPtr->tail;

        listPtr->tail = newNodePtr; // new tail is the new node
    }

    listPtr->nodeCount++; // increment node count;
}

ReturnCodes_e LnkLstInsertFront(void *data, size_t dataSize, LinkedListId id)
{
    LinkedList_t *listPtr; // desired linked list
    LinkedListNode_t* newNodePtr; // the node to be added
    void* recordPtr; // new data will be recorded in here so user of the library doesn't have to think about lifetime of the data

    /*checks*/
    INIT_CHECK();
    NULL_CHECK(data);

    if(dataSize == 0)
        return R_ERROR;
    
    listPtr = FindList(id);
    if (listPtr == NULL)
        return R_NOT_EXIST;
    /*end of checks*/

    recordPtr = malloc(dataSize); // allocating enough memory to data
    memcpy(recordPtr,data,dataSize); // copying
    
    newNodePtr = (LinkedListNode_t*)malloc(sizeof(LinkedListNode_t)); // creating new node
    newNodePtr->data = recordPtr;
    newNodePtr->dataSize = dataSize;

    if(listPtr->head == NULL) // if the first insert
    {
        if(listPtr->nodeCount != 0 ) // redundant control
        {
            DEBUG_PRINT("head is null but node count is not 0");
        }    
        
        // set head and tail
        listPtr->head = newNodePtr;
        listPtr->tail = newNodePtr;
        
        //set newNode
        newNodePtr->next = NULL;
        newNodePtr->previous = NULL;
    }
    else // there is a head
    {
        // make previous of the old head the new node and make the next of newNode old head
        listPtr->head->previous = newNodePtr;
        newNodePtr->next = listPtr->head;

        listPtr->head = newNodePtr; // new head is new node
    }

    listPtr->nodeCount++; // increment node count;
}

ReturnCodes_e LnkLstInsertAfter( void* toBeInsertedAfter,size_t sizeToBeInsertedAfter,void *data,size_t dataSize, LinkedListId id)
{
    LinkedList_t* listPtr;
    LinkedListNode_t* nodePtr;
    LinkedListNode_t* newNodePtr;
    void* recordPtr;

    INIT_CHECK();
    GET_LIST(id);
    NULL_CHECK(data);
    NULL_CHECK(toBeInsertedAfter);
    if(dataSize == 0 || sizeToBeInsertedAfter == 0)
        return R_ERROR;
    
    nodePtr = FindTheNode(toBeInsertedAfter,sizeToBeInsertedAfter,listPtr);
    if(nodePtr == NULL)
        return R_NOT_EXIST;
    
    recordPtr = malloc(dataSize); // allocating enough memory to data
    memcpy(recordPtr,data,dataSize); // copying
    
    newNodePtr = (LinkedListNode_t*)malloc(sizeof(LinkedListNode_t)); // creating new node
    newNodePtr->data = recordPtr;
    newNodePtr->dataSize = dataSize;

    if(nodePtr->next != NULL)
    {
        nodePtr->next->previous = newNodePtr;
        newNodePtr->next = nodePtr->next;
        newNodePtr->previous = nodePtr;
        nodePtr->next = newNodePtr;
    }
    else
    {
        if(nodePtr != listPtr->tail) // redundant check
        {
            DEBUG_PRINT("the node has no next but it is not the tail");
        }

        nodePtr->next = newNodePtr;
        newNodePtr->previous = nodePtr;
        newNodePtr->next = NULL;
        listPtr->tail = newNodePtr;
    }

    return R_OK;
}

ReturnCodes_e LnkLstInsertBefore(const LinkedListNode_t *node, void *data, LinkedListId id)
{
}

ReturnCodes_e LnkLstInsertAt(unsigned int index, void *data, LinkedListId id)
{
}

ReturnCodes_e LnkLstDeleteEnd(void *data, LinkedListId id)
{
}

ReturnCodes_e LnkLstDeleteFront(void *data, LinkedListId id)
{
}

ReturnCodes_e LnkLstDeleteAfter(const LinkedListNode_t *node, void *data, LinkedListId id)
{
}

ReturnCodes_e LnkLstDeleteBefore(const LinkedListNode_t *node, void *data, LinkedListId id)
{
}

ReturnCodes_e LnkLstDeleteAt(unsigned int index, void *data, LinkedListId id)
{
}

ReturnCodes_e LnkLstGetFirst(LinkedListNode_t **nodeOut, LinkedListId id)
{
    LinkedList_t* listPtr;

    INIT_CHECK();
    GET_LIST(id);

    *nodeOut = listPtr->head;
}

ReturnCodes_e LnkLstGetLast(LinkedListNode_t **nodeOut, LinkedListId id)
{
    LinkedList_t* listPtr;

    INIT_CHECK();
    GET_LIST(id);

    *nodeOut = listPtr->tail;
}

ReturnCodes_e LnkLstGetAt(LinkedListNode_t **nodeOut, unsigned long int index, LinkedListId id)
{
}


/*private function definitions*/
static void EmptyList(LinkedList_t* listPtr)
{
    LinkedListNode_t* currNodePtr;
    LinkedListNode_t* nextNodePtr;

    if(listPtr->head != NULL   )
        currNodePtr = listPtr->head;
    else if(listPtr->nodeCount != 0 /*redundant control*/) // List is already empty
        return;
    else // this should never be executed
    {
        DEBUG_PRINT("something went wrong in the EmptyList function");
    }
    
    while(1)
    {
        nextNodePtr = currNodePtr->next;

        free(currNodePtr->data);
        free(currNodePtr);
        listPtr->nodeCount--;
        if(nextNodePtr == NULL)
        {
            if(listPtr->nodeCount == 0 /*everything is allright*/)
            {
                listPtr->head = NULL;
                listPtr->tail = NULL;
                break;
            }
            else // something went wrong
            {
                DEBUG_PRINT("there was an error while emptying the list");
            }
        }
        else
        {
            currNodePtr = nextNodePtr;
        }
    }
}

static LinkedList_t *FindList(LinkedListId id)
{
    LinkedListNode_t *currNodePtr;

    currNodePtr = lists.head;
    while (currNodePtr->data != id)
    {
        if (currNodePtr->next != NULL)
        {
            currNodePtr = currNodePtr->next;
        }
        else
        {
            return NULL;
        }
    }
    return (LinkedList_t *)currNodePtr->data;
}

static LinkedListNode_t* FindTheNode(void* data, size_t dataSize, LinkedList_t* listPtr)
{
    LinkedListNode_t* currentNodePtr;

    if(listPtr->nodeCount == 0)
        return NULL;
    
    currentNodePtr = listPtr->head;
    for(int i=0; i<listPtr->nodeCount;i++)
    {
        if(currentNodePtr->dataSize != dataSize)
            continue;
        if(memcmp(data,currentNodePtr->data,dataSize))
            continue;
        return currentNodePtr;
    }
    return NULL;
}