/*
 * File: Basic.cpp
 * ---------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <iostream>
#include <string>
#include "console.h"
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "tokenscanner.h"
#include "simpio.h"
#include "strlib.h"
#include "statement.h"
using namespace std;

/* Function prototypes */

void processLine(string line, Program & program, EvalState & state);
void list(Program & program, int lineNumber);
void run(Program & program, EvalState &state);

/* Main program */

int main() {
    EvalState state;
    Program program;
    cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            processLine(getLine(), program, state);
        } catch (ErrorException & ex) {
            cerr << "Error: " << ex.getMessage() << endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program & program, EvalState & state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);
    string token = scanner.nextToken();
    TokenType type = scanner.getTokenType(token);
    if (type == NUMBER){
        int lineNumber = stringToInteger(token);
        if (!scanner.hasMoreTokens()) {
            program.removeSourceLine(lineNumber);
        }else{
            program.addSourceLine(lineNumber,line);
            program.setParsedStatement(lineNumber,parseStatement(scanner));
        }
    }else if (token == "CLEAR") program.clear();
    else if (token == "LIST") list(program, program.getFirstLineNumber());
    else if (token == "RUN") run(program, state);
    else if (token == "QUIT") exit(0);
    return;
}

void list (Program & program, int lineNumber){
    cout << program.getSourceLine(lineNumber) << endl;
    if (program.getNextLineNumber(lineNumber) != -1){
        list (program, program.getNextLineNumber(lineNumber));
    }
}


void run(Program &program, EvalState &state){
    state.setCurrentLine(program.getFirstLineNumber());
    while (true){
        Statement *stmt = program.getParsedStatement(state.getCurrentLine());
        state.setCurrentLine(program.getNextLineNumber(state.getCurrentLine()));
        stmt->execute(state);
        if (state.getCurrentLine() == -1 || stmt->getType() == END) break;
    }
    return;
}
