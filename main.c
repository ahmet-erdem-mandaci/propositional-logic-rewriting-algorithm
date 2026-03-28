#include "core.h"
#include <time.h>

int main() {

    clock_t start_time = clock();

    char input[512];

    printf("Expression example:\n");
    printf("!(a & b) -> (c <-|-> d) <-> (e | d) \n");
    printf("t or T for true, f or F for false (dont use t,T,f,F for variable)\n");

    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    int max_tokens = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (!isspace(input[i])) max_tokens++;
    }
    max_tokens++;

    Token* tokens = calloc(max_tokens, sizeof(Token));

    Lexer* lex = calloc(1, sizeof(Lexer));

    lex->s = input;
    lex->pos = 0;

    int count = 0;
    int lparcount = 0;
    int rparcount = 0;

    while (1) {
        next_token(lex);
        tokens[count] = lex->current;

        if (lex->current.type == TOK_LPAR) lparcount++;
        if (lex->current.type == TOK_RPAR) rparcount++;
        if (lex->current.type == TOK_END) break;

        count++;
    }

    if (!check_syntax(tokens, count) || lparcount != rparcount || count == 0) {
        printf("Syntax error !");
        exit(1);
    }

    Node* root = get_Node(tokens, 0, count - 1);

    char output[512];

    printf("Derivaiton expression from main expression that needs to be found :\n");

    fgets(output, sizeof(output), stdin);
    output[strcspn(output, "\n")] = '\0';

    int max_tokens_2 = 0;
    for (int i = 0; output[i] != '\0'; i++) {
        if (!isspace(output[i])) max_tokens_2++;
    }
    max_tokens_2++;

    Token* tokens_2 = calloc(max_tokens_2, sizeof(Token));

    Lexer* lex_2 = calloc(1, sizeof(Lexer));

    lex_2->s = output;
    lex_2->pos = 0;

    int count_2 = 0;
    int lparcount_2 = 0;
    int rparcount_2 = 0;

    while (1) {
        next_token(lex_2);
        tokens_2[count_2] = lex_2->current;

        if (lex_2->current.type == TOK_LPAR) lparcount_2++;
        if (lex_2->current.type == TOK_RPAR) rparcount_2++;
        if (lex_2->current.type == TOK_END) break;

        count_2++;
    }

    if (!check_syntax(tokens_2, count_2) || lparcount_2 != rparcount_2 || count_2 == 0) {
        printf("Syntax error !");
        exit(1);
    }

    Node* end = get_Node(tokens_2, 0, count_2 - 1);
    end->hash = tree_hash(end);

    root->size = tree_size(root);
    root->similarity = similarity(root, end);

    AllGenerations gens;
    gens.generation_count = 0;
    gens.generation_capacity = 1;
    gens.generations = calloc(gens.generation_capacity, sizeof(Generation));   

    Generation gen0;
    gen0.tree_count = 0;
    gen0.tree_capacity = 1;
    gen0.tree_array = calloc(gen0.tree_capacity, sizeof(Node*));

    add_tree_to_gen(&gen0, root);
    add_gen_to_all_gens(&gens, &gen0);

    bool found = create_all_equivalent(&gens, end);

    if (!found) printf("\n Could not derive target. \n");

    free_tree(end);
    free_all_gens(&gens);
    free(tokens);
    free(tokens_2);
    free(lex);
    free(lex_2);
    clear_hash_table();

    clock_t end_time = clock();
    printf("\n Total runtime: %.3f seconds \n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
    return found ? 0 : 1;
}