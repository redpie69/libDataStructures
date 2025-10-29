#define DEBUG // comment this to inactivate debug messages
#ifdef DEBUG
#include <stdio.h>
#define DEBUG_PRINT(format, ...) printf(format, ##__VA_ARGS__); abort()
#else
#define DEBUG_PRINT(msg)
#endif

#define NULL_CHECK(object) if(object == NULL)return R_OBJECT_IS_NULL
#define MALLOC_CHECK(ptr) if(ptr == NULL)return R_HEAP_IS_FULL

typedef enum 
{
    R_OK,
    R_ERROR,
    R_NOT_ENOUGH_SPACE,
    R_HEAP_IS_FULL,
    R_OBJECT_IS_NULL,
    R_NOT_INITIALIZED,
    R_NOT_EXIST
}ReturnCodes_e;