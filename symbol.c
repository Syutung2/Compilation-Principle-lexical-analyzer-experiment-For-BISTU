
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "symbol.h"
int id_count = 0;

// 创建符号表
SymbolList* createSymbolList(){
    SymbolList* symbolList = (SymbolList*)malloc(sizeof(SymbolList));
    symbolList->head = NULL;
    symbolList->tail = NULL;
    symbolList->size = 0;
    return symbolList;
}
// 判断符号表是否为空
int isEmpty(SymbolList* symbolList){
    return symbolList->size == 0;
}
// 获取char*类型的指针在内存中的地址数值
long getAddress(char* str){
    return (int)str;
}


// 向符号表中插入一个符号
int insertSymbol(SymbolList* symbolList, char* symbol){
    SymbolNode* symbolNode = (SymbolNode*)malloc(sizeof(SymbolNode));
    symbolNode->symbol = (char*)malloc(sizeof(char) * (strlen(symbol) + 1));
    strcpy(symbolNode->symbol, symbol);
    symbolNode->id = id_count;
    id_count++;

    // 将symbol的指针地址制赋值给symbolNode.value
    symbolNode->value = getAddress(symbolNode->symbol);

    symbolNode->next = NULL;
    if(isEmpty(symbolList)){
        symbolList->head = symbolNode;
        symbolList->tail = symbolNode;
    }else{
        symbolList->tail->next = symbolNode;
        symbolList->tail = symbolNode;
    }
    symbolList->size++;
    return 0;
}
// 从符号表中删除一个符号
int deleteSymbol(SymbolList* symbolList, char* symbol){
    SymbolNode* symbolNode = symbolList->head;
    SymbolNode* preSymbolNode = NULL;
    while(symbolNode != NULL){
        if(strcmp(symbolNode->symbol, symbol) == 0){
            if(preSymbolNode == NULL){
                symbolList->head = symbolNode->next;
            }else{
                preSymbolNode->next = symbolNode->next;
            }
            free(symbolNode->symbol);
            free(symbolNode);
            symbolList->size--;
            return 0;
        }
        preSymbolNode = symbolNode;
        symbolNode = symbolNode->next;
    }
    return -1;
}
// 查找符号表中是否存在一个符号
int isExist(SymbolList* symbolList, char* symbol){
    SymbolNode* symbolNode = symbolList->head;
    while(symbolNode != NULL){
        if(strcmp(symbolNode->symbol, symbol) == 0){
            return 1;
        }
        symbolNode = symbolNode->next;
    }
    return 0;
}
// 查找符号表的一个符号
char* findSymbol(SymbolList* symbolList, char* symbol){
    SymbolNode* symbolNode = symbolList->head;
    while(symbolNode != NULL){
        if(strcmp(symbolNode->symbol, symbol) == 0){
            return symbolNode->value;
        }
        symbolNode = symbolNode->next;
    }
    return NULL;
}
// 销毁符号表
void destroySymbolTable(SymbolList* table){
    SymbolNode* symbolNode = table->head;
    while(symbolNode != NULL){
        SymbolNode* nextSymbolNode = symbolNode->next;
        free(symbolNode->symbol);
        free(symbolNode);
        symbolNode = nextSymbolNode;
    }
    free(table);
}
char* findSymbolId(SymbolList* symbolList, char* symbol){
    SymbolNode* symbolNode = symbolList->head;
    while(symbolNode != NULL){
        if(strcmp(symbolNode->symbol, symbol) == 0){
            // change int which isn't known length to string
            char* id = (char*)malloc(sizeof(char) * (strlen(symbolNode->symbol) + 1));
            sprintf(id, "%d", symbolNode->id);
            return id;
        }
        symbolNode = symbolNode->next;
    }
    return NULL;
}
// 保存符号表到文件
void saveSymbolTable(SymbolList* table){
    FILE* fp = fopen("./output/symbol.soo", "w");
    SymbolNode* symbolNode = table->head;
    while(symbolNode != NULL){
        fprintf(fp, "%d %s 0x%x\n", symbolNode->id,symbolNode->symbol, symbolNode->value);
        symbolNode = symbolNode->next;
    }
    fclose(fp);
}