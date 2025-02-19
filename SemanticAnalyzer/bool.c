#include "tree.h"
// int main()
// {
//     char file_path[50] = "";
//     FILE file_stream = fopen(file_path,"r");
    
//     TREE * node = (TREE *)malloc(sizeof(TREE));
//     TREE *root = node;
//     bool_(node);
// }
TREE * join();
TREE * not();
TREE * rel();
int rop(TREE * node);

TREE * join(){
    TREE *l,*r,*f,*root;
    if((l=not())!=NULL)
    {
        while(1)
        {
            save = p;
            if((Buffer[p++].token == 19) && (r=not())!=NULL)
            {
                TREE * fnode = (TREE *)malloc(sizeof(TREE));
                f = fnode;

                f->left  = l;
                f->right = r;
                f->third = NULL;

                f->token = 19;
                strcpy(f->value,"&&");
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
        perror("bool match failure!");
    }
    root = l;
    return root;
}
TREE * not(){

    TREE * r,*root;
    if(Buffer[p].token == 20)
    {
        p++;
        TREE * node = (TREE *)malloc(sizeof(TREE));
        if((r = rel())!=NULL)
        {
           node->left = r;
           node->right = NULL;
           node->third = NULL;
           strcpy(node->value,"!");
           root = r;
        }
        else
        {
            perror("not match failure!");
            return NULL;
        }
    }
    else
    {
        if((root = rel()) == NULL)
        return NULL;
    }
    return root;
}
TREE * rel(){

        TREE * l,*r;
        TREE * root = (TREE *)malloc(sizeof(TREE)); 
        if((l = expr())!=NULL && rop(root) && (r=expr())!=NULL)
        {
            root->left = l;
            root->right = r;
            root->third = NULL;
        }
        else
        {
            perror("rel match failure!");
            return NULL;
        }


}
int rop(TREE * node){
    int i = Buffer[p++].token;
    node->token = i;
    switch(i){
        case 22:
        strcpy(node->value,">");
        break;
        case 21:
        strcpy(node->value,"<");
        break;
        case 23:
        strcpy(node->value,"<=");
        break;
        case 24:
        strcpy(node->value,">=");
        break;
        case 25:
        strcpy(node->value,"==");
        break;
        case 26:
        strcpy(node->value,"!=");
        break;
        default:
        {
            free(node);
            perror("rop match failure!");
            return 0;
        }

    }
    node->left = NULL;
    node->right = NULL;
    node->third = NULL;
    return 1;

}

TREE * bool_()
{
    TREE *l,*r,*f,*root;
    if((l=join())!=NULL)
    {
        while(1)
        {
            save = p;
            if((Buffer[p++].token == 18) && (r=join())!=NULL)
            {
                TREE * fnode = (TREE *)malloc(sizeof(TREE));
                f = fnode;

                f->left  = l;
                f->right = r;
                f->third = NULL;

                f->token = 18;
                strcpy(f->value,"||");
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
        perror("bool match failure!");
    }
    root = l;
    return root;
}