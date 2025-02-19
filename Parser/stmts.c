#include "tree.h"
// int main()
// {
//     char file_path[50] = "";
//     FILE * file_stream = fopen(file_path,"r");
//     TREE * node = (TREE *)malloc(sizeof(TREE));
//     TREE * root = node;
//     stmts(node);
//     return 0;
// }
TREE *stmt();

TREE *stmts()
{
    printf("invoking stmts!\n");
    TREE *left_child, *right_child, *father, *root; // left_child,right_child,father

    father = NULL;
    save = p;
    if ((left_child = stmt()) != NULL)
    {
        while (1)
        {
            save = p;
            if ((right_child = stmt()) != NULL)
            {
                TREE *fnode = (TREE *)malloc(sizeof(TREE));
                father = fnode;

                father->left = left_child;
                father->right = right_child;
                father->third = NULL;

                father->token = 40;
                strcpy(father->value, "stmts");

                left_child = father;
            }
            else
            {
                p = save;
                break;
            }
        }
    }
    else
    {
        p = save;
        perror("stmts match failure!");
        root = NULL;
    }
    root = left_child;
    return root;
}

TREE * stmt()
{
    TREE * root;
    TREE * node;
    printf("invoking stmt!\n");
    printf("before switch p = %d\n",p);
    int i = Buffer[p++].token;
    switch (i)
    {
    case 1: // id
    {
        TREE *l,*r;
        l = (TREE *)malloc(sizeof(TREE));
        l->left = NULL;
        l->right = NULL;
        l->third = NULL;
        i = Buffer[p++].token;
        if (i == 27)
        {
            l->token = Buffer[p-2].token;
            strcpy(l->value,Buffer[p-2].morpheme);

            save = p;
            r = bool_();
            if(r!=NULL)
            {
                node = (TREE *)malloc(sizeof(TREE));
                node->left = l;
                node->right = r;
                node->third = NULL;
                node->token = 27;
                strcpy(node->value,":=");
            }
            else
            {
                p = save;
                return NULL;
            }
        }    
        else if (i == 28)
        {
            l->token = Buffer[p-2].token;
            strcpy(l->value,Buffer[p-2].morpheme);
            
            save = p;
            r = expr();
            if(r!=NULL)
            {
                node = (TREE *)malloc(sizeof(TREE));
                node->left = l;
                node->right = r;
                node->third = NULL;
                node->token = 28;
                strcpy(node->value,"=");
            }
            else
            {
                p = save;
                return NULL;
            }
        }   
        else
        {
            perror("stmt id= expr match failure!");
            free(l);
            return NULL;
        }
        if(Buffer[p++].token != 34)
        {
            perror("stmt_id : lack of character ';'\n");
            return NULL;
        }
        else
        break;
    }
    case 9: // if
    {

        if (Buffer[p++].token == 1 && Buffer[p++].token == 11)
        {
            int i;
            TREE *right;
            TREE *left = (TREE *)malloc(sizeof(TREE));

            left->left = NULL;
            left->right = NULL;
            left->third = NULL;

            left->token = Buffer[p-2].token;
            strcpy(left->value, Buffer[p - 2].morpheme);

            if ((right = stmt()) != NULL)
            {
                node = (TREE *)malloc(sizeof(TREE));
                node->left = left;
                node->right = right;
                

                if (Buffer[p++].token == 10) 
                {
                    //p++;
                    TREE *third;
                    third = stmt();

                    node->third = third;

                    node->token = 43;
                    strcpy(node->value, "if_then_else");
                }
                else
                {
                    p--;
                    node->third = NULL;

                    node->token = 42;
                    strcpy(node->value, "if_then");
                }
            }
        }
        else
        {
            perror("stmt IF match failure!");
            return NULL;
        }
       
        break;
    }
    case 7: // while
    {
        TREE *left,*right;
        if(Buffer[p++].token == 1 && Buffer[p++].token == 8)
        {
            left = (TREE *)malloc(sizeof(TREE));
            left->left = NULL;
            left->right = NULL;
            left->third = NULL;

            left->token = Buffer[p-2].token;
            strcpy(left->value,Buffer[p-2].morpheme);
            
            if((right = stmt())!=NULL)
            {
                node = (TREE *)malloc(sizeof(TREE));

                node->token = 44;
                strcpy(node->value,"while_do");
                node->left = left;
                node->right = right;
                node->third = NULL;
            }
            else
            {
                free(left);
                return NULL;
            }
        }
        else
        {
            printf("while match failure!\n");
            return NULL;
        }
        break;
    }
    case 31: //{
    {
        save = p;
        if((node = stmts())!=NULL)
        {
            node->token = 45;
            strcpy(node->value, "block_stmt");
            node->third = NULL;
        }
        else
        {
            p = save;
            return NULL;
        }
        if(Buffer[p++].token != 32)
        {
            free(node);
            perror("stmt_while : lack of character '}'\n");
            return NULL;
        }
        break;
    }
    case 12: // write
    {
        if (Buffer[p].token == 1 && Buffer[p+1].token == 34)
        {
           
            TREE *left = (TREE *)malloc(sizeof(TREE));
            left->left = NULL;
            left->right = NULL;
            left->third = NULL;

            node = (TREE *)malloc(sizeof(TREE));
            
            node->left = left;
            node->right = NULL;
            node->third = NULL;

            left->token = Buffer[p].token;
            node->token = 12;
            strcpy(left->value, Buffer[p].morpheme);
            strcpy(node->value, "write");
            p = p+2;
        }
        else
        {
            perror("stmt_write : match failure!");
            return NULL;
        }
        break;
    }
    case 13: // read
    {
        if (Buffer[p].token == 1 && Buffer[p+1].token == 34)
        {
            
            TREE *left = (TREE *)malloc(sizeof(TREE));
            left->left = NULL;
            left->right = NULL;
            left->third = NULL;

            node = (TREE *)malloc(sizeof(TREE));
            node->left = left;
            node->right = NULL;
            node->third = NULL;

            left->token = Buffer[p].token;
            strcpy(left->value, Buffer[p].morpheme);
            node->token = 13;
            strcpy(node->value, "read");
            p = p+2;
        }
        else
        {
            perror("stmt_read : match failure!");
            return NULL;
        }
        break;
    }
    default:
    {
        p--;
        printf("stmt_default : match failure! p =%d\n",p);
        return NULL;
    }
    }
    root = node;
    return root;
}