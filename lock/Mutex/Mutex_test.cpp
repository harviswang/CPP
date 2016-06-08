#include <stdio.h>
#include "Mutex.h"

int main(int argc, char **arg)
{
    printf("Mutex class test ...\n");
    Mutex mMutex;
    {
        AutoMutex _l(mMutex);
    }
    {
        Mutex::Autolock _m(mMutex);
    }
    return 0;
}
