#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

#define MAXSIZE 50
#define MAXBUFFERSIZE 100

typedef struct Node {
	char morpheme[MAXSIZE];//词素
	int token;             //类别id
} TOKEN;      
typedef struct tree{
	struct tree * left;
	struct tree * right;
	struct tree * third;
	char value[20];
	int token;             //类别id
	int inh;           	   //继承属性 1 means "int", 0 means "bool"
	int sys;               //综合属性 1 means "int", 0 means "bool"
}TREE;
typedef struct node{
    char id[MAXSIZE];  //标识符
    int type;     	   //类型 1 means "int", 0 means "bool"
    int value;         //常数值
    int is_valued;     //valued or not,1 means values,0 means invalued.
}SYMBOL;//符号表的基本元素

int p,save; //p是token数组的下标,save为指针回溯后的值
TOKEN Buffer[MAXBUFFERSIZE],SubBuffer[MAXBUFFERSIZE]; //buffer存储读入内存的文件中的token数据,SubBuffer应对指针的回溯问题
//TOKEN * current;   //current指向当前的token,current 等于 Buffer[p];
int lex(char * source_file,TOKEN * buf);

TREE * stmts();
TREE * expr();
TREE * decls();
TREE * bool_();

int s_s_c(TREE * node);
int code_translate(TREE * root);

