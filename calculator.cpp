
#include "calculator.hpp"
#include <string>
#include <cmath>
#include <iostream>
// Scanner implementation
// You may have to modify this constructor, although it might not be neccessary.
Scanner::Scanner() : line(1),
value(0),
recent(NULL){
	// WRITEME
}
Scanner::~Scanner(){

	delete recent;
}
// You need to fill this method with the appropriate code for it to work as described in the project description.
Token Scanner::nextToken() {
	if (recent == NULL){
		recent = new Token();
		*recent = moveP();
	}
	while (*recent == T_NEWLN)
		*recent = moveP();

	return *recent;
	
	return T_EOF;
}
void Scanner::eatToken(Token toConsume) {
	if (toConsume == nextToken()){
		*recent = moveP();
	}
	else
		mismatchError(line, toConsume, nextToken());
	
}
Token Scanner::moveP(){
	char c;
	//Skipping Whitespaces
	while (std::cin.peek() == ' ' || std::cin.peek()=='\t')
		c = getchar();

	while ((c = getchar()) != -1)
	{

		switch (c)
		{
		case '+':
			return  T_PLUS;
		case '-':
			return T_MINUS;
		case '*':
			return (std::cin.peek() == '*' && getchar()) ? T_EXP : T_MULTIPLY;
		case '/':
			return T_DIVIDE;
		case '%':
			return T_MODULO;
		case '(':
			return T_OPENPAREN;
		case ')':
			return T_CLOSEPAREN;
		case ';':
			return T_SEMICOLON;
		case '\n':
			line++;
			return T_NEWLN;
		case EOF:
			return T_EOF;
		default:
			if (isdigit(c))
			{
				std::string number(1, c);
				while (isdigit(std::cin.peek()))
				{
					number += getchar();
				}
				try{
					
					value = std::stoi(number);
				}
				catch (std::exception e){
					std::cerr << "Semantic error: number " << number << " out of bounds at line " << line << std::endl;
					exit(1);
					
				}
				return T_NUMBER;
			}
			else
				scanError(line, c);

		}
	}
	return T_EOF;

}
int Scanner::lineNumber() {
	return this->line;
}
int Scanner::getNumberValue() {
	return this->value;
}
// Parser implementation

     Parser::Parser(bool eval) : evaluate(eval) {
		 subsequentExp = false;

}
	 template<class T>
	 T Parser::top_pop(std::stack<T>& input){
		 T temp = input.top();
		 input.pop();
		 return temp;
	 }
void Parser::lastEval(bool eval){

	if (eval)
	{
		while (ops.empty() == false)
			numbers.push(evaluator(top_pop(ops), top_pop(numbers), top_pop(numbers)));
		results.push_back(numbers.top());

	}

}
void Parser::parse() {
	
	start();
	lastEval(evaluate);
	printResults();

}
void Parser::printResults()
	{
	for (unsigned i = 0; i != results.size(); i++) 
		std::cout << results[i] << std::endl;
	}
int  Parser::preval(Token t){
	switch (t){
	case T_PLUS:
	case T_MINUS:
		return 3;
	case T_MULTIPLY:
	case T_DIVIDE:
	case T_MODULO:
		return 2;
	case T_EXP:
		return 1;
	case T_OPENPAREN:
		return 4;
	default:
		return 5;
	}
}
bool Parser::hasPrecedence(Token x, Token y){
	if (x == y && x == T_EXP){
		subsequentExp = true;
		return false;
	}
	else
		return preval(x) <= preval(y);

}
void Parser::MidEval(bool eval){
	

	while (	eval &&
			!ops.empty() &&
			hasPrecedence(ops.top(), scanner.nextToken()) &&
			numbers.size() > 1
		  )	
		{
		subsequentExp = false;
		numbers.push(evaluator(top_pop(ops), top_pop(numbers), top_pop(numbers)));

		}
	
}
void Parser::start(){	
	X();
	if (scanner.nextToken() != T_EOF)
		parseError(scanner.lineNumber(), scanner.nextToken()); 
}
void Parser::A(){

	B();
	C();
}
void Parser::B(){
	switch (scanner.nextToken())
	{
	case T_OPENPAREN:
		scanner.eatToken(T_OPENPAREN);
		if (evaluate)
			ops.push(T_OPENPAREN);
		A();
		scanner.eatToken(T_CLOSEPAREN);
		if (evaluate)
		{
			while (ops.top() != T_OPENPAREN)
				numbers.push(evaluator(top_pop(ops), top_pop(numbers), top_pop(numbers)));
			ops.pop();
		}
		break;

	case T_NUMBER:
		if (evaluate)
			numbers.push(scanner.getNumberValue());
		scanner.eatToken(T_NUMBER);
		break;
	
	default:
		parseError(scanner.lineNumber(), scanner.nextToken());
		break;
	}
	


}
void Parser::X(){
	A();
	Y();
}
void Parser::Y(){
	//Y -> ;X | EPSILON
	switch (scanner.nextToken())
	{
	
	case T_SEMICOLON:
		scanner.eatToken(T_SEMICOLON);
		lastEval(evaluate);
		X();
		break;
	default:
		break;
	}

}
void Parser::C(){
	Token x = scanner.nextToken();
	switch (x)
	{
	case T_PLUS:	case T_MINUS:	case T_MULTIPLY:
	case T_DIVIDE:	case T_MODULO:	case T_EXP:
		MidEval(evaluate);
		ops.push(x);
		scanner.eatToken(x);
		A();
		break;
	
	default:
		break;
	}
}
int  Parser::evaluator(Token op, int value1, int value2){
	// validator!!
	validator(value1, value2,op);
	switch (op)
	{
	case T_PLUS:
		return value1 + value2;
	case T_MINUS:
		return value1 - value2;
	case T_MULTIPLY:
		return value1 * value2;
	case T_DIVIDE:
		return value1 / value2;
	case T_EXP:
		return int(std::pow(value1 ,value2));
	case T_MODULO:
		return value1 % value2;
	default:
		std::cout << "Error in Operation\n";
		return -1;
			
	}
}
void Parser::validator(int value1, int value2, Token t){
	if (value2 == 0 &&(t==T_DIVIDE||t==T_MODULO))
		divideByZeroError(scanner.lineNumber(), value1);
	if (value1 > INT_MAX)
		outOfBoundsError(scanner.lineNumber(), value1);
	if (value2 > INT_MAX)
		outOfBoundsError(scanner.lineNumber(), value1);
	if ((value1 > INT_MAX - value2) && (t == T_PLUS))
		outOfBoundsError(scanner.lineNumber(), value1+value2);
	if ((value2 > INT_MAX - value1) && (t == T_PLUS))
		outOfBoundsError(scanner.lineNumber(), value1+value2);
	if (abs(long(value1)-long(value2)) > INT_MAX  && (t == T_MINUS))
		outOfBoundsError(scanner.lineNumber(), value1-value2);
	if (long(value1)*long(value2) > INT_MAX && (t == T_MULTIPLY))
		outOfBoundsError(scanner.lineNumber(), value1*value2);
	if (pow(value1, value2) > INT_MAX && t == T_EXP){
		long x = long(pow(value1, value2));
		outOfBoundsError(scanner.lineNumber(),x);
	}

}