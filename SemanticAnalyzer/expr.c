#include "tree.h"

TREE * mul();
TREE * add();
TREE * factor();
TREE * nega();
TREE * term();
TREE * expr()
{
    TREE *l,*r,*f,*root;
    if((l=term())!=NULL)
    {
        save = p;
        while((f=add())!=NULL && (r=term())!=NULL)
        {
            f->left  = l;
            f->right = r;
            f->third = NULL;
            l = f;
            save = p;
        }
        p = save;
    }
    root = l;
    return root;
}

TREE * term()
{
    printf("invoking term!\n");
    TREE *l,*r,*f,*root;
    if((l=nega())!=NULL)
    {
        while(1)
        {
            save = p;
            if((f=mul())!=NULL && (r=nega())!=NULL)
            {
                f->left = l;
                f->right = r;
                f->third = NULL;
                l = f;
            }
            else
            {
                p = save;
                break;
            }
        }
    }
    root = l;
    return root;
}
TREE * nega()
{
    return factor();
}
TREE * factor()
{
    TREE * node,*root;
    int t = Buffer[p].token;
    if(t == 29) //29对应着'('
    {
        p++;
        node = expr();
        if(Buffer[p++].token != 30) //30对应着')'
        return NULL;
    }
    else if(t == 1)//1对应着id
    {  
        node = (TREE *)malloc(sizeof(TREE));
        node->left = NULL;
        node->right = NULL;
        node->third = NULL;

        node->token = Buffer[p].token;
        strcpy(node->value,Buffer[p].morpheme);
        p++;
    }
    else if(t == 2)//2对应着int值
    {
        node = (TREE *)malloc(sizeof(TREE));
        node->left = NULL;
        node->right = NULL;
        node->third = NULL;

        node->token = Buffer[p].token;
        strcpy(node->value,Buffer[p].morpheme);
        p++;
    }
    else
    {
        return NULL;
    }
    root = node;
    return root;
}
TREE * add()
{
    
    TREE * root;
    TREE * node = (TREE *)malloc(sizeof(TREE));
    node->left = NULL;
    node->right = NULL;
    node->third = NULL;

    if(Buffer[p].token == 14) //'+'
    {
        ++p;
        node->token = 14;
        strcpy(node->value,"+");
    }
    else if(Buffer[p].token == 15)//'-'
    {
        ++p;
        node->token = 15;
        strcpy(node->value,"-");
    }
    else
    {
        free(node);
        perror("add match failure!");
        return NULL;
    }
    root = node;
    return root;
}
TREE * mul()
{
    
    TREE * root;
    TREE * node = (TREE *)malloc(sizeof(TREE));
    node->left = NULL;
    node->right = NULL;
    node->third = NULL;
    
    if(Buffer[p].token == 16)
    {
        ++p;
        node->token = 16;
        strcpy(node->value,"*");
    }
    else if(Buffer[p].token == 17)
    {
        ++p;
        node->token = 17;
        strcpy(node->value,"/");
    }
    else
    {
        free(node);
        perror("mul match failure!");
        return NULL;
    }
    root = node;
    return root;
}