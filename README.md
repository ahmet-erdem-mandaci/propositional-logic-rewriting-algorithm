# propositional-logic-rewriting-algorithm
B. Sc. Thesis - Automated Equivalence Transformations for Propositional Logic - Rewrite-Based Derivation of Propositional Formulas

## Contents
- `core.h` – data structures and function prototypes
- `main.c` – main program
- `parser.c` – lexer and parser
- `rewrite.c` – generation control and rule application
- `rules.c` – rewriting rules
- `tree.c` – tree operations
- `test.c` – output and truth table functions

## Compilation
```bash
gcc -o logic main.c parser.c rewrite.c rules.c tree.c test.c
