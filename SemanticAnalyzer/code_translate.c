#include "tree.h"

int pc = -1; // 指令的地址，第i行的地址为i;

typedef struct symbol_address
{
    char symbol[20];
    int addr;              // 指令地址
} SYMBOL_ADDR;             // 符号与地址关联
SYMBOL_ADDR sa_table[100]; // 地址关联表

typedef struct instruction
{
    char operater[10]; // 操作符
    char src1[20];
    char src2[20];
    char result[10];
} INST;
FILE *wf;
int t = 0;   // 标识符的下标
void ct_stmts(TREE *node);
void ct_stmt(TREE *node);
void ct_expr(TREE *node);
void assign(TREE *node, int token);
void ct_if(TREE *node);
void ct_block(TREE *node);
void ct_while(TREE *node);
void ct_read(TREE *node);
void ct_write(TREE *node);
void printf_(INST *inst);
void printf_to(INST *inst, int row);

int code_translate(TREE *root)
{
    wf = fopen("D:\\CoderLife\\src_c\\Compilation_principle\\chapter_four\\result2.txt", "w");
    ct_stmts(root->right);
    fclose(wf);
    return 0;
}

void ct_stmts(TREE *node)
{
    TREE *left = node->left;
    TREE *right = node->right;
    if (left->token == 40) // node->value == “stmts”
    {
        ct_stmts(left);
    }
    else
        ct_stmt(left);

    ct_stmt(right);
}
void ct_stmt(TREE *node)
{
    if (node == NULL)
        return;
    int token = node->token;
    switch (token)
    {
    case 27: //:=
        assign(node, 0);
        break;
    case 28: //=
        assign(node, 1);
        break;
    case 42: // if_then
        ct_if(node);
        break;
    case 43: // if_then_else
        ct_if(node);
        break;
    case 44:
        ct_while(node); // while
        break;
    case 45:
        ct_block(node); // block
        break;
    case 12: // read
        ct_read(node);
        break;
    case 13: // write
        ct_write(node);
        break;
    default:
        break;
    }
}
void assign(TREE *node, int token) // 0 means :=,1 means =
{
    TREE *left = node->left;
    TREE *right = node->right;

    INST inst1;
    if (token == 0)
        strcpy(inst1.operater, ":=");
    else if(token == 1)
        strcpy(inst1.operater, "=");
    else
        perror("input error!\n");
    strcpy(inst1.src1, left->value);

    if (right->token == 2 || right->token == 1) // 为int值或int型变量
        strcpy(inst1.src2, right->value);
    else
    {
        ct_expr(right);
        char src1[20] = {'t', '0' + t-1};
        strcpy(inst1.src2, src1);
    }
    strcpy(inst1.result, "_");
    printf_(&inst1);
}

void ct_expr(TREE * node) // 布尔表达式，算术表达式
{
    INST inst;
    TREE *left = node->left;
    TREE *right = node->right;
    strcpy(inst.operater, node->value);
    if (left->token >= 14 && left->token <= 26) // 操作符
    {
        ct_expr(left);

        char src1[20] = {'t', '0' + t-1};
        strcpy(inst.src1, src1);
    }
    else // ID和int值和布尔值
        strcpy(inst.src1, left->value);

    if (right->token >= 14 && right->token <= 26)
    {
        ct_expr(right);
        char src2[20] = {'t', '0' + t-1};
        strcpy(inst.src2, src2);
    }
    else
        strcpy(inst.src2, right->value);

    char result[20] = {'t', '0' + t++};
    strcpy(inst.result, result);

    printf_(&inst);
}

void ct_if(TREE *node)
{
    pc++;
    int if_pc = pc;
    INST inst;
    strcpy(inst.operater, "je");
    strcpy(inst.src1, node->left->value);
    strcpy(inst.src2, "0");

    fprintf(wf,"\n");
    ct_stmt(node->right);

    char ch[10];
    strcpy(inst.result, itoa(pc+1, ch, 10));
    if (strcpy(node->value, "if_then_else"))
        ct_stmt(node->third);

    printf_to(&inst, if_pc);
}
void ct_while(TREE *node)
{
    pc++;
    int while_pc = pc;
    INST inst;
    TREE *left = node->left;
    TREE *right = node->right;
    strcpy(inst.operater, "je");
    strcpy(inst.src1, left->value);
    strcpy(inst.src2, "0");

    fprintf(wf,"\n");
    ct_stmt(right);

    char ch[10];
    strcpy(inst.result, itoa(pc + 1, ch, 10));

    printf_to(&inst,while_pc);
}
void ct_block(TREE *node)
{
    ct_stmts(node->left);
    ct_stmt(node->right);
}

void ct_read(TREE *node)
{
    INST inst;
    strcpy(inst.operater, "out");
    strcpy(inst.src1, node->left->value);
    strcpy(inst.src2, "_");
    strcpy(inst.result, "_");
    printf_(&inst);
}
void ct_write(TREE *node)
{
    INST inst;
    strcpy(inst.operater, "in");
    strcpy(inst.src1, node->left->value);
    strcpy(inst.src2, "_");
    strcpy(inst.result, "_");
    printf_(&inst);
}
void printf_(INST *inst)
{
    pc++;
    if(fseek(wf,44*pc,SEEK_SET))
    return;
    
    if (inst != NULL)
        fprintf(wf, "%3d (%3s %10s %10s %10s)\n", pc, inst->operater, inst->src1, inst->src2, inst->result);
    fseek(wf, 0, SEEK_END);
}
void printf_to(INST *inst, int row)
{
    if(fseek(wf,44*row,SEEK_SET))
    return;
    
    if (inst != NULL)
        fprintf(wf, "%3d (%3s %10s %10s %10s)\n", row, inst->operater, inst->src1, inst->src2, inst->result);
    fseek(wf, 0, SEEK_END);
}
