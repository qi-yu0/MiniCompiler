#include "tree.h"
#include <unistd.h>
int write_in(TREE * root,char * des_file_name);
void pre_order_traverse(TREE * T,FILE * des_stream);
int prog(TREE * node);

int main()
{
    char source_file[100] = "D:\\CoderLife\\src_c\\Compilation_principle\\chapter_three\\sourceProgram1.txt";
    char des_file[100] = "D:\\CoderLife\\src_c\\Compilation_principle\\chapter_three\\parse_out5.txt";
    TREE * node = (TREE *)malloc(sizeof(TREE));
    TREE * root = node;
    lex(source_file,Buffer);//词法分析，将分析后的词素写入结构数组Buffer
    prog(node);  //进行语法分析
    write_in(root,des_file); //将语法分析的结果写入目标文件
    return 0;
}

int write_in(TREE * root,char * des_file_name)
{
    FILE * des_stream = fopen(des_file_name,"w");
    if(des_stream == NULL)
    {
        perror("file open error!");
        exit(0);
    }
    TREE * T = root;
    pre_order_traverse(T,des_stream);//将抽象语法树以前缀遍历的方式写进txt文件
    if(fclose(des_stream)!= 0)
    {
        perror("fclose");
        exit(0);
    }
    return 0;
}

void pre_order_traverse(TREE * T,FILE * des_stream) {
	if (T) {	
        sleep(0.1);
        fprintf(des_stream,"%s\n",T->value);
        //printf("%s\n",T->value);
		pre_order_traverse(T->left, des_stream);  
		pre_order_traverse(T->right, des_stream);
        pre_order_traverse(T->third, des_stream);
        
	}
}

int prog(TREE * node){

    p = 0;
    TREE * left;
    TREE * right;
    
    if(Buffer[p++].token == 31)
    {
       
        left = decls();
        right = stmts();
        node->left = left;
        node->right = right;
        node->third = NULL;

        node->token = 38;
        strcpy(node->value,"prog");
        
        if(Buffer[p].token = 32)
        {
            printf("match success!\n");
        }
    }
    else
    {
        free(node);
        printf("prog match failure!\n");
        return 0;
    }

}