/*
** 2000-05-29
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Driver template for the LEMON parser generator.
**
** The "lemon" program processes an LALR(1) input grammar file, then uses
** this template to construct a parser.  The "lemon" program inserts text
** at each "%%" line.  Also, any "P-a-r-s-e" identifer prefix (without the
** interstitial "-" characters) contained in this template is changed into
** the value of the %name directive from the grammar.  Otherwise, the content
** of this template is copied straight through into the generate parser
** source file.
**
** The following is the concatenation of all %include directives from the
** input grammar file:
*/
#include <stdio.h>
/************ Begin %include sections from the grammar ************************/
#line 3 "grammar.y"

    #include "ast.h"
    #include <stdio.h>
    #include <assert.h>

    int yylex();
    extern int yylineno;
    extern char* yy_filename;
#line 37 "expression_parser.cpp"
/**************** End of %include directives **********************************/
/* These constants specify the various numeric values for terminal symbols
** in a format understandable to "makeheaders".  This section is blank unless
** "lemon" is run with the "-m" command-line option.
***************** Begin makeheaders token definitions *************************/
/**************** End makeheaders token definitions ***************************/

/* The next sections is a series of control #defines.
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used to store the integer codes
**                       that represent terminal and non-terminal symbols.
**                       "unsigned char" is used if there are fewer than
**                       256 symbols.  Larger types otherwise.
**    YYNOCODE           is a number of type YYCODETYPE that is not used for
**                       any terminal or nonterminal symbol.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       (also known as: "terminal symbols") have fall-back
**                       values which should be used if the original symbol
**                       would not parse.  This permits keywords to sometimes
**                       be used as identifiers, for example.
**    YYACTIONTYPE       is the data type used for "action codes" - numbers
**                       that indicate what to do in response to the next
**                       token.
**    NxParserTOKENTYPE     is the data type used for minor type for terminal
**                       symbols.  Background: A "minor type" is a semantic
**                       value associated with a terminal or non-terminal
**                       symbols.  For example, for an "ID" terminal symbol,
**                       the minor type might be the name of the identifier.
**                       Each non-terminal can have a different minor type.
**                       Terminal symbols all have the same minor type, though.
**                       This macros defines the minor type for terminal 
**                       symbols.
**    YYMINORTYPE        is the data type used for all minor types.
**                       This is typically a union of many types, one of
**                       which is NxParserTOKENTYPE.  The entry in the union
**                       for terminal symbols is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    NxParserARG_SDECL     A static variable declaration for the %extra_argument
**    NxParserARG_PDECL     A parameter declaration for the %extra_argument
**    NxParserARG_STORE     Code to store %extra_argument into yypParser
**    NxParserARG_FETCH     Code to extract %extra_argument from yypParser
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YY_MAX_SHIFT       Maximum value for shift actions
**    YY_MIN_SHIFTREDUCE Minimum value for shift-reduce actions
**    YY_MAX_SHIFTREDUCE Maximum value for shift-reduce actions
**    YY_MIN_REDUCE      Maximum value for reduce actions
**    YY_ERROR_ACTION    The yy_action[] code for syntax error
**    YY_ACCEPT_ACTION   The yy_action[] code for accept
**    YY_NO_ACTION       The yy_action[] code for no-op
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/************* Begin control #defines *****************************************/
#define YYCODETYPE unsigned char
#define YYNOCODE 43
#define YYACTIONTYPE unsigned char
#define NxParserTOKENTYPE  Token_t * 
typedef union {
  int yyinit;
  NxParserTOKENTYPE yy0;
  int yy4;
  Expr * yy74;
  Statement * yy80;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define NxParserARG_SDECL
#define NxParserARG_PDECL
#define NxParserARG_FETCH
#define NxParserARG_STORE
#define YYNSTATE             35
#define YYNRULE              35
#define YY_MAX_SHIFT         34
#define YY_MIN_SHIFTREDUCE   61
#define YY_MAX_SHIFTREDUCE   95
#define YY_MIN_REDUCE        96
#define YY_MAX_REDUCE        130
#define YY_ERROR_ACTION      131
#define YY_ACCEPT_ACTION     132
#define YY_NO_ACTION         133
/************* End control #defines *******************************************/

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N <= YY_MAX_SHIFT             Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   N between YY_MIN_SHIFTREDUCE       Shift to an arbitrary state then
**     and YY_MAX_SHIFTREDUCE           reduce by rule N-YY_MIN_SHIFTREDUCE.
**
**   N between YY_MIN_REDUCE            Reduce by rule N-YY_MIN_REDUCE
**     and YY_MAX_REDUCE

**   N == YY_ERROR_ACTION               A syntax error has occurred.
**
**   N == YY_ACCEPT_ACTION              The parser accepts its input.
**
**   N == YY_NO_ACTION                  No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as
**
**      yy_action[ yy_shift_ofst[S] + X ]
**
** If the index value yy_shift_ofst[S]+X is out of range or if the value
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X or if yy_shift_ofst[S]
** is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
** and that yy_default[S] should be used instead.  
**
** The formula above is for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
**
*********** Begin parsing tables **********************************************/
#define YY_ACTTAB_COUNT (98)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    75,   76,   77,   78,   79,   80,   73,   67,   66,   62,
 /*    10 */    70,   68,    8,   13,   12,   73,   67,   66,   91,   18,
 /*    20 */    68,   65,   67,   66,   28,   33,   68,   65,   67,   66,
 /*    30 */    19,   33,   68,   33,   90,   29,   82,   11,   74,   64,
 /*    40 */    67,   66,   27,   11,   68,   11,   10,   30,   27,   69,
 /*    50 */    27,   14,   24,   92,    7,   32,   22,   24,   92,   23,
 /*    60 */    24,   92,   94,   95,   20,   24,   92,   34,   93,    5,
 /*    70 */    21,   24,   92,   83,   84,   85,   17,    3,  132,    4,
 /*    80 */    13,   12,   13,   12,   25,   92,   26,   92,   13,   12,
 /*    90 */     1,    9,   16,   15,    2,   31,   96,    6,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     8,    9,   10,   11,   12,   13,   27,   28,   29,    1,
 /*    10 */    31,   32,   37,   21,   22,   27,   28,   29,   36,   31,
 /*    20 */    32,   27,   28,   29,   30,    2,   32,   27,   28,   29,
 /*    30 */    30,    2,   32,    2,   36,    6,   39,   14,    7,   27,
 /*    40 */    28,   29,   19,   14,   32,   14,    3,    5,   19,   33,
 /*    50 */    19,   34,   35,   36,    1,   38,   34,   35,   36,   34,
 /*    60 */    35,   36,   19,    4,   34,   35,   36,   41,   25,    1,
 /*    70 */    34,   35,   36,   16,   17,   18,   15,    1,   40,   41,
 /*    80 */    21,   22,   21,   22,   35,   36,   35,   36,   21,   22,
 /*    90 */     1,   20,   23,   24,    1,    4,    0,    3,
};
#define YY_SHIFT_USE_DFLT (-9)
#define YY_SHIFT_COUNT (34)
#define YY_SHIFT_MIN   (-8)
#define YY_SHIFT_MAX   (96)
static const signed char yy_shift_ofst[] = {
 /*     0 */     8,   29,   29,   23,   23,   31,   43,   23,   43,   43,
 /*    10 */    43,   43,   43,   43,   -8,   43,   43,   57,   42,   53,
 /*    20 */    59,   61,   67,   67,   69,   69,   69,   71,   68,   76,
 /*    30 */    89,   93,   91,   94,   96,
};
#define YY_REDUCE_USE_DFLT (-26)
#define YY_REDUCE_COUNT (19)
#define YY_REDUCE_MIN   (-25)
#define YY_REDUCE_MAX   (51)
static const signed char yy_reduce_ofst[] = {
 /*     0 */    38,  -21,  -12,   -6,    0,   12,   17,   12,   22,   25,
 /*    10 */    30,   36,   49,   51,  -25,  -18,   -2,   -3,   16,   26,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */    98,  131,  131,  131,  131,  131,  131,   97,  131,  131,
 /*    10 */   131,  131,  131,  131,  131,  131,  131,  131,  106,   98,
 /*    20 */   131,  116,  107,  121,  124,  123,  122,  131,  131,  131,
 /*    30 */   131,  131,  131,  131,  131,
};
/********** End of lemon-generated parsing tables *****************************/

/* The next table maps tokens (terminal symbols) into fallback tokens.  
** If a construct like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
**
** This feature can be used, for example, to cause some keywords in a language
** to revert to identifiers if they keyword does not apply in the context where
** it appears.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
**
** After the "shift" half of a SHIFTREDUCE action, the stateno field
** actually contains the reduce action for the second half of the
** SHIFTREDUCE.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number, or reduce action in SHIFTREDUCE */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  int yyidx;                    /* Index of top element in stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyidxMax;                 /* Maximum value of yyidx */
#endif
#ifndef YYNOERRORRECOVERY
  int yyerrcnt;                 /* Shifts left before out of the error */
#endif
  NxParserARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void NxParserTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  "$",             "TK_EOL",        "TK_RW_IF",      "TK_L_PAR",    
  "TK_R_PAR",      "TK_RW_ELSE",    "TK_L_BRACE",    "TK_R_BRACE",  
  "TK_COMP_EQUAL",  "TK_COMP_NEQUAL",  "TK_COMP_GREATER",  "TK_COMP_GREATER_EQUAL",
  "TK_COMP_LESS",  "TK_COMP_LESS_EQUAL",  "TK_RW_PRINT",   "TK_COMMA",    
  "TK_BIN",        "TK_HEX",        "TK_DEC",        "TK_VARIABLE", 
  "TK_EQUAL",      "OP_ADD",        "OP_SUB",        "OP_MULT",     
  "OP_DIV",        "TK_NUMBER",     "error",         "stmt",        
  "assign",        "print",         "statement_list",  "block_statement",
  "if_stmt",       "opt_else",      "expr",          "term",        
  "factor",        "compare_options",  "conditional_expr",  "print_option",
  "input",         "opt_eol",     
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "input ::= opt_eol statement_list opt_eol",
 /*   1 */ "opt_eol ::= TK_EOL",
 /*   2 */ "opt_eol ::=",
 /*   3 */ "statement_list ::= statement_list TK_EOL stmt",
 /*   4 */ "statement_list ::= stmt",
 /*   5 */ "stmt ::= print",
 /*   6 */ "stmt ::= assign",
 /*   7 */ "stmt ::= if_stmt",
 /*   8 */ "if_stmt ::= TK_RW_IF TK_L_PAR conditional_expr TK_R_PAR TK_EOL block_statement opt_else",
 /*   9 */ "opt_else ::= TK_RW_ELSE TK_EOL block_statement",
 /*  10 */ "opt_else ::=",
 /*  11 */ "conditional_expr ::= expr compare_options expr",
 /*  12 */ "block_statement ::= stmt",
 /*  13 */ "block_statement ::= TK_L_BRACE TK_EOL statement_list TK_EOL TK_R_BRACE",
 /*  14 */ "compare_options ::= TK_COMP_EQUAL",
 /*  15 */ "compare_options ::= TK_COMP_NEQUAL",
 /*  16 */ "compare_options ::= TK_COMP_GREATER",
 /*  17 */ "compare_options ::= TK_COMP_GREATER_EQUAL",
 /*  18 */ "compare_options ::= TK_COMP_LESS",
 /*  19 */ "compare_options ::= TK_COMP_LESS_EQUAL",
 /*  20 */ "print ::= TK_RW_PRINT expr",
 /*  21 */ "print ::= TK_RW_PRINT expr TK_COMMA print_option",
 /*  22 */ "print_option ::= TK_BIN",
 /*  23 */ "print_option ::= TK_HEX",
 /*  24 */ "print_option ::= TK_DEC",
 /*  25 */ "assign ::= TK_VARIABLE TK_EQUAL expr",
 /*  26 */ "expr ::= expr OP_ADD term",
 /*  27 */ "expr ::= expr OP_SUB term",
 /*  28 */ "expr ::= term",
 /*  29 */ "term ::= term OP_MULT factor",
 /*  30 */ "term ::= term OP_DIV factor",
 /*  31 */ "term ::= factor",
 /*  32 */ "factor ::= TK_NUMBER",
 /*  33 */ "factor ::= TK_VARIABLE",
 /*  34 */ "factor ::= TK_L_PAR expr TK_R_PAR",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.
*/
static void yyGrowStack(yyParser *p){
  int newSize;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  if( pNew ){
    p->yystack = pNew;
    p->yystksz = newSize;
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows to %d entries!\n",
              yyTracePrompt, p->yystksz);
    }
#endif
  }
}
#endif

/* Datatype of the argument to the memory allocated passed as the
** second argument to NxParserAlloc() below.  This can be changed by
** putting an appropriate #define in the %include section of the input
** grammar.
*/
#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
#endif

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to NxParser and NxParserFree.
*/
void *NxParserAlloc(void *(*mallocProc)(YYMALLOCARGTYPE)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
  if( pParser ){
    pParser->yyidx = -1;
#ifdef YYTRACKMAXSTACKDEPTH
    pParser->yyidxMax = 0;
#endif
#if YYSTACKDEPTH<=0
    pParser->yystack = NULL;
    pParser->yystksz = 0;
    yyGrowStack(pParser);
#endif
  }
  return pParser;
}

/* The following function deletes the "minor type" or semantic value
** associated with a symbol.  The symbol can be either a terminal
** or nonterminal. "yymajor" is the symbol code, and "yypminor" is
** a pointer to the value to be deleted.  The code used to do the 
** deletions is derived from the %destructor and/or %token_destructor
** directives of the input grammar.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  NxParserARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are *not* used
    ** inside the C code.
    */
/********* Begin destructor definitions ***************************************/
/********* End destructor definitions *****************************************/
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
*/
static void yy_pop_parser_stack(yyParser *pParser){
  yyStackEntry *yytos;
  assert( pParser->yyidx>=0 );
  yytos = &pParser->yystack[pParser->yyidx--];
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yy_destructor(pParser, yytos->major, &yytos->minor);
}

/* 
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void NxParserFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
#ifndef YYPARSEFREENEVERNULL
  if( pParser==0 ) return;
#endif
  while( pParser->yyidx>=0 ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  free(pParser->yystack);
#endif
  (*freeProc)((void*)pParser);
}

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int NxParserStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyidxMax;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
*/
static int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  if( stateno>=YY_MIN_REDUCE ) return stateno;
  assert( stateno <= YY_SHIFT_COUNT );
  do{
    i = yy_shift_ofst[stateno];
    if( i==YY_SHIFT_USE_DFLT ) return yy_default[stateno];
    assert( iLookAhead!=YYNOCODE );
    i += iLookAhead;
    if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
      if( iLookAhead>0 ){
#ifdef YYFALLBACK
        YYCODETYPE iFallback;            /* Fallback token */
        if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
               && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
          }
#endif
          assert( yyFallback[iFallback]==0 ); /* Fallback loop must terminate */
          iLookAhead = iFallback;
          continue;
        }
#endif
#ifdef YYWILDCARD
        {
          int j = i - iLookAhead + YYWILDCARD;
          if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
            j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
            j<YY_ACTTAB_COUNT &&
#endif
            yy_lookahead[j]==YYWILDCARD
          ){
#ifndef NDEBUG
            if( yyTraceFILE ){
              fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
                 yyTracePrompt, yyTokenName[iLookAhead],
                 yyTokenName[YYWILDCARD]);
            }
#endif /* NDEBUG */
            return yy_action[j];
          }
        }
#endif /* YYWILDCARD */
      }
      return yy_default[stateno];
    }else{
      return yy_action[i];
    }
  }while(1);
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( i!=YY_REDUCE_USE_DFLT );
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser){
   NxParserARG_FETCH;
   yypParser->yyidx--;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
/******** Begin %stack_overflow code ******************************************/
/******** End %stack_overflow code ********************************************/
   NxParserARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Print tracing information for a SHIFT action
*/
#ifndef NDEBUG
static void yyTraceShift(yyParser *yypParser, int yyNewState){
  if( yyTraceFILE ){
    if( yyNewState<YYNSTATE ){
      fprintf(yyTraceFILE,"%sShift '%s', go to state %d\n",
         yyTracePrompt,yyTokenName[yypParser->yystack[yypParser->yyidx].major],
         yyNewState);
    }else{
      fprintf(yyTraceFILE,"%sShift '%s'\n",
         yyTracePrompt,yyTokenName[yypParser->yystack[yypParser->yyidx].major]);
    }
  }
}
#else
# define yyTraceShift(X,Y)
#endif

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  NxParserTOKENTYPE yyMinor        /* The minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yyidx++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( yypParser->yyidx>yypParser->yyidxMax ){
    yypParser->yyidxMax = yypParser->yyidx;
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yyidx>=YYSTACKDEPTH ){
    yyStackOverflow(yypParser);
    return;
  }
#else
  if( yypParser->yyidx>=yypParser->yystksz ){
    yyGrowStack(yypParser);
    if( yypParser->yyidx>=yypParser->yystksz ){
      yyStackOverflow(yypParser);
      return;
    }
  }
#endif
  yytos = &yypParser->yystack[yypParser->yyidx];
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor.yy0 = yyMinor;
  yyTraceShift(yypParser, yyNewState);
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 40, 3 },
  { 41, 1 },
  { 41, 0 },
  { 30, 3 },
  { 30, 1 },
  { 27, 1 },
  { 27, 1 },
  { 27, 1 },
  { 32, 7 },
  { 33, 3 },
  { 33, 0 },
  { 38, 3 },
  { 31, 1 },
  { 31, 5 },
  { 37, 1 },
  { 37, 1 },
  { 37, 1 },
  { 37, 1 },
  { 37, 1 },
  { 37, 1 },
  { 29, 2 },
  { 29, 4 },
  { 39, 1 },
  { 39, 1 },
  { 39, 1 },
  { 28, 3 },
  { 34, 3 },
  { 34, 3 },
  { 34, 1 },
  { 35, 3 },
  { 35, 3 },
  { 35, 1 },
  { 36, 1 },
  { 36, 1 },
  { 36, 3 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  int yyruleno                 /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  NxParserARG_FETCH;
  yymsp = &yypParser->yystack[yypParser->yyidx];
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno>=0 
        && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    yysize = yyRuleInfo[yyruleno].nrhs;
    fprintf(yyTraceFILE, "%sReduce [%s], go to state %d.\n", yyTracePrompt,
      yyRuleName[yyruleno], yymsp[-yysize].stateno);
  }
#endif /* NDEBUG */

  /* Check that the stack is large enough to grow by a single entry
  ** if the RHS of the rule is empty.  This ensures that there is room
  ** enough on the stack to push the LHS value */
  if( yyRuleInfo[yyruleno].nrhs==0 ){
#ifdef YYTRACKMAXSTACKDEPTH
    if( yypParser->yyidx>yypParser->yyidxMax ){
      yypParser->yyidxMax = yypParser->yyidx;
    }
#endif
#if YYSTACKDEPTH>0 
    if( yypParser->yyidx>=YYSTACKDEPTH-1 ){
      yyStackOverflow(yypParser);
      return;
    }
#else
    if( yypParser->yyidx>=yypParser->yystksz-1 ){
      yyGrowStack(yypParser);
      if( yypParser->yyidx>=yypParser->yystksz-1 ){
        yyStackOverflow(yypParser);
        return;
      }
    }
#endif
  }

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
/********** Begin reduce actions **********************************************/
        YYMINORTYPE yylhsminor;
      case 0: /* input ::= opt_eol statement_list opt_eol */
#line 35 "grammar.y"
{ printf("-->\n"); yymsp[-1].minor.yy80->exec(); }
#line 824 "expression_parser.cpp"
        break;
      case 3: /* statement_list ::= statement_list TK_EOL stmt */
#line 40 "grammar.y"
{ yylhsminor.yy80 = yymsp[-2].minor.yy80; ((BlockStatement *)yylhsminor.yy80)->addStatement(yymsp[0].minor.yy80); }
#line 829 "expression_parser.cpp"
  yymsp[-2].minor.yy80 = yylhsminor.yy80;
        break;
      case 4: /* statement_list ::= stmt */
#line 41 "grammar.y"
{ yylhsminor.yy80 = new BlockStatement; ((BlockStatement *)yylhsminor.yy80)->addStatement(yymsp[0].minor.yy80); }
#line 835 "expression_parser.cpp"
  yymsp[0].minor.yy80 = yylhsminor.yy80;
        break;
      case 5: /* stmt ::= print */
      case 6: /* stmt ::= assign */ yytestcase(yyruleno==6);
      case 7: /* stmt ::= if_stmt */ yytestcase(yyruleno==7);
      case 12: /* block_statement ::= stmt */ yytestcase(yyruleno==12);
#line 43 "grammar.y"
{ yylhsminor.yy80 = yymsp[0].minor.yy80; }
#line 844 "expression_parser.cpp"
  yymsp[0].minor.yy80 = yylhsminor.yy80;
        break;
      case 8: /* if_stmt ::= TK_RW_IF TK_L_PAR conditional_expr TK_R_PAR TK_EOL block_statement opt_else */
#line 47 "grammar.y"
{ yymsp[-6].minor.yy80 = new IfStatement(yymsp[-4].minor.yy74, yymsp[-1].minor.yy80, yymsp[0].minor.yy80); }
#line 850 "expression_parser.cpp"
        break;
      case 9: /* opt_else ::= TK_RW_ELSE TK_EOL block_statement */
#line 49 "grammar.y"
{ yymsp[-2].minor.yy80 = yymsp[0].minor.yy80; }
#line 855 "expression_parser.cpp"
        break;
      case 10: /* opt_else ::= */
#line 50 "grammar.y"
{ yymsp[1].minor.yy80 = NULL; }
#line 860 "expression_parser.cpp"
        break;
      case 11: /* conditional_expr ::= expr compare_options expr */
#line 52 "grammar.y"
{ yylhsminor.yy74 = yymsp[-1].minor.yy74; ((BinaryExpr*)yylhsminor.yy74)->expr1 = yymsp[-2].minor.yy74; ((BinaryExpr*)yylhsminor.yy74)->expr2 = yymsp[0].minor.yy74; }
#line 865 "expression_parser.cpp"
  yymsp[-2].minor.yy74 = yylhsminor.yy74;
        break;
      case 13: /* block_statement ::= TK_L_BRACE TK_EOL statement_list TK_EOL TK_R_BRACE */
#line 56 "grammar.y"
{ yymsp[-4].minor.yy80 = yymsp[-2].minor.yy80; }
#line 871 "expression_parser.cpp"
        break;
      case 14: /* compare_options ::= TK_COMP_EQUAL */
#line 58 "grammar.y"
{ yymsp[0].minor.yy74 = new EqualRelationalExpr(); }
#line 876 "expression_parser.cpp"
        break;
      case 15: /* compare_options ::= TK_COMP_NEQUAL */
#line 59 "grammar.y"
{ yymsp[0].minor.yy74 = new NotEqualRelationalExpr(); }
#line 881 "expression_parser.cpp"
        break;
      case 16: /* compare_options ::= TK_COMP_GREATER */
#line 60 "grammar.y"
{ yymsp[0].minor.yy74 = new GreaterThanRelationalExpr(); }
#line 886 "expression_parser.cpp"
        break;
      case 17: /* compare_options ::= TK_COMP_GREATER_EQUAL */
#line 61 "grammar.y"
{ yymsp[0].minor.yy74 = new GreaterOrEqualThanRelationalExpr(); }
#line 891 "expression_parser.cpp"
        break;
      case 18: /* compare_options ::= TK_COMP_LESS */
#line 62 "grammar.y"
{ yymsp[0].minor.yy74 = new LessThanRelationalExpr(); }
#line 896 "expression_parser.cpp"
        break;
      case 19: /* compare_options ::= TK_COMP_LESS_EQUAL */
#line 63 "grammar.y"
{ yymsp[0].minor.yy74 = new LessOrEqualThanRelationalExpr(); }
#line 901 "expression_parser.cpp"
        break;
      case 20: /* print ::= TK_RW_PRINT expr */
#line 65 "grammar.y"
{ yymsp[-1].minor.yy80 = new PrintStatement(yymsp[0].minor.yy74, DEC); }
#line 906 "expression_parser.cpp"
        break;
      case 21: /* print ::= TK_RW_PRINT expr TK_COMMA print_option */
#line 66 "grammar.y"
{ yymsp[-3].minor.yy80 = new PrintStatement(yymsp[-2].minor.yy74, yymsp[0].minor.yy4); }
#line 911 "expression_parser.cpp"
        break;
      case 22: /* print_option ::= TK_BIN */
      case 23: /* print_option ::= TK_HEX */ yytestcase(yyruleno==23);
      case 24: /* print_option ::= TK_DEC */ yytestcase(yyruleno==24);
#line 68 "grammar.y"
{ yylhsminor.yy4 = yymsp[0].minor.yy0->int_value; }
#line 918 "expression_parser.cpp"
  yymsp[0].minor.yy4 = yylhsminor.yy4;
        break;
      case 25: /* assign ::= TK_VARIABLE TK_EQUAL expr */
#line 72 "grammar.y"
{ yylhsminor.yy80 = new AssignStatement(yymsp[0].minor.yy74,yymsp[-2].minor.yy0->str_value); /*delete yymsp[-2].minor.yy0->str_value;*/ }
#line 924 "expression_parser.cpp"
  yymsp[-2].minor.yy80 = yylhsminor.yy80;
        break;
      case 26: /* expr ::= expr OP_ADD term */
#line 74 "grammar.y"
{ yylhsminor.yy74 = new AddExpr(yymsp[-2].minor.yy74,yymsp[0].minor.yy74); }
#line 930 "expression_parser.cpp"
  yymsp[-2].minor.yy74 = yylhsminor.yy74;
        break;
      case 27: /* expr ::= expr OP_SUB term */
#line 75 "grammar.y"
{ yylhsminor.yy74 = new SubExpr(yymsp[-2].minor.yy74,yymsp[0].minor.yy74); }
#line 936 "expression_parser.cpp"
  yymsp[-2].minor.yy74 = yylhsminor.yy74;
        break;
      case 28: /* expr ::= term */
      case 31: /* term ::= factor */ yytestcase(yyruleno==31);
#line 76 "grammar.y"
{ yylhsminor.yy74 = yymsp[0].minor.yy74; }
#line 943 "expression_parser.cpp"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 29: /* term ::= term OP_MULT factor */
#line 78 "grammar.y"
{ yylhsminor.yy74 = new MulExpr(yymsp[-2].minor.yy74, yymsp[0].minor.yy74); }
#line 949 "expression_parser.cpp"
  yymsp[-2].minor.yy74 = yylhsminor.yy74;
        break;
      case 30: /* term ::= term OP_DIV factor */
#line 79 "grammar.y"
{ yylhsminor.yy74 = new DivExpr(yymsp[-2].minor.yy74, yymsp[0].minor.yy74); }
#line 955 "expression_parser.cpp"
  yymsp[-2].minor.yy74 = yylhsminor.yy74;
        break;
      case 32: /* factor ::= TK_NUMBER */
#line 82 "grammar.y"
{ yylhsminor.yy74 = new NumberExpr(yymsp[0].minor.yy0->int_value); }
#line 961 "expression_parser.cpp"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 33: /* factor ::= TK_VARIABLE */
#line 83 "grammar.y"
{ yylhsminor.yy74 = new VarExpr(yymsp[0].minor.yy0->str_value); /*delete yymsp[0].minor.yy0->str_value;*/ }
#line 967 "expression_parser.cpp"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 34: /* factor ::= TK_L_PAR expr TK_R_PAR */
#line 84 "grammar.y"
{ yymsp[-2].minor.yy74 = yymsp[-1].minor.yy74; }
#line 973 "expression_parser.cpp"
        break;
      default:
      /* (1) opt_eol ::= TK_EOL */ yytestcase(yyruleno==1);
      /* (2) opt_eol ::= */ yytestcase(yyruleno==2);
        break;
/********** End reduce actions ************************************************/
  };
  assert( yyruleno>=0 && yyruleno<sizeof(yyRuleInfo)/sizeof(yyRuleInfo[0]) );
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yyact = yy_find_reduce_action(yymsp[-yysize].stateno,(YYCODETYPE)yygoto);
  if( yyact <= YY_MAX_SHIFTREDUCE ){
    if( yyact>YY_MAX_SHIFT ) yyact += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
    yypParser->yyidx -= yysize - 1;
    yymsp -= yysize-1;
    yymsp->stateno = (YYACTIONTYPE)yyact;
    yymsp->major = (YYCODETYPE)yygoto;
    yyTraceShift(yypParser, yyact);
  }else{
    assert( yyact == YY_ACCEPT_ACTION );
    yypParser->yyidx -= yysize;
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  NxParserARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
/************ Begin %parse_failure code ***************************************/
/************ End %parse_failure code *****************************************/
  NxParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  NxParserTOKENTYPE yyminor         /* The minor type of the error token */
){
  NxParserARG_FETCH;
#define TOKEN yyminor
/************ Begin %syntax_error code ****************************************/
/************ End %syntax_error code ******************************************/
  NxParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  NxParserARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
/*********** Begin %parse_accept code *****************************************/
/*********** End %parse_accept code *******************************************/
  NxParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "NxParserAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void NxParser(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  NxParserTOKENTYPE yyminor       /* The value for the token */
  NxParserARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  int yyact;            /* The parser action. */
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  int yyendofinput;     /* True if we are at the end of input */
#endif
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  /* (re)initialize the parser, if necessary */
  yypParser = (yyParser*)yyp;
  if( yypParser->yyidx<0 ){
#if YYSTACKDEPTH<=0
    if( yypParser->yystksz <=0 ){
      yyStackOverflow(yypParser);
      return;
    }
#endif
    yypParser->yyidx = 0;
#ifndef YYNOERRORRECOVERY
    yypParser->yyerrcnt = -1;
#endif
    yypParser->yystack[0].stateno = 0;
    yypParser->yystack[0].major = 0;
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sInitialize. Empty stack. State 0\n",
              yyTracePrompt);
    }
#endif
  }
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  yyendofinput = (yymajor==0);
#endif
  NxParserARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput '%s'\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact <= YY_MAX_SHIFTREDUCE ){
      if( yyact > YY_MAX_SHIFT ) yyact += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
      yy_shift(yypParser,yyact,yymajor,yyminor);
#ifndef YYNOERRORRECOVERY
      yypParser->yyerrcnt--;
#endif
      yymajor = YYNOCODE;
    }else if( yyact <= YY_MAX_REDUCE ){
      yy_reduce(yypParser,yyact-YY_MIN_REDUCE);
    }else{
      assert( yyact == YY_ERROR_ACTION );
      yyminorunion.yy0 = yyminor;
#ifdef YYERRORSYMBOL
      int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminor);
      }
      yymx = yypParser->yystack[yypParser->yyidx].major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor, &yyminorunion);
        yymajor = YYNOCODE;
      }else{
        while(
          yypParser->yyidx >= 0 &&
          yymx != YYERRORSYMBOL &&
          (yyact = yy_find_reduce_action(
                        yypParser->yystack[yypParser->yyidx].stateno,
                        YYERRORSYMBOL)) >= YY_MIN_REDUCE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yyidx < 0 || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          yy_shift(yypParser,yyact,YYERRORSYMBOL,yyminor);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor, yyminor);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      yymajor = YYNOCODE;
      
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor, yyminor);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yyidx>=0 );
#ifndef NDEBUG
  if( yyTraceFILE ){
    int i;
    fprintf(yyTraceFILE,"%sReturn. Stack=",yyTracePrompt);
    for(i=1; i<=yypParser->yyidx; i++)
      fprintf(yyTraceFILE,"%c%s", i==1 ? '[' : ' ', 
              yyTokenName[yypParser->yystack[i].major]);
    fprintf(yyTraceFILE,"]\n");
  }
#endif
  return;
}
