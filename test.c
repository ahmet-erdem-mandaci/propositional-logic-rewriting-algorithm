#include "core.h"

void print_tree(Node* node) {
    if (!node) return;
    
    Tokentype r = node->right ? node->right->Nodetype.type : TOK_END;
    Tokentype l = node->left ? node->left->Nodetype.type : TOK_END;

    switch (node->Nodetype.type) {
        case TOK_VAR:
            printf("%c", node->Nodetype.var);
            return;

        case TOK_TRUE:
            printf("T");
            return;

        case TOK_FALSE:
            printf("F");
            return;

        case TOK_NOT:
            printf("!");
            if (r != TOK_VAR && r!= TOK_TRUE && r!= TOK_FALSE && r!= TOK_NOT) printf("(");
            print_tree(node->right);
            if (r != TOK_VAR && r!= TOK_TRUE && r!= TOK_FALSE && r!= TOK_NOT) printf(")");
            return;

        case TOK_AND:
            if (l == TOK_AND || l == TOK_OR || l == TOK_XOR || l == TOK_IMPLIES || l == TOK_BICOND) printf("(");
            print_tree(node->left);
            if (l == TOK_AND || l == TOK_OR || l == TOK_XOR || l == TOK_IMPLIES || l == TOK_BICOND) printf(")");

            printf(" & ");

            if (r == TOK_OR || r == TOK_XOR || r == TOK_IMPLIES || r == TOK_BICOND) printf("(");
            print_tree(node->right);
            if (r == TOK_OR || r == TOK_XOR || r == TOK_IMPLIES || r == TOK_BICOND) printf(")");
            return;

        case TOK_OR:
            if (l == TOK_OR || l == TOK_XOR || l == TOK_IMPLIES || l == TOK_BICOND) printf("(");
            print_tree(node->left);
            if (l == TOK_OR || l == TOK_XOR || l == TOK_IMPLIES || l == TOK_BICOND) printf(")");

            printf(" | ");

            if (r == TOK_XOR || r == TOK_IMPLIES || r == TOK_BICOND) printf("(");
            print_tree(node->right);
            if (r == TOK_XOR || r == TOK_IMPLIES || r == TOK_BICOND) printf(")");
            return;

        case TOK_XOR:
            if (l == TOK_XOR || l == TOK_IMPLIES || l == TOK_BICOND) printf("(");
            print_tree(node->left);
            if (l == TOK_XOR || l == TOK_IMPLIES || l == TOK_BICOND) printf(")");

            printf(" <-|-> ");

            if (r == TOK_IMPLIES || r == TOK_BICOND) printf("(");
            print_tree(node->right);
            if (r == TOK_IMPLIES || r == TOK_BICOND) printf(")");
            return;

        case TOK_BICOND:
            if (l == TOK_BICOND) printf("(");
            print_tree(node->left);
            if (l == TOK_BICOND) printf(")");

            printf(" <-> ");

            print_tree(node->right);
            return;

        case TOK_IMPLIES:
            if (l == TOK_BICOND) printf("(");
            print_tree(node->left);
            if (l == TOK_BICOND) printf(")");

            printf(" -> ");

            if (r == TOK_IMPLIES || r == TOK_BICOND) printf("(");
            print_tree(node->right);
            if (r == TOK_IMPLIES || r == TOK_BICOND) printf(")");
            return;
    }
}

void print_path(Node* node) {

    if (node->parent_tree != NULL) {
        print_path(node->parent_tree);
        printf("\n\n [%s] \n\n", node->rule_used);
    }
   
    print_tree(node); 
}
 
void collect_vars(Node* node, char* vars, int* count) {
    if (node == NULL) return;

    if (node->Nodetype.type == TOK_VAR) {

        for (int i = 0; i < *count; i++)
        {
            if (vars[i] == node->Nodetype.var) return;
        }

        vars[(*count)++] = node->Nodetype.var;
    }

    collect_vars(node->left,  vars, count);
    collect_vars(node->right, vars, count);
}
 
int eval_tree(Node* node, char* vars, int* vals, int count) {
    if (node == NULL) return 0;
 
    switch (node->Nodetype.type) {
        case TOK_TRUE:    return 1;

        case TOK_FALSE:   return 0;

        case TOK_VAR:
            for (int i = 0; i < count; i++)
            {
                if (vars[i] == node->Nodetype.var) return vals[i];
            }
            return 0;

        case TOK_NOT:     return !eval_tree(node->right, vars, vals, count);

        case TOK_AND:     return  eval_tree(node->left, vars, vals, count) & eval_tree(node->right, vars, vals, count);

        case TOK_OR:      return  eval_tree(node->left, vars, vals, count) | eval_tree(node->right, vars, vals, count);

        case TOK_IMPLIES: return !eval_tree(node->left, vars, vals, count) | eval_tree(node->right, vars, vals, count);

        case TOK_BICOND:  return  eval_tree(node->left, vars, vals, count) == eval_tree(node->right, vars, vals, count);

        case TOK_XOR:     return  eval_tree(node->left, vars, vals, count) != eval_tree(node->right, vars, vals, count);

        default: return 0;
    }
}
 
void print_truth_table(Node* start, Node* end) {
 
    char vars[26];
    int  count = 0;
 
    collect_vars(start, vars, &count);
    collect_vars(end,   vars, &count);
 
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (vars[i] > vars[j]) 
            { 
                char tmp = vars[i]; 
                vars[i] = vars[j]; 
                vars[j] = tmp; 
            }
        }
    }
 
    printf("\n Truth Table: \n");
    for (int i = 0; i < count; i++) printf("  %c", vars[i]);
    printf("  | Start | End \n");
 
    int vals[26];
    int rows = 1 << count;
 
    for (int r = 0; r < rows; r++) 
    {
        for (int i = 0; i < count; i++) 
        {
            vals[i] = (r >> (count - 1 - i)) & 1;
            printf("  %d", vals[i]);
        }
        
        printf("  |   %d   |  %d \n", eval_tree(start, vars, vals, count), eval_tree(end, vars, vals, count));
    }
    printf("\n");
}
