/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
#include "exp.h"
#include "error.h"
#include "evalstate.h"
#include "parser.h"
#include "simpio.h"
using namespace std;

/* Implementation of the Statement class */


Statement::Statement() {
   /* Empty */
}

Statement::~Statement() {
   /* Empty */
}


/*
 * Implementation notes: the PrintStmt subclass
 */
PrintStmt::PrintStmt(TokenScanner & scanner){
    exp = readE(scanner, 0);
    if (scanner.hasMoreTokens()){
        error("Extranouse token" +  scanner.nextToken());
    }
}

PrintStmt::~PrintStmt(){
    delete exp;
}

void PrintStmt::execute(EvalState &state){
    cout << exp->eval(state) <<endl;
}

 StatementType PrintStmt::getType(){
    return PRINT;
}

/*
 * Implementation notes: the RemStmt subclass
 */

 RemStmt::RemStmt(TokenScanner & scanner){ }
 void RemStmt::execute(EvalState &state){ }

 StatementType RemStmt::getType(){
     return REM;
 }
 /*
 * Implementation notes: the LetStmt
 * --------------------------------
*/

LetStmt::LetStmt(TokenScanner & scanner){
    string token = scanner.nextToken();
    lhs = new IdentifierExp(token);
    if (scanner.nextToken() == "="){
        exp = readE(scanner, 0);
    }
    if (scanner.hasMoreTokens()){
        error("Extranouse token" +  scanner.nextToken());
    }
}

LetStmt::~LetStmt(){
    delete exp;
    delete lhs;
}

void LetStmt::execute(EvalState &state){
    state.setValue(lhs->toString(), exp->eval(state));
}

StatementType LetStmt::getType(){
   return LET;
}

/*
 * Implementation notes: the InputStmt
 * -----------------------------------
*/

InputStmt::InputStmt(TokenScanner &scanner){
    var = scanner.nextToken();
    if (scanner.hasMoreTokens()){
        error("Extranouse token" +  scanner.nextToken());
    }
}

void InputStmt::execute(EvalState &state){
    value = getInteger(var + "? ");
    state.setValue(var, value);
}

StatementType InputStmt::getType(){
   return INPUT;
}

/*
 * Implementation notes: the GotoStmt
 * -----------------------------------
 */

GotoStmt::GotoStmt(TokenScanner &scanner){
    string token = scanner.nextToken();
    TokenType type = scanner.getTokenType(token);
    if (type == NUMBER){
        n = stringToInteger(token);
    }else {
        error("Not a valid line number for goto statement.");
    }
}

void GotoStmt::execute(EvalState &state){
    state.setCurrentLine(n);
}

StatementType GotoStmt::getType(){
   return GOTO;
}

/*
 * Implementation notes: the IfStmt
 * -----------------------------------
*/

IfStmt::IfStmt(TokenScanner &scanner){
    string expToken = "";
    string nextToken = "";
    bool containsThen = false;
    while(true){
        nextToken = scanner.nextToken();
        if (nextToken == "THEN" || !scanner.hasMoreTokens()){
            containsThen = true;
            break;
        }
        expToken += nextToken;
    }
    if (scanner.hasMoreTokens() && containsThen) {
        nextToken = scanner.nextToken();
        TokenType type = scanner.getTokenType(nextToken);
        if (type == NUMBER) n = stringToInteger(nextToken);
        else error("Not a valid If statement.");
    }else{
        error("Not a valid If statement.");
    }

    int opIndex = findOpIndex(expToken);
    if (opIndex != -1){
        op = expToken.at(opIndex);
        scanner.setInput(expToken.substr(0, opIndex));
        exp1 = readE(scanner,0);
        scanner.setInput(expToken.substr(opIndex + 1));
        exp2 = readE(scanner,0);
    }else {
        error("Not a valid conditional expression");
    }
}

void IfStmt::execute(EvalState &state){
    if(evalCondition(exp1, op, exp2, state)){
        state.setCurrentLine(n);
    }
}

bool IfStmt::evalCondition(Expression *exp1, string op, Expression *exp2, EvalState & state){
    if (op == "=") return (exp1->eval(state) == exp2->eval(state));
    if (op == ">") return (exp1->eval(state) > exp2->eval(state));
    if (op == "<") return (exp1->eval(state) < exp2->eval(state));
    return false;

}

int IfStmt::findOpIndex(string expToken){
    for (int i = 0; i < expToken.length(); i++){
        if (expToken[i] == '=' || expToken[i]== '>' || expToken[i] == '<') return i;
    }
    return -1;
}

IfStmt::~IfStmt(){
    delete exp1;
    delete exp2;
}

StatementType IfStmt::getType(){
   return IF;
}

/*
 * Implementation notes: the EndStmt
 * -----------------------------------
*/

EndStmt::EndStmt(){ }
EndStmt::~EndStmt() { }
void EndStmt::execute(EvalState &state){ }
StatementType EndStmt::getType(){
   return END;
}
