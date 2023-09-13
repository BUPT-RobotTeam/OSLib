/*******************************************************************************
 * Copyright:		BUPT
 * File Name:		hash.c
 * Author:			wangzhi
 * Description:		None
 * Bug:				None
 * Version:			0.1
 * Data:			2019/09/20 Fri 20:15
 * History:         Modified 2019/09/20 by ZeroVoid
 * Todo:			None
 *******************************************************************************/
#include "oslib_config.h"
#include "utils/oslib_hash.h"

#include "FreeRTOS.h"
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

static int cmpAtom(const void *x, const void *y)
{
    return x != y;
}

static unsigned long hashAtom(const void *key)
{
    return (unsigned long)key >> 2;
}

static void freeKeyAtom(const void *key)
{
}

int cmpStr(const void *x, const void *y)
{
    return strcmp((char *)x, (char *)y) != 0;
}

unsigned int hashStr(const void *key)
{
    unsigned int hash = 5381;
    char *ch = (char *)key;
    while (*ch)
    {
        hash += (hash << 5) + (*ch++);
    }
    return (hash & 0x7FFFFFFF);
}

HashTable HashTable_create(int (*cmp)(const void *, const void *),
                           unsigned int (*hash)(const void *),
                           void (*freeKey)(const void *key))
{
    int i;
    HashTable hashTable;
    //确定bucket的数目，分配内存，bucket内存紧跟着hashTable
    hashTable = pvPortMalloc(sizeof(*hashTable) + BUCKET_SIZE * sizeof(HashNode));

    //初始化
    hashTable->cmp = cmp ? cmp : cmpAtom;
    hashTable->hash = hash ? hash : (unsigned int (*)(void const *))hashAtom;
    hashTable->freeKey = freeKey ? freeKey : freeKeyAtom;
    hashTable->bucket = (HashNode **)(hashTable + 1);
    for (i = 0; i < BUCKET_SIZE; ++i)
        hashTable->bucket[i] = NULL;
    hashTable->length = 0;
    hashTable->timestamp = 0;

    return hashTable;
}

void HashTable_destroy(HashTable *hashTable)
{
    assert(hashTable && *hashTable);

    if ((*hashTable)->length > 0)
    {
        HashNode *p, *q;
        for (int i = 0; i < (*hashTable)->length; ++i)
        {
            for (p = (*hashTable)->bucket[i]; p; p = q)
            {
                q = p->next;
                ((*hashTable)->freeKey)(p->key);
                vPortFree(p);
            }
        }
    }
    vPortFree(hashTable);
}

int HashTable_length(HashTable hashTable)
{
    return hashTable->length;
}

void *HashTable_insert(HashTable hashTable, const void *key, void *value)
{
    void *prev = NULL; //之前的值
    HashNode *p;
    unsigned int index;

    assert(hashTable);
    assert(key);

    //search hashTable for key
    index = hashTable->hash(key) % BUCKET_SIZE;
    //printf("insert index:%d\n", index);
    for (p = hashTable->bucket[index]; p; p = p->next)
    {
        if (hashTable->cmp(key, p->key) == 0)
        {
            break;
        }
    }

    if (p == NULL)
    {
        p = pvPortMalloc(sizeof(*p));
        if (p == NULL)
        {
            return NULL;
        }
        p->key = key;
        p->next = hashTable->bucket[index];
        hashTable->bucket[index] = p;
        hashTable->length++;
    }
    else
    {
        prev = p->value;
    }

    p->value = value;
    hashTable->timestamp++;

    return prev;
}

void *HashTable_get(HashTable hashTable, const void *key)
{
    unsigned int index;
    HashNode *p;

    assert(hashTable);
    assert(key);

    index = hashTable->hash(key) % BUCKET_SIZE;
    //printf("get index:%d\n", index);
    for (p = hashTable->bucket[index]; p; p = p->next)
    {
        if (hashTable->cmp(key, p->key) == 0)
        {
            break;
        }
    }

    return p ? p->value : NULL;
}

void *HashTable_remove(HashTable hashTable, const void *key)
{
    HashNode **pp;
    unsigned int index;

    assert(hashTable);
    assert(key);

    //printf("key:%d\n", *(int *) key);

    hashTable->timestamp++;
    index = (hashTable->hash)(key) % BUCKET_SIZE;
    //printf("index:%d\n", index);
    for (pp = &hashTable->bucket[index]; *pp; pp = &((*pp)->next))
    {
        if ((hashTable->cmp)(key, (*pp)->key) == 0)
        {
            HashNode *p = *pp;
            void *value = p->value;
            *pp = p->next;
            (hashTable->freeKey)(p->key);
            vPortFree(p);
            hashTable->length--;
            return value;
        }
    }

    return NULL;
}

void HashTable_map(HashTable hashTable, void (*apply)(const void *key, void **value, void *c1), void *c1)
{
    HashNode *p;
    unsigned int stamp;

    assert(hashTable);
    assert(apply);

    stamp = hashTable->timestamp;
    for (int i = 0; i < BUCKET_SIZE; ++i)
    {
        for (p = hashTable->bucket[i]; p; p = p->next)
        {
            apply(p->key, &(p->value), c1);
            assert(stamp == hashTable->timestamp);
        }
    }
}

void **HashTable_toArray(HashTable hashTable, void *end)
{
    int i, j = 0;
    void **array;
    HashNode *p;
    assert(hashTable);
    array = pvPortMalloc((2 * hashTable->length + 1) * sizeof(*array));
    for (i = 0; i < BUCKET_SIZE; i++)
        for (p = hashTable->bucket[i]; p; p = p->next)
        {
            array[j++] = (void *)p->key;
            array[j++] = p->value;
        }
    array[j] = end;
    return array;
}
