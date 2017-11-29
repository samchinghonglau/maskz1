// file:        interpreter.cpp
// author:      Jack S-J, Geoff T
// date:        2017-10-11

////////////////////////////////////////////////////////////////////////////////
// Information about the operators and operands used in the Interpreter class //
////////////////////////////////////////////////////////////////////////////////
//
// Operators include postfix mathematical operators +, -, *, /, ^
// +: addition
// -: subtraction
// *: multiplication
// /: (integer) division
// ^: exponentiation, implemented recursively as repeated squares as in HW2
// ***IMPORTANT*** note that the result of the mathematical operators
//   is to be enqueued in tube, rather than pushed to bin.
// 
// Additionally, there are special operators RQ, PRINT, and #
// RQ: re-enqueues all bin contents into the tube (in order of stack removal),
//     but bin must remain unchanged after execution of RQ.
//     A space-delimited string is constructed from the removed bin contents
// PRINT: empties the tube, constructs and returns a space-delimited string
//        from the removed items
// #: This executes nothing and never explicitly appears in any instruction string,
//    but is added to the tube to indicate the end of a loaded instruction
//
// Operands are integers and can take positive or negative values.
// Note that they will be stored in the tube as strings, and therefore
// you must be able to distinguish the string "-3" as the operand -3,
// and "- 3" as the subtraction operator followed by the operand 3.
// 

#include "interpreter.h"
#include <string.h>
///remove at the end
#include <iostream>
#include <cstdlib>

// default constructor
Interpreter::Interpreter() {
    tube = Queue<std::string>();
    bin = Stack<int>();
    sparebin = Stack<int>(); //use for temporally storing excuted result???
}

// copy constructor
Interpreter::Interpreter(const Interpreter& itpt) {
    tube = itpt.tube;
    bin = itpt.bin;
    sparebin = itpt.sparebin;
}

// assignment operator
Interpreter& Interpreter::operator=(const Interpreter& itpt) {
    tube = itpt.tube;
    bin = itpt.bin;
    sparebin = itpt.sparebin;
    return *this;
}

// Loads a single instruction into the tube
// Requires parsing the instruction into operators and operands
// If tube already contains items, the instruction is loaded
//   without clearing the tube contents
// This should inject a "#" operator at the end
void Interpreter::load(std::string instruction) {
    std::string ins = instruction;
    std::string delimiter = " ";

    int pos = 0;
    std::string token;
    while(pos != std::string::npos){
        pos = ins.find(delimiter);
        token = ins.substr(0, pos);
        ins = ins.substr(pos+1, std::string::npos);
        // std::cout << "value:" << token << std::endl;
        tube.enqueue(token);
    }
    tube.enqueue("#");    

}

// Returns whether there are still items in the tube to be processed
bool Interpreter::hasMoreInstructions() {
    return !tube.isEmpty();
}

// Remove and execute next instruction
// i.e. until a #, empty tube, print, or other error encountered
// The output of any operator(s) will be returned
// If an attempt to execute an operator causes an exception,
//   throw the caught exception
// In next(), you pop elements from the tube and execute them 
// (unless the stack is empty, or you encounter # or other operands). 
// The function should return a concatenated string of all the executed elements.
std::string Interpreter::next() {
    std::string operators = "+ - * / ^ RQ PRINT #";
    std::string element;
    std::string output;  
    if (tube.isEmpty()){
        throw std::out_of_range("Tube is empty!");
    }
    //std::cout << element << std::endl;
    while (!tube.isEmpty()){
        element = tube.peek();
        tube.dequeue();
        if (operators.find(element) == std::string::npos){
            // found operands, transform it from string to int then push into bin
            bin.push(toInt(element));
        } else {
            // found operators, call execOp to execute them. If it causes an exception, 
            // throw the caught exception
            try{
                output = execOp(element);
                if(element == "#" || element == "RQ" || element == "PRINT"){
                    break;
                }
                
            } catch (std::exception e){
                std::cout << "Exception caught: " << e.what() << std::endl;
            }
        }
    }
    return output;
    
}

// Execute remaining instructions
// The outputs of each instruction will be
// appended into a single string, separated by the
// newline character '\n'. The last instruction
// will NOT be followed by a newline.
std::string Interpreter::run() {
    std::string output = "";
    while (!tube.isEmpty()){
        output.append(next());

		if( tube.size() > 1 ) {
			output.append("\n");
			}
    }
    return output;
}

// Clears the contents of all member containers (tube, bin, sparebin)
void Interpreter::clear() {
    while (!tube.isEmpty()){
        tube.dequeue();
    }
    while (!bin.isEmpty()){
        bin.pop();
    }
    while (!sparebin.isEmpty()){
        sparebin.pop();
    }
}

// executes an operator, returns a string of execution result
// most operators will return the empty string "", except for RQ, PRINT
// If an operator causes a container exception, throw the caught exception
std::string Interpreter::execOp(std::string optr) {
    if (optr == "#"){
        return "";
    }
    if (optr == "RQ"){
        // make a deep copy of the current bin and then re-enqueue the bincopy items into tube
		std::string out = "";
        while (!bin.isEmpty()){
			int temp = bin.pop();
			out.append(toString(temp));
            sparebin.push(temp);
			tube.enqueue(toString(temp));

			if( bin.size() >= 1 )
				out.append(" ");
        }
        while(!sparebin.isEmpty()){
            int temp = sparebin.pop();
            bin.push(temp);
        }
        return out;
    } 
    if (optr == "PRINT"){
        std::string out;
        while (!tube.isEmpty()){
            std::string temp = tube.dequeue();
            out = out.append(temp);
            out.append(" ");
        }
        return out;
    }
    if (bin.isEmpty()){
        throw std::out_of_range( "bin is empty" );
    }
    int op1 = bin.pop();
    int op2 = bin.pop();
    int result;
    if (optr == "+"){
        result = op1 + op2;
        tube.enqueue(toString(result));           
        return "";
    }
    if (optr == "-"){
        result = op2 - op1;
        tube.enqueue(toString(result));
        return "";
    }
    if (optr == "*"){
        result = op1 * op2;
        tube.enqueue(toString(result));
        return "";
    }
    if (optr == "/"){
        result = op2 / op1;
        tube.enqueue(toString(result));
        return "";
    }
    if (optr == "^"){
        result = recpow(op2, op1);
        tube.enqueue(toString(result));
        return "";
    }

	return "";
}

// Exponentiation function calculates x^y
//   by repeated squaring (refer to HW2)
// You may implement this either iteratively or recursively
// PRE: y >= 0
int Interpreter::recpow(int x, int y) {
    if (y <= 0) {
        return 1; 
    }
    int temp = recpow(x, y/2);
    int result = temp*temp;
    if (y % 2 == 0) {
        return result;
    }
    return result*x;
}