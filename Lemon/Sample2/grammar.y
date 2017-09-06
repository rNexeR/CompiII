%include{
    #include <stdio.h>
    #include <assert.h>
}

%token_type {int}
%default_type {int}

%start_symbol start

start ::= expr(E) . { printf("Resultado: %d\n", E); }


expr(E) ::= expr(L) OP_ADD term(R) . { E = L + R; } 
expr(E) ::= expr(L) OP_SUB term(R) . { E = L - R; }
expr(E) ::= term(T) . { E = T; }

term(T) ::= term(L) OP_MULT factor(R) . { T = L * R; }
term(T) ::= term(L) OP_DIV factor(R) .  { T = L / R; }
term(T) ::= factor(L) . { T = L; }

factor(F) ::= TK_NUMBER(N) . { F = N; }
factor(F) ::= TK_L_PAR expr(E) TK_R_PAR . { F = E; }