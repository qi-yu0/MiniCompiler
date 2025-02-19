#include "tree.h"

#define MAXTABLESIZE 20
int decls_(TREE *node);

int stmts_(TREE *node);
int names_(TREE *node);
int match(char *str);
int _bool_(TREE *node);
int expr_(TREE *node);
int sub1_expr(TREE *l);

SYMBOL table[MAXTABLESIZE];
int t_p = 0;
int is_error = 0;
int s_s_c(TREE *node) // static semantic checking.
{
    int is_null = 0;
    TREE *l, *r;
    if (node != NULL)
    {
        l = node->left;
        r = node->right;
        if (l != NULL)
            decls_(l);
        else
            is_null = 1;

        if (r != NULL)
            stmts_(r);
        else
            is_null = 1;
    }
    else
        is_null = 0;

    if (is_error)
        perror("errors in static semantic checking\n");
    else
        printf("success in static semantic checking\n");
    return 0;
}
int decls_(TREE *node)
{
    TREE *l, *r, *cur_r;
    l = node->left;
    cur_r = r = node->right;

    int t = node->token;
    if (t == 39 && l != NULL) // decls
    {
        decls_(l);
    }
    else if (t == 3) // int
    {
        node->inh = 1; // 继承属性为“int”，所以node->int 的值为1
        l->inh = 1;
        if (r != NULL)
            r->inh = 1;

        names_(l);
    }
    else if (t == 4) // bool
    {
        node->inh = 0;
        l->inh = 0;
        if (r != NULL)
            r->inh = 0;
        names_(l);
    }
    else
    {
        perror("decls errors!\n");
    }
    if (r != NULL)
    {
        if (r->token == 3)
        {
            r->inh = 1;
            l = r->left;
            l->inh = 1;
        }
        else if (r->token == 4)
        {
            r->inh = 0;
            l = r->left;
            l->inh = 0;
        }
        else
        {
            perror("decls error!\n");
        }
        names_(l);
    }
}
int names_(TREE *node)
{
    TREE *l, *r;
    if (node != NULL)
    {
        l = node->left;
        r = node->right;
    }
    if (node->token == 41) // 41 names
    {
        if (l != NULL)
        {
            l->inh = node->inh;
            r->inh = node->inh;
        }
        names_(l);
    }
    else // node->value is id;
    {
        strcpy(table[t_p].id, node->value); // 写入符号表
        table[t_p].type = node->inh;
        table[t_p].is_valued = 0;
        t_p++;
    }

    if (r != NULL && r->token == 1)
    {
        strcpy(table[t_p].id, r->value);
        table[t_p].type = r->inh;
        table[t_p].is_valued = 0;
        t_p++;
    }
}
int stmts_(TREE *node)
{
    if (node == NULL)
        return 0;
    TREE *l, *r, *current_r;
    // TREE *tl, *tr; // temporary l,r;
    if (node->left != NULL)
        l = node->left;
    if (node->right != NULL)
        r = node->right;

    current_r = r;
    int p = node->token;
    switch (p)
    {

    case 40: // stmts
    {
        stmts_(l);
        stmts_(r);
        break;
    }
    case 42: // if_then
    {

        SYMBOL T = table[match(l->value)];
        if (T.type == 0 && T.is_valued == 1)
        {
            stmts_(r);
        }
        else
        {
            is_error = 1;
            perror("if_then NOT boolean constant!\n");
        }
        break;
    }
    case 43: // if_then_else
    {
        TREE *third = node->third;
        SYMBOL T = table[match(l->value)];
        if (T.type == 0 && T.is_valued == 1)
        {
            stmts_(r);
            stmts_(third);
        }
        else
        {
            is_error = 1;
            perror("if_then NOT boolean constant!\n");
        }
        break;
    }
    case 44: // while_do
    {
        SYMBOL T = table[match(l->value)];
        if (T.type == 0 && T.is_valued == 1)
        {
            stmts_(r);
        }
        else
        {
            is_error = 1;
            perror("while_do NOT boolean constant!\n");
        }
        break;
    }
    case 45: // block_stmt
    {
        stmts_(l);
        stmts_(r);
        break;
    }
    case 27: //:=
    {
        _bool_(r);

        int l_ = match(l->value);
        if (table[l_].type != r->sys)
        {
            is_error = 1;
            perror("stmt error:expr type mismatch\n");
        }
        else
        {
            table[l_].is_valued = 1;
        }
        break;
    }
    case 28: // =
    {
        expr_(r);
        int l_ = match(l->value);
        if (table[l_].type != r->sys)
        {
            is_error = 1;
            perror("stmt error:expr type mismatch\n");
        }
        else
        {
            table[l_].is_valued = 1;
        }
        break;
    }
    case 12: // write
    {
        if (l != NULL)
        {
            int t = match(l->value);
            if (t >= 0 && l->token == 1) // id
                table[t].is_valued = 1;
            else
            {
                is_error = 1;
                perror("write error:type error!\n");
            }
        }
        break;
    }
    case 13: // read
    {
        if (l != NULL)
        {
            int t = match(l->value);
            if (t != -1 && l->token == 1) // id
                table[t].is_valued = 1;
            else
            {
                is_error = 1;
                perror("read error:type error!\n");
            }
        }
        break;
    }
    default:
    {
        perror("stmts error!\n");
    }
    }
    return 0;
}
int _bool_(TREE *node)
{
    if (node == NULL)
        return 0;
    TREE *l, *r;
    TREE *tl, *tr;

    if (node->left != NULL)
        l = node->left;
    if (node->right != NULL)
        r = node->right;

    if (node->token == 1) // id
    {
        if (table[match(node->value)].type == 1) // int型id
        {
            is_error = 1;
            perror("bool error:type mismatch!\n");
        }
        else
            node->sys = 0;
    }
    else if (node->token == 2) // int 值
    {
        node->sys = 1;
    }
    else if (node->token <= 26 && node->token >= 21)
    {
        expr_(l);
        expr_(r);
        if (l->sys != r->sys)
        {
            is_error = 1;
            perror("bool error: type mismatch!\n");
        }
        else
            node->sys = 0;
    }
    else if (node->token == 18 || node->token == 19) // ||,&&,!
    {
        _bool_(l);
        _bool_(r);
        if (r != NULL)
        {
            if (l->sys != r->sys)
            {
                is_error = 1;
                perror("bool error: type mismatch!\n");
            }
        }
        node->sys = 0;
    }
    else
    {
        perror("input illegal!\n");
    }
    return 0;
}
int expr_(TREE *node) // 表达式需要自下而上遍历，采用中序遍历
{
    if (node == NULL)
        return 0;
    TREE *l, *r;
    l = node->left;
    r = node->right;
    if (l == NULL && r == NULL)
    {
        if (node->token == 2) // int值
        {
            node->sys = 1;
        }
        else if (node->token == 1) // id
        {
            node->sys = table[match(node->value)].type;
            if (node->sys == 0) // bool
            {
                is_error = 1;
                perror("stmt error:return bool\n");
            }
        }
        else
            printf("error!\n");
    }
    else if (l != NULL && r != NULL)
    {
        expr_(l);
        expr_(r);
        if (l->sys != r->sys)
        {
            is_error = 1;
            perror("expr:type mismatch!\n");
        }
        else
            node->sys = l->sys;
    }
    else
        perror("input illegal!\n");
    return 0;
}
int sub1_expr(TREE *l)
{
    char ch = l->value[0];
    if ((ch == '+' || ch == '-' || ch == '*' || ch == '/') && l->value[1] == '\0')
    {
        l = l->left;
        expr_(l);
    }
    else if (l->token == 1) // l->value 为int值
    {
        l->sys = 1;
    }
    else if (l->token == 2) // l->value 为bool值
    {
        l->sys = 0;
    }
    else if (l->token = 3) // l->value 为identifier
    {
        l->sys = table[match(l->value)].type;
    }
    else
        perror("expr error!\n");

    return 0;
}

int match(char *str)
{
    int i = 0;
    while (i < MAXTABLESIZE)
    {
        if (strcmp(str, table[i].id) == 0)
            return i;
        else
            i++;
    }
    is_error = 1;
    printf("Variable %s not declared!\n", str);
    return -1;
}
