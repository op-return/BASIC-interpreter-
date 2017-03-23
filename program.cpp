/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
using namespace std;

Program::Program() {
   capacity = INITIAL_CAPACITY;
   lines = new Element*[capacity];
   for (int i = 0; i < capacity; i++){
       lines[i] = NULL;
   }
}

Program::~Program() {
    clear();
    delete[] lines;
}

void Program::clear() {
    int firstLineNumber = getFirstLineNumber();
    if (firstLineNumber != -1){
        Element *elem = lines[firstLineNumber];
        while (elem != NULL){
            Element *next = elem->next;
            Statement *stmt = elem->stmt;
            Element *oldElem = elem;
            delete stmt;
            delete oldElem;
            elem = next;
        }
        for (int i = 0; i < capacity; i++){
            lines[i] = NULL;
        }
    }
}

void Program::addSourceLine(int lineNumber, string line) {

    if (lineNumber > capacity - 1) expandCapacity(lineNumber);

    if (lines[lineNumber] == NULL){
        Element *elem = new Element;
        elem->line = line;
        elem->stmt = NULL;
        elem->lineNumber = lineNumber;
        lines[lineNumber] = elem;

        /* Putting the new element into our linked list */
        int prevLineNumber = getPreviousLineNumber(lineNumber);
        int nextLineNumber = getNextLineNumber(lineNumber);
        if (prevLineNumber == -1 && nextLineNumber != -1){          // First line
            lines[lineNumber]->next = lines[nextLineNumber];
        } else if (prevLineNumber != -1 && nextLineNumber != -1){   // A line in between
            elem->next = lines[prevLineNumber]->next;
            lines[prevLineNumber]->next = elem;
        }else if (prevLineNumber != -1 && nextLineNumber == -1){     // Last line
            lines[lineNumber]->next = NULL;
        }
    }else{
        lines[lineNumber]->line = line;
        lines[lineNumber]->lineNumber = lineNumber;
        lines[lineNumber]->stmt = NULL;
    }
}

void Program::removeSourceLine(int lineNumber) {
    if (lines[lineNumber] != NULL){
        int prevLineNumber = getPreviousLineNumber(lineNumber);
        Element *elem = lines[lineNumber];
        if (prevLineNumber != -1)
            lines[prevLineNumber]->next = lines[lineNumber]->next;
        Statement *stmt = elem->stmt;
        delete stmt;
        delete elem;
        lines[lineNumber] = NULL;
    }
}

string Program::getSourceLine(int lineNumber) {
    if (lines[lineNumber] != NULL)
        return lines[lineNumber]->line;
    return "";
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    if (lines[lineNumber] == NULL){
        error("Error: No such line exists.");
    }else{
        Statement *oldStmt = lines[lineNumber]->stmt;
        if (oldStmt != NULL) delete oldStmt;
        lines[lineNumber]->stmt = stmt;
    }
}

Statement *Program::getParsedStatement(int lineNumber) {
   if (lines[lineNumber]->stmt != NULL) return lines[lineNumber]->stmt;
   return NULL;
}

int Program::getFirstLineNumber() {
    for (int i = 0;  i < capacity; i++)
        if (lines[i] != NULL) return i;
    return -1;

}

int Program::getNextLineNumber(int lineNumber) {
    for (int i = lineNumber + 1; i < capacity; i++){
        if (lines[i] != NULL) return i;
    }
    return -1;
}

int Program::getPreviousLineNumber(int lineNumber){
    for (int i = lineNumber - 1; i >= 0; i--){
        if (lines[i] != NULL) return i;
    }
    return -1;
}

void Program::expandCapacity(int lineNumber){
    Element **oldLines = lines;
    int oldCapacity = capacity;
    capacity = lineNumber * 2;
    lines = new Element*[capacity];
    for (int i = 0; i < capacity; i++){
        if (i < oldCapacity) lines[i] = oldLines[i];
        else lines[i] = NULL;
    }
    delete[] oldLines;
}
