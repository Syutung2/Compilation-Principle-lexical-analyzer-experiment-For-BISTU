#include "mapList.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <io.h>
// 创建一个MapList
MapList *createMapList()
{
    MapList *mapList = (MapList *)malloc(sizeof(MapList));
    mapList->next = NULL;
    return mapList;
}
MapList *createMapList2(Map *map)
{
    MapList *mapList = (MapList *)malloc(sizeof(MapList));
    mapList->next = NULL;
    mapList->data = map;
    return mapList;
}
// 创建一个Map
Map *createMap(char *key, char *value)
{
    Map *map = (Map *)malloc(sizeof(Map));
    map->key = key;
    map->value = value;
    return map;
}
// 创建一个HashMap
HashMap *createHashMap()
{
    HashMap *hashMap = (HashMap *)malloc(sizeof(HashMap));
    hashMap->head = createMapList();
    hashMap->length = 0;
    hashMap->head = hashMap->head;
    hashMap->tail = hashMap->head;
    return hashMap;
}

// 向含有头指针和尾指针HashMap中添加一个Map
void addMap(HashMap *hashMap, Map *map)
{
    MapList *mapList = createMapList2(map);
    hashMap->tail->next = mapList;
    hashMap->tail = mapList;
    hashMap->length++;
}

// print HashMap
void printHashMap(MapList *mapList)
{
    // 如果链表为空，则返回
    if (mapList == NULL)
    {
        printf("HashMap is empty!\n");
        return;
    }
    mapList = mapList->next;

    // 创建目录 in windows
    if (access("output", 0) == -1)
    {
        mkdir("output");
    }
    FILE *fp = fopen("./output/HashMap.soo", "w");
    // 保存在运行目录下的HashMap.txt文件中
    while (mapList != NULL)
    {
        Map *map = mapList->data;
        fprintf(fp, "(%s,%s)\n", map->value, map->key);
        // printMap(map);
        mapList = mapList->next;
    }
    fclose(fp);
}
void printMap(Map *p)
{
    printf("(%s,%s)\n", p->value, p->key);
}
