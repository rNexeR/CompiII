#ifndef _AST_H
#define _AST_H

#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <map>

using namespace std;

#define DEC 2
#define BIN 0
#define HEX 1

// typedef struct variable_info{
//   string identifier;
//   int value;
// } variable_info_t;

class Token
{
public:
  Token()
  {
    this->str_value = new string();
  }
  Token(string *str, int ent)
  {
    this->str_value = str;
    this->int_value = ent;
  }
  string *str_value;
  int int_value;
};

class Expr
{
public:
  virtual int eval() = 0;
};

class BinaryExpr : public Expr
{
protected:
  BinaryExpr(Expr *expr1, Expr *expr2)
  {
    this->expr1 = expr1;
    this->expr2 = expr2;
  }

public:
  Expr *expr1, *expr2;
};

class AddExpr : public BinaryExpr
{
public:
  AddExpr(Expr *expr1, Expr *expr2) : BinaryExpr(expr1, expr2) {}
  int eval();
};

class SubExpr : public BinaryExpr
{
public:
  SubExpr(Expr *expr1, Expr *expr2) : BinaryExpr(expr1, expr2) {}
  int eval();
};

class MulExpr : public BinaryExpr
{
public:
  MulExpr(Expr *expr1, Expr *expr2) : BinaryExpr(expr1, expr2) {}
  int eval();
};

class DivExpr : public BinaryExpr
{
public:
  DivExpr(Expr *expr1, Expr *expr2) : BinaryExpr(expr1, expr2) {}
  int eval();
};

class EqualRelationalExpr : public BinaryExpr
{
public:
  EqualRelationalExpr() : BinaryExpr(NULL, NULL) {}
  EqualRelationalExpr(Expr *epxr1, Expr *expr2) : BinaryExpr(expr1, expr2) {}

  int eval();
};

class NotEqualRelationalExpr : public BinaryExpr
{
public:
  NotEqualRelationalExpr() : BinaryExpr(NULL, NULL) {}
  NotEqualRelationalExpr(Expr *epxr1, Expr *expr2) : BinaryExpr(expr1, expr2) {}

  int eval();
};

class GreaterThanRelationalExpr : public BinaryExpr
{
public:
  GreaterThanRelationalExpr() : BinaryExpr(NULL, NULL) {}
  GreaterThanRelationalExpr(Expr *epxr1, Expr *expr2) : BinaryExpr(expr1, expr2) {}

  int eval();
};

class GreaterOrEqualThanRelationalExpr : public BinaryExpr
{
public:
  GreaterOrEqualThanRelationalExpr() : BinaryExpr(NULL, NULL) {}
  GreaterOrEqualThanRelationalExpr(Expr *epxr1, Expr *expr2) : BinaryExpr(expr1, expr2) {}

  int eval();
};

class LessThanRelationalExpr : public BinaryExpr
{
public:
  LessThanRelationalExpr() : BinaryExpr(NULL, NULL) {}
  LessThanRelationalExpr(Expr *epxr1, Expr *expr2) : BinaryExpr(expr1, expr2) {}

  int eval();
};

class LessOrEqualThanRelationalExpr : public BinaryExpr
{
public:
  LessOrEqualThanRelationalExpr() : BinaryExpr(NULL, NULL) {}
  LessOrEqualThanRelationalExpr(Expr *epxr1, Expr *expr2) : BinaryExpr(expr1, expr2) {}

  int eval();
};

class NumberExpr : public Expr
{
public:
  NumberExpr(int number)
  {
    this->number = number;
  }
  int eval() { return number; }
  int number;
};

class VarExpr : public Expr
{
public:
  VarExpr(string *identifier)
  {
    this->identifier = *identifier;
    // cout << "VarExpr: " << this->identifier << endl;
    // printf("%\np", identifier);
  }
  int eval();
  string identifier;
};

class Statement
{
public:
  virtual void exec() = 0;
};

class PrintStatement : public Statement
{
public:
  PrintStatement() : Statement() {}
  PrintStatement(Expr *expr, int format) : Statement()
  {
    this->expr = expr;
    this->format = format;
  }

  void exec();

  Expr *expr;
  int format;
};

class AssignStatement : public Statement
{
public:
  AssignStatement() : Statement() {}
  AssignStatement(Expr *expr, string *identifier) : Statement()
  {
    this->expr = expr;
    this->identifier = *identifier;
    // cout << "Assign: " << this->identifier << endl;
    // printf("%p\n", identifier);
  }

  void exec();

  Expr *expr;
  string identifier;
};

class BlockStatement : public Statement
{
public:
  BlockStatement() : Statement() {}

  void addStatement(Statement *statement)
  {
    statementList.push_back(statement);
  }

  void exec();

  list<Statement *> statementList;
};

class ConditionalStatement : public Statement
{
protected:
  ConditionalStatement(Expr *expr)
  {
    this->expr = expr;
  }

public:
  Expr *expr;
};

class IfStatement : public ConditionalStatement
{
public:
  IfStatement(Expr *expr, Statement *body, Statement *else_body) : ConditionalStatement(expr)
  {
    this->body = body;
    this->else_body = else_body;
  }

  void exec();

  Statement *body;
  Statement *else_body;
};

class WhileStatement : public Statement
{
public:
  WhileStatement(Expr *expr, Statement *body) : Statement()
  {
    this->expr = expr;
    this->body = body;
  }

  void exec();

  Expr *expr;
  Statement *body;
};

class FunctionDefinitionStatement : public Statement
{
public:
  FunctionDefinitionStatement(string *name, list<string> *parameters, Statement *body) : Statement()
  {
    this->name = *name;
    this->parameters = parameters;
    this->body = body;
  }
  void exec();
  string name;
  list<string> *parameters;
  Statement *body;
};

class ReturnStatement : public Statement
{
public:
  ReturnStatement(Expr *expr) : Statement()
  {
    this->expr = expr;
  }

  Expr *expr;
  void exec();
};

class FunctionCallStatement : public Statement
{
public:
  FunctionCallStatement(string *name, list<Expr *> *args) : Statement()
  {
    this->name = *name;
    this->args = args;
  }

  string name;
  list<Expr *> *args;
  void exec();
};

#endif