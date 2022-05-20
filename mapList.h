#ifndef MAPLIST_H
#define MAPLIST_H
/***
 * Map 节点
 * 包含一个Map节点的信息
 */
typedef struct Map{
    char *key;
    char *value;
} Map;
// 结构体，包括一个指向链表尾部的指针，一个指向链表头部的指针，一个指向Map指针
typedef struct MapList{
    struct MapList *next;
    struct Map *data;
}MapList;

// MapsList的 结构体，包含MapList、和MapList长度、和MapList头指针、和MapList尾指针
typedef struct HashMap{
    struct MapList *head;
    int length;
    struct MapList *tail;
}HashMap;

// 创建一个MapList, 包含一个Map和一个指向下一个MapList的指针
MapList *createMapList2(Map *map);
MapList *createMapList();
Map *createMap(char *key, char *value);
// 创建一个HashMap
HashMap *createHashMap();

// 向含有头指针和尾指针HashMap中添加一个Map
void addMap(HashMap *hashMap, Map *map);

// print HashMap
void printHashMap(MapList *mapList);
void printMap(Map *p);


#endif

