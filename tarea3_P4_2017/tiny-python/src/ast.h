#ifndef _AST_H_
#define _AST_H_

#include <string>
#include <list>
#include <map>
#include <stack>
#include <set>
#include <stdexcept>
using namespace std;

extern map<string, int> vars;

int expt(int p, unsigned int q);

typedef struct mips_data{
    string code;
    string place;
} mips_data_t;

extern set<string> global_vars;
extern stack<string> temps;
extern unsigned int next_label;
extern unsigned int next_var;

void initialize_temps();
void generate_header();
void generate_tail();

enum BuiltInFunct {
    FN_TIMECLOCK,
    FN_RANDSEED,
    FN_RANDINT
};

enum ExprKind {
  LT_EXPR,
  LTE_EXPR,
  GT_EXPR,
  GTE_EXPR,
  NE_EXPR,
  EQ_EXPR,
  ADD_EXPR,
  SUB_EXPR,
  MULT_EXPR,
  DIV_EXPR,
  MOD_EXPR,
  EXPT_EXPR,
  NUM_EXPR,
  ID_EXPR,
  STRING_EXPR,
  INPUT_EXPR,
  CALL_EXPR,
  ARRAY_ACCESS_EXPR
};

class Expr;
typedef list<Expr*> ExprList;

class Expr {
public:
    virtual int evaluate() = 0;
    virtual int getKind() = 0;
    virtual void discover_vars() {}
    virtual void generate_code(mips_data_t *ret) {}
    bool isA(int kind) { return (getKind() == kind); }
};

class BinaryExpr: public Expr {
public:
    BinaryExpr(Expr *expr1, Expr *expr2) {
        this->expr1 = expr1;
        this->expr2 = expr2;
    }
    void discover_vars();

    Expr *expr1;
    Expr *expr2;
};

class LTExpr: public BinaryExpr {
public:
    LTExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() < expr2->evaluate(); }
    int getKind() { return LT_EXPR; }
    void generate_code(mips_data_t *ret);
};

class GTExpr: public BinaryExpr {
public:
    GTExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() > expr2->evaluate(); }
    int getKind() { return GT_EXPR; }
    void generate_code(mips_data_t *ret);
};

class LTEExpr: public BinaryExpr {
public:
    LTEExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() <= expr2->evaluate(); }
    int getKind() { return LTE_EXPR; }
    void generate_code(mips_data_t *ret);
};

class GTEExpr: public BinaryExpr {
public:
    GTEExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() >= expr2->evaluate(); }
    int getKind() { return GTE_EXPR; }
    void generate_code(mips_data_t *ret);
};

class NEExpr: public BinaryExpr {
public:
    NEExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() != expr2->evaluate(); }
    int getKind() { return NE_EXPR; }
    void generate_code(mips_data_t *ret);
};

class EQExpr: public BinaryExpr {
public:
    EQExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() == expr2->evaluate(); }
    int getKind() { return NE_EXPR; }
    void generate_code(mips_data_t *ret);
};

class AddExpr: public BinaryExpr {
public:
    AddExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() + expr2->evaluate(); }
    int getKind() { return ADD_EXPR; }
    void generate_code(mips_data_t *ret);
};

class SubExpr: public BinaryExpr {
public:
    SubExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() - expr2->evaluate(); }
    int getKind() { return SUB_EXPR; }
    void generate_code(mips_data_t *ret);
};

class MultExpr: public BinaryExpr {
public:
    MultExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() * expr2->evaluate(); }
    int getKind() { return MULT_EXPR; }
    void generate_code(mips_data_t *ret);
};

class DivExpr: public BinaryExpr {
public:
    DivExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() / expr2->evaluate(); }
    int getKind() { return DIV_EXPR; }
    void generate_code(mips_data_t *ret);
};

class ModExpr: public BinaryExpr {
public:
    ModExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expr1->evaluate() % expr2->evaluate(); }
    int getKind() { return EXPT_EXPR; }
    void generate_code(mips_data_t *ret);
};

class ExponentExpr: public BinaryExpr {
public:
    ExponentExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2) {}

    int evaluate() { return expt(expr1->evaluate(), expr2->evaluate()); }
    int getKind() { return MOD_EXPR; }
    void generate_code(mips_data_t *ret);
};

class NumExpr: public Expr {
public:
    NumExpr(int value) { this->value = value; }
    int evaluate() { return value; }
    int getKind() { return NUM_EXPR; }
    void discover_vars() {}
    void generate_code(mips_data_t *ret);

    int value;
};

class IdExpr: public Expr {
public:
    IdExpr(string id) { this->id = id; }
    int evaluate() { return vars[id]; }
    int getKind() { return ID_EXPR; }
    void discover_vars();
    void generate_code(mips_data_t *ret);

    string id;
};

class StringExpr: public Expr {
public:
    StringExpr(string str) : Expr() { this->str = str; }
    int evaluate() { return 0; }
    int getKind() { return STRING_EXPR; }
    void discover_vars();
    void generate_code(mips_data_t *ret);

    string str;
    string location;
};

class InputExpr: public Expr {
public:
    InputExpr(string prompt) { this->prompt = prompt; }
    int evaluate();
    int getKind() { return INPUT_EXPR; }
    void discover_vars();
    void generate_code(mips_data_t *ret);

    string prompt;
    string location;
};

class CallExpr: public Expr {
public:
    CallExpr(BuiltInFunct fnId) { 
        this->fnId = fnId; 
    }
    CallExpr(BuiltInFunct fnId, Expr *arg0, Expr *arg1) { 
        this->fnId = fnId;
        this->arg0 = arg0;
        this->arg1 = arg1;
    }
    int evaluate();
    int getKind() { return CALL_EXPR; }
    void discover_vars();
    void generate_code(mips_data_t *ret);

    BuiltInFunct fnId;
    Expr *arg0, *arg1;
};

class ArrayAccessExpr: public Expr{
public:
    ArrayAccessExpr(string id, Expr* expr){
        this->id = id;
        this->expr = expr;
    }
    int evaluate(){ return 0;};
    int getKind() { return ARRAY_ACCESS_EXPR; }
    void generate_code(mips_data_t *ret);
    string id
    ;Expr* expr;
};

enum StatementKind {
    BLOCK_STATEMENT,
    PRINT_STATEMENT,
    ASSIGN_STATEMENT,
    IF_STATEMENT,
    WHILE_STATEMENT,
	FOR_STATEMENT,
	PASS_STATEMENT,
    CALL_STATEMENT,
    ARRAY_STATEMENT,
    ARRAY_ASSIGNMENT_STATEMENT
};

class Statement {
public:
    virtual void execute() = 0;
    virtual StatementKind getKind() = 0;
    virtual void discover_vars() = 0;
    virtual void generate_code(mips_data_t *ret) {}
};

class AssignArrayStatement: public Statement{
public:
    AssignArrayStatement(string id, Expr* padding, Expr* assignation){
        this->id = id;
        this->padding = padding;
        this->assignation = assignation;
    }
    void discover_vars();
    void execute() {  };
    StatementKind getKind() { return ARRAY_ASSIGNMENT_STATEMENT; }
    void generate_code(mips_data_t *ret);
    string id;
    Expr* padding, * assignation;
};


class ArrayDeclaration: public Statement{
public:
    ArrayDeclaration(string id, Expr* size){
        this->id = id;
        
        if(size->isA(NUM_EXPR)){
            this->size = ((NumExpr*)size)->value;;
        }else{
            std::invalid_argument("Error: array declaration must be declared with a constant value\n");
        }

    }
    void discover_vars();
    void execute() {  };
    StatementKind getKind() { return ARRAY_STATEMENT; }
    void generate_code(mips_data_t *ret);
    string id;
    int size;
};
class BlockStatement: public Statement {
public:
    BlockStatement() {}
    void execute();
    StatementKind getKind() { return BLOCK_STATEMENT; }
	void add(Statement *st) { stList.push_back(st); }
    void discover_vars();
    void generate_code(mips_data_t *ret);

    list<Statement *> stList;
};


class AssignStatement: public Statement {
public:
    AssignStatement(string id, Expr *expr) {
        this->id = id;
        this->expr = expr;
    }
    void execute();
    StatementKind getKind() { return ASSIGN_STATEMENT; }
    void discover_vars();
    void generate_code(mips_data_t *ret);

    string id;
    Expr *expr;
};

class PrintStatement: public Statement {
public:
    PrintStatement(ExprList lexpr) {
        this->lexpr = lexpr;
    }
    void execute();
    StatementKind getKind() { return PRINT_STATEMENT; }
    void discover_vars();
    void generate_code(mips_data_t  *ret);

    ExprList lexpr;
};

class IfStatement: public Statement {
public:
    IfStatement(Expr *cond, Statement *trueBlock, Statement *falseBlock) {
        this->cond = cond;
        this->trueBlock = trueBlock;
        this->falseBlock = falseBlock;
    }
    void execute();
    StatementKind getKind() { return IF_STATEMENT; }
    void discover_vars();
    void generate_code(mips_data_t *ret);

    Expr *cond;
    Statement *trueBlock;
    Statement *falseBlock;
};

class PassStatement: public Statement {
public:
    PassStatement() {
    }
    void execute() {} ;
    StatementKind getKind() { return PASS_STATEMENT; }
    void discover_vars() { };
    void generate_code(mips_data_t *ret){}
};

class WhileStatement: public Statement {
public:
    WhileStatement(Expr *cond, Statement *block) {
        this->cond = cond;
        this->block = block;
    }
    void execute();
    StatementKind getKind() { return WHILE_STATEMENT; }
    void discover_vars();
    void generate_code(mips_data_t *ret);

    Expr *cond;
    Statement *block;
};

class ForStatement: public Statement {
public:
    ForStatement(string id, Expr *startExpr, Expr *endExpr, Statement *block) {
        this->id = id;
        this->startExpr = startExpr;
        this->endExpr = endExpr;
        this->block = block;
    }
    void execute();
    StatementKind getKind() { return FOR_STATEMENT; }
    void discover_vars();
    void generate_code(mips_data_t *ret);

    string id;
    Expr *startExpr;
    Expr *endExpr;
    Statement *block;
};

class CallStatement: public Statement {
public:
    CallStatement(BuiltInFunct fnId, Expr *arg0) {
        this->fnId = fnId;
        this->arg0 = arg0;
        this->arg1 = NULL;
    }
    CallStatement(BuiltInFunct fnId, Expr *arg0, Expr *arg1): CallStatement(fnId, arg0) {
        this->arg1 = arg1;
    }
    void execute();
    StatementKind getKind() { return CALL_STATEMENT; }
    void discover_vars();
    void generate_code(mips_data_t *ret);

    BuiltInFunct fnId;
    Expr *arg0, *arg1;
};

#endif
