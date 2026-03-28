#include "core.h"

Node* A1(Node* node) {
    if(node->Nodetype.type == TOK_AND ||
    node->Nodetype.type == TOK_OR)
    {
        Node* new = calloc(1, sizeof(Node));

        new->Nodetype.type = node->Nodetype.type;

        new->right = copy_tree(node->left);
        new->left = copy_tree(node->right);
        return new;
    }
    return NULL;
}

Node* A3(Node* node) {
    if(node->Nodetype.type == TOK_AND && node->right->Nodetype.type == TOK_OR) {
        Node* new = calloc(1, sizeof(Node));
        new->left = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_OR;
        new->left->Nodetype.type = new->right->Nodetype.type = TOK_AND;
        new->left->left = copy_tree(node->left);
        new->left->right = copy_tree(node->right->left);
        new->right->left = copy_tree(node->left);
        new->right->right = copy_tree(node->right->right);
        return new;
    }

    if(node->Nodetype.type == TOK_OR && node->right->Nodetype.type == TOK_AND) {
        Node* new = calloc(1, sizeof(Node));
        new->left = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_AND;
        new->left->Nodetype.type = new->right->Nodetype.type = TOK_OR;
        new->left->left = copy_tree(node->left);
        new->left->right = copy_tree(node->right->left);
        new->right->left = copy_tree(node->left);
        new->right->right = copy_tree(node->right->right);
        return new;
    }

    return NULL;
}

Node* A3_Back(Node* node){
    if(node->Nodetype.type == TOK_OR &&
       node->left->Nodetype.type == TOK_AND &&
       node->right->Nodetype.type == TOK_AND &&
      (identical_or_not(node->left->left, node->right->left)))
    {
        Node* new = calloc(1, sizeof(Node));

        new->Nodetype.type = TOK_AND;

        new->right = calloc(1, sizeof(Node));

        new->right->Nodetype.type = TOK_OR;
        new->left = copy_tree(node->left->left);
        new->right->left = copy_tree(node->left->right);
        new->right->right = copy_tree(node->right->right);
        return new;
    }
    if(node->Nodetype.type == TOK_AND &&
       node->left->Nodetype.type == TOK_OR &&
       node->right->Nodetype.type == TOK_OR &&
      (identical_or_not(node->left->left, node->right->left)))
    {
        Node* new = calloc(1, sizeof(Node));

        new->Nodetype.type = TOK_OR;

        new->right = calloc(1, sizeof(Node));

        new->right->Nodetype.type = TOK_AND;
        new->left = copy_tree(node->left->left);
        new->right->left = copy_tree(node->left->right);
        new->right->right = copy_tree(node->right->right);
        return new;   
    }

    return NULL;
}

Node* A4(Node* node) {
    if ((node->Nodetype.type == TOK_OR || node->Nodetype.type == TOK_AND) &&
        identical_or_not(node->left, node->right)) {
        return copy_tree(node->left);
    }

    return NULL;
}

Node* A4_Back_1(Node* node) {
    Node* new = calloc(1, sizeof(Node));
    new->Nodetype.type = TOK_AND;
    new->left = copy_tree(node);
    new->right = copy_tree(node);
    return new;
}

Node* A4_Back_2(Node* node) {
    Node* new = calloc(1, sizeof(Node));
    new->Nodetype.type = TOK_OR;
    new->left = copy_tree(node);
    new->right = copy_tree(node);
    return new;
}

Node* A5(Node* node){
    if(((node->Nodetype.type == TOK_AND && node->right->Nodetype.type == TOK_OR) ||
        (node->Nodetype.type == TOK_OR && node->right->Nodetype.type == TOK_AND)) &&
         identical_or_not(node->left, node->right->left))
    {
        return copy_tree(node->left);
    }

    return NULL;
}

Node* A6(Node* node) {
    if(node->Nodetype.type == TOK_AND &&
       node->right->Nodetype.type == TOK_TRUE)
    {
        return copy_tree(node->left);
    }
    if(node->Nodetype.type == TOK_OR &&
       node->right->Nodetype.type == TOK_FALSE)
    {
        return copy_tree(node->left);
    }
    
    return NULL;
}

Node* A6_Back_1(Node* node) {
    Node* new = calloc(1, sizeof(Node));
    new->right = calloc(1, sizeof(Node));
    new->Nodetype.type = TOK_AND;
    new->right->Nodetype.type = TOK_TRUE;
    new->left = copy_tree(node);
    return new;
}

Node* A6_Back_2(Node* node) {
    Node* new = calloc(1, sizeof(Node));
    new->right = calloc(1, sizeof(Node));
    new->Nodetype.type = TOK_OR;
    new->right->Nodetype.type = TOK_FALSE;
    new->left = copy_tree(node);
    return new;
}

Node* A7(Node* node){
    if(node->Nodetype.type == TOK_OR &&
       node->right->Nodetype.type == TOK_TRUE)
    {
        Node* new = calloc(1, sizeof(Node));

        new->Nodetype.type = TOK_TRUE;
        return new;
    }
    if(node->Nodetype.type == TOK_AND &&
       node->right->Nodetype.type == TOK_FALSE)
    {
        Node* new = calloc(1, sizeof(Node));

        new->Nodetype.type = TOK_FALSE;      
        return new;
    }
    
    return NULL;
}

Node* A8(Node* node) {
    if(node->Nodetype.type == TOK_AND  &&
        node->right->Nodetype.type == TOK_NOT &&
        identical_or_not(node->left, node->right->right))
    {
        Node* new = calloc(1, sizeof(Node));

        new->Nodetype.type = TOK_FALSE;
        return new;
    }
    if(node->Nodetype.type == TOK_OR &&
        node->right->Nodetype.type == TOK_NOT &&
        identical_or_not(node->left, node->right->right))
    {
        Node* new = calloc(1, sizeof(Node));

        new->Nodetype.type = TOK_TRUE;
        return new;
    }    

    return NULL;
}

Node* A9(Node* node){
    if(node->Nodetype.type == TOK_NOT &&
       node->right->Nodetype.type == TOK_NOT)
    {
        return copy_tree(node->right->right);
    }
    return NULL;
}

Node* A9_Back(Node* node) {
    Node* new = calloc(1, sizeof(Node));
    new->right = calloc(1, sizeof(Node));
    new->Nodetype.type = new->right->Nodetype.type = TOK_NOT;
    new->right->right = copy_tree(node);
    return new;
}

Node* A10(Node* node){
    if(node->Nodetype.type == TOK_NOT &&
       (node->right->Nodetype.type == TOK_AND || node->right->Nodetype.type == TOK_OR))
    {
        Node* new = calloc(1, sizeof(Node));

        if(node->right->Nodetype.type == TOK_AND) new->Nodetype.type = TOK_OR;
        if(node->right->Nodetype.type == TOK_OR) new->Nodetype.type = TOK_AND;        
        
        new->left = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));

        new->left->Nodetype.type = TOK_NOT;
        new->right->Nodetype.type = TOK_NOT;
        new->left->right = copy_tree(node->right->left);
        new->right->right = copy_tree(node->right->right);
        return new;
    }

    return NULL;
}

Node* A10_Backward(Node* node) {
    if((node->Nodetype.type == TOK_AND || node->Nodetype.type == TOK_OR) &&
        node->left->Nodetype.type == TOK_NOT &&
        node->right->Nodetype.type == TOK_NOT)
    {
        Node* new = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_NOT;
        if(node->Nodetype.type == TOK_OR) new->right->Nodetype.type = TOK_AND;
        if(node->Nodetype.type == TOK_AND) new->right->Nodetype.type = TOK_OR;
        new->right->left = copy_tree(node->left->right);
        new->right->right = copy_tree(node->right->right);
        return new;
    }

    return NULL;
}

Node* B1(Node* node) {
    if(node->Nodetype.type == TOK_XOR )
    {
        Node* new = calloc(1, sizeof(Node));

        new->Nodetype.type = node->Nodetype.type;

        new->right = copy_tree(node->left);
        new->left = copy_tree(node->right);
        return new;
    }
    return NULL;
}

Node* B3(Node* node) {
    if(node->Nodetype.type == TOK_AND && node->right->Nodetype.type == TOK_XOR) {
        Node* new = calloc(1, sizeof(Node));
        new->left = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_XOR;
        new->left->Nodetype.type = new->right->Nodetype.type = TOK_AND;
        new->left->left = copy_tree(node->left);
        new->left->right = copy_tree(node->right->left);
        new->right->left = copy_tree(node->left);
        new->right->right = copy_tree(node->right->right);
        return new;
    }

    return NULL;
}

Node* B3_Back(Node* node){
    if(node->Nodetype.type == TOK_XOR &&
       node->left->Nodetype.type == TOK_AND &&
       node->right->Nodetype.type == TOK_AND &&
      (identical_or_not(node->left->left, node->right->left)))
    {
        Node* new = calloc(1, sizeof(Node));

        new->Nodetype.type = TOK_AND;

        new->right = calloc(1, sizeof(Node));

        new->right->Nodetype.type = TOK_XOR;
        new->left = copy_tree(node->left->left);
        new->right->left = copy_tree(node->left->right);
        new->right->right = copy_tree(node->right->right);
        return new;
    } 

    return NULL;
}

Node* B4(Node* node) {
    if(node->Nodetype.type == TOK_XOR &&
        node->right->Nodetype.type == TOK_FALSE)
    {
        return copy_tree(node->left);
    }

    return NULL;
}

Node* B4_Back(Node* node) {
    Node* new = calloc(1, sizeof(Node));
    new->right = calloc(1, sizeof(Node));
    new->Nodetype.type = TOK_XOR;
    new->right->Nodetype.type = TOK_FALSE;
    new->left = copy_tree(node);
    return new;
}

Node* B5_Above(Node* node) {
    if(node->Nodetype.type == TOK_XOR &&
        node->right->Nodetype.type == TOK_TRUE)
    {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_NOT;
        new->right = copy_tree(node->left);
        return new;
    }

    return NULL;    
}

Node* B5_Above_Back(Node* node) {
    if(node->Nodetype.type == TOK_NOT) {
        Node* new = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_XOR;
        new->right->Nodetype.type = TOK_TRUE;
        new->left = copy_tree(node->right);
        return new;        
    }

    return NULL;
}

Node* B5_Below_1(Node* node) {
    if(node->Nodetype.type == TOK_NOT &&
        node->right->Nodetype.type == TOK_XOR)
    {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_XOR;
        new->left = calloc(1, sizeof(Node));
        new->left->Nodetype.type = TOK_NOT;
        new->left->right = copy_tree(node->right->left);
        new->right = copy_tree(node->right->right);
        return new;
    }

    return NULL;
}

Node* B5_Below_2(Node* node) {
    if(node->Nodetype.type == TOK_XOR &&
        node->left->Nodetype.type == TOK_NOT)
    {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_XOR;
        new->right = calloc(1, sizeof(Node));
        new->right->Nodetype.type = TOK_NOT;
        new->right->right = copy_tree(node->right);
        new->left = copy_tree(node->left->right);   
        return new;     
    }

    return NULL;
}

Node* B5_Below_Back_1(Node* node) {
    if(node->Nodetype.type == TOK_XOR &&
        node->left->Nodetype.type == TOK_NOT)
    {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_NOT;
        new->right = calloc(1, sizeof(Node));
        new->right->Nodetype.type = TOK_XOR;
        new->right->right = copy_tree(node->right);
        new->right->left = copy_tree(node->left->right); 
        return new;       
    }

    return NULL;
}

Node* B5_Below_Back_2(Node* node) {
    if(node->Nodetype.type == TOK_XOR &&
        node->right->Nodetype.type == TOK_NOT)
    {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_XOR;
        new->left = calloc(1, sizeof(Node));
        new->left->Nodetype.type = TOK_NOT;
        new->left->right = copy_tree(node->left);
        new->right = copy_tree(node->right->right); 
        return new;       
    }

    return NULL;  
}

Node* B6(Node* node) {
    if(node->Nodetype.type == TOK_XOR &&
       identical_or_not(node->left, node->right))
    {
        Node* new = calloc(1, sizeof(Node));

        new->Nodetype.type = TOK_FALSE;
        return new;
    }

    if(node->Nodetype.type == TOK_XOR &&
       node->right->Nodetype.type == TOK_NOT &&
       identical_or_not(node->left, node->right->right))
    {
        Node* new = calloc(1, sizeof(Node));

        new->Nodetype.type = TOK_TRUE;
        return new;
    }

    return NULL;
}

Node* B7(Node* node) {
    if (node->Nodetype.type == TOK_XOR &&
        node->right->Nodetype.type == TOK_XOR &&
        identical_or_not(node->left, node->right->left) &&
        identical_or_not(node->left, node->right->right)) {
        return copy_tree(node->left);
    }

    return NULL;    
}

Node* B7_Back(Node* node) {
    Node* new = calloc(1, sizeof(Node));
    new->right = calloc(1, sizeof(Node));
    new->Nodetype.type = new->right->Nodetype.type = TOK_XOR;
    new->left = copy_tree(node);
    new->right->left = copy_tree(node);
    new->right->right = copy_tree(node);
    return new;
}

Node* B8(Node* node) {
    if(node->Nodetype.type == TOK_XOR) {
        Node* new = calloc(1, sizeof(Node));
        new->left = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_XOR;
        new->left->Nodetype.type = new->right->Nodetype.type = TOK_NOT;
        new->left->right = copy_tree(node->left);
        new->right->right = copy_tree(node->right);
        return new;
    }

    return NULL;
}

Node* B8_Back(Node* node) {
    if(node->Nodetype.type == TOK_XOR &&
        node->left->Nodetype.type == TOK_NOT &&
        node->right->Nodetype.type == TOK_NOT) 
    {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_XOR;
        new->left = copy_tree(node->left->right);
        new->right = copy_tree(node->right->right);
        return new;
    }

    return NULL;
}

Node* B9(Node* node) {
    if(node->Nodetype.type == TOK_XOR) {
        Node* new = calloc(1, sizeof(Node));
        new->left = calloc(1, sizeof(Node));
        new->left->right = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->right->left = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_OR;
        new->left->Nodetype.type = new->right->Nodetype.type = TOK_AND;
        new->left->right->Nodetype.type = new->right->left->Nodetype.type = TOK_NOT;
        new->left->left = copy_tree(node->left);
        new->left->right->right = copy_tree(node->right);
        new->right->left->right = copy_tree(node->left);
        new->right->right = copy_tree(node->right);
        return new;
    }

    return NULL;
}

Node* B9_Back(Node* node) {
    if(node->Nodetype.type == TOK_OR &&
        node->left->Nodetype.type == TOK_AND &&
        node->right->Nodetype.type == TOK_AND)
        {
            if(node->left->right->Nodetype.type == TOK_NOT &&
                node->right->left->Nodetype.type == TOK_NOT &&
                identical_or_not(node->left->left, node->right->left->right) &&
                identical_or_not(node->left->right->right, node->right->right)){
                    Node* new = calloc(1, sizeof(Node));
                    new->Nodetype.type = TOK_XOR;
                    new->left = copy_tree(node->left->left);
                    new->right = copy_tree(node->right->right);
                    return new;
                }
        }
    
    return NULL;
}

Node* B10(Node* node) {
    if(node->Nodetype.type == TOK_XOR) {
        Node* new = calloc(1, sizeof(Node));
        new->left = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->right->left = calloc(1, sizeof(Node));
        new->right->right = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_AND;
        new->left->Nodetype.type = new->right->Nodetype.type = TOK_OR;
        new->right->right->Nodetype.type = new->right->left->Nodetype.type = TOK_NOT;
        new->left->left = copy_tree(node->left);
        new->left->right = copy_tree(node->right);
        new->right->left->right = copy_tree(node->left);
        new->right->right->right = copy_tree(node->right);
        return new;
    }

    return NULL;
}

Node* B10_Back(Node* node) {
    if(node->Nodetype.type == TOK_AND &&
        node->left->Nodetype.type == TOK_OR &&
        node->right->Nodetype.type == TOK_OR)
        {
            if(node->right->right->Nodetype.type == TOK_NOT &&
                node->right->left->Nodetype.type == TOK_NOT &&
                identical_or_not(node->left->left, node->right->left->right) &&
                identical_or_not(node->left->right, node->right->right->right)){
                    Node* new = calloc(1, sizeof(Node));
                    new->Nodetype.type = TOK_XOR;
                    new->left = copy_tree(node->left->left);
                    new->right = copy_tree(node->left->right);
                    return new;
                }
        }
    
    return NULL;
}

Node* B11(Node* node) {
    if(node->Nodetype.type == TOK_XOR) {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_NOT;
        new->right = calloc(1, sizeof(Node));
        new->right->Nodetype.type = TOK_BICOND;
        new->right->left = copy_tree(node->left);
        new->right->right = copy_tree(node->right);
        return new;
    }

    return NULL;
}

Node* B11_Back(Node* node) {
    if(node->Nodetype.type == TOK_NOT &&
        node->right->Nodetype.type == TOK_BICOND) {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_XOR;
        new->left = copy_tree(node->right->left);
        new->right = copy_tree(node->right->right);
        return new;
    }

    return NULL;
}

Node* B12(Node* node) {
    if(node->Nodetype.type == TOK_XOR) {
        Node* new = calloc(1, sizeof(Node));
        new->left = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->right->right = calloc(1, sizeof(Node));
        new->Nodetype.type = new->left->Nodetype.type = new->right->right->Nodetype.type = TOK_IMPLIES;
        new->right->Nodetype.type = TOK_NOT;
        new->left->left = copy_tree(node->left);
        new->right->right->right = copy_tree(node->left);
        new->left->right = copy_tree(node->right);
        new->right->right->left = copy_tree(node->right);
        return new;
    }

    return NULL;
}

Node* B12_Back(Node* node) {
    if(node->Nodetype.type == TOK_IMPLIES &&
        node->left->Nodetype.type == TOK_IMPLIES &&
        node->right->Nodetype.type == TOK_NOT &&
        node->right->right->Nodetype.type == TOK_IMPLIES) {
            if(identical_or_not(node->left->left, node->right->right->right) &&
                identical_or_not(node->left->right, node->right->right->left)){
                    Node* new = calloc(1, sizeof(Node));
                    new->Nodetype.type = TOK_XOR;
                    new->left = copy_tree(node->left->left);
                    new->right = copy_tree(node->left->right);
                    return new;
                }
    }
    
    return NULL;
}

Node* B13(Node* node) {
    if(node->Nodetype.type == TOK_OR) {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_XOR;
        new->right = calloc(1, sizeof(Node));
        new->right->Nodetype.type = TOK_XOR;
        new->right->right = calloc(1, sizeof(Node));
        new->right->right->Nodetype.type = TOK_AND;
        new->left = copy_tree(node->left);
        new->right->left = copy_tree(node->right);
        new->right->right->left = copy_tree(node->left);
        new->right->right->right = copy_tree(node->right);
        return new;
    }

    return NULL;
}

Node* B13_Back(Node* node) {
    if(node->Nodetype.type == TOK_XOR &&
        node->right->Nodetype.type == TOK_XOR &&
        node->right->right->Nodetype.type == TOK_AND) {
            if(identical_or_not(node->left, node->right->right->left) &&
                identical_or_not(node->right->left, node->right->right->right)) {
                    Node* new = calloc(1, sizeof(Node));
                    new->Nodetype.type = TOK_OR;
                    new->left = copy_tree(node->left);
                    new->right = copy_tree(node->right->left);
                    return new;
                }
    }

    return NULL;
}

Node* B14(Node* node) {
    if(node->Nodetype.type == TOK_AND) {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_XOR;
        new->right = calloc(1, sizeof(Node));
        new->right->Nodetype.type = TOK_XOR;
        new->right->right = calloc(1, sizeof(Node));
        new->right->right->Nodetype.type = TOK_OR;
        new->left = copy_tree(node->left);
        new->right->left = copy_tree(node->right);
        new->right->right->left = copy_tree(node->left);
        new->right->right->right = copy_tree(node->right);
        return new;
    }

    return NULL;
}

Node* B14_Back(Node* node) {
    if(node->Nodetype.type == TOK_XOR &&
        node->right->Nodetype.type == TOK_XOR &&
        node->right->right->Nodetype.type == TOK_OR) {
            if(identical_or_not(node->left, node->right->right->left) &&
                identical_or_not(node->right->left, node->right->right->right)) {
                    Node* new = calloc(1, sizeof(Node));
                    new->Nodetype.type = TOK_AND;
                    new->left = copy_tree(node->left);
                    new->right = copy_tree(node->right->left);
                    return new;
                }
    }

    return NULL;
}

Node* C1(Node* node) {
    if(node->Nodetype.type == TOK_BICOND)
    {
        Node* new = calloc(1, sizeof(Node));

        new->Nodetype.type = node->Nodetype.type;

        new->right = copy_tree(node->left);
        new->left = copy_tree(node->right);
        return new;
    }
    return NULL;
}

Node* C3(Node* node) {
    if(node->Nodetype.type == TOK_OR && node->right->Nodetype.type == TOK_BICOND) {
        Node* new = calloc(1, sizeof(Node));
        new->left = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_BICOND;
        new->left->Nodetype.type = new->right->Nodetype.type = TOK_OR;
        new->left->left = copy_tree(node->left);
        new->left->right = copy_tree(node->right->left);
        new->right->left = copy_tree(node->left);
        new->right->right = copy_tree(node->right->right);
        return new;
    }

    return NULL;
}

Node* C3_Back(Node* node){  
    if(node->Nodetype.type == TOK_BICOND &&
       node->left->Nodetype.type == TOK_OR &&
       node->right->Nodetype.type == TOK_OR &&
      (identical_or_not(node->left->left, node->right->left)))
    {
        Node* new = calloc(1, sizeof(Node));

        new->Nodetype.type = TOK_OR;

        new->right = calloc(1, sizeof(Node));

        new->right->Nodetype.type = TOK_BICOND;
        new->left = copy_tree(node->left->left);
        new->right->left = copy_tree(node->left->right);
        new->right->right = copy_tree(node->right->right);
        return new;
    }

    return NULL;
}

Node* C4(Node* node) {
    if(node->Nodetype.type == TOK_BICOND &&
        node->right->Nodetype.type == TOK_TRUE)
    {
        return copy_tree(node->left);
    }

    return NULL;
}

Node* C4_Back(Node* node) {
    Node* new = calloc(1, sizeof(Node));
    new->right = calloc(1, sizeof(Node));
    new->Nodetype.type = TOK_BICOND;
    new->right->Nodetype.type = TOK_TRUE;
    new->left = copy_tree(node);
    return new;
}

Node* C5_Above(Node* node) {
    if(node->Nodetype.type == TOK_BICOND &&
        node->right->Nodetype.type == TOK_FALSE)
    {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_NOT;
        new->right = copy_tree(node->left);
        return new;
    }

    return NULL;    
}

Node* C5_Above_Back(Node* node) {
    if(node->Nodetype.type == TOK_NOT) {
        Node* new = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_BICOND;
        new->right->Nodetype.type = TOK_FALSE;
        new->left = copy_tree(node->right);
        return new;        
    }

    return NULL;
}

Node* C5_Below_1(Node* node) {
    if(node->Nodetype.type == TOK_NOT &&
        node->right->Nodetype.type == TOK_BICOND)
    {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_BICOND;
        new->left = calloc(1, sizeof(Node));
        new->left->Nodetype.type = TOK_NOT;
        new->left->right = copy_tree(node->right->left);
        new->right = copy_tree(node->right->right);
        return new;
    }

    return NULL;
}

Node* C5_Below_2(Node* node) {
    if(node->Nodetype.type == TOK_BICOND &&
        node->left->Nodetype.type == TOK_NOT)
    {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_BICOND;
        new->right = calloc(1, sizeof(Node));
        new->right->Nodetype.type = TOK_NOT;
        new->right->right = copy_tree(node->right);
        new->left = copy_tree(node->left->right); 
        return new;       
    }

    return NULL;
}

Node* C5_Below_Back_1(Node* node) {
    if(node->Nodetype.type == TOK_BICOND &&
        node->left->Nodetype.type == TOK_NOT)
    {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_NOT;
        new->right = calloc(1, sizeof(Node));
        new->right->Nodetype.type = TOK_BICOND;
        new->right->right = copy_tree(node->right);
        new->right->left = copy_tree(node->left->right);  
        return new;      
    }

    return NULL;
}

Node* C5_Below_Back_2(Node* node) {
    if(node->Nodetype.type == TOK_BICOND &&
        node->right->Nodetype.type == TOK_NOT)
    {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_BICOND;
        new->left = calloc(1, sizeof(Node));
        new->left->Nodetype.type = TOK_NOT;
        new->left->right = copy_tree(node->left);
        new->right = copy_tree(node->right->right);  
        return new;      
    }

    return NULL;  
}

Node* C6(Node* node) {
    if(node->Nodetype.type == TOK_BICOND &&
       identical_or_not(node->left, node->right))
    {
        Node* new = calloc(1, sizeof(Node));

        new->Nodetype.type = TOK_TRUE;
        return new;
    }

    if(node->Nodetype.type == TOK_BICOND &&
       node->right->Nodetype.type == TOK_NOT &&
       identical_or_not(node->left, node->right->right))
    {
        Node* new = calloc(1, sizeof(Node));

        new->Nodetype.type = TOK_FALSE;
        return new;
    }

    return NULL;
}

Node* C7(Node* node) {
    if (node->Nodetype.type == TOK_BICOND &&
        node->right->Nodetype.type == TOK_BICOND &&
        identical_or_not(node->left, node->right->left) &&
        identical_or_not(node->left, node->right->right)) {
        return copy_tree(node->left);
    }

    return NULL;    
}

Node* C7_Back(Node* node) {
    Node* new = calloc(1, sizeof(Node));
    new->right = calloc(1, sizeof(Node));
    new->Nodetype.type = new->right->Nodetype.type = TOK_BICOND;
    new->left = copy_tree(node);
    new->right->left = copy_tree(node);
    new->right->right = copy_tree(node);
    return new;
}

Node* C8(Node* node) {
    if(node->Nodetype.type == TOK_BICOND) {
        Node* new = calloc(1, sizeof(Node));
        new->left = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_BICOND;
        new->left->Nodetype.type = new->right->Nodetype.type = TOK_NOT;
        new->left->right = copy_tree(node->left);
        new->right->right = copy_tree(node->right);
        return new;
    }

    return NULL;
}

Node* C8_Back(Node* node) {
    if(node->Nodetype.type == TOK_BICOND &&
        node->left->Nodetype.type == TOK_NOT &&
        node->right->Nodetype.type == TOK_NOT) 
    {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_BICOND;
        new->left = copy_tree(node->left->right);
        new->right = copy_tree(node->right->right);
        return new;
    }

    return NULL;
}

Node* C9(Node* node) {
    if(node->Nodetype.type == TOK_BICOND) {
        Node* new = calloc(1, sizeof(Node));
        new->left = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->right->left = calloc(1, sizeof(Node));
        new->right->right = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_OR;
        new->left->Nodetype.type = new->right->Nodetype.type = TOK_AND;
        new->right->right->Nodetype.type = new->right->left->Nodetype.type = TOK_NOT;
        new->left->left = copy_tree(node->left);
        new->left->right = copy_tree(node->right);
        new->right->left->right = copy_tree(node->left);
        new->right->right->right = copy_tree(node->right);
        return new;
    }

    return NULL;
}

Node* C9_Back(Node* node) {
    if(node->Nodetype.type == TOK_OR &&
        node->left->Nodetype.type == TOK_AND &&
        node->right->Nodetype.type == TOK_AND)
        {
            if(node->right->right->Nodetype.type == TOK_NOT &&
                node->right->left->Nodetype.type == TOK_NOT &&
                identical_or_not(node->left->left, node->right->left->right) &&
                identical_or_not(node->left->right, node->right->right->right)){
                    Node* new = calloc(1, sizeof(Node));
                    new->Nodetype.type = TOK_BICOND;
                    new->left = copy_tree(node->left->left);
                    new->right = copy_tree(node->left->right);
                    return new;
                }
        }
    
    return NULL;
}

Node* C10(Node* node) {
    if(node->Nodetype.type == TOK_BICOND) {
        Node* new = calloc(1, sizeof(Node));
        new->left = calloc(1, sizeof(Node));
        new->left->right = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->right->left = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_AND;
        new->left->Nodetype.type = new->right->Nodetype.type = TOK_OR;
        new->left->right->Nodetype.type = new->right->left->Nodetype.type = TOK_NOT;
        new->left->left = copy_tree(node->left);
        new->left->right->right = copy_tree(node->right);
        new->right->left->right = copy_tree(node->left);
        new->right->right = copy_tree(node->right);
        return new;
    }

    return NULL;
}

Node* C10_Back(Node* node) {
    if(node->Nodetype.type == TOK_AND &&
        node->left->Nodetype.type == TOK_OR &&
        node->right->Nodetype.type == TOK_OR)
        {
            if(node->left->right->Nodetype.type == TOK_NOT &&
                node->right->left->Nodetype.type == TOK_NOT &&
                identical_or_not(node->left->left, node->right->left->right) &&
                identical_or_not(node->left->right->right, node->right->right)){
                    Node* new = calloc(1, sizeof(Node));
                    new->Nodetype.type = TOK_BICOND;
                    new->left = copy_tree(node->left->left);
                    new->right = copy_tree(node->right->right);
                    return new;
                }
        }
    
    return NULL;
}

Node* C11(Node* node) {
    if(node->Nodetype.type == TOK_BICOND) {
        Node* new = calloc(1, sizeof(Node));
        new->left = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_AND;
        new->left->Nodetype.type = new->right->Nodetype.type = TOK_IMPLIES;
        new->left->left = copy_tree(node->left);
        new->left->right = copy_tree(node->right);
        new->right->left = copy_tree(node->right);
        new->right->right = copy_tree(node->left);
        return new;
    }

    return NULL;
}

Node* C11_Back(Node* node) {
    if(node->Nodetype.type == TOK_AND &&
        node->left->Nodetype.type == TOK_IMPLIES &&
        node->right->Nodetype.type == TOK_IMPLIES) {
            if(identical_or_not(node->left->left, node->right->right) &&
                identical_or_not(node->left->right, node->right->left)) {
                    Node* new = calloc(1, sizeof(Node));
                    new->Nodetype.type = TOK_BICOND;
                    new->left = copy_tree(node->left->left);
                    new->right = copy_tree(node->left->right);
                    return new;
                }
        }
    
    return NULL;
}

Node* C12(Node* node) {
    if(node->Nodetype.type == TOK_BICOND) {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_NOT;
        new->right = calloc(1, sizeof(Node));
        new->right->Nodetype.type = TOK_XOR;
        new->right->left = copy_tree(node->left);
        new->right->right = copy_tree(node->right);
        return new;
    }

    return NULL;
}

Node* C12_Back(Node* node) {
    if(node->Nodetype.type == TOK_NOT &&
        node->right->Nodetype.type == TOK_XOR) {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_BICOND;
        new->left = copy_tree(node->right->left);
        new->right = copy_tree(node->right->right);
        return new;
    }

    return NULL;
}

Node* C13(Node* node) {
    if(node->Nodetype.type == TOK_OR) {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_BICOND;
        new->right = calloc(1, sizeof(Node));
        new->right->Nodetype.type = TOK_BICOND;
        new->right->right = calloc(1, sizeof(Node));
        new->right->right->Nodetype.type = TOK_AND;
        new->left = copy_tree(node->left);
        new->right->left = copy_tree(node->right);
        new->right->right->left = copy_tree(node->left);
        new->right->right->right = copy_tree(node->right);
        return new;
    }

    return NULL;
}

Node* C13_Back(Node* node) {
    if(node->Nodetype.type == TOK_BICOND &&
        node->right->Nodetype.type == TOK_BICOND &&
        node->right->right->Nodetype.type == TOK_AND) {
            if(identical_or_not(node->left, node->right->right->left) &&
                identical_or_not(node->right->left, node->right->right->right)) {
                    Node* new = calloc(1, sizeof(Node));
                    new->Nodetype.type = TOK_OR;
                    new->left = copy_tree(node->left);
                    new->right = copy_tree(node->right->left);
                    return new;
                }
    }

    return NULL;
}

Node* C14(Node* node) {
    if(node->Nodetype.type == TOK_AND) {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_BICOND;
        new->right = calloc(1, sizeof(Node));
        new->right->Nodetype.type = TOK_BICOND;
        new->right->right = calloc(1, sizeof(Node));
        new->right->right->Nodetype.type = TOK_OR;
        new->left = copy_tree(node->left);
        new->right->left = copy_tree(node->right);
        new->right->right->left = copy_tree(node->left);
        new->right->right->right = copy_tree(node->right);
        return new;
    }

    return NULL;
}

Node* C14_Back(Node* node) {
    if(node->Nodetype.type == TOK_BICOND &&
        node->right->Nodetype.type == TOK_BICOND &&
        node->right->right->Nodetype.type == TOK_OR) {
            if(identical_or_not(node->left, node->right->right->left) &&
                identical_or_not(node->right->left, node->right->right->right)) {
                    Node* new = calloc(1, sizeof(Node));
                    new->Nodetype.type = TOK_AND;
                    new->left = copy_tree(node->left);
                    new->right = copy_tree(node->right->left);
                    return new;
                }
    }

    return NULL;
}

Node* D1(Node* node){
    if(node->Nodetype.type == TOK_IMPLIES){
        Node* new = calloc(1, sizeof(Node));
        new->left = calloc(1, sizeof(Node));

        new->Nodetype.type = TOK_OR;
        new->left->Nodetype.type = TOK_NOT;
        new->right = copy_tree(node->right);
        new->left->right = copy_tree(node->left);
        return new;
    }

    return NULL;
}

Node* D1_Back(Node* node) {
    if(node->Nodetype.type == TOK_OR &&
        node->left->Nodetype.type == TOK_NOT) 
    {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_IMPLIES;
        new->left = copy_tree(node->left->right);
        new->right = copy_tree(node->right);
        return new;
    }

    return NULL;
}

Node* D2(Node* node){
    if(node->Nodetype.type == TOK_NOT &&
       node->right->Nodetype.type == TOK_IMPLIES)
    {

        Node* new = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));

        new->Nodetype.type = TOK_AND;
        new->right->Nodetype.type = TOK_NOT;
        new->right->right = copy_tree(node->right->right);
        new->left = copy_tree(node->right->left);
        return new;      
    }

    return NULL;    
}

Node* D2_Back(Node* node) {
    if(node->Nodetype.type == TOK_AND &&
        node->right->Nodetype.type == TOK_NOT)
    {
        Node* new = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_NOT;
        new->right->Nodetype.type = TOK_IMPLIES;
        new->right->left = copy_tree(node->left);
        new->right->right = copy_tree(node->right->right);
        return new;
    }

    return NULL;
}

Node* D3(Node* node) {
    if(node->Nodetype.type == TOK_IMPLIES) {
        Node* new = calloc(1, sizeof(Node));
        new->left = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_IMPLIES;
        new->left->Nodetype.type = new->right->Nodetype.type = TOK_NOT;
        new->left->right = copy_tree(node->right);
        new->right->right = copy_tree(node->left);
        return new;
    }

    return NULL;
}

Node* D3_Back(Node* node) {
    if(node->Nodetype.type == TOK_IMPLIES &&
        node->left->Nodetype.type == TOK_NOT &&
        node->right->Nodetype.type == TOK_NOT) {
            Node* new = calloc(1, sizeof(Node));
            new->Nodetype.type = TOK_IMPLIES;
            new->left = copy_tree(node->right->right);
            new->right = copy_tree(node->left->right);
            return new;
    }

    return NULL;
}

Node* D4(Node* node) {
    if(node->Nodetype.type != TOK_IMPLIES) return NULL;
    if(node->right->Nodetype.type == TOK_TRUE)
    {
        Node* new = calloc(1, sizeof(Node));

        new->Nodetype.type = TOK_TRUE;
        return new;        
    }
    if(node->left->Nodetype.type == TOK_FALSE)
    {
        Node* new = calloc(1, sizeof(Node));

        new->Nodetype.type = TOK_TRUE;
        return new;        
    }
    
    return NULL;
}

Node* D5(Node* node) {
    if(node->Nodetype.type != TOK_IMPLIES) return NULL;
    if(node->left->Nodetype.type == TOK_TRUE)
    {
        return copy_tree(node->right);
    }
    if(node->right->Nodetype.type == TOK_FALSE)
    {
        Node* new = calloc(1, sizeof(Node));

        new->Nodetype.type = TOK_NOT;
        new->right = copy_tree(node->left);
        return new;       
    }    

    return NULL;
}

Node* D5_Left_Back(Node* node) {
    Node* new = calloc(1, sizeof(Node));
    new->left = calloc(1, sizeof(Node));
    new->Nodetype.type = TOK_IMPLIES;
    new->left->Nodetype.type = TOK_TRUE;
    new->right = copy_tree(node);
    return new;
}

Node* D5_Right_Back(Node* node) {
    if(node->Nodetype.type == TOK_NOT) {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_IMPLIES;
        new->right = calloc(1, sizeof(Node));
        new->right->Nodetype.type = TOK_FALSE;
        new->left = copy_tree(node->right);
        return new;
    }

    return NULL;
}

Node* D6(Node* node){
    if(node->Nodetype.type != TOK_IMPLIES) return NULL;
    if(identical_or_not(node->left, node->right))
    {
        Node* new = calloc(1, sizeof(Node));

        new->Nodetype.type = TOK_TRUE;
        return new;        
    }
    if(node->left->Nodetype.type == TOK_NOT &&
       identical_or_not(node->left->right, node->right))
    {
        return copy_tree(node->right);
    }
    if(node->right->Nodetype.type == TOK_NOT &&
       identical_or_not(node->left, node->right->right))
    {
        Node* new = calloc(1, sizeof(Node));

        new->Nodetype.type = TOK_NOT;
        new->right = copy_tree(node->left);
        return new;        
    }

    return NULL;
}

Node* D6_Middle_Back(Node* node) {
    Node* new = calloc(1, sizeof(Node));
    new->left = calloc(1, sizeof(Node));
    new->Nodetype.type = TOK_IMPLIES;
    new->left->Nodetype.type = TOK_NOT;
    new->right = copy_tree(node);
    new->left->right = copy_tree(node);
    return new;
}

Node* D6_Right_Back(Node* node) {
    if(node->Nodetype.type == TOK_NOT) {
        Node* new = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_IMPLIES;
        new->right->Nodetype.type = TOK_NOT;
        new->left = copy_tree(node->right);
        new->right->right = copy_tree(node->right);
        return new;        
    }

    return NULL;
}

Node* D7(Node* node) {
    if(node->Nodetype.type == TOK_IMPLIES &&
        node->right->Nodetype.type == TOK_AND)
    {
        Node* new = calloc(1, sizeof(Node));
        new->left = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_AND;
        new->left->Nodetype.type = new->right->Nodetype.type = TOK_IMPLIES;
        new->left->left = copy_tree(node->left);
        new->left->right = copy_tree(node->right->left);
        new->right->left = copy_tree(node->left);
        new->right->right = copy_tree(node->right->right);
        return new;
    }

    if(node->Nodetype.type == TOK_IMPLIES &&
        node->left->Nodetype.type == TOK_AND)
    {
        Node* new = calloc(1, sizeof(Node));
        new->left = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_OR;
        new->left->Nodetype.type = new->right->Nodetype.type = TOK_IMPLIES; 
        new->left->left = copy_tree(node->left->left);
        new->left->right = copy_tree(node->right);
        new->right->left = copy_tree(node->left->right);
        new->right->right = copy_tree(node->right);    
        return new;  
    }

    return NULL;
}

Node* D7_Back(Node* node){  
    if(node->Nodetype.type == TOK_AND &&
       node->left->Nodetype.type == TOK_IMPLIES &&
       node->right->Nodetype.type == TOK_IMPLIES &&
       identical_or_not(node->left->left, node->right->left))
    {
        Node* new = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));

        new->Nodetype.type = TOK_IMPLIES;
        new->right->Nodetype.type = TOK_AND;
        new->right->left = copy_tree(node->left->right);
        new->right->right = copy_tree(node->right->right);
        new->left = copy_tree(node->left->left);
        return new;
    }  
    if(node->Nodetype.type == TOK_OR &&
       node->left->Nodetype.type == TOK_IMPLIES &&
       node->right->Nodetype.type == TOK_IMPLIES &&
       identical_or_not(node->left->right, node->right->right))
    {
        Node* new = calloc(1, sizeof(Node));
        new->left = calloc(1, sizeof(Node));

        new->Nodetype.type = TOK_IMPLIES;
        new->right = copy_tree(node->right->right);
        new->left->Nodetype.type = TOK_AND;
        new->left->left = copy_tree(node->left->left);
        new->left->right = copy_tree(node->right->left);
        return new;
    }  

    return NULL;
}

Node* D8(Node* node) {
    if(node->Nodetype.type == TOK_IMPLIES &&
        node->right->Nodetype.type == TOK_OR)
    {
        Node* new = calloc(1, sizeof(Node));
        new->left = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_OR;
        new->left->Nodetype.type = new->right->Nodetype.type = TOK_IMPLIES;
        new->left->left = copy_tree(node->left);
        new->left->right = copy_tree(node->right->left);
        new->right->left = copy_tree(node->left);
        new->right->right = copy_tree(node->right->right);
        return new;
    }

    if(node->Nodetype.type == TOK_IMPLIES &&
        node->left->Nodetype.type == TOK_OR)
    {
        Node* new = calloc(1, sizeof(Node));
        new->left = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_AND;
        new->left->Nodetype.type = new->right->Nodetype.type = TOK_IMPLIES; 
        new->left->left = copy_tree(node->left->left);
        new->left->right = copy_tree(node->right);
        new->right->left = copy_tree(node->left->right);
        new->right->right = copy_tree(node->right);  
        return new;    
    }

    return NULL;
}

Node* D8_Back(Node* node){   
    if(node->Nodetype.type == TOK_OR &&
       node->left->Nodetype.type == TOK_IMPLIES &&
       node->right->Nodetype.type == TOK_IMPLIES &&
       identical_or_not(node->left->left, node->right->left))
    {
        Node* new = calloc(1, sizeof(Node));

        new->right = calloc(1, sizeof(Node));

        new->Nodetype.type = TOK_IMPLIES;
        new->right->Nodetype.type = TOK_OR;
        new->right->left = copy_tree(node->left->right);
        new->right->right = copy_tree(node->right->right);
        new->left = copy_tree(node->left->left);
        return new;
    }  
    if(node->Nodetype.type == TOK_AND &&
       node->left->Nodetype.type == TOK_IMPLIES &&
       node->right->Nodetype.type == TOK_IMPLIES &&
       identical_or_not(node->left->right, node->right->right))
    {
        Node* new = calloc(1, sizeof(Node));

        new->left = calloc(1, sizeof(Node));

        new->Nodetype.type = TOK_IMPLIES;
        new->right = copy_tree(node->right->right);
        new->left->Nodetype.type = TOK_OR;
        new->left->left = copy_tree(node->left->left);
        new->left->right = copy_tree(node->right->left);
        return new;
    }     

    return NULL;
}

Node* D9_Above(Node* node) {
    if(node->Nodetype.type == TOK_IMPLIES &&
        node->right->Nodetype.type == TOK_IMPLIES) 
    {
        Node* new = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->Nodetype.type = new->right->Nodetype.type = TOK_IMPLIES;
        new->left = copy_tree(node->right->left);
        new->right->left = copy_tree(node->left);
        new->right->right = copy_tree(node->right->right);
        return new;
    }

    return NULL;
}

Node* D9_Below(Node* node) {
    if(node->Nodetype.type == TOK_IMPLIES &&
        node->right->Nodetype.type == TOK_IMPLIES) 
    {
        Node* new = calloc(1, sizeof(Node));
        new->left = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_IMPLIES;
        new->left->Nodetype.type = TOK_AND;
        new->left->left = copy_tree(node->left);
        new->left->right = copy_tree(node->right->left);
        new->right = copy_tree(node->right->right);
        return new;
    }

    return NULL;
}

Node* D9_Below_Back(Node* node) {
    if(node->Nodetype.type == TOK_IMPLIES &&
        node->left->Nodetype.type == TOK_AND) 
    {
        Node* new = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->Nodetype.type = new->right->Nodetype.type = TOK_IMPLIES;
        new->left = copy_tree(node->left->left);
        new->right->left = copy_tree(node->left->right)   ;
        new->right->right = copy_tree(node->right);
        return new;     
    }

    return NULL;
}

Node* D10(Node* node){ 
    if(node->Nodetype.type == TOK_IMPLIES &&
       node->right->Nodetype.type == TOK_IMPLIES &&
       identical_or_not(node->left, node->right->left))
    {
        Node* new = calloc(1, sizeof(Node));
  
        new->Nodetype.type = TOK_IMPLIES;
        new->left = copy_tree(node->left);
        new->right = copy_tree(node->right->right);
        return new;
    }
    
    return NULL;
}

Node* D10_Back(Node* node) {
    if(node->Nodetype.type == TOK_IMPLIES)
    {
        Node* new = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->Nodetype.type = new->right->Nodetype.type = TOK_IMPLIES;
        new->left = copy_tree(node->left);
        new->right->left = copy_tree(node->left);
        new->right->right = copy_tree(node->right);
        return new;
    }

    return NULL;
}

Node* D11(Node* node) {
    if(node->Nodetype.type == TOK_IMPLIES) 
    {
        Node* new = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->right->right = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_NOT;
        new->right->Nodetype.type = TOK_XOR;
        new->right->right->Nodetype.type = TOK_AND;
        new->right->left = copy_tree(node->left);
        new->right->right->left = copy_tree(node->left);
        new->right->right->right = copy_tree(node->right);
        return new;
    }

    return NULL;
}

Node* D11_Back(Node* node) {
    if(node->Nodetype.type == TOK_NOT &&
        node->right->Nodetype.type == TOK_XOR)
    {
        if(node->right->right->Nodetype.type == TOK_AND &&
            identical_or_not(node->right->left, node->right->right->left))
        {
            Node* new = calloc(1, sizeof(Node));
            new->Nodetype.type = TOK_IMPLIES;
            new->left = copy_tree(node->right->left);
            new->right = copy_tree(node->right->right->right);
            return new;
        }
    }

    return NULL;
}

Node* D12_1(Node* node) {
    if(node->Nodetype.type == TOK_IMPLIES)
    {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_BICOND;
        new->right = calloc(1, sizeof(Node));
        new->right->Nodetype.type = TOK_AND;
        new->left = copy_tree(node->left);
        new->right->left = copy_tree(node->left);
        new->right->right = copy_tree(node->right);
        return new;
    }

    return NULL;
}

Node* D12_2(Node* node) {
    if(node->Nodetype.type == TOK_BICOND &&
        node->right->Nodetype.type == TOK_AND)
    {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_BICOND;
        new->right = calloc(1, sizeof(Node));
        new->right->Nodetype.type = TOK_OR;
        new->left = copy_tree(node->right->right);
        new->right->left = copy_tree(node->left);
        new->right->right = copy_tree(node->right->right);
        return new;       
    }

    return NULL;
}

Node* D12_1_Back(Node* node) {
    if(node->Nodetype.type == TOK_BICOND &&
        node->right->Nodetype.type == TOK_AND)
    {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_IMPLIES;
        new->left = copy_tree(node->left);
        new->right = copy_tree(node->right->right);
        return new;       
    }

    return NULL;
}

Node* D12_2_Back(Node* node) {
    if(node->Nodetype.type == TOK_BICOND &&
        node->right->Nodetype.type == TOK_OR)
    {
        Node* new = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_BICOND;
        new->right->Nodetype.type = TOK_AND;
        new->left = copy_tree(node->right->left);
        new->right->left = copy_tree(node->right->left);
        new->right->right = copy_tree(node->right->right);
        return new;
    }

    return NULL;
}

Node* D13_1(Node* node) {
    if(node->Nodetype.type == TOK_OR) 
    {
        Node* new = calloc(1, sizeof(Node));
        new->left = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_IMPLIES;
        new->left->Nodetype.type = TOK_NOT;
        new->right = copy_tree(node->right);
        new->left->right = copy_tree(node->left);
        return new;
    }

    return NULL;
}

Node* D13_2(Node* node) {
    if(node->Nodetype.type == TOK_IMPLIES &&
        node->left->Nodetype.type == TOK_NOT) 
    {
        Node* new = calloc(1, sizeof(Node));
        new->left = calloc(1, sizeof(Node));
        new->Nodetype.type = new->left->Nodetype.type = TOK_IMPLIES;
        new->right = copy_tree(node->right);
        new->left->right = copy_tree(node->right);
        new->left->left = copy_tree(node->left->right);
        return new;
    }

    return NULL;
}

Node* D13_1_Back(Node* node) {
    if(node->Nodetype.type == TOK_IMPLIES &&
        node->left->Nodetype.type == TOK_NOT) 
    {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_OR;
        new->right = copy_tree(node->right);
        new->left = copy_tree(node->left->right);
        return new;
    }

    return NULL;
}

Node* D13_2_Back(Node* node) {
    if(node->Nodetype.type == TOK_IMPLIES &&
        node->left->Nodetype.type == TOK_IMPLIES &&
        identical_or_not(node->right, node->left->right))
    {
        Node* new = calloc(1, sizeof(Node));
        new->left = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_IMPLIES;
        new->left->Nodetype.type = TOK_NOT;
        new->right = copy_tree(node->right);
        new->left->right = copy_tree(node->left->left);
        return new;        
    }

    return NULL;
}

Node* D14(Node* node) {
    if(node->Nodetype.type == TOK_AND) 
    {
        Node* new = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->right->right = calloc(1, sizeof(Node));
        new->Nodetype.type = new->right->right->Nodetype.type = TOK_NOT;
        new->right->Nodetype.type = TOK_IMPLIES;
        new->right->left = copy_tree(node->left);
        new->right->right->right = copy_tree(node->right);

        return new;
    }

    return NULL;    
}

Node* D14_Back(Node* node) {
    if(node->Nodetype.type == TOK_NOT &&
        node->right->Nodetype.type == TOK_IMPLIES &&
        node->right->right->Nodetype.type == TOK_NOT)
    {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_AND;
        new->right = copy_tree(node->right->right->right);
        new->left = copy_tree(node->right->left);
        return new;        
    }

    return NULL;
}


Node* E2(Node* node) {

    Node* new = calloc(1, sizeof(Node));
    new->Nodetype.type = TOK_TRUE;
    return new;

}

Node* E4(Node* node) {
    if(node->Nodetype.type == TOK_AND) {
        return copy_tree(node->left);
    }

    return NULL;
}

Node* E6(Node* node) {
    if(node->Nodetype.type == TOK_AND) {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_OR;
        new->left = copy_tree(node->left);
        new->right = copy_tree(node->right);
        return new;
    }

    return NULL;
}

Node* E9(Node* node) {
    if(node->Nodetype.type == TOK_AND &&
       node->right->Nodetype.type == TOK_IMPLIES &&
       identical_or_not(node->left, node->right->left)) {
        return copy_tree(node->right->right);
       }
    if(node->Nodetype.type == TOK_AND &&
       node->right->Nodetype.type == TOK_IMPLIES &&
       node->right->left->Nodetype.type == TOK_OR &&
       identical_or_not(node->left, node->right->left->left)) {
        return copy_tree(node->right->right);
       }
    
    return NULL;
}

Node* E10(Node* node) {
    if(node->Nodetype.type == TOK_AND &&
       node->right->Nodetype.type == TOK_IMPLIES &&
       node->left->Nodetype.type == TOK_NOT &&
       identical_or_not(node->left->right, node->right->right)) {
        Node* new = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_NOT;
        new->right = copy_tree(node->right->left);
        return new;
       }
    
    return NULL;
}

Node* E11(Node* node) {
    if(node->Nodetype.type == TOK_AND &&
        node->left->Nodetype.type == TOK_IMPLIES &&
        node->right->Nodetype.type == TOK_IMPLIES &&
        identical_or_not(node->left->right, node->right->left)) {
            Node* new = calloc(1, sizeof(Node));
            new->Nodetype.type = TOK_IMPLIES;
            new->left = copy_tree(node->left->left);
            new->right = copy_tree(node->right->right);
            return new;
        }
    if(node->Nodetype.type == TOK_AND &&
        node->left->Nodetype.type == TOK_IMPLIES &&
        node->right->Nodetype.type == TOK_IMPLIES &&
        node->right->left->Nodetype.type == TOK_OR &&
        identical_or_not(node->left->right, node->right->left->left)) {
            Node* new = calloc(1, sizeof(Node));
            new->Nodetype.type = TOK_IMPLIES;
            new->left = copy_tree(node->left->left);
            new->right = copy_tree(node->right->right);
            return new;
        }
    
    return NULL;
}

Node* E12(Node* node) {
    if(node->Nodetype.type == TOK_AND &&
        node->left->Nodetype.type == TOK_IMPLIES &&
        node->right->Nodetype.type == TOK_IMPLIES) 
    {
        if(node->right->left->Nodetype.type == TOK_NOT &&
            identical_or_not(node->left->left, node->right->left->right) &&
            identical_or_not(node->left->right, node->right->right))
        {
            Node* new = copy_tree(node->right->right);
            return new;
        }
    }

    return NULL;
}


Node* E13(Node* node) {
    if(node->Nodetype.type == TOK_AND &&
        node->left->Nodetype.type == TOK_IMPLIES &&
        node->right->Nodetype.type == TOK_IMPLIES) 
    {
        if(node->right->right->Nodetype.type == TOK_NOT &&
            identical_or_not(node->left->left, node->right->left) &&
            identical_or_not(node->left->right, node->right->right->right))
        {
            Node* new = calloc(1, sizeof(Node));
            new->Nodetype.type = TOK_NOT;
            new->right = copy_tree(node->left->left);
            return new;
        }
    }

    return NULL;
}

Node* E14(Node* node) {
    if(node->Nodetype.type == TOK_AND &&
        node->left->Nodetype.type == TOK_IMPLIES &&
        node->right->Nodetype.type == TOK_IMPLIES)
    {
        Node* new = calloc(1, sizeof(Node));
        new->left = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_IMPLIES;
        new->left->Nodetype.type = new->right->Nodetype.type = TOK_AND;
        new->left->left = copy_tree(node->left->left);
        new->left->right = copy_tree(node->right->left);
        new->right->left = copy_tree(node->left->right);
        new->right->right = copy_tree(node->right->right);
        return new;
    }

    return NULL;
}

Node* E15(Node* node) {
    if(node->Nodetype.type == TOK_AND &&
        node->left->Nodetype.type == TOK_IMPLIES &&
        node->right->Nodetype.type == TOK_IMPLIES)
    {
        Node* new = calloc(1, sizeof(Node));
        new->left = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->Nodetype.type = TOK_IMPLIES;
        new->left->Nodetype.type = new->right->Nodetype.type = TOK_OR;
        new->left->left = copy_tree(node->left->left);
        new->left->right = copy_tree(node->right->left);
        new->right->left = copy_tree(node->left->right);
        new->right->right = copy_tree(node->right->right);
        return new;
    }

    return NULL;
}

Node* E16(Node* node) {
    if(node->Nodetype.type == TOK_AND && 
        node->left->Nodetype.type == TOK_OR &&
        node->right->Nodetype.type == TOK_AND)
    {
        if(node->right->left->Nodetype.type == TOK_IMPLIES &&
            node->right->right->Nodetype.type == TOK_IMPLIES)
        {
            if(identical_or_not(node->left->left, node->right->left->left) &&
                identical_or_not(node->left->right, node->right->right->left) &&
                identical_or_not(node->right->left->right, node->right->right->right))
                {
                    return copy_tree(node->right->right->right);
                }
            }
    }
   
    return NULL;
}

Node* E17(Node* node) {
    if(node->Nodetype.type == TOK_AND && 
        node->left->Nodetype.type == TOK_OR &&
        node->right->Nodetype.type == TOK_AND)
    {
        if(node->right->left->Nodetype.type == TOK_IMPLIES &&
            node->right->right->Nodetype.type == TOK_IMPLIES)
        {
            if(identical_or_not(node->left->left, node->right->left->left) &&
                identical_or_not(node->left->right, node->right->right->left))
                {
                    Node* new = calloc(1, sizeof(Node));
                    new->Nodetype.type = TOK_OR;
                    new->left = copy_tree(node->right->left->right);
                    new->right = copy_tree(node->right->right->right);
                    return new;
                }
            }
    }
   
    return NULL;
}

Node* E18(Node* node) {
    if(node->Nodetype.type == TOK_AND && 
        node->left->Nodetype.type == TOK_AND &&
        node->right->Nodetype.type == TOK_AND)
    {
        if(node->right->left->Nodetype.type == TOK_IMPLIES &&
            node->right->right->Nodetype.type == TOK_IMPLIES)
        {
            if(identical_or_not(node->left->left, node->right->left->left) &&
                identical_or_not(node->left->right, node->right->right->left))
                {
                    Node* new = calloc(1, sizeof(Node));
                    new->Nodetype.type = TOK_AND;
                    new->left = copy_tree(node->right->left->right);
                    new->right = copy_tree(node->right->right->right);
                    return new;
                }
            }
    }
   
    return NULL;
}

Node* E19(Node* node) {
    if(node->Nodetype.type != TOK_IMPLIES) return NULL;

    if(node->left->Nodetype.type == TOK_IMPLIES && 
        node->right->Nodetype.type == TOK_IMPLIES &&
        identical_or_not(node->left->left, node->right->left)){
            Node* new = calloc(1, sizeof(Node));
            new->right = calloc(1, sizeof(Node));
            new->Nodetype.type = new->right->Nodetype.type = TOK_IMPLIES;
            new->left = copy_tree(node->left->left);
            new->right->left = copy_tree(node->left->right);
            new->right->right = copy_tree(node->right->right);
            return new;
    }

    if(node->right->Nodetype.type == TOK_IMPLIES) {
        Node* new = calloc(1, sizeof(Node));
        new->left = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->Nodetype.type = new->left->Nodetype.type = new->right->Nodetype.type = TOK_IMPLIES;
        new->left->left = copy_tree(node->left);
        new->left->right = copy_tree(node->right->left);
        new->right->left = copy_tree(node->left);
        new->right->right = copy_tree(node->right->right);
        return new;
    }

    if(node->left->Nodetype.type == TOK_IMPLIES) {
        Node* new = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->Nodetype.type = new->right->Nodetype.type = TOK_IMPLIES;
        new->left = copy_tree(node->left->left);
        new->right->left = copy_tree(node->left->right);
        new->right->right = copy_tree(node->right);
        return new;        
    }

    return NULL;
}

Node* E21(Node* node) {
    if(node->Nodetype.type == TOK_AND &&
        node->left->Nodetype.type == TOK_IMPLIES &&
        node->right->Nodetype.type == TOK_IMPLIES)
    {
        Node* new = calloc(1, sizeof(Node));
        new->right = calloc(1, sizeof(Node));
        new->right->left = calloc(1, sizeof(Node));
        new->Nodetype.type = new->right->Nodetype.type = new->right->left->Nodetype.type = TOK_IMPLIES;
        new->left = copy_tree(node->left->left);
        new->right->right = copy_tree(node->right->right);
        new->right->left->left = copy_tree(node->left->right);
        new->right->left->right = copy_tree(node->right->left);
        return new;
    }

    return NULL;
}

