#include "tree.h"
#include <unistd.h>

TREE * decl();
TREE * names();
TREE * name();

TREE * decls(){
    TREE * l,*r,*f,*root;
    save = p;
    if((l = decl())!=NULL)              //left node
    {
        while(1)
        {
            save = p;
            if((r = decl()) != NULL)  // right node exist!
            {
                TREE * fnode = (TREE *)malloc(sizeof(TREE)); //father node
                f = fnode;

                f->left = l;
                f->right = r;
                f->third = NULL;
                f->token = 39;
                strcpy(f->value,"decls");
                l = f;
            }
            else    //right node does not exist!
            {
                p = save;
                break;
            }
        }
    }
    else  //left node does not exist!
    {
        p = save;
        root = NULL;
    }
    root = l;
    return root;
}
TREE * decl(){
    save = p;

    TREE * node = (TREE *)malloc(sizeof(TREE));
    int t = Buffer[p++].token;
    
    if(t == 3)
    {
        node->left = names();
        if(node->left!=NULL && Buffer[p++].token == 34)
        {
            node->right = NULL;
            node->third = NULL;
            node->token = 3;
            strcpy(node->value,"int");
        }
        else
        {
            p = save;
            return NULL;
        }
    }
    else if(t == 4)
    {
        node->left = names();
        if(node->left!=NULL && Buffer[p++].token == 34)
        {
            node->right = NULL;
            node->third = NULL;

            node->token = 4;
            strcpy(node->value,"bool");
        }
        else
        {
            p = save;
            return NULL;
        }
    }
    else
    {
        free(node);
        return NULL;
    }
    return node;
}
TREE * names()
{
    TREE *l,*r,*f,*root;
    
    if((l=name())!=NULL)
    {
        while(1)
        {
            if(Buffer[p++].token != 33)
            {
                p--;
                break;
            }
            save = p;
            if((r=name())!=NULL)
            {
                TREE * fnode = (TREE *)malloc(sizeof(TREE));
                f = fnode;

                f->left = l;
                f->right = r;
                f->third = NULL;
                f->token = 41;
                strcpy(f->value,"names");
                
                l = f;
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
        perror("names match failure!");
        return NULL;
    }
    root = l;
    return root;
}
TREE * name(){
        TREE * node;
        if(Buffer[p].token == 1)
        {
            node = (TREE *)malloc(sizeof(TREE));
            node->left = NULL;
            node->right = NULL;
            node->third = NULL;

            node->token = Buffer[p].token;
            strcpy(node->value,Buffer[p++].morpheme);
            
            return node;
        }
        else
        return NULL;   
}

