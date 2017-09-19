#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include "ast.h"

using namespace std;

map<string, int> vars;
set<string> global_vars;
set<pair<string,int> > global_arrays;
map<string, string> global_strings;
stack<string> temps;
unsigned int next_label;
unsigned int next_var;

int expt(int p, unsigned int q)
{
    int r = 1;

    while (q != 0)
    {
        if (q % 2 == 1)
        { // q is odd
            r *= p;
            q--;
        }
        p *= p;
        q /= 2;
    }

    return r;
}

void initialize_temps()
{
    for (int i = 0; i < 10; i++)
    {
        char str[4];
        sprintf(str, "$t%d", 9 - i);
        temps.push(string(str));
    }
    next_label = 0;
    next_var = 0;
}

void release_temp(string temp)
{
    if (temp[0] == '$' && temp[1] == 't' && temp[2] >= '0' && temp[2] <= '9')
        temps.push(temp);
}

void generate_header()
{
    printf("#include \"screen.h\"\n");
    printf("#include \"system.h\"\n");
    printf(".global main\n\n");
    printf(".data\n");

    printf("enter: .asciz \"\\n\"\n");
    printf("quo_xyz: .word 0\n");
    printf("rem_xyz: .word 0\n");
    // printf("next_var: %d\n", next_var);
    map<string, string>::iterator i;
    for (i = global_strings.begin(); i != global_strings.end(); i++)
    {
        printf("%s: %s", (*i).first.c_str(), (*i).second.c_str());
    }

    set<string>::iterator it;
    for (it = global_vars.begin(); it != global_vars.end(); it++)
        printf("%s: .word 0\n", (*it).c_str());

    auto it_a = global_arrays.begin();
    for(;it_a != global_arrays.end(); ++it_a){
        printf("%s: .space %d\n",(*it_a).first.c_str(),(*it_a).second);
    }

    printf("\n");

    printf(".text\n\n");
    printf("main:\n");
    printf("li $a0, BRIGHT_WHITE\n");
    printf("li $a1, BLACK\n");
    printf("jal set_color\n");
    printf("jal clear_screen\n");
    printf("jal keypad_init\n\n");
}

void generate_tail()
{
    printf("jr $ra\n");
}

int InputExpr::evaluate()
{
    int result;

    cout << prompt;
    fflush(stdin);
    scanf("%d", &result);

    return result;
}

void InputExpr::discover_vars()
{
    char str[25];
    sprintf(str, "str_%d", next_var++);
    // printf("globar var: %s\n", this->str.c_str());
    this->location = string(str);
    global_strings[location] = ".asciz \"" + this->prompt + "\"\n";
}

void InputExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    char str[25];
    sprintf(str, "input_%d", next_label++);

    ret->code = "la $a0, " + this->location + "\n";
    ret->code += "jal puts\n";
    ret->code += string(str) + ":\n";
    ret->code += "jal keypad_getkey\n";
    ret->code += "beqz $v0, " + string(str) + "\n";
    ret->code += "la $a0, enter\n";
    ret->code += "jal puts\n";
    ret->place = "$v0";
}

void IdExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string place = temps.top();
    temps.pop();
    ret->code += "lw " + place + ", " + this->id + "\n";
    ret->place = place;
}

void BinaryExpr::discover_vars()
{
    this->expr1->discover_vars();
    this->expr2->discover_vars();
}

void StringExpr::discover_vars()
{
    char str[25];
    sprintf(str, "str_%d", next_var++);
    // printf("globar var: %s\n", this->str.c_str());
    this->location = string(str);
    global_strings[location] = ".asciz \"" + this->str + "\"\n";
}

void StringExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string temp = temps.top();
    temps.pop();
    ret->code = "la " + temp + ", " + location + "\n";
    ret->place = temp;
}

int CallExpr::evaluate()
{
    switch (fnId)
    {
    case FN_TIMECLOCK:
        return clock();
    case FN_RANDINT:
    {
        int start = arg0->evaluate();
        int end = arg1->evaluate();
        int range = end - start + 1;

        return (rand() % range) + start;
    }
    default:
        return 0;
    }
}

void CallExpr::discover_vars()
{
    if (fnId == FN_RANDINT)
    {
        arg0->discover_vars();
        arg1->discover_vars();
    }
}

void CallExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    switch (fnId)
    {
    case FN_TIMECLOCK:
    {
        ret->place = temps.top();
        temps.pop();
        ret->code = "lw " + ret->place + ", MS_COUNTER_REG_ADDR\n";
        return;
    }
    case FN_RANDINT:
    {
        string code = "";
        string place;
        arg0->generate_code(ret);
        place = ret->place;
        code = ret->code;
        arg1->generate_code(ret);
        code += ret->code;
        release_temp(ret->place);
        release_temp(place);
        code += "move $a0, " + place + "\n";
        code += "move $a1, " + ret->place + "\n";
        code += "jal rand\n";
        ret->code = code;
        ret->place = "$v0";
        return;
    }
    default:
        return;
    }
}

void LTEExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string code, place;
    expr1->generate_code(ret);
    code = ret->code;
    place = ret->place;
    ret->code = "";
    ret->place = "";
    expr2->generate_code(ret);
    release_temp(ret->place);
    code += ret->code;

    code += "sle " + place + ", " + place + ", " + ret->place + "\n";

    ret->place = place;
    ret->code = code;
}

void GTEExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string code, place;
    expr1->generate_code(ret);
    code = ret->code;
    place = ret->place;
    ret->code = "";
    ret->place = "";
    expr2->generate_code(ret);
    release_temp(ret->place);
    code += ret->code;

    code += "sge " + place + ", " + place + ", " + ret->place + "\n";

    ret->place = place;
    ret->code = code;
}

void LTExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string code, place;
    expr1->generate_code(ret);
    code = ret->code;
    place = ret->place;
    ret->code = "";
    ret->place = "";
    expr2->generate_code(ret);
    release_temp(ret->place);
    code += ret->code;

    code += "slt " + place + ", " + place + ", " + ret->place + "\n";

    ret->place = place;
    ret->code = code;
}

void GTExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string code, place;
    expr1->generate_code(ret);
    code = ret->code;
    place = ret->place;
    ret->code = "";
    ret->place = "";
    expr2->generate_code(ret);
    release_temp(ret->place);
    code += ret->code;

    code += "sgt " + place + ", " + place + ", " + ret->place + "\n";

    ret->place = place;
    ret->code = code;
}

void NEExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string code, place;
    expr1->generate_code(ret);
    code = ret->code;
    place = ret->place;
    ret->code = "";
    ret->place = "";
    expr2->generate_code(ret);
    release_temp(ret->place);
    code += ret->code;

    code += "sne " + place + ", " + place + ", " + ret->place + "\n";

    ret->place = place;
    ret->code = code;
}

void EQExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string code, place;
    expr1->generate_code(ret);
    code = ret->code;
    place = ret->place;
    ret->code = "";
    ret->place = "";
    expr2->generate_code(ret);
    release_temp(ret->place);
    code += ret->code;

    code += "seq " + place + ", " + place + ", " + ret->place + "\n";

    ret->place = place;
    ret->code = code;
}

void NumExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    ret->place = temps.top();
    temps.pop();
    char val[25];
    sprintf(val, "%d", this->value);
    ret->code = "li " + ret->place + ", " + string(val) + "\n";
}

void AddExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string code, place;
    expr1->generate_code(ret);
    code = ret->code;
    place = ret->place;
    ret->code = "";
    ret->place = "";
    expr2->generate_code(ret);
    release_temp(ret->place);
    code += ret->code;

    code += "add " + place + ", " + place + ", " + ret->place + "\n";

    ret->place = place;
    ret->code = code;
}

void SubExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string code, place;
    expr1->generate_code(ret);
    code = ret->code;
    place = ret->place;
    ret->code = "";
    ret->place = "";
    expr2->generate_code(ret);
    release_temp(ret->place);
    code += ret->code;

    code += "sub " + place + ", " + place + ", " + ret->place + "\n";

    ret->place = place;
    ret->code = code;
}

void MultExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string code, place;
    expr1->generate_code(ret);
    code = ret->code;
    place = ret->place;
    ret->code = "";
    ret->place = "";
    expr2->generate_code(ret);
    release_temp(ret->place);
    release_temp(place);
    code += ret->code;

    code += "move $a0, " + place + "\n";
    code += "move $a1, " + ret->place + "\n";
    code += "jal mult\n";

    ret->place = temps.top();
    temps.pop();
    code += "move " + ret->place + ", $v0\n";
    ret->code = code;
}

void DivExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string code, place;
    expr1->generate_code(ret);
    code = ret->code;
    place = ret->place;
    ret->code = "";
    ret->place = "";
    expr2->generate_code(ret);
    release_temp(ret->place);
    release_temp(place);
    code += ret->code;

    code += "move $a0, " + place + "\n";
    code += "move $a1, " + ret->place + "\n";
    code += "la $a2, quo_xyz\n";
    code += "la $a3, rem_xyz\n";
    code += "jal divide\n";

    ret->place = temps.top();
    temps.pop();
    code += "lw " + ret->place + ", quo_xyz\n";
    ret->code = code;
}

void ModExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string code, place;
    expr1->generate_code(ret);
    code = ret->code;
    place = ret->place;
    ret->code = "";
    ret->place = "";
    expr2->generate_code(ret);
    release_temp(ret->place);
    release_temp(place);
    code += ret->code;

    code += "move $a0, " + place + "\n";
    code += "move $a1, " + ret->place + "\n";
    code += "la $a2, quo_xyz\n";
    code += "la $a3, rem_xyz\n";
    code += "jal divide\n";

    ret->place = temps.top();
    temps.pop();
    code += "lw " + ret->place + ", rem_xyz\n";
    ret->code = code;
}

void ExponentExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string code, place;
    expr1->generate_code(ret);
    code = ret->code;
    place = ret->place;
    ret->code = "";
    ret->place = "";
    expr2->generate_code(ret);
    release_temp(ret->place);
    release_temp(place);
    code += ret->code;

    code += "move $a0, " + place + "\n";
    code += "move $a1, " + ret->place + "\n";
    code += "jal pow\n";

    ret->place = "$v0";
    ret->code = code;
}

void BlockStatement::execute()
{
    list<Statement *>::iterator it = stList.begin();

    while (it != stList.end())
    {
        Statement *st = *it;

        st->execute();
        it++;
    }
}

void BlockStatement::discover_vars()
{
    list<Statement *>::iterator it = stList.begin();

    while (it != stList.end())
    {
        Statement *st = *it;

        st->discover_vars();
        it++;
    }
}

void BlockStatement::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    list<Statement *>::iterator it = stList.begin();
    string code;

    while (it != stList.end())
    {
        ret->code = "";
        ret->place = "";
        Statement *st = *it;

        st->generate_code(ret);
        code += ret->code;
        it++;
    }
    ret->place = "";
    ret->code = code;
}

void AssignStatement::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    expr->generate_code(ret);
    release_temp(ret->place);

    ret->code += "sw " + ret->place + ", " + id + "\n";
    ret->place = "";
}

void PrintStatement::execute()
{
    list<Expr *>::iterator it = lexpr.begin();

    while (it != lexpr.end())
    {
        Expr *expr = *it;

        if (expr->isA(STRING_EXPR))
        {
            printf("%s", ((StringExpr *)expr)->str.c_str());
        }
        else
        {
            int result = expr->evaluate();
            printf("%d", result);
        }

        it++;
    }
    printf("\n");
}

void IfStatement::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string code = "";

    string if_false, endif;
    char str[25];
    sprintf(str, "if_false_%d", next_label);
    if_false = string(str);
    sprintf(str, "endif%d", next_label++);
    endif = string(str);

    cond->generate_code(ret);
    release_temp(ret->place);
    code += ret->code;
    code += "beqz " + ret->place + ", " + if_false + "\n";

    ret->code = "";
    ret->place = "";
    trueBlock->generate_code(ret);
    code += ret->code;

    code += "j " + endif + "\n";
    code += if_false + ": \n";

    ret->code = "";
    ret->place = "";
    falseBlock->generate_code(ret);
    code += ret->code;

    ret->code = "";
    ret->place = "";
    code += endif + ":\n";

    ret->code = code;
}

void WhileStatement::generate_code(mips_data_t *ret)
{
    string code = "";
    ret->code = "";
    ret->place = "";

    cond->generate_code(ret);
    char str[25];
    sprintf(str, "while_%d", next_label);
    string w_start = string(str);
    sprintf(str, "end_while_%d", next_label++);
    string w_end = string(str);

    code += w_start + ":\n";
    code += ret->code;
    release_temp(ret->place);
    code += "beqz " + ret->place + ", " + w_end + "\n";

    ret->code = "";
    ret->place = "";
    block->generate_code(ret);
    code += ret->code;

    code += "j " + w_start + "\n";

    code += w_end + ":\n";

    ret->code = code;
}

void ForStatement::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string code = "";
    char str[25];
    sprintf(str, "for_%d", next_label);
    string f_start = string(str);
    sprintf(str, "end_for_%d", next_label++);
    string f_end = string(str);

    startExpr->generate_code(ret);
    release_temp(ret->place);
    code += ret->code;
    code += "sw " + ret->place + ", " + id + "\n";

    code += f_start + ":\n";
    string place = temps.top();
    temps.pop();
    code += "lw " + place + ", " + id + "\n";

    ret->code = "";
    ret->place = "";
    endExpr->generate_code(ret);

    code += ret->code;
    code += "slt " + place + ", " + place + ", " + ret->place + "\n";
    temps.pop();
    code += "beqz " + place + ", " + f_end + "\n";
    release_temp(ret->place);
    release_temp(place);

    ret->code = "";
    ret->place = "";
    block->generate_code(ret);
    code += ret->code;

    place = temps.top();

    code += "lw " + place + ", " + id + "\n";
    code += "addi " + place + ", " + place + ", 1\n";
    code += "sw " + place + ", " + id + "\n";

    release_temp(place);

    code += "j " + f_start + "\n";

    code += f_end + ":\n";

    ret->code = code;
}

void IdExpr::discover_vars()
{
    global_vars.insert(id);
}

void PrintStatement::discover_vars()
{
    list<Expr *>::iterator it = lexpr.begin();

    while (it != lexpr.end())
    {
        Expr *expr = *it;
        // printf("print discovering, e type = %d\n", expr->getKind());

        expr->discover_vars();

        it++;
    }
}

void PrintStatement::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    list<Expr *>::iterator it = lexpr.begin();
    string code;
    // code += "start print\n";

    while (it != lexpr.end())
    {
        Expr *expr = *it;

        expr->generate_code(ret);
        release_temp(ret->place);
        code += ret->code;
        code += "move $a0, " + ret->place + "\n";
        string fn = "put_udecimal";
        if (expr->getKind() == STRING_EXPR)
            fn = "puts";
        code += "jal " + fn + "\n";

        it++;
        ret->code = "";
        ret->place = "";
    }
    code += "la $a0, enter\n";
    code += "jal puts\n";
    ret->code = code;
    ret->place = "";
}

void AssignStatement::execute()
{
    int result = expr->evaluate();
    vars[id] = result;
}

void AssignStatement::discover_vars()
{
    global_vars.insert(id);
    expr->discover_vars();
}

void IfStatement::execute()
{
    int result = cond->evaluate();

    if (result)
    {
        trueBlock->execute();
    }
    else if (falseBlock != 0)
    {
        falseBlock->execute();
    }
}

void IfStatement::discover_vars()
{
    cond->discover_vars();
    trueBlock->discover_vars();
    falseBlock->discover_vars();
}

void WhileStatement::execute()
{
    int result = cond->evaluate();

    while (result)
    {
        block->execute();

        result = cond->evaluate();
    }
}

void WhileStatement::discover_vars()
{
    cond->discover_vars();
    block->discover_vars();
}

void ForStatement::execute()
{
    int val = startExpr->evaluate();
    vars[id] = val;

    val = endExpr->evaluate();
    while (vars[id] < val)
    {
        block->execute();
        vars[id] = vars[id] + 1;
    }
}

void ForStatement::discover_vars()
{
    global_vars.insert(id);
    startExpr->discover_vars();
    endExpr->discover_vars();
    block->discover_vars();
}

void CallStatement::execute()
{
    switch (fnId)
    {
    case FN_RANDSEED:
    {
        int arg = arg0->evaluate();
        srand(arg);
    }
    default:
    {
    }
    }
}

void CallStatement::discover_vars()
{
    switch (fnId)
    {
    case FN_RANDSEED:
    {
        arg0->discover_vars();
    }
    default:
    {
    }
    }
}

void CallStatement::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    switch (fnId)
    {
    case FN_RANDSEED:
    {
        arg0->generate_code(ret);
        release_temp(ret->place);
        string code = ret->code;
        code += "move $a0, " + ret->place + "\n";
        ret->place = "";
        code += "jal rand_seed\n";
        ret->code = code;
        ret->place = "";
        return;
    }
    default:
    {
    }
    }
}

void ArrayDeclaration::generate_code(mips_data_t *ret){

}

void ArrayDeclaration::discover_vars(){
    global_arrays.insert(pair<string,int>(id, size*4));
}


void ArrayAccessExpr::generate_code(mips_data_t *ret){
    ret->code = "";
    ret->place = "";
    string code;
    expr->generate_code(ret);
    code = ret->code;
    code += "move $a0, " + ret->place + "\n";
    code += "li $a1, 4\n";
    code += "jal mult\n";
    code += "la "+ret->place+", "+id+"\n";
    code += "addu $v0, $v0, "+ret->place+"\n";
    code += "lw "+ret->place+", 0($v0)\n";
    ret->code = code;
}


void AssignArrayStatement::discover_vars(){}


void AssignArrayStatement::generate_code(mips_data_t *ret){
    ret->code = "";
    ret->place = "";
    string code, place1, place2;

    padding->generate_code(ret);
    code+= ret->code;

    code += "move $a0, " + ret->place+"\n";
    code += "li $a1, 4\n";
    release_temp(ret->place);
    code += "jal mult\n";

    assignation->generate_code(ret);
    code += ret->code;
    place1 = ret->place;

    place2 = temps.top(); temps.pop();
    code += "la " + place2 + ", " + id+"\n";

    code += "addu " + place2 + ", " + place2 + ", $v0\n";

    code += "sw " + place1 + ", (" + place2 + ")" + "\n";

    release_temp(place2);
    // release_temp(ret->place);
    release_temp(place1);

    ret->code = code;
}

