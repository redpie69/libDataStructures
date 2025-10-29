#include <stdio.h>
#include "linkedList.h"

int main(int argc, char* argv)
{
    LnkLstInitialize();
    LinkedListId myId;
    LnkLstCreateList(&myId);
    LnkLstRemoveList(myId);
    return 0;
}