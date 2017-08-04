#ifndef _AST_H
#define _AST_H

#include <list>
#include <stdio.h>

using namespace std;

#define DEC 2
#define HEX 1
#define BIN 0

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
  EqualRelationalExpr() : BinaryExpr(NULL, NULL) { }
  EqualRelationalExpr(Expr *epxr1, Expr *expr2) : BinaryExpr(expr1, expr2) {}

  int eval();
};

class NotEqualRelationalExpr : public BinaryExpr
{
public:
  NotEqualRelationalExpr() : BinaryExpr(NULL, NULL) { }
  NotEqualRelationalExpr(Expr *epxr1, Expr *expr2) : BinaryExpr(expr1, expr2) {}

  int eval();
};

class GreaterThanRelationalExpr : public BinaryExpr
{
public:
  GreaterThanRelationalExpr() : BinaryExpr(NULL, NULL) { }
  GreaterThanRelationalExpr(Expr *epxr1, Expr *expr2) : BinaryExpr(expr1, expr2) {}

  int eval();
};

class GreaterOrEqualThanRelationalExpr : public BinaryExpr
{
public:
  GreaterOrEqualThanRelationalExpr() : BinaryExpr(NULL, NULL) { }
  GreaterOrEqualThanRelationalExpr(Expr *epxr1, Expr *expr2) : BinaryExpr(expr1, expr2) {}

  int eval();
};

class LessThanRelationalExpr : public BinaryExpr
{
public:
  LessThanRelationalExpr() : BinaryExpr(NULL, NULL) { }
  LessThanRelationalExpr(Expr *epxr1, Expr *expr2) : BinaryExpr(expr1, expr2) {}

  int eval();
};

class LessOrEqualThanRelationalExpr : public BinaryExpr
{
public:
  LessOrEqualThanRelationalExpr() : BinaryExpr(NULL, NULL) { }
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
  VarExpr(int idx)
  {
    this->idx = idx;
  }
  int eval();
  int idx;
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
  AssignStatement(Expr *expr, int idx) : Statement()
  {
    this->expr = expr;
    this->idx = idx;
  }

  void exec();

  Expr *expr;
  int idx;
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

#endif