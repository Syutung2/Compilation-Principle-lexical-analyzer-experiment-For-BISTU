#include <stdio.h>
#include "keywords.h"
# include <string.h>
char *keywords[] = {
        "main","auto", "break", "case", "const", "continue", "default", "do",  "else",  "extern",
        "then", "for", "goto", "if",  "register", "return",  "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union", "unsigned", "volatile", "while", "printf", "scanf","def"};
char *types[] = {"int", "float", "double", "char", "void", "long","enum", "short","string"};
// 创建字母、数字、_ 的字符集
char *alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";
/**
 * 判断是否是关键字字母、数字、_ 的字符
 * @param c  字符
 * @return   是否是关键字字母、数字、_ 的字符 (0,1) , 0 不是, 1 是
 */
int is_alphabet(char c)
{
    if (strchr(alphabet, c) != NULL)
        return 1;
    else
        return 0;
}
int isKeyword(char *str) {
    int i;
    for (i = 0; i < 28; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}
int isType(char *str) {
    int i;
    for (i = 0; i < 9; i++) {
        if (strcmp(str, types[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// 根据str返回关键字的种别码
char* code_keyword(const char* str){
    // if str == identifier
    if(strcmp(str, "identifier") == 0){ // 如果是标识符
        return "1";
    }
    else if(strcmp(str, "number") == 0){ // 如果是数字
        return "2";
    }
    else if(strcmp(str, "keyword") == 0){ // 如果是关键字
        return "3";
    }
    else if(strcmp(str, "operator") == 0){ // 如果是运算符
        return "4";
    }
    else if(strcmp(str, "separator") == 0){ // 如果是分隔符
        return "5";
    }
    else if(strcmp(str, "string") == 0){ // 如果是分隔符
        return "6";
    }
    else if(strcmp(str, "type") == 0){ // 如果是类型，如int，char，float，double，void，enum。则返回类型码,7
        return "7";
    }
    return "0";
}