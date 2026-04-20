#include "core.h"
 
Node* (*rules[])(Node*) = {
    A1, A2, A2_Back, A3, A3_Back, A4, A4_Back_1, A4_Back_2, A5, A6, A6_Back_1, A6_Back_2, A7, A8, A9, A9_Back, A10, A10_Backward,
    B1, B2, B2_Back, B3, B3_Back, B4, B4_Back, B5_Above, B5_Above_Back, B5_Below_1, B5_Below_2, B5_Below_Back_1, B5_Below_Back_2, B6, B7, B7_Back, B8, B8_Back, B9, B9_Back, B10, B10_Back, B11, B11_Back, B12, B12_Back, B13, B13_Back, B14, B14_Back,
    C1, C2, C2_Back, C3, C3_Back, C4, C4_Back, C5_Above, C5_Above_Back, C5_Below_1, C5_Below_2, C5_Below_Back_1, C5_Below_Back_2, C6, C7, C7_Back, C8, C8_Back, C9, C9_Back, C10, C10_Back, C11, C11_Back, C12, C12_Back, C13, C13_Back, C14, C14_Back,
    D1, D1_Back, D2, D2_Back, D3, D3_Back, D4, D5, D5_Left_Back, D5_Right_Back, D6, D6_Middle_Back, D6_Right_Back, D7, D7_Back, D8, D8_Back, D9_Above, D9_Below, D9_Below_Back, D10, D10_Back, D11, D11_Back, D12_1, D12_2, D12_1_Back, D12_2_Back, D13_1, D13_2, D13_1_Back, D13_2_Back, D14, D14_Back,
    E2, E4, E6, E9, E10, E11, E12, E13, E14, E15, E16, E17, E18, E19, E21
};
 
const char* rule_names[] = {
    "A1", "A2", "A2", "A3", "A3", "A4", "A4", "A4", "A5", "A6", "A6", "A6", "A7", "A8", "A9", "A9", "A10", "A10", 
    "B1", "B2", "B2", "B3", "B3", "B4", "B4", "B5", "B5", "B5", "B5", "B5", "B5", "B6", "B7", "B7", "B8", "B8", "B9", "B9", "B10", "B10", "B11", "B11", "B12", "B12", "B13", "B13", "B14", "B14",
    "C1", "C2", "C2", "C3", "C3", "C4", "C4", "C5", "C5", "C5", "C5", "C5", "C5", "C6", "C7", "C7", "C8", "C8", "C9", "C9", "C10", "C10", "C11", "C11", "C12", "C12", "C13", "C13", "C14", "C14",
    "D1", "D1", "D2", "D2", "D3", "D3", "D4", "D5", "D5", "D5", "D6", "D6", "D6", "D7", "D7", "D8", "D8", "D9", "D9", "D9", "D10", "D10", "D11", "D11", "D12", "D12", "D12", "D12", "D13", "D13", "D13", "D13", "D14", "D14",
    "E2", "E4", "E6", "E9", "E10", "E11", "E12", "E13", "E14", "E15", "E16", "E17", "E18", "E19", "E21"
};
static const int RULE_COUNT = sizeof(rules) / sizeof(rules[0]);
 
static int cmp_similarity(const void* a, const void* b) {
 
    Node* na = *(Node**)a;
    Node* nb = *(Node**)b;
 
    if (nb->similarity > na->similarity) return 1;
    if (nb->similarity < na->similarity) return -1;
    return 0;
}
 
bool create_all_equivalent(AllGenerations* gens, Node* end) {
 
    for (int i = 0; i < gens->generation_count && i< GEN_LIMIT; i++) {
        printf("Processing generation %d (%d trees)\n", i, gens->generations[i].tree_count);
 
        Generation next_gen;
        next_gen.tree_count = 0;
        next_gen.tree_capacity = 1;
        next_gen.tree_array = calloc(next_gen.tree_capacity, sizeof(Node*));
 
        for (int j = 0; j < gens->generations[i].tree_count; j++)
        {
            Node* tree = gens->generations[i].tree_array[j];
 
            if (try_all_rules_on_all_nodes(tree, tree, gens, &next_gen, end, true)) {
                free_gen(&next_gen);   
                return true;
            }
        }
 
        if (next_gen.tree_count == 0) 
        {
            free_gen(&next_gen); 
            break; 
        }
 
        if (next_gen.tree_count > BEAM_WIDTH) 
        {
            qsort(next_gen.tree_array, next_gen.tree_count, sizeof(Node*), cmp_similarity);
 
            for (int k = BEAM_WIDTH; k < next_gen.tree_count; k++) 
            {
                free_tree(next_gen.tree_array[k]); 
            }
 
            next_gen.tree_count = BEAM_WIDTH;
        }
 
        add_gen_to_all_gens(gens, &next_gen);
    }
 
    return false;
}
 
bool try_all_rules_on_all_nodes(Node* tree, Node* current_node, AllGenerations* gens, Generation* next_gen, Node* end, bool ancestors_and_or) {
    if (current_node == NULL) return false;

    bool current_is_and_or = (current_node->Nodetype.type == TOK_AND || current_node->Nodetype.type == TOK_OR);

    if (try_all_rules_on_all_nodes(tree, current_node->left,  gens, next_gen, end, ancestors_and_or && current_is_and_or)) return true;
    if (try_all_rules_on_all_nodes(tree, current_node->right, gens, next_gen, end, ancestors_and_or && current_is_and_or)) return true;
 
    for (int i = 0; i < RULE_COUNT; i++) {

        if (rule_names[i][0] == 'E' && !ancestors_and_or) continue;
 
        Node* new_sub = rules[i](current_node);
        if (!new_sub) continue;
 
        Node* new_tree = copy_tree_and_replace(tree, current_node, new_sub);
        free_tree(new_sub);  
 
        int oldsize = tree->size;
        new_tree->size = tree_size(new_tree);
        tree_hash(new_tree);
        new_tree->similarity = similarity(new_tree, end);
 
        new_tree->parent_tree = tree;
        new_tree->rule_used = rule_names[i];
 
        if(new_tree->similarity == 1 && new_tree->hash == end->hash) {
            printf("\n Target found! Derivation path:\n\n");          
            print_path(new_tree);
            Node* root = tree;
            while (root->parent_tree != NULL) root = root->parent_tree;
            print_truth_table(root, end);
            free_tree(new_tree);
            return true; 
        }
 
        if (new_tree->size > MAX_SIZE || new_tree->size > oldsize + MAX_GROWTH) {
            free_tree(new_tree);
            continue;
        }
 
        if (is_tree_unique(new_tree))
            add_tree_to_gen(next_gen, new_tree);
        else
            free_tree(new_tree);
    }
 
    return false;
}
 
Node* copy_tree_and_replace(Node* original, Node* target, Node* replacement) {
    if (original == NULL) return NULL;
 
    if (original == target) {
        return copy_tree(replacement);
    }
 
    Node* new_node = calloc(1, sizeof(Node));
    if (!new_node) exit(1);
 
    new_node->Nodetype.type = original->Nodetype.type;
    if (original->Nodetype.type == TOK_VAR) {
        new_node->Nodetype.var = original->Nodetype.var;
    }
 
    new_node->left = copy_tree_and_replace(original->left, target, replacement);
    new_node->right = copy_tree_and_replace(original->right, target, replacement);
    return new_node;
}
 
int tree_diff_count(Node* a, Node* b) {
    if (a == NULL && b == NULL) return 0;
    if (a == NULL) return tree_size(b);
    if (b == NULL) return tree_size(a);
 
    int cost = 0;

    if (a->Nodetype.type != b->Nodetype.type) cost = 1;
    else if (a->Nodetype.type == TOK_VAR && a->Nodetype.var != b->Nodetype.var) cost = 1;
 
    return cost + tree_diff_count(a->left, b->left) + tree_diff_count(a->right, b->right);
}
 
double similarity(Node* a, Node* b) {

    int size = tree_size(a) + tree_size(b);

    if (size == 0) return 1.0;
    else return 1.0 - ((double)tree_diff_count(a, b) / size);
}
