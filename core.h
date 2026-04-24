#ifndef core_h
#define core_h

//Includes
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


//Type Definitions

typedef enum {
    TOK_END,
    TOK_VAR,
    TOK_AND,
    TOK_OR,
    TOK_NOT,
    TOK_IMPLIES,
    TOK_BICOND,
    TOK_XOR,
    TOK_LPAR,
    TOK_RPAR,
    TOK_TRUE,
    TOK_FALSE,
} Tokentype;

typedef struct {
    Tokentype type;
    char var;
} Token;

typedef struct {
    const char* s;
    int pos;
    Token current;
} Lexer;

typedef struct Node{
    Token Nodetype;
    struct Node* left;
    struct Node* right;
    unsigned long long hash;
    int size;
    double similarity;
    struct Node* parent_tree;
    const char* rule_used;
} Node;

typedef struct {
    Node** tree_array;
    int tree_count;
    int tree_capacity;
} Generation;

typedef struct {
    Generation* generations;
    int generation_count;
    int generation_capacity;
} AllGenerations;

typedef struct HashEntry{
    unsigned long long hash;
    struct HashEntry* next;
} HashEntry;


//Function Definitions

//Lexer/Parser/AST Functions
void next_token(Lexer* lex);
bool check_syntax(Token* tokens, int count);
Node* get_Node(Token* tokens, int start, int end);

//Hashing, Tree and Generation 
#define NUM_KEYS 10000003
#define FNV_OFFSET_BASIS 0xcbf29ce484222325
#define FNV_PRIME 0x00000100000001b3
#define GEN_LIMIT 50
#define BEAM_WIDTH 3000
#define MAX_SIZE 20
#define MAX_GROWTH 10
unsigned long long tree_hash(Node* node);
void clear_hash_table();
void free_tree(Node* node);
void free_gen(Generation* gen);
void free_all_gens(AllGenerations* gens);
bool identical_or_not(Node* a, Node* b);
bool is_tree_unique(Node* node);
Node* copy_tree(Node* node);
void add_tree_to_gen(Generation* gen, Node* node);
void add_gen_to_all_gens(AllGenerations* gens, Generation* gen);
int tree_size(Node* node);

//Rewriting Algorithm Functions
Node* copy_tree_and_replace(Node* original, Node* target, Node* replacement);
bool try_all_rules_on_all_nodes(Node* tree, Node* current_node, AllGenerations* gens, Generation* next_gen, Node* end, bool ancestors_and_or);
bool create_all_equivalent(AllGenerations* gens, Node* end);
int tree_diff_count(Node* a, Node* b);
double similarity(Node* a, Node* b);

//Printing and Testing Functions
void print_tree(Node* node);
void print_path(Node* node);
void collect_vars(Node* node, char* vars, int* count);
int eval_tree(Node* node, char* vars, int* vals, int count);
void print_truth_table(Node* start, Node* end);

//Rewriting/Equivalence Rules Functions
Node* A1(Node* node);
Node* A2(Node* node);
Node* A2_Back(Node* node);
Node* A3(Node* node);
Node* A3_Back(Node* node);
Node* A4(Node* node);
Node* A4_Back_1(Node* node);
Node* A4_Back_2(Node* node);
Node* A5(Node* node);
Node* A6(Node* node);
Node* A6_Back_1(Node* node);
Node* A6_Back_2(Node* node);
Node* A7(Node* node);
Node* A8(Node* node);
Node* A9(Node* node);
Node* A9_Back(Node* node);
Node* A10(Node* node);
Node* A10_Backward(Node* node);
Node* B1(Node* node);
Node* B2(Node* node);
Node* B2_Back(Node* node);
Node* B3(Node* node);
Node* B3_Back(Node* node);
Node* B4(Node* node);
Node* B4_Back(Node* node);
Node* B5_Above(Node* node);
Node* B5_Above_Back(Node* node);
Node* B5_Below_1(Node* node);
Node* B5_Below_2(Node* node);
Node* B5_Below_Back_1(Node* node);
Node* B5_Below_Back_2(Node* node);
Node* B6(Node* node);
Node* B7(Node* node);
Node* B7_Back(Node* node);
Node* B8(Node* node);
Node* B8_Back(Node* node);
Node* B9(Node* node);
Node* B9_Back(Node* node);
Node* B10(Node* node);
Node* B10_Back(Node* node);
Node* B11(Node* node);
Node* B11_Back(Node* node);
Node* B12(Node* node);
Node* B12_Back(Node* node);
Node* B13(Node* node);
Node* B13_Back(Node* node);
Node* B14(Node* node);
Node* B14_Back(Node* node);
Node* C1(Node* node);
Node* C2(Node* node);
Node* C2_Back(Node* node);
Node* C3(Node* node);
Node* C3_Back(Node* node);
Node* C4(Node* node);
Node* C4_Back(Node* node);
Node* C5_Above(Node* node);
Node* C5_Above_Back(Node* node);
Node* C5_Below_1(Node* node);
Node* C5_Below_2(Node* node);
Node* C5_Below_Back_1(Node* node);
Node* C5_Below_Back_2(Node* node);
Node* C6(Node* node);
Node* C7(Node* node);
Node* C7_Back(Node* node);
Node* C8(Node* node);
Node* C8_Back(Node* node);
Node* C9(Node* node);
Node* C9_Back(Node* node);
Node* C10(Node* node);
Node* C10_Back(Node* node);
Node* C11(Node* node);
Node* C11_Back(Node* node);
Node* C12(Node* node);
Node* C12_Back(Node* node);
Node* C13(Node* node);
Node* C13_Back(Node* node);
Node* C14(Node* node);
Node* C14_Back(Node* node);
Node* D1(Node* node);
Node* D1_Back(Node* node);
Node* D2(Node* node);
Node* D2_Back(Node* node);
Node* D3(Node* node);
Node* D3_Back(Node* node);
Node* D4(Node* node);
Node* D5(Node* node);
Node* D5_Left_Back(Node* node);
Node* D5_Right_Back(Node* node);
Node* D6(Node* node);
Node* D6_Middle_Back(Node* node);
Node* D6_Right_Back(Node* node);
Node* D7(Node* node);
Node* D7_Back(Node* node);
Node* D8(Node* node);
Node* D8_Back(Node* node);
Node* D9_Above(Node* node);
Node* D9_Below(Node* node);
Node* D9_Below_Back(Node* node);
Node* D10(Node* node);
Node* D10_Back(Node* node);
Node* D11(Node* node);
Node* D11_Back(Node* node);
Node* D12_1(Node* node);
Node* D12_2(Node* node);
Node* D12_1_Back(Node* node);
Node* D12_2_Back(Node* node);
Node* D13_1(Node* node);
Node* D13_2(Node* node);
Node* D13_1_Back(Node* node);
Node* D13_2_Back(Node* node);
Node* D14(Node* node);
Node* D14_Back(Node* node);
Node* E2(Node* node);
Node* E4(Node* node);
Node* E6(Node* node);
Node* E9(Node* node);
Node* E10(Node* node);
Node* E11(Node* node);
Node* E12(Node* node);
Node* E13(Node* node);
Node* E14(Node* node);
Node* E15(Node* node);
Node* E16(Node* node);
Node* E17(Node* node);
Node* E18(Node* node);
Node* E19(Node* node);
Node* E21(Node* node);

#endif
