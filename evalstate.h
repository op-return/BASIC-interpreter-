/*
 * File: evalstate.h
 * -----------------
 * This interface exports a class called EvalState, which keeps track
 * of additional information required by the evaluator, most notably
 * the values of variables.
 */

#ifndef _evalstate_h
#define _evalstate_h

#include <string>
#include "map.h"

/*
 * Class: EvalState
 * ----------------
 * This class is passed by reference through the recursive levels
 * of the evaluator and contains information from the evaluation
 * environment that the evaluator may need to know.  In this
 * version, the only information maintained by the EvalState class
 * is a symbol table that maps variable names into their values.
 * Several of the exercises, however, require you to include
 * additional information in the EvalState class.
 */

class EvalState {

public:

/*
 * Constructor: EvalState
 * Usage: EvalState state;
 * -----------------------
 * Creates a new EvalState object with no variable bindings.
 */

   EvalState();

/*
 * Destructor: ~EvalState
 * Usage: usually implicit
 * -----------------------
 * Frees all heap storage associated with this object.
 */

   ~EvalState();

/*
 * Method: setValue
 * Usage: state.setValue(var, value);
 * ----------------------------------
 * Sets the value associated with the specified var.
 */

   void setValue(std::string var, int value);

/*
 * Method: getValue
 * Usage: int value = state.getValue(var);
 * ---------------------------------------
 * Returns the value associated with the specified variable.
 */

   int getValue(std::string var);

/*
 * Method: isDefined
 * Usage: if (state.isDefined(var)) . . .
 * --------------------------------------
 * Returns true if the specified variable is defined.
 */

   bool isDefined(std::string var);

 /*
  * Method: setCurrentLine
  * Usage: setCurrentLine (int lineNumber)
  * --------------------------------------
  * This method sets the currentLine variable which keeps track of the current
  * line being executed.
  */
   void setCurrentLine(int currentLine);


   /*
    * Method: getCurrentLine
    * Usage: int currentLine = getCurrentLine()
    * -----------------------------------------
    * This method returns the currentLine variable which keeps track of the current
    * line being executed.
    */
   int getCurrentLine();

private:

   Map<std::string,int> symbolTable;
   int currentLine;

};

#endif
