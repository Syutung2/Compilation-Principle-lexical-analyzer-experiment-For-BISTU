#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "./mapList.h"
#include "keywords.h"
#include "symbol.h"
SymbolList *symbolList;
// 词法分析器
// 词法分析器的输入是文件、输出是词法分析器的结果、输出的结果是一个HashMap
// HashMap的结构体包含一个MapList、MapList的结构体包含一个Map、Map的结构体包含一个key、value
char *sub(char *str, int start, int end);
void analysis(char *str, HashMap *new);
void checkString(char *string, HashMap *pMap, int i);
void RemoveExtraSpace(char *str);
// 主函数，argc是参数个数，argv是参数数组
// 参数个数为1，即只有一个参数，即程序名
// 参数个数为2，即有两个参数，即程序名和需要解析的文件名
int main(int argc, char *argv[])
{
    FILE *fp;
    int c;
    int lines = 0;
    if (argc != 2)
    {
        printf("Usage: scc  <filename>\n");
        return -1;
    }
    fp = fopen(argv[1], "r");             // 打开文件，参数为文件名，只读方式，返回文件指针，如果打开失败，返回NULL
    char *suffix = strrchr(argv[1], '.'); // 取出文件名中的后缀，返回指向后缀的指针
    if (fp == NULL)                       // 打开文件失败
    {
        printf("Cannot open file %s\n", argv[1]);
        return 1;
    }
    if (strcmp(suffix, ".syu") != 0)
    {
        printf("%s is not a .syu file\n", argv[1]);
        return 1;
    } // 如果不是.syu文件，返回

    char *buf = (char *)malloc(sizeof(char) * 300);
    // 分配300个字节的空间，用于存储一行文本,这里的300是个随便的数字，可以自己设置
    HashMap *m = createHashMap(); // 创建一个HashMap
     symbolList = createSymbolList(); // 创建一个符号表

    while ((fgets(buf, 300, fp)) != NULL)
    // 从文件中读取一行文本，并存储在buf中，直到文件结束
    {
        analysis(buf, m); // 分析token，并将结果存储在new中
        lines++;          // 行数加1 , 记录代码行数
    }

    // log  maplist
    printf("\n");
    printf("MapList:\n");
    printHashMap(m->head);
    printf("\n\n");
    // log  maplist's size
    printf("MapList's size: %d\n", m->length);
    printf("\n");
    // saveSymbolTable(symbolList);
    fclose(fp);
    return 0;
}

void checkString(char *string, HashMap *pMap, int i)
{
    // 如果 str 为空、制表符、换行符 则 跳过
    if (string == NULL || string[0] == '\t' || string[0] == '\n')
    {
        return;
    }
    if (i == 0)
    {
        // if string' former <字母> or _{<字母>|<数字> }*
        // 如果 str 为 字母 | _ | 数字 则 将 字符串保存在 map 中国
        if (isKeyword(string) == 1)
        {
            Map *newMap = createMap(string, code_keyword("keyword"));
            // 将newMap添加到HashMap中
            return addMap(pMap, newMap);
        }
        else if (isType(string) == 1)
        {
            Map *newMap = createMap(string, code_keyword("type"));
            // 将newMap添加到HashMap中
            return addMap(pMap, newMap);
        }
        else if (string[0] >= 'a' && string[0] <= 'z' || string[0] >= 'A' && string[0] <= 'Z' || string[0] == '_')
        {

            //添加到符号表
            if (isExist(symbolList,string)==0){
                insertSymbol(symbolList, string);
            }
             char* id = findSymbolId(symbolList,string);
            // 创建一个新的map，键为str，值为2
            Map *newMap = createMap(string, code_keyword("identifier"));
            // 将newMap添加到HashMap中
            addMap(pMap, newMap);
        }
        // if string former's  <数字>{<数字>}*,这里要判断里面是不是包含有非法字符
        else if (string[0] >= '0' && string[0] <= '9')
        {
            Map *newMap = createMap(string, code_keyword("number"));
            // 将newMap添加到HashMap中
            return addMap(pMap, newMap);
        }
    }
    else
    {
        Map *newMap = createMap(string, code_keyword("string"));
        // 将newMap添加到HashMap中
        return addMap(pMap, newMap);
    }
}
/***
 * clean the surplus space
 * 去除多余的空格
 */
void RemoveExtraSpace(char *str)
{
    int keep_space = 0; //表示是否允许保存空格，初始为false用于去除字符串刚开始的多余空格
    int new_str_end = 0;
    for (int i = 0; str[i]; ++i)
    {
        if (str[i] != ' ' ) //如果遍历到的是非空格字符，则进行赋值
        {
            str[new_str_end++] = str[i];
            keep_space = 1; //表示允许保存一个空格
        }
        else if (keep_space == 1) //如果允许有一个空格
        {
            str[new_str_end++] = str[i];
            keep_space = 0;
        }
    }
    //处理字符串最后的多余空格，经过上述处理，字符串最多是在index-1位置多出一个空格。
    if (new_str_end > 0 && str[new_str_end - 1] == ' ')
    {
        str[new_str_end - 1] = '\0';
    }
    else
    {
        str[new_str_end] = '\0';
    }
}

/***
 * 分析字符串,
 * @param str 字符串
 * @param new HashMap
 */
void analysis(char *str, HashMap *new)
{
    // 如果 str 为空、制表符、换行符 则 跳过
    if (str == NULL || str[0] == '\t' || str[0] == '\n')
    {
        return;
    }
    // 清除str里面多余的空格，只保留一个空格
    RemoveExtraSpace(str);
    int len = strlen(str); // 获取字符串的长度
    int pre = 0;           // 子字符串的下界
    int is_string = 0;     // 判断是否是字符串
    int flag = 0;          // 判断是否在查找运算符或连接符中，0表示没有，1表示在查找
    for (int i = 0; i <= len; i++)
    {
        if (is_string == 0)
        {
            // 如果 str[i] 是 字母、下划线
            if (str[i] >= 'a' && str[i] <= 'z' || str[i] >= 'A' && str[i] <= 'Z' || str[i] == '_')
            {
                // 如果已经找到运算符或连接符，则开始新一轮的查找
                if (flag == 0)
                {
                    pre = i;  // 子字符串的下界，即第一个字符的下标
                    flag = 1; // 标记已经找到运算符或连接符
                }
            }
            else if (str[i] == ' ')
            {
                char *substr = sub(str, pre, i);
                if (strlen(substr) > 0)
                {
                    checkString(substr, new, is_string);
                    pre = i + 1;
                    flag = 0;
                }
            }
            else if (str[i] >= '0' && str[i] <= '9')
            {
                // 如果已经找到运算符或连接符，则开始新一轮的查找
                if (flag == 0)
                {
                    pre = i;
                    flag = 1;
                }
            }
            else if (str[i] == '(' || str[i] == ')' || str[i] == '{' || str[i] == '}' || str[i] == ';' || str[i] == ',' ||
                     (str[i] == '=' && is_alphabet(str[i + 1]) == 1))
            {
                char *substr = sub(str, pre, i); // 子字符串，下界为pre，上界为i
                // print substr;
                checkString(substr, new, is_string); // 检查子字符串，是关键字、保留字还是标识符
                pre = i + 1;
                flag = 0; // 标记已经找过运算符或连接符
                // print str[i]
                // 将 str[i] 转换为char*
                char *c = (char *)malloc(sizeof(char));
                c[0] = str[i];
                c[1] = '\0';
                // 创建一个新的map，并将其保存在HashMap中，key为symbolstr，value为5
                Map *newMap = createMap(c, code_keyword("separator"));
                // 将newMap添加到HashMap中
                addMap(new, newMap);
            }
            else if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || (str[i] == '>' && str[i + 1] != '=') || (str[i] == '<' && str[i + 1] != '=') || (str[i] == '!' && str[i + 1] != '=') || (str[i] == ':' && str[i + 1] != '=') || (str[i] == '&' && str[i + 1] != '='))
            {
                char *substr = sub(str, pre, i);     // 子字符串，下界为pre，上界为i
                checkString(substr, new, is_string); // 检查子字符串，是关键字、保留字还是标识符
                pre = i + 1;
                flag = 0; // 标记已经找过运算符或连接符

                char *symbolstr = (char *)malloc(sizeof(char) * 2);
                sprintf(symbolstr, "%c", str[i]);
                // 创建一个新的map，并将其保存在HashMap中，key为symbolstr，value为4
                Map *newMap = createMap(symbolstr, code_keyword("operator"));
                // 将newMap添加到HashMap中
                addMap(new, newMap);
            }
            else if (str[i + 1] == '=' && is_alphabet(str[i]) == 0)
            {
                char *substr = sub(str, pre, i);
                checkString(substr, new, is_string); // 检查子字符串，是关键字、保留字还是标识符
                pre = i + 2;
                flag = 0;
                char *symbolstr = (char *)malloc(sizeof(char) * 2);
                sprintf(symbolstr, "%c=", str[i]);
                // 创建一个新的map，并将其保存在HashMap中，key为":="，value为4
                Map *newMap = createMap(symbolstr, code_keyword("operator"));
                // 将newMap添加到HashMap中
                addMap(new, newMap);
                i++;
            }
            else if (str[i] == '"')
            { // 如果 str[i] 是 ",则将is_string设置为1，表示开始处理字符串
                // add " to map，
                char *c = (char *)malloc(sizeof(char));
                c[0] = str[i];
                c[1] = '\0';
                addMap(new, createMap(c, code_keyword("separator")));
                is_string = 1;
                pre = i + 1;
            }
        }
        else if (is_string == 1)
        {
            if (str[i] == '"')
            {
                // 如果 str[i] 是 "，且 is_string 为1，则将is_string设置为0，表示结束处理字符串
                char *substr = sub(str, pre, i);
                checkString(substr, new, is_string);
                Map *newMap = createMap("\"", code_keyword("separator"));
                // 将newMap添加到HashMap中
                addMap(new, newMap);
                is_string = 0;
                pre = i + 1;
            }
        }
        if (i == len)
        {
            if (is_string == 1)
            {
                printf("error 8001: The Token \" must be closed.\n");
                exit(8001);
            }
            char *substr = sub(str, pre, i);
            checkString(substr, new, 0);
            pre = i + 1;
            flag = 0;
        }
    }
}

/***
 * 获取字符串的子串
 * @param str 字符串
 * @param start 下标
 * @param end 上标
 * @return 获得子串[start,end)
 */
char *sub(char *str, int start, int end)
{
    char *substr = (char *)malloc(sizeof(char) * 20);
    int i = 0;
    int j = 0;
    while (start < end)
    {
        substr[i] = str[start];
        i++;
        start++;
    }
    substr[i] = '\0';
    return substr;
}