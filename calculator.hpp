#pragma once

#include "helpers.hpp"
#include <stack>
#include <set>          // 
#include <climits>
#include <vector>

// SCANNER DEFINITION

// You can freely add member fields and functions to this class.
class Scanner {
	int line;
	int value;
	Token *recent;

public:
	// You really need to implement these four methods for the scanner to work.
	Token nextToken();
	void eatToken(Token);
	int lineNumber();
	int getNumberValue();
	/*
	Advances to the next token and returns the value of next token
	*/
	Token moveP();

	Scanner();
	~Scanner();
};

// PARSER DEFINITION

// You can freely add member fields and functions to this class.
class Parser {
	Scanner scanner;
	std::stack<int> numbers;
	std::stack<Token> ops;
	std::vector<int> results;
	bool evaluate;
	bool subsequentExp;



	
	
	/*Parser Methods*/
	
	/**
    initiates the parser, and it throws an error if the file doesnt end with EOF
*/
	void start();
	/**
    Required methods to that simulate the CFG
*/
	void A(); void B(); void C(); void X(); void Y();
	
	/**
    Helper function that reads and pops the top of the stack
    
	@param input Stack's refrence
    @return the top value of the stack
*/
	template<class T> T top_pop(std::stack<T>& input);
	
	/*Evaluation Methods*/
	/**
    Assigns a precedence value to the operator

    @param Token takes in an operator token
    @return the precedence value given to that token
*/
	int preval(Token);
	
	/**
    Evaluates the expression inside the stack while the program hasnt reached EOF

    @param bool the value determines if the expression needs to be evaluated
   
*/
	void MidEval(bool);
	
	/**
    Evaluates the leftover expression inside the stack after the program hasnt reached EOF

    @param bool the value determines if the expression needs to be evaluated
    
*/
	void lastEval(bool);
	/**
    Prints out all the evaluated expressions that were seperated by semicolon
*/
	void printResults();
	/**
    Validates if the values are good to be evaluated
  
*/
	void validator(int value1, int value2, Token t);
	/**
    checks for the precedence among the operators
*/
	bool hasPrecedence(Token x, Token y);
	/**
    Evaluates the value given the operator
*/
	int evaluator(Token op, int value1, int value2);
public:

	void parse();
	
	Parser(bool);
	
};



