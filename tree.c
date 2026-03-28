#include "core.h"

static HashEntry* hash_table[NUM_KEYS] = {0};

bool is_tree_unique(Node* node) {

    unsigned long long h = tree_hash(node);
    unsigned long long index = h % NUM_KEYS;

    HashEntry* current = hash_table[index];
    while (current) {
        if (current->hash == h) return false;
        current = current->next;
    }

    HashEntry* entry = calloc(1, sizeof(HashEntry));
    if (!entry) 
    { 
        printf("Memory allocation fail!"); exit(1); 
    }

    entry->hash = h;
    entry->next = hash_table[index];
    hash_table[index] = entry;
    return true;
}

unsigned long long tree_hash(Node* node) {
    if (!node) return FNV_OFFSET_BASIS;

    unsigned long long h = FNV_OFFSET_BASIS;
    h ^= node->Nodetype.type;
    h *= FNV_PRIME;

    if (node->Nodetype.type == TOK_VAR) {
        h ^= node->Nodetype.var;
        h *= FNV_PRIME;
    }

    h ^= tree_hash(node->left);
    h *= FNV_PRIME;
    h ^= tree_hash(node->right);
    h *= FNV_PRIME;
    node->hash = h;
    return h;
}

void free_tree(Node* node) {
    if (node == NULL) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

void free_gen(Generation* gen) {
    if (gen == NULL) return;

    for (int i = 0; i < gen->tree_count; i++)
    {
        free_tree(gen->tree_array[i]);
    }

    free(gen->tree_array);
}

void free_all_gens(AllGenerations* gens) {
    if (gens == NULL) return;

    for (int i = 0; i < gens->generation_count; i++)
    {
        free_gen(&gens->generations[i]);
    }

    free(gens->generations);
}

void add_tree_to_gen(Generation* gen, Node* node) {

    if (gen->tree_count >= gen->tree_capacity) 
    {
        int new_cap = gen->tree_capacity * 2;

        Node** temp = realloc(gen->tree_array, new_cap * sizeof(Node*));
        if (!temp) 
        { 
            printf("Memory allocation fail !"); 
            exit(1); 
        }

        gen->tree_array = temp;
        gen->tree_capacity = new_cap;
    }

    gen->tree_array[gen->tree_count] = node;
    gen->tree_count++;
}

void add_gen_to_all_gens(AllGenerations* gens, Generation* gen) {
    
    if (gens->generation_count >= gens->generation_capacity) 
    {
        int new_cap = gens->generation_capacity * 2;

        Generation* temp = realloc(gens->generations, new_cap * sizeof(Generation));
        if (!temp) 
        { 
            printf("Memory allocation fail !"); 
            exit(1); 
        }

        gens->generations = temp;
        gens->generation_capacity = new_cap;
    }

    gens->generations[gens->generation_count] = *gen;
    gens->generation_count++;
}

Node* copy_tree(Node* node) {
    if (node == NULL) return NULL;

    Node* new = calloc(1, sizeof(Node));
    if (!new) 
    { 
        printf("Memory allocation fail !"); 
        exit(1); 
    }

    new->Nodetype.type = node->Nodetype.type;

    if (node->Nodetype.type == TOK_VAR) new->Nodetype.var = node->Nodetype.var;

    new->left  = copy_tree(node->left);
    new->right = copy_tree(node->right);
    return new;
}

bool identical_or_not(Node* a, Node* b) {
    if (a == NULL && b == NULL) return true;

    if (a == NULL || b == NULL) return false;

    if (a->Nodetype.type != b->Nodetype.type) return false;

    if (a->Nodetype.type == TOK_VAR && a->Nodetype.var != b->Nodetype.var) return false;

    return identical_or_not(a->left, b->left) && identical_or_not(a->right, b->right);
}

void clear_hash_table() {
    for (int i = 0; i < NUM_KEYS; i++) 
    {
        HashEntry* current = hash_table[i];
        while (current) {
            HashEntry* next = current->next;
            free(current);
            current = next;
        }
        hash_table[i] = NULL;
    }
}

int tree_size(Node* node) {
    if (node == NULL) return 0;

    return 1 + tree_size(node->left) + tree_size(node->right);
}