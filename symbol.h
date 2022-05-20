//
// Created by BISTU_WangSyutung on 2022/4/24.
//

#ifndef UNTITLED_SYMBOL_H
#define UNTITLED_SYMBOL_H
#include "stdio.h"
#define MAX_SYMBOL_LENGTH 100

// 定义符号表的链式节点
typedef struct SymbolNode {
    int id;
    char* symbol;
    int value;
    struct SymbolNode *next;
} SymbolNode;

// 定义符号表的带头尾指针链式结构
typedef struct {
    SymbolNode* head;
    SymbolNode* tail;
    int size;
} SymbolList;
// 创建符号表
SymbolList* createSymbolList();
// 判断符号表是否为空
int isEmpty(SymbolList* symbolList);


// 向符号表中插入一个符号
int insertSymbol(SymbolList* symbolList, char* symbol);
// 从符号表中删除一个符号
int deleteSymbol(SymbolList* symbolList, char* symbol);
// 查找符号表中是否存在一个符号
int isExist(SymbolList* symbolList, char* symbol);
// 查找符号表的一个符号
char* findSymbol(SymbolList* symbolList, char* symbol);
// 查找符号表的一个符号,并返回符号的id
char* findSymbolId(SymbolList* symbolList, char* symbol);
// 销毁符号表
void destroySymbolTable(SymbolList* table);
// 保存符号表到文件
void saveSymbolTable(SymbolList* table);

#endif //UNTITLED_SYMBOL_H
