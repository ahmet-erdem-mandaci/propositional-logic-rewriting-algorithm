#include "core.h"

void next_token(Lexer* lex) {

    while (isspace(lex->s[lex->pos])) { 
        lex->pos++;
    }

    char c = lex->s[lex->pos]; 

    if (c == '\0') {
        lex->current.type = TOK_END; 
        return;
    }

    if( c == 't' || c == 'T'){
        lex->current.type = TOK_TRUE;
        lex->pos++;
        return;
    }

    if( c == 'f' || c == 'F'){
        lex->current.type = TOK_FALSE;
        lex->pos++;
        return;       
    }

    if (c >= 'a' && c <= 'z') {
        lex->current.type = TOK_VAR;
        lex->current.var = c;
        lex->pos++;
        return;
    }

    int p = lex->pos;
    while (isspace(lex->s[p])) p++;

    if (lex->s[p]=='<' && lex->s[p+1]=='-' && lex->s[p+2]=='>') {
        lex->current.type = TOK_BICOND;
        lex->pos = p+3; 
        return;
    }
    if (lex->s[p]=='<' && lex->s[p+1]=='-' && lex->s[p+2]=='|' && lex->s[p+3]=='-' && lex->s[p+4]=='>') {
        lex->current.type = TOK_XOR;
        lex->pos = p+5;
        return;
    }
    if (lex->s[p]=='-' && lex->s[p+1]=='>') {
        lex->current.type = TOK_IMPLIES;
        lex->pos = p+2;
        return;
    }


    if (c == '!') { lex->current.type = TOK_NOT;  lex->pos++; return; }
    if (c == '&') { lex->current.type = TOK_AND;  lex->pos++; return; }
    if (c == '|') { lex->current.type = TOK_OR;   lex->pos++; return; }
    if (c == '(') { lex->current.type = TOK_LPAR; lex->pos++; return; }
    if (c == ')') { lex->current.type = TOK_RPAR; lex->pos++; return; }

    printf("Lexer error: unknown character '%c'\n", c);
    exit(1);
}


bool check_syntax(Token* tokens, int count) {
    bool valid = true;

    if (!(tokens[0].type == TOK_VAR ||
          tokens[0].type == TOK_TRUE ||
          tokens[0].type == TOK_FALSE ||
          tokens[0].type == TOK_NOT ||
          tokens[0].type == TOK_LPAR)) {
        return false;
    }

    for (int i = 0; valid && tokens[i].type != TOK_END; i++) {

        Tokentype t1 = tokens[i].type;
        Tokentype t2 = tokens[i + 1].type;

        if (t1 == TOK_VAR || t1 == TOK_TRUE || t1 == TOK_FALSE) {
            if (!(t2 == TOK_AND || t2 == TOK_OR || t2 == TOK_IMPLIES ||
                  t2 == TOK_BICOND || t2 == TOK_XOR ||
                  t2 == TOK_RPAR || t2 == TOK_END))
                valid = false;
        }

        else if (t1 == TOK_NOT) {
            if (!(t2 == TOK_VAR || t2 == TOK_TRUE || t2 == TOK_FALSE || t2 == TOK_LPAR || t2 == TOK_NOT))
                valid = false;
        }

        else if (t1 == TOK_LPAR) {
            if (!(t2 == TOK_VAR || t2 == TOK_TRUE || t2 == TOK_FALSE || t2 == TOK_NOT || t2 == TOK_LPAR))
                valid = false;
        }

        else if (t1 == TOK_RPAR) {
            if (!(t2 == TOK_AND || t2 == TOK_OR || t2 == TOK_IMPLIES ||
                  t2 == TOK_BICOND || t2 == TOK_XOR ||
                  t2 == TOK_RPAR || t2 == TOK_END))
                valid = false;
        }

        else if (t1 == TOK_AND || t1 == TOK_OR ||
                 t1 == TOK_IMPLIES || t1 == TOK_BICOND || t1 == TOK_XOR) {
            if (!(t2 == TOK_VAR || t2 == TOK_TRUE || t2 == TOK_FALSE || t2 == TOK_NOT || t2 == TOK_LPAR))
                valid = false;
        }
    }
    return valid;
}

Node* get_Node(Token* tokens, int start, int end) {

    Node* newNode = calloc(1, sizeof(Node));
    
    if (start == end) {
        newNode->Nodetype = tokens[start]; 
        return newNode;
    }

    int d = 0;

    for (int i = start; i <= end; i++) {
        if (tokens[i].type == TOK_LPAR) d++;
        if (tokens[i].type == TOK_RPAR) d--;

        if (tokens[i].type == TOK_BICOND && d == 0) {
            newNode->Nodetype = tokens[i];
            newNode->left = get_Node(tokens, start, i-1);
            newNode->right = get_Node(tokens, i+1, end);
            return newNode;
        }
    }

    d = 0;
    for (int i = end; i >= start; i--) {
        if (tokens[i].type == TOK_LPAR) d++;
        if (tokens[i].type == TOK_RPAR) d--;

        if (tokens[i].type == TOK_IMPLIES && d == 0) {
            newNode->Nodetype = tokens[i];
            newNode->left = get_Node(tokens, start, i-1);
            newNode->right = get_Node(tokens, i+1, end);
            return newNode;
        }
    }

    d = 0;
    for (int i = start; i <= end; i++) {
        if (tokens[i].type == TOK_LPAR) d++;
        if (tokens[i].type == TOK_RPAR) d--;

        if (tokens[i].type == TOK_XOR && d == 0) {
            newNode->Nodetype = tokens[i];
            newNode->left = get_Node(tokens, start, i-1);
            newNode->right = get_Node(tokens, i+1, end);
            return newNode;
        }
    }

    d = 0;
    for (int i = start; i <= end; i++) {
        if (tokens[i].type == TOK_LPAR) d++;
        if (tokens[i].type == TOK_RPAR) d--;

        if (tokens[i].type == TOK_OR && d == 0) {
            newNode->Nodetype = tokens[i];
            newNode->left = get_Node(tokens, start, i-1);
            newNode->right = get_Node(tokens, i+1, end);
            return newNode;
        }
    }

    d = 0;
    for (int i = start; i <= end; i++) {
        if (tokens[i].type == TOK_LPAR) d++;
        if (tokens[i].type == TOK_RPAR) d--;

        if (tokens[i].type == TOK_AND && d == 0) {
            newNode->Nodetype = tokens[i];
            newNode->left = get_Node(tokens, start, i-1);
            newNode->right = get_Node(tokens, i+1, end);
            return newNode;
        }
    }

    d = 0;
    for (int i = start; i <= end; i++) {
        if (tokens[i].type == TOK_LPAR) d++;
        if (tokens[i].type == TOK_RPAR) d--;

        if (tokens[i].type == TOK_NOT && d == 0) {
            newNode->Nodetype = tokens[i];
            newNode->right = get_Node(tokens, i+1, end);
            return newNode;
        }
    }

    if (tokens[start].type == TOK_LPAR &&
        tokens[end].type == TOK_RPAR) {
        free(newNode);
        return get_Node(tokens, start + 1, end - 1);
    }

    return NULL;

}