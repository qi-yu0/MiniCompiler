#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXSIZE 50

typedef struct Node {
	char morpheme[MAXSIZE];//词素
	int token;             //类别id
} TOKEN;      
typedef struct tree{
	struct tree * left;
	struct tree * right;
	struct tree * third;
	char value[10];
	int token;             //类别id
	char inh[10];          //继承属性
	int sys;               //综合属性 1 means "int", 0 means "bool"
	
}TREE;
typedef struct node{
    char id[MAXSIZE];  //标识符
    char type[10];     //类型
    int value;         //值
    int is_valued;       //valid or not,1 means valid,0 means invalid.
}SYMBOL;


int p,save; //p是token数组的下标,save为指针回溯后的值
TOKEN Buffer[100],SubBuffer[100]; //buffer存储读入内存的文件中的token数据,SubBuffer应对指针的回溯问题
//TOKEN * current;   //current指向当前的token,current 等于 Buffer[p];
int lex(char * source_file,TOKEN * buf);

TREE * stmts();
TREE * expr();
TREE * decls();
TREE * bool_();

int s_s_c(TREE * node);


