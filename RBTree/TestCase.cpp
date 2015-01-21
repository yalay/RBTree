// TestCase.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "RBTree.h"

int _tmain(int argc, _TCHAR* argv[])
{
    int i, count = 900000;
    key_t key;
    rb_node_t* root = NULL, *node = NULL;
    srand(time(NULL));
    for (i = 1; i < count; ++i)
    {
        key = rand() % count;
        if ((root = rb_insert(key, i, root)))
        {
            printf("[i = %d] insert key %d success!\n", i, key);
        }
        else
        {
            printf("[i = %d] insert key %d error!\n", i, key);
            exit(-1);
        }
        if ((node= rb_search(key, root)))
        {
            printf("[i = %d] search key %d success!\n", i, key);
        }
        else
        {
            printf("[i = %d] search key %d error!\n", i, key);
            exit(-1);
        }
        if (!(i % 10))
        {
            if ((root = rb_erase(key, root)))
            {
                printf("[i = %d] erasekey %d success\n", i, key);
            }
            else
            {
                printf("[i = %d] erasekey %d error\n", i, key);
            }
        }
    }
    return 0;
}
